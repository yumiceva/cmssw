// -*- C++ -*-

//
// Package:     TFWLiteSelector
// Class  :     TFWLiteSelectorBasic
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Tue Jun 27 17:58:10 EDT 2006
//

// user include files
#include "FWCore/TFWLiteSelector/interface/TFWLiteSelectorBasic.h"

#include "DataFormats/Common/interface/RefCoreStreamer.h"
#include "DataFormats/Common/interface/WrapperOwningHolder.h"
#include "DataFormats/Provenance/interface/BranchDescription.h"
#include "DataFormats/Provenance/interface/BranchIDListHelper.h"
#include "DataFormats/Provenance/interface/BranchIDListRegistry.h"
#include "DataFormats/Provenance/interface/BranchListIndex.h"
#include "DataFormats/Provenance/interface/BranchMapper.h"
#include "DataFormats/Provenance/interface/BranchType.h"
#include "DataFormats/Provenance/interface/EventAuxiliary.h"
#include "DataFormats/Provenance/interface/EventEntryDescription.h" // kludge to allow compilation
#include "DataFormats/Provenance/interface/EventSelectionID.h"
#include "DataFormats/Provenance/interface/FileFormatVersion.h"
#include "DataFormats/Provenance/interface/LuminosityBlockAuxiliary.h"
#include "DataFormats/Provenance/interface/ModuleDescription.h"
#include "DataFormats/Provenance/interface/ParameterSetBlob.h"
#include "DataFormats/Provenance/interface/ProcessConfiguration.h"
#include "DataFormats/Provenance/interface/ProcessHistory.h"
#include "DataFormats/Provenance/interface/ProcessHistoryRegistry.h"
#include "DataFormats/Provenance/interface/ProductRegistry.h"
#include "DataFormats/Provenance/interface/RunAuxiliary.h"
#include "FWCore/Framework/interface/DelayedReader.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventPrincipal.h"
#include "FWCore/Framework/interface/LuminosityBlockPrincipal.h"
#include "FWCore/Framework/interface/RunPrincipal.h"
#include "FWCore/ParameterSet/interface/FillProductRegistryTransients.h"
#include "FWCore/ParameterSet/interface/Registry.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "FWCore/Utilities/interface/FriendlyName.h"
#include "FWCore/Utilities/interface/WrappedClassName.h"

// system include files
#include "TBranch.h"
#include "TChain.h"
#include "TFile.h"
#include "TTree.h"
#include "Reflex/Type.h"

namespace edm {
  namespace root {
    class FWLiteDelayedReader : public DelayedReader {
     public:
      FWLiteDelayedReader() : entry_(-1), eventTree_(0), reg_() {}
      void setEntry(Long64_t iEntry) { entry_ = iEntry; }
      void setTree(TTree* iTree) {eventTree_ = iTree;}
      void set(boost::shared_ptr<ProductRegistry const> iReg) { reg_ = iReg;}
     private:
      virtual WrapperOwningHolder getProduct_(BranchKey const& k, WrapperInterfaceBase const* interface, EDProductGetter const* ep) const;
      virtual std::auto_ptr<EventEntryDescription> getProvenance_(BranchKey const&) const {
        return std::auto_ptr<EventEntryDescription>();
      }
      virtual void mergeReaders_(DelayedReader*) {}
      virtual void reset_() {}
      Long64_t entry_;
      TTree* eventTree_;
      boost::shared_ptr<ProductRegistry const>(reg_);
    };

    WrapperOwningHolder
    FWLiteDelayedReader::getProduct_(BranchKey const& k, WrapperInterfaceBase const* /*interface*/, EDProductGetter const* /*ep*/) const {
      ProductRegistry::ProductList::const_iterator itFind= reg_->productList().find(k);
      if(itFind == reg_->productList().end()) {
        throw Exception(errors::ProductNotFound) << "could not find entry for product " << k;
      }
      BranchDescription const& bDesc = itFind->second;

      TBranch* branch= eventTree_->GetBranch(bDesc.branchName().c_str());
      if(0 == branch) {
        throw cms::Exception("MissingBranch")
        << "could not find branch named '" << bDesc.branchName() << "'"
        << "\n Perhaps the data being requested was not saved in this file?";
      }
      //find the class type
      std::string const fullName = wrappedClassName(bDesc.className());
      Reflex::Type classType = Reflex::Type::ByName(fullName);
      if(classType == Reflex::Type()) {
        throw cms::Exception("MissingDictionary")
        << "could not find dictionary for type '" << fullName << "'"
        << "\n Please make sure all the necessary libraries are available.";
      }

      //use reflex to create an instance of it
      Reflex::Object wrapperObj = classType.Construct();
      if(0 == wrapperObj.Address()) {
        throw cms::Exception("FailedToCreate") << "could not create an instance of '" << fullName << "'";
      }
      void* address  = wrapperObj.Address();
      branch->SetAddress(&address);


      branch->GetEntry(entry_);
      return WrapperOwningHolder(address, bDesc.getInterface());
    }

    struct TFWLiteSelectorMembers {
      TFWLiteSelectorMembers() :
      tree_(0),
      reg_(new ProductRegistry()),
      processNames_(),
      reader_(new FWLiteDelayedReader),
      prov_(),
      pointerToBranchBuffer_(),
      mapper_(new edm::BranchMapper) {
        reader_->set(reg_);
      }
      void setTree(TTree* iTree) {
        tree_ = iTree;
        reader_->setTree(iTree);
      }
      TTree* tree_;
      boost::shared_ptr<ProductRegistry> reg_;
      ProcessHistory processNames_;
      boost::shared_ptr<FWLiteDelayedReader> reader_;
      std::vector<EventEntryDescription> prov_;
      std::vector<EventEntryDescription*> pointerToBranchBuffer_;
      FileFormatVersion fileFormatVersion_;

      boost::shared_ptr<edm::BranchMapper> mapper_;
      edm::ProcessConfiguration pc_;
      boost::shared_ptr<edm::EventPrincipal> ep_;
      edm::ModuleDescription md_;
    };
  }
}


//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
TFWLiteSelectorBasic::TFWLiteSelectorBasic() : m_(new edm::root::TFWLiteSelectorMembers),
                                     everythingOK_(false) {
}

// TFWLiteSelectorBasic::TFWLiteSelectorBasic(TFWLiteSelectorBasic const& rhs)
// {
//    // do actual copying here;
// }

TFWLiteSelectorBasic::~TFWLiteSelectorBasic() {
}

//
// assignment operators
//
// TFWLiteSelectorBasic const& TFWLiteSelectorBasic::operator=(TFWLiteSelectorBasic const& rhs)
// {
//   //An exception safe implementation is
//   TFWLiteSelectorBasic temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
void
TFWLiteSelectorBasic::Begin(TTree * iTree) {
  Init(iTree);
  begin(fInput);
}

void
TFWLiteSelectorBasic::SlaveBegin(TTree *iTree) {
  Init(iTree);
  preProcessing(fInput, *fOutput);
}

void
TFWLiteSelectorBasic::Init(TTree *iTree) {
  if(iTree == 0) return;
  m_->setTree(iTree);
}


Bool_t
TFWLiteSelectorBasic::Notify() {
  //std::cout << "Notify start" << std::endl;
  //we have switched to a new file
  //get new file from Tree
  if(0 == m_->tree_) {
    std::cout << "No tree" << std::endl;
    return kFALSE;
  }
  TFile* file = m_->tree_->GetCurrentFile();
  if(0 == file) {
     //When in Rome, do as the Romans
     TChain* chain = dynamic_cast<TChain*>(m_->tree_);
     if(0 == chain) {
        std::cout << "No file" << std::endl;
        return kFALSE;
     }
     file = chain->GetFile();
     if(0 == file) {
        std::cout << "No file" << std::endl;
        return kFALSE;
     }
  }
  setupNewFile(*file);
  return everythingOK_ ? kTRUE: kFALSE;
}

namespace  {
   struct Operate {
      Operate(edm::EDProductGetter const* iGetter): old_(setRefCoreStreamer(iGetter)) {
      }

      ~Operate() {setRefCoreStreamer(old_);}
   private:
      edm::EDProductGetter const* old_;
   };
}

Bool_t
TFWLiteSelectorBasic::Process(Long64_t iEntry) {
   //std::cout << "Process start" << std::endl;
   if(everythingOK_) {
      std::auto_ptr<edm::EventAuxiliary> eaux(new edm::EventAuxiliary());
      edm::EventAuxiliary& aux = *eaux;
      edm::EventAuxiliary* pAux= eaux.get();
      TBranch* branch = m_->tree_->GetBranch(edm::BranchTypeToAuxiliaryBranchName(edm::InEvent).c_str());

      branch->SetAddress(&pAux);
      branch->GetEntry(iEntry);

//NEW      m_->processNames_ = aux.processHistory();

//      std::cout << "ProcessNames\n";
//      for(edm::ProcessNameList::const_iterator itName = m_->processNames_.begin(),
//        itNameEnd = m_->processNames_.end();
//          itName != itNameEnd;
//          ++itName) {
//         std::cout << "  " << *itName << std::endl;
//     }

      boost::shared_ptr<edm::EventSelectionIDVector> eventSelectionIDs_(new edm::EventSelectionIDVector);
      edm::EventSelectionIDVector* pEventSelectionIDVector = eventSelectionIDs_.get();
      TBranch* eventSelectionsBranch = m_->tree_->GetBranch(edm::poolNames::eventSelectionsBranchName().c_str());
      if(!eventSelectionsBranch) {
        throw edm::Exception(edm::errors::FatalRootError)
            << "Failed to find event Selections branch in event tree";
      }
      eventSelectionsBranch->SetAddress(&pEventSelectionIDVector);
      eventSelectionsBranch->GetEntry(iEntry);

      boost::shared_ptr<edm::BranchListIndexes> branchListIndexes_(new edm::BranchListIndexes);
      edm::BranchListIndexes* pBranchListIndexes = branchListIndexes_.get();
      TBranch* branchListIndexBranch = m_->tree_->GetBranch(edm::poolNames::branchListIndexesBranchName().c_str());
      if(!branchListIndexBranch) {
        throw edm::Exception(edm::errors::FatalRootError)
            << "Failed to find branch list index branch in event tree";
      }
      branchListIndexBranch->SetAddress(&pBranchListIndexes);
      branchListIndexBranch->GetEntry(iEntry);
      edm::BranchIDListHelper::fixBranchListIndexes(*branchListIndexes_);

      try {
         m_->reader_->setEntry(iEntry);
         boost::shared_ptr<edm::RunAuxiliary> runAux(new edm::RunAuxiliary(aux.run(), aux.time(), aux.time()));
         boost::shared_ptr<edm::RunPrincipal> rp(new edm::RunPrincipal(runAux, m_->reg_, m_->pc_));
         boost::shared_ptr<edm::LuminosityBlockAuxiliary> lumiAux(
                new edm::LuminosityBlockAuxiliary(rp->run(), 1, aux.time(), aux.time()));
         boost::shared_ptr<edm::LuminosityBlockPrincipal>lbp(
            new edm::LuminosityBlockPrincipal(lumiAux, m_->reg_, m_->pc_, rp));
         m_->ep_->fillEventPrincipal(*eaux, lbp, eventSelectionIDs_, branchListIndexes_, m_->mapper_, m_->reader_.get());
         m_->processNames_ = m_->ep_->processHistory();

         edm::Event event(*m_->ep_, m_->md_);

         //Make the event principal accessible to edm::Ref's
         Operate sentry(m_->ep_->prodGetter());
         process(event);
      } catch(std::exception const& iEx) {
         std::cout << "While processing entry " << iEntry << " the following exception was caught \n"
                   << iEx.what() << std::endl;
      } catch(...) {
         std::cout << "While processing entry " << iEntry << " an unknown exception was caught" << std::endl;
      }
  }
  return everythingOK_ ? kTRUE: kFALSE;
}

void
TFWLiteSelectorBasic::SlaveTerminate() {
  postProcessing(*fOutput);
}

void
TFWLiteSelectorBasic::Terminate() {
  terminate(*fOutput);
}

void
TFWLiteSelectorBasic::setupNewFile(TFile& iFile) {
  //look up meta-data
  //get product registry

  //std::vector<edm::EventProcessHistoryID> eventProcessHistoryIDs_;
  TTree* metaDataTree = dynamic_cast<TTree*>(iFile.Get(edm::poolNames::metaDataTreeName().c_str()));
  if(!metaDataTree) {
    std::cout << "could not find TTree " << edm::poolNames::metaDataTreeName() << std::endl;
    everythingOK_ = false;
    return;
  }
  edm::FileFormatVersion* fftPtr = &(m_->fileFormatVersion_);
  if(metaDataTree->FindBranch(edm::poolNames::fileFormatVersionBranchName().c_str()) != 0) {
    metaDataTree->SetBranchAddress(edm::poolNames::fileFormatVersionBranchName().c_str(), &fftPtr);
    }


  edm::ProductRegistry* pReg = &(*m_->reg_);
  metaDataTree->SetBranchAddress(edm::poolNames::productDescriptionBranchName().c_str(), &(pReg));

  m_->reg_->setFrozen();

  typedef std::map<edm::ParameterSetID, edm::ParameterSetBlob> PsetMap;
  PsetMap psetMap;
  PsetMap *psetMapPtr = &psetMap;
  if(metaDataTree->FindBranch(edm::poolNames::parameterSetMapBranchName().c_str()) != 0) {
    metaDataTree->SetBranchAddress(edm::poolNames::parameterSetMapBranchName().c_str(), &psetMapPtr);
  } else {
    TTree* psetTree = dynamic_cast<TTree *>(iFile.Get(edm::poolNames::parameterSetsTreeName().c_str()));
    if(0 == psetTree) {
      throw edm::Exception(edm::errors::FileReadError) << "Could not find tree " << edm::poolNames::parameterSetsTreeName()
      << " in the input file.\n";
    }
    typedef std::pair<edm::ParameterSetID, edm::ParameterSetBlob> IdToBlobs;
    IdToBlobs idToBlob;
    IdToBlobs* pIdToBlob = &idToBlob;
    psetTree->SetBranchAddress(edm::poolNames::idToParameterSetBlobsBranchName().c_str(), &pIdToBlob);
    for(long long i = 0; i != psetTree->GetEntries(); ++i) {
      psetTree->GetEntry(i);
      psetMap.insert(idToBlob);
    }
  }

  edm::ProcessHistoryRegistry::vector_type pHistVector;
  edm::ProcessHistoryRegistry::vector_type *pHistVectorPtr = &pHistVector;
  if(metaDataTree->FindBranch(edm::poolNames::processHistoryBranchName().c_str()) != 0) {
     metaDataTree->SetBranchAddress(edm::poolNames::processHistoryBranchName().c_str(), &pHistVectorPtr);
  }


  edm::ProcessConfigurationVector procConfigVector;
  edm::ProcessConfigurationVector* procConfigVectorPtr = &procConfigVector;
  if(metaDataTree->FindBranch(edm::poolNames::processConfigurationBranchName().c_str()) != 0) {
     metaDataTree->SetBranchAddress(edm::poolNames::processConfigurationBranchName().c_str(), &procConfigVectorPtr);
  }

  std::auto_ptr<edm::BranchIDListRegistry::collection_type> branchIDListsAPtr(new edm::BranchIDListRegistry::collection_type);
  edm::BranchIDListRegistry::collection_type *branchIDListsPtr = branchIDListsAPtr.get();
  if(metaDataTree->FindBranch(edm::poolNames::branchIDListBranchName().c_str()) != 0) {
    metaDataTree->SetBranchAddress(edm::poolNames::branchIDListBranchName().c_str(), &branchIDListsPtr);
  }

  metaDataTree->GetEntry(0);

  // Merge into the registries. For now, we do NOT merge the product registry.
  edm::pset::Registry& psetRegistry = *edm::pset::Registry::instance();
  for(PsetMap::const_iterator i = psetMap.begin(), iEnd = psetMap.end();
      i != iEnd; ++i) {
    edm::ParameterSet pset(i->second.pset());
    pset.setID(i->first);
    psetRegistry.insertMapped(pset);
  }

  edm::ProcessHistoryRegistry::instance()->insertCollection(pHistVector);
  edm::ProcessConfigurationRegistry::instance()->insertCollection(procConfigVector);

  m_->pointerToBranchBuffer_.erase(m_->pointerToBranchBuffer_.begin(),
                                   m_->pointerToBranchBuffer_.end());

  fillProductRegistryTransients(procConfigVector, *m_->reg_);
  std::auto_ptr<edm::ProductRegistry> newReg(new edm::ProductRegistry());

  edm::ProductRegistry::ProductList const& prodList = m_->reg_->productList();
  {
     for(edm::ProductRegistry::ProductList::const_iterator it = prodList.begin(), itEnd = prodList.end();
            it != itEnd; ++it) {
         edm::BranchDescription const& prod = it->second;
         //std::cout << "productname = " << it->second << " end " << std::endl;
         std::string newFriendlyName = edm::friendlyname::friendlyName(prod.className());
         if(newFriendlyName == prod.friendlyClassName()) {
           newReg->copyProduct(prod);
         } else {
           if(m_->fileFormatVersion_.splitProductIDs()) {
             throw edm::Exception(edm::errors::UnimplementedFeature)
               << "Cannot change friendly class name algorithm without more development work\n"
               << "to update BranchIDLists.  Contact the framework group.\n";
           }
           edm::BranchDescription newBD(prod);
           newBD.updateFriendlyClassName();
           newReg->copyProduct(newBD);
           // Need to call init to get old branch name.
           prod.init();
         }
       }

    newReg->setFrozen();
    m_->reg_.reset(newReg.release());
  }

  edm::ProductRegistry::ProductList const& prodList2 = m_->reg_->productList();
  std::vector<edm::EventEntryDescription> temp(prodList2.size(), edm::EventEntryDescription());
  m_->prov_.swap(temp);
  std::vector<edm::EventEntryDescription>::iterator itB = m_->prov_.begin();
  m_->pointerToBranchBuffer_.reserve(prodList2.size());

  for(edm::ProductRegistry::ProductList::const_iterator it = prodList2.begin(), itEnd = prodList2.end();
       it != itEnd; ++it, ++itB) {
    edm::BranchDescription const& prod = it->second;
    if(prod.branchType() == edm::InEvent) {
      prod.init();
      //NEED to do this and check to see if branch exists
      if(m_->tree_->GetBranch(prod.branchName().c_str()) == 0) {
        prod.setDropped();
      }

      //std::cout << "id " << it->first << " branch " << it->second << std::endl;
      //m_->pointerToBranchBuffer_.push_back(&(*itB));
      //void* tmp = &(m_->pointerToBranchBuffer_.back());
      //edm::EventEntryDescription* tmp = &(*itB);
      //CDJ need to fix provenance and be backwards compatible, for now just don't read the branch
      //m_->metaTree_->SetBranchAddress(prod.branchName().c_str(), tmp);
    }
  }
  edm::BranchIDListHelper::updateFromInput(*branchIDListsAPtr, "");
  m_->ep_.reset(new edm::EventPrincipal(m_->reg_, m_->pc_));
  everythingOK_ = true;
}

//
// const member functions
//

//
// static member functions
//
