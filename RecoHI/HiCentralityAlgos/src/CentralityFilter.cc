// -*- C++ -*-
//
// Package:    CentralityFilter
// Class:      CentralityFilter
// 
/**\class CentralityFilter CentralityFilter.cc CmsHi/CentralityFilter/src/CentralityFilter.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Yetkin Yilmaz,32 4-A08,+41227673039,
//         Created:  Tue Jun 29 12:19:49 CEST 2010
// $Id: CentralityFilter.cc,v 1.2 2010/06/29 11:48:35 yilmaz Exp $
//
//


// system include files
#include <memory>
#include <vector>
#include <string>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDFilter.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "DataFormats/HeavyIonEvent/interface/Centrality.h"


//
// class declaration
//

class CentralityFilter : public edm::EDFilter {
   public:
      explicit CentralityFilter(const edm::ParameterSet&);
      ~CentralityFilter();

   private:
      virtual void beginJob() ;
      virtual bool filter(edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;
      
      // ----------member data ---------------------------
  const CentralityBins * cbins_;
  std::vector<int> selectedBins_;
  std::string centralityBase_;

  edm::InputTag src_;
};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
CentralityFilter::CentralityFilter(const edm::ParameterSet& iConfig) :
  cbins_(0),
  selectedBins_(iConfig.getParameter<std::vector<int> >("selectedBins")),
  centralityBase_(iConfig.getParameter<std::string>("centralityBase")),
  src_(iConfig.getParameter<edm::InputTag>("src"))
{
   //now do what ever initialization is needed

}


CentralityFilter::~CentralityFilter()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called on each new Event  ------------
bool
CentralityFilter::filter(edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  bool result = false;

   using namespace edm;
   if(!cbins_) cbins_ = getCentralityBinsFromDB(iSetup);

   edm::Handle<reco::Centrality> cent;
   iEvent.getByLabel(src_,cent);

   double hf = cent->EtHFhitSum();
   double hftp = cent->EtHFtowerSumPlus();
   double hftm = cent->EtHFtowerSumMinus();
   double eb = cent->EtEBSum();
   double ee = cent->EtEESum();

   int bin = -1;

   if(centralityBase_.compare("HF") == 0){
     bin = cbins_->getBin(hf);
   }else if(centralityBase_.compare("EE")){
     bin = cbins_->getBin(ee);
   }else{
     throw cms::Exception("BadConfig") << "Centrality for "<<centralityBase_<<" not supported.";
   }
   
   for(unsigned int i = 0; i < selectedBins_.size(); ++i){
     if(bin == selectedBins_[i]) result = true;
   }

   return result;
}

// ------------ method called once each job just before starting event loop  ------------
void 
CentralityFilter::beginJob()
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
CentralityFilter::endJob() {
}

//define this as a plug-in
DEFINE_FWK_MODULE(CentralityFilter);
