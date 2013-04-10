/*----------------------------------------------------------------------
----------------------------------------------------------------------*/
#include "FWCore/Framework/interface/InputSource.h"

#include "DataFormats/Provenance/interface/ProcessHistory.h"
#include "DataFormats/Provenance/interface/ProcessHistoryRegistry.h"
#include "DataFormats/Provenance/interface/ProductRegistry.h"
#include "DataFormats/Provenance/interface/FullHistoryToReducedHistoryMap.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/ExceptionHelpers.h"
#include "FWCore/Framework/interface/FileBlock.h"
#include "FWCore/Framework/interface/InputSourceDescription.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "FWCore/Framework/interface/LuminosityBlockPrincipal.h"
#include "FWCore/Framework/interface/MessageReceiverForSource.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/RunPrincipal.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/ActivityRegistry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "FWCore/Utilities/interface/do_nothing_deleter.h"
#include "FWCore/Utilities/interface/GlobalIdentifier.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/TimeOfDay.h"

#include <cassert>
#include <fstream>
#include <iomanip>

namespace edm {

  namespace {
        std::string const& suffix(int count) {
          static std::string const st("st");
          static std::string const nd("nd");
          static std::string const rd("rd");
          static std::string const th("th");
          // *0, *4 - *9 use "th".
          int lastDigit = count % 10;
          if(lastDigit >= 4 || lastDigit == 0) return th;
          // *11, *12, or *13 use "th".
          if(count % 100 - lastDigit == 10) return th;
          return (lastDigit == 1 ? st : (lastDigit == 2 ? nd : rd));
        }
        template <typename T>
        boost::shared_ptr<T> createSharedPtrToStatic(T* ptr) {
          return boost::shared_ptr<T>(ptr, do_nothing_deleter());
        }
  }

  InputSource::InputSource(ParameterSet const& pset, InputSourceDescription const& desc) :
      ProductRegistryHelper(),
      actReg_(desc.actReg_),
      maxEvents_(desc.maxEvents_),
      remainingEvents_(maxEvents_),
      maxLumis_(desc.maxLumis_),
      remainingLumis_(maxLumis_),
      readCount_(0),
      processingMode_(RunsLumisAndEvents),
      moduleDescription_(desc.moduleDescription_),
      productRegistry_(createSharedPtrToStatic<ProductRegistry const>(desc.productRegistry_)),
      branchIDListHelper_(desc.branchIDListHelper_),
      primary_(pset.getParameter<std::string>("@module_label") == std::string("@main_input")),
      processGUID_(primary_ ? createGlobalIdentifier() : std::string()),
      time_(),
      newRun_(true),
      newLumi_(true),
      eventCached_(false),
      state_(IsInvalid),
      runAuxiliary_(),
      lumiAuxiliary_(),
      statusFileName_(),
      receiver_(),
      numberOfEventsBeforeBigSkip_(0) {

    if(pset.getUntrackedParameter<bool>("writeStatusFile", false)) {
      std::ostringstream statusfilename;
      statusfilename << "source_" << getpid();
      statusFileName_ = statusfilename.str();
    }

    // Secondary input sources currently do not have a product registry.
    if(primary_) {
      assert(desc.productRegistry_ != 0);
    }
    std::string const defaultMode("RunsLumisAndEvents");
    std::string const runMode("Runs");
    std::string const runLumiMode("RunsAndLumis");

    // The default value provided as the second argument to the getUntrackedParameter function call
    // is not used when the ParameterSet has been validated and the parameters are not optional
    // in the description.  As soon as all primary input sources and all modules with a secondary
    // input sources have defined descriptions, the defaults in the getUntrackedParameterSet function
    // calls can and should be deleted from the code.
    std::string processingMode = pset.getUntrackedParameter<std::string>("processingMode", defaultMode);
    if(processingMode == runMode) {
      processingMode_ = Runs;
    } else if(processingMode == runLumiMode) {
      processingMode_ = RunsAndLumis;
    } else if(processingMode != defaultMode) {
      throw Exception(errors::Configuration)
        << "InputSource::InputSource()\n"
        << "The 'processingMode' parameter for sources has an illegal value '" << processingMode << "'\n"
        << "Legal values are '" << defaultMode << "', '" << runLumiMode << "', or '" << runMode << "'.\n";
    }
  }

  InputSource::~InputSource() {}

  void
  InputSource::fillDescriptions(ConfigurationDescriptions& descriptions) {
    ParameterSetDescription desc;
    desc.setUnknown();
    descriptions.addDefault(desc);
  }

  void
  InputSource::prevalidate(ConfigurationDescriptions& ) {
  }
  

  static std::string const kBaseType("Source");

  std::string const&
  InputSource::baseType() {
    return kBaseType;
  }

  void
  InputSource::fillDescription(ParameterSetDescription& desc) {
    std::string defaultString("RunsLumisAndEvents");
    desc.addUntracked<std::string>("processingMode", defaultString)->setComment(
    "'RunsLumisAndEvents': process runs, lumis, and events.\n"
    "'RunsAndLumis':       process runs and lumis (not events).\n"
    "'Runs':               process runs (not lumis or events).");
    desc.addUntracked<bool>("writeStatusFile", false)->setComment("Write a status file. Intended for use by workflow management.");
  }

  bool
  InputSource::skipForForking() {
    if(eventLimitReached()) {
      return false;
    }
    if(receiver_ && 0 == numberOfEventsBeforeBigSkip_) {
      receiver_->receive();
      unsigned long toSkip = receiver_->numberToSkip();
      if(0 != toSkip) {
        skipEvents(toSkip);
        decreaseRemainingEventsBy(toSkip);
      }
      numberOfEventsBeforeBigSkip_ = receiver_->numberOfConsecutiveIndices();
      if(0 == numberOfEventsBeforeBigSkip_ or 0 == remainingEvents() or 0 == remainingLuminosityBlocks()) {
        return false;
      }
    }
    return true;
  }

  // This next function is to guarantee that "runs only" mode does not return events or lumis,
  // and that "runs and lumis only" mode does not return events.
  // For input sources that are not random access (e.g. you need to read through the events
  // to get to the lumis and runs), this is all that is involved to implement these modes.
  // For input sources where events or lumis can be skipped, getNextItemType() should
  // implement the skipping internally, so that the performance gain is realized.
  // If this is done for a source, the 'if' blocks in this function will never be entered
  // for that source.
  InputSource::ItemType
  InputSource::nextItemType_() {
    ItemType itemType = callWithTryCatchAndPrint<ItemType>( [this](){ return getNextItemType(); }, "Calling InputSource::getNextItemType" );

    if(itemType == IsEvent && processingMode() != RunsLumisAndEvents) {
      skipEvents(1);
      return nextItemType_();
    }
    if(itemType == IsLumi && processingMode() == Runs) {
      // QQQ skipLuminosityBlock_();
      return nextItemType_();
    }
    return itemType;
  }

  InputSource::ItemType
  InputSource::nextItemType() {
    ItemType oldState = state_;
    if(eventLimitReached()) {
      // If the maximum event limit has been reached, stop.
      state_ = IsStop;
    } else if(lumiLimitReached()) {
      // If the maximum lumi limit has been reached, stop
      // when reaching a new file, run, or lumi.
      if(oldState == IsInvalid || oldState == IsFile || oldState == IsRun || processingMode() != RunsLumisAndEvents) {
        state_ = IsStop;
      } else {
        ItemType newState = nextItemType_();
        if(newState == IsEvent) {
          assert (processingMode() == RunsLumisAndEvents);
          state_ = IsEvent;
        } else {
          state_ = IsStop;
        }
      }
    } else {
      ItemType newState = nextItemType_();
      if(newState == IsStop) {
        state_ = IsStop;
      } else if(newState == IsFile || oldState == IsInvalid) {
        state_ = IsFile;
      } else if(newState == IsRun || oldState == IsFile) {
        runAuxiliary_ = readRunAuxiliary();
        state_ = IsRun;
      } else if(newState == IsLumi || oldState == IsRun) {
        assert (processingMode() != Runs);
        lumiAuxiliary_ = readLuminosityBlockAuxiliary();
        state_ = IsLumi;
      } else {
        assert (processingMode() == RunsLumisAndEvents);
        state_ = IsEvent;
      }
    }
    if(state_ == IsStop) {
      lumiAuxiliary_.reset();
      runAuxiliary_.reset();
    }
    return state_;
  }

  boost::shared_ptr<LuminosityBlockAuxiliary>
  InputSource::readLuminosityBlockAuxiliary() {
    return callWithTryCatchAndPrint<boost::shared_ptr<LuminosityBlockAuxiliary> >( [this](){ return readLuminosityBlockAuxiliary_(); },
                                                                                   "Calling InputSource::readLuminosityBlockAuxiliary_" );
  }

  boost::shared_ptr<RunAuxiliary>
  InputSource::readRunAuxiliary() {
    return callWithTryCatchAndPrint<boost::shared_ptr<RunAuxiliary> >( [this](){ return readRunAuxiliary_(); },
                                                                       "Calling InputSource::readRunAuxiliary_" );
  }

  void
  InputSource::doBeginJob() {
    this->beginJob();
  }

  void
  InputSource::doEndJob() {
    endJob();
  }

  void
  InputSource::registerProducts() {
    if(!typeLabelList().empty()) {
      addToRegistry(typeLabelList().begin(), typeLabelList().end(), moduleDescription(), productRegistryUpdate());
    }
  }

  // Return a dummy file block.
  boost::shared_ptr<FileBlock>
  InputSource::readFile() {
    assert(state_ == IsFile);
    assert(!limitReached());
    boost::shared_ptr<FileBlock> fb = callWithTryCatchAndPrint<boost::shared_ptr<FileBlock> >( [this](){ return readFile_(); },
                                                                                               "Calling InputSource::readFile_" );
    return fb;
  }

  void
  InputSource::closeFile(boost::shared_ptr<FileBlock> fb, bool cleaningUpAfterException) {
    if(fb) fb->close();
    callWithTryCatchAndPrint<void>( [this](){ closeFile_(); },
                                    "Calling InputSource::closeFile_",
                                    cleaningUpAfterException );
    return;
  }

  // Return a dummy file block.
  // This function must be overridden for any input source that reads a file
  // containing Products.
  boost::shared_ptr<FileBlock>
  InputSource::readFile_() {
    return boost::shared_ptr<FileBlock>(new FileBlock);
  }

  boost::shared_ptr<RunPrincipal>
  InputSource::readAndCacheRun(HistoryAppender& historyAppender) {
    RunSourceSentry sentry(*this);
    boost::shared_ptr<RunPrincipal> rp(new RunPrincipal(runAuxiliary(), productRegistry_, processConfiguration(), &historyAppender));
    callWithTryCatchAndPrint<boost::shared_ptr<RunPrincipal> >( [this,&rp](){ return readRun_(rp); }, "Calling InputSource::readRun_" );
    return rp;
  }

  void
  InputSource::readAndMergeRun(boost::shared_ptr<RunPrincipal> rp) {
    RunSourceSentry sentry(*this);
    callWithTryCatchAndPrint<boost::shared_ptr<RunPrincipal> >( [this,&rp](){ return readRun_(rp); }, "Calling InputSource::readRun_" );
  }

  boost::shared_ptr<LuminosityBlockPrincipal>
  InputSource::readAndCacheLumi(HistoryAppender& historyAppender) {
    LumiSourceSentry sentry(*this);
    boost::shared_ptr<LuminosityBlockPrincipal> lbp(
      new LuminosityBlockPrincipal(luminosityBlockAuxiliary(),
                                   productRegistry_,
                                   processConfiguration(),
                                   &historyAppender));
    callWithTryCatchAndPrint<boost::shared_ptr<LuminosityBlockPrincipal> >( [this,&lbp](){ return readLuminosityBlock_(lbp); },
                                                                            "Calling InputSource::readLuminosityBlock_" );
    if(remainingLumis_ > 0) {
      --remainingLumis_;
    }
    return lbp;
  }

  void
  InputSource::readAndMergeLumi(boost::shared_ptr<LuminosityBlockPrincipal> lbp) {
    LumiSourceSentry sentry(*this);
    callWithTryCatchAndPrint<boost::shared_ptr<LuminosityBlockPrincipal> >( [this,&lbp](){ return readLuminosityBlock_(lbp); },
                                                                            "Calling InputSource::readLuminosityBlock_" );
    if(remainingLumis_ > 0) {
      --remainingLumis_;
    }
  }

  boost::shared_ptr<RunPrincipal>
  InputSource::readRun_(boost::shared_ptr<RunPrincipal> runPrincipal) {
    // Note: For the moment, we do not support saving and restoring the state of the
    // random number generator if random numbers are generated during processing of runs
    // (e.g. beginRun(), endRun())
    runPrincipal->fillRunPrincipal();
    return runPrincipal;
  }

  boost::shared_ptr<LuminosityBlockPrincipal>
  InputSource::readLuminosityBlock_(boost::shared_ptr<LuminosityBlockPrincipal> lumiPrincipal) {
    lumiPrincipal->fillLuminosityBlockPrincipal();
    return lumiPrincipal;
  }

  EventPrincipal*
  InputSource::readEvent(EventPrincipal& ep) {
    assert(state_ == IsEvent);
    assert(!eventLimitReached());

    EventPrincipal* result = callWithTryCatchAndPrint<EventPrincipal*>( [this,&ep](){ return readEvent_(ep); }, "Calling InputSource::readEvent_" );
    if(receiver_) {
      --numberOfEventsBeforeBigSkip_;
    }

    if(result != 0) {
      Event event(*result, moduleDescription());
      postRead(event);
      if(remainingEvents_ > 0) --remainingEvents_;
      ++readCount_;
      setTimestamp(result->time());
      issueReports(result->id());
    }
    return result;
  }

  EventPrincipal*
  InputSource::readEvent(EventPrincipal& ep, EventID const& eventID) {
    EventPrincipal* result = 0;

    if(!limitReached()) {
      //result = callWithTryCatchAndPrint<EventPrincipal*>( [this,ep,&eventID](){ return readIt(eventID, ep); }, "Calling InputSource::readIt" );
      result = readIt(eventID, ep);

      if(result != 0) {
        Event event(*result, moduleDescription());
        postRead(event);
        if(remainingEvents_ > 0) --remainingEvents_;
        ++readCount_;
        issueReports(result->id());
      }
    }
    return result;
  }

  void
  InputSource::skipEvents(int offset) {
    callWithTryCatchAndPrint<void>( [this,&offset](){ skip(offset); }, "Calling InputSource::skip" );
  }

  bool
  InputSource::goToEvent(EventID const& eventID) {
    return callWithTryCatchAndPrint<bool>( [this,&eventID](){ return goToEvent_(eventID); }, "Calling InputSource::goToEvent_" );
  }

  void
  InputSource::rewind() {
    state_ = IsInvalid;
    remainingEvents_ = maxEvents_;
    setNewRun();
    setNewLumi();
    resetEventCached();
    callWithTryCatchAndPrint<void>( [this](){ rewind_(); }, "Calling InputSource::rewind_" );
    if(receiver_) {
      unsigned int numberToSkip = receiver_->numberToSkip();
      skip(numberToSkip);
      decreaseRemainingEventsBy(numberToSkip);
    }
  }

  void
  InputSource::issueReports(EventID const& eventID) {
    if(isInfoEnabled()) {
      LogVerbatim("FwkReport") << "Begin processing the " << readCount_
                               << suffix(readCount_) << " record. Run " << eventID.run()
                               << ", Event " << eventID.event()
                               << ", LumiSection " << eventID.luminosityBlock()
                               << " at " << std::setprecision(3) << TimeOfDay();
    }
    if(!statusFileName_.empty()) {
      std::ofstream statusFile(statusFileName_.c_str());
      statusFile << eventID << " time: " << std::setprecision(3) << TimeOfDay() << '\n';
      statusFile.close();
    }

    // At some point we may want to initiate checkpointing here
  }

  EventPrincipal*
  InputSource::readIt(EventID const&, EventPrincipal&) {
    throw Exception(errors::LogicError)
      << "InputSource::readIt()\n"
      << "Random access is not implemented for this type of Input Source\n"
      << "Contact a Framework Developer\n";
  }

  void
  InputSource::setRun(RunNumber_t) {
    throw Exception(errors::LogicError)
      << "InputSource::setRun()\n"
      << "Run number cannot be modified for this type of Input Source\n"
      << "Contact a Framework Developer\n";
  }

  void
  InputSource::setLumi(LuminosityBlockNumber_t) {
    throw Exception(errors::LogicError)
      << "InputSource::setLumi()\n"
      << "Luminosity Block ID cannot be modified for this type of Input Source\n"
      << "Contact a Framework Developer\n";
  }

  void
  InputSource::skip(int) {
    throw Exception(errors::LogicError)
      << "InputSource::skip()\n"
      << "Forking and random access are not implemented for this type of Input Source\n"
      << "Contact a Framework Developer\n";
  }

  bool
  InputSource::goToEvent_(EventID const&) {
    throw Exception(errors::LogicError)
      << "InputSource::goToEvent_()\n"
      << "Random access is not implemented for this type of Input Source\n"
      << "Contact a Framework Developer\n";
    return true;
  }

  void
  InputSource::rewind_() {
    throw Exception(errors::LogicError)
      << "InputSource::rewind()\n"
      << "Forking and random access are not implemented for this type of Input Source\n"
      << "Contact a Framework Developer\n";
  }

  void
  InputSource::decreaseRemainingEventsBy(int iSkipped) {
    if(-1 == remainingEvents_) {
      return;
    }
    if(iSkipped < remainingEvents_) {
      remainingEvents_ -= iSkipped;
    } else {
      remainingEvents_ = 0;
    }
  }

  void
  InputSource::postRead(Event& event) {
    Service<RandomNumberGenerator> rng;
    if(rng.isAvailable()) {
      rng->postEventRead(event);
    }
  }

  void
  InputSource::doBeginRun(RunPrincipal& rp) {
    Run run(rp, moduleDescription());
    callWithTryCatchAndPrint<void>( [this,&run](){ beginRun(run); }, "Calling InputSource::beginRun" );
    run.commit_();
  }

  void
  InputSource::doEndRun(RunPrincipal& rp, bool cleaningUpAfterException) {
    rp.setEndTime(time_);
    rp.setComplete();
    Run run(rp, moduleDescription());
    callWithTryCatchAndPrint<void>( [this,&run](){ endRun(run); }, "Calling InputSource::endRun", cleaningUpAfterException );
    run.commit_();
  }

  void
  InputSource::doBeginLumi(LuminosityBlockPrincipal& lbp) {
    LuminosityBlock lb(lbp, moduleDescription());
    callWithTryCatchAndPrint<void>( [this,&lb](){ beginLuminosityBlock(lb); }, "Calling InputSource::beginLuminosityBlock" );
    lb.commit_();
  }

  void
  InputSource::doEndLumi(LuminosityBlockPrincipal& lbp, bool cleaningUpAfterException) {
    lbp.setEndTime(time_);
    lbp.setComplete();
    LuminosityBlock lb(lbp, moduleDescription());
    callWithTryCatchAndPrint<void>( [this,&lb](){ endLuminosityBlock(lb); }, "Calling InputSource::endLuminosityBlock", cleaningUpAfterException );
    lb.commit_();
  }

  void
  InputSource::doPreForkReleaseResources() {
    callWithTryCatchAndPrint<void>( [this](){ preForkReleaseResources(); }, "Calling InputSource::preForkReleaseResources" );
  }

  void
  InputSource::doPostForkReacquireResources(boost::shared_ptr<multicore::MessageReceiverForSource> iReceiver) {
    callWithTryCatchAndPrint<void>( [this, &iReceiver](){ postForkReacquireResources(iReceiver); },
                                    "Calling InputSource::postForkReacquireResources" );
  }

  bool
  InputSource::randomAccess() const {
    return callWithTryCatchAndPrint<bool>( [this](){ return randomAccess_(); },
                                           "Calling InputSource::randomAccess_" );
  }

  ProcessingController::ForwardState
  InputSource::forwardState() const {
    return callWithTryCatchAndPrint<ProcessingController::ForwardState>( [this](){ return forwardState_(); },
                                                                         "Calling InputSource::forwardState_" );
  }

  ProcessingController::ReverseState
  InputSource::reverseState() const {
    return callWithTryCatchAndPrint<ProcessingController::ReverseState>( [this](){ return reverseState_(); },
                                                                         "Calling InputSource::reverseState__" );
  }

  void
  InputSource::beginLuminosityBlock(LuminosityBlock&) {}

  void
  InputSource::endLuminosityBlock(LuminosityBlock&) {}

  void
  InputSource::beginRun(Run&) {}

  void
  InputSource::endRun(Run&) {}

  void
  InputSource::beginJob() {}

  void
  InputSource::endJob() {}

  void
  InputSource::preForkReleaseResources() {}

  void
  InputSource::postForkReacquireResources(boost::shared_ptr<multicore::MessageReceiverForSource> iReceiver) {
    receiver_ = iReceiver;
    receiver_->receive();
    numberOfEventsBeforeBigSkip_ = receiver_->numberOfConsecutiveIndices();
    rewind();
  }

  bool
  InputSource::randomAccess_() const {
    return false;
  }

  ProcessingController::ForwardState
  InputSource::forwardState_() const {
    return ProcessingController::kUnknownForward;
  }

  ProcessingController::ReverseState
  InputSource::reverseState_() const {
    return ProcessingController::kUnknownReverse;
  }

  ProcessHistoryID const&
  InputSource::reducedProcessHistoryID() const {
    assert(runAuxiliary());
    return ProcessHistoryRegistry::instance()->extra().reduceProcessHistoryID(runAuxiliary()->processHistoryID());
  }

  RunNumber_t
  InputSource::run() const {
    assert(runAuxiliary());
    return runAuxiliary()->run();
  }

  LuminosityBlockNumber_t
  InputSource::luminosityBlock() const {
    assert(luminosityBlockAuxiliary());
    return luminosityBlockAuxiliary()->luminosityBlock();
  }

  InputSource::SourceSentry::SourceSentry(Sig& pre, Sig& post) : post_(post) {
    pre();
  }

  InputSource::SourceSentry::~SourceSentry() {
    post_();
  }

  InputSource::EventSourceSentry::EventSourceSentry(InputSource const& source) :
     sentry_(source.actReg()->preSourceSignal_, source.actReg()->postSourceSignal_) {
  }

  InputSource::LumiSourceSentry::LumiSourceSentry(InputSource const& source) :
     sentry_(source.actReg()->preSourceLumiSignal_, source.actReg()->postSourceLumiSignal_) {
  }

  InputSource::RunSourceSentry::RunSourceSentry(InputSource const& source) :
     sentry_(source.actReg()->preSourceRunSignal_, source.actReg()->postSourceRunSignal_) {
  }

  InputSource::FileOpenSentry::FileOpenSentry(InputSource const& source) :
     sentry_(source.actReg()->preOpenFileSignal_, source.actReg()->postOpenFileSignal_) {
  }

  InputSource::FileCloseSentry::FileCloseSentry(InputSource const& source) :
     post_(source.actReg()->postCloseFileSignal_) {
     source.actReg()->preCloseFileSignal_("", false);
  }

  InputSource::FileCloseSentry::FileCloseSentry(InputSource const& source, std::string const& lfn, bool usedFallback) :
     post_(source.actReg()->postCloseFileSignal_) {
     source.actReg()->preCloseFileSignal_(lfn, usedFallback);
  }

  InputSource::FileCloseSentry::~FileCloseSentry() {
     post_();
  }
}
