#ifndef IOPool_Input_RootInputFileSequence_h
#define IOPool_Input_RootInputFileSequence_h

/*----------------------------------------------------------------------

RootInputFileSequence: This is an InputSource

----------------------------------------------------------------------*/

#include "InputType.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ProductSelectorRules.h"
#include "FWCore/Framework/interface/ProcessingController.h"
#include "FWCore/Sources/interface/EventSkipperByID.h"
#include "FWCore/Sources/interface/VectorInputSource.h"
#include "DataFormats/Provenance/interface/BranchDescription.h"
#include "DataFormats/Provenance/interface/ProcessHistoryID.h"
#include "DataFormats/Provenance/interface/IndexIntoFile.h"

#include <memory>
#include <string>
#include <vector>

namespace CLHEP {
  class RandFlat;
}

namespace edm {

  class DuplicateChecker;
  class FileCatalogItem;
  class InputFileCatalog;
  class ParameterSetDescription;
  class PoolSource;
  class RootFile;

  class RootInputFileSequence {
  public:
    explicit RootInputFileSequence(ParameterSet const& pset, PoolSource const& input, InputFileCatalog const& catalog, InputType::InputType inputType);
    virtual ~RootInputFileSequence();

    RootInputFileSequence(RootInputFileSequence const&) = delete; // Disallow copying and moving
    RootInputFileSequence& operator=(RootInputFileSequence const&) = delete; // Disallow copying and moving

    typedef boost::shared_ptr<RootFile> RootFileSharedPtr;
    EventPrincipal* readEvent(EventPrincipal& cache);
    boost::shared_ptr<LuminosityBlockAuxiliary> readLuminosityBlockAuxiliary_();
    boost::shared_ptr<LuminosityBlockPrincipal> readLuminosityBlock_(boost::shared_ptr<LuminosityBlockPrincipal> lumiPrincipal);
    boost::shared_ptr<RunAuxiliary> readRunAuxiliary_();
    boost::shared_ptr<RunPrincipal> readRun_(boost::shared_ptr<RunPrincipal> runPrincipal);
    boost::shared_ptr<FileBlock> readFile_();
    void closeFile_();
    void endJob();
    InputSource::ItemType getNextItemType();
    bool skipEvents(int offset);
    bool goToEvent(EventID const& eventID);
    bool skipToItem(RunNumber_t run, LuminosityBlockNumber_t lumi, EventNumber_t event, bool currentFileFirst = true);
    bool skipToItemInNewFile(RunNumber_t run, LuminosityBlockNumber_t lumi, EventNumber_t event);
    void rewind_();
    EventPrincipal* readOneRandom();
    EventPrincipal* readOneRandomWithID(LuminosityBlockID const& id);
    EventPrincipal* readOneSequential();
    EventPrincipal* readOneSequentialWithID(LuminosityBlockID const& id);
    EventPrincipal* readOneSpecified(EventID const& id);

    void dropUnwantedBranches_(std::vector<std::string> const& wantedBranches);
    boost::shared_ptr<ProductRegistry const> fileProductRegistry() const;
    boost::shared_ptr<BranchIDListHelper const> fileBranchIDListHelper() const;
    static void fillDescription(ParameterSetDescription & desc);
    ProcessingController::ForwardState forwardState() const;
    ProcessingController::ReverseState reverseState() const;
  private:
    void initFile(bool skipBadFiles);
    bool nextFile();
    bool previousFile();
    void rewindFile();
    std::vector<FileCatalogItem> const& fileCatalogItems() const;

    boost::shared_ptr<ProductRegistry const> productRegistry() const;
    ProcessConfiguration const& processConfiguration() const;
    ProductRegistry & productRegistryUpdate() const;
    int remainingEvents() const;
    int remainingLuminosityBlocks() const;

    PoolSource const& input_;
    InputType::InputType inputType_;
    InputFileCatalog const& catalog_;
    bool firstFile_;
    std::string lfn_;
    std::vector<FileCatalogItem>::const_iterator fileIterBegin_;
    std::vector<FileCatalogItem>::const_iterator fileIterEnd_;
    std::vector<FileCatalogItem>::const_iterator fileIter_;
    std::vector<FileCatalogItem>::const_iterator fileIterLastOpened_;
    RootFileSharedPtr rootFile_;
    BranchDescription::MatchMode parametersMustMatch_;
    BranchDescription::MatchMode branchesMustMatch_;

    std::unique_ptr<CLHEP::RandFlat> flatDistribution_;
    std::vector<boost::shared_ptr<IndexIntoFile> > indexesIntoFiles_;
    std::vector<ProcessHistoryID> orderedProcessHistoryIDs_;

    boost::shared_ptr<EventSkipperByID> eventSkipperByID_;
    int eventsRemainingInFile_;
    int initialNumberOfEventsToSkip_;
    bool noEventSort_;
    bool skipBadFiles_;
    unsigned int treeCacheSize_;
    int const treeMaxVirtualSize_;
    RunNumber_t setRun_;
    ProductSelectorRules productSelectorRules_;
    boost::shared_ptr<DuplicateChecker> duplicateChecker_;
    bool dropDescendants_;
    bool labelRawDataLikeMC_;
    bool usingGoToEvent_;
    bool enablePrefetching_;
    bool usedFallback_;
  }; // class RootInputFileSequence
}
#endif
