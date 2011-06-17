/** \class MuonProducer
 *  See header file.
 *
 *  $Date: 2011/06/15 09:44:53 $
 *  $Revision: 1.10 $
 *  \author R. Bellan - UCSB <riccardo.bellan@cern.ch>
 */

#include "RecoMuon/MuonIdentification/plugins/MuonProducer.h"

#include "RecoMuon/MuonIsolation/interface/MuPFIsoHelper.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/MuonReco/interface/Muon.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/MuonReco/interface/MuonTimeExtraMap.h"
#include "DataFormats/MuonReco/interface/MuonShower.h"
#include "DataFormats/MuonReco/interface/MuonCosmicCompatibility.h"
#include "DataFormats/MuonReco/interface/MuonToMuonMap.h"

#include "DataFormats/ParticleFlowCandidate/interface/PFCandidate.h"
#include "DataFormats/ParticleFlowCandidate/interface/PFCandidateFwd.h"

#include "DataFormats/Common/interface/ValueMap.h"
#include "DataFormats/RecoCandidate/interface/IsoDepositFwd.h"
#include "DataFormats/RecoCandidate/interface/IsoDeposit.h"

#include <boost/foreach.hpp>
#define foreach BOOST_FOREACH

#ifndef dout
#define dout if(debug_) std::cout
#endif

using std::endl;

typedef std::map<reco::MuonRef, reco::Candidate::LorentzVector> MuToPFMap;

namespace reco {
  typedef edm::ValueMap<reco::MuonShower> MuonShowerMap;
}


/// Constructor
MuonProducer::MuonProducer(const edm::ParameterSet& pSet):debug_(pSet.getUntrackedParameter<bool>("ActivateDebug",false)){

  setAlias(pSet.getParameter<std::string>("@module_label"));

  theMuonsCollectionLabel = pSet.getParameter<edm::InputTag>("InputMuons");
  thePFCandLabel = pSet.getParameter<edm::InputTag>("PFCandidates");


  // Variables to switch on-off the differnt parts
  fillSelectors_ =  pSet.getParameter<bool>("FillSelectorMaps");
  fillCosmicsIdMap_ =  pSet.getParameter<bool>("FillCosmicsIdMap");

  produces<reco::MuonCollection>();
  produces<reco::MuonTimeExtraMap>("combined");
  produces<reco::MuonTimeExtraMap>("dt");
  produces<reco::MuonTimeExtraMap>("csc");
  
  //  if (fillIsolation_ && writeIsoDeposits_){
    theTrackDepositName = pSet.getParameter<edm::InputTag>("TrackIsoDeposits");
    produces<reco::IsoDepositMap>(theTrackDepositName.label());
    theJetDepositName = pSet.getParameter<edm::InputTag>("JetIsoDeposits");
    produces<reco::IsoDepositMap>(theJetDepositName.label());
    theEcalDepositName = pSet.getParameter<edm::InputTag>("EcalIsoDeposits");
    produces<reco::IsoDepositMap>(theEcalDepositName.instance());
    theHcalDepositName = pSet.getParameter<edm::InputTag>("HcalIsoDeposits");
    produces<reco::IsoDepositMap>(theHcalDepositName.instance());
    theHoDepositName = pSet.getParameter<edm::InputTag>("HoIsoDeposits");
    produces<reco::IsoDepositMap>(theHoDepositName.instance());
    //  }
    
    if(fillSelectors_){
      theSelectorMapNames = pSet.getParameter<InputTags>("SelectorMaps");
      
      for(InputTags::const_iterator tag = theSelectorMapNames.begin(); tag != theSelectorMapNames.end(); ++tag)
	produces<edm::ValueMap<bool> >(tag->label());
    }

    theShowerMapName = pSet.getParameter<edm::InputTag>("ShowerInfoMap");
    produces<edm::ValueMap<reco::MuonShower> >(theShowerMapName.label());

    if(fillCosmicsIdMap_){
      theCosmicCompMapName = pSet.getParameter<edm::InputTag>("CosmicIdMap");
      produces<edm::ValueMap<reco::MuonCosmicCompatibility> >(theCosmicCompMapName.label());
      produces<edm::ValueMap<unsigned int> >(theCosmicCompMapName.label());
    }

    theMuToMuMapName = theMuonsCollectionLabel.label()+"2"+theAlias+"sMap";
    produces<edm::ValueMap<reco::MuonRef> >(theMuToMuMapName);


    thePFIsoHelper = new MuPFIsoHelper(pSet.getParameter<edm::ParameterSet>("PFIsolation"));

}

/// Destructor
MuonProducer::~MuonProducer(){ 
  if (thePFIsoHelper) delete thePFIsoHelper;
}


/// reconstruct muons
void MuonProducer::produce(edm::Event& event, const edm::EventSetup& eventSetup){

   const std::string metname = "Muon|RecoMuon|MuonIdentification|MuonProducer";

   // the muon collection, it will be loaded in the event
   std::auto_ptr<reco::MuonCollection> outputMuons(new reco::MuonCollection());
   reco::MuonRefProd outputMuonsRefProd = event.getRefBeforePut<reco::MuonCollection>();

   edm::Handle<reco::MuonCollection> inputMuons; 
   event.getByLabel(theMuonsCollectionLabel, inputMuons);
   edm::OrphanHandle<reco::MuonCollection> inputMuonsOH(inputMuons.product(), inputMuons.id());
   
   edm::Handle<reco::PFCandidateCollection> pfCandidates; 
   event.getByLabel(thePFCandLabel, pfCandidates);


   // fetch collections for PFIso
   thePFIsoHelper->beginEvent(event);

   
   // Fill timing information
   edm::Handle<reco::MuonTimeExtraMap> timeMapCmb;
   edm::Handle<reco::MuonTimeExtraMap> timeMapDT;
   edm::Handle<reco::MuonTimeExtraMap> timeMapCSC;
   
   int nMuons=inputMuons->size();

   std::vector<reco::MuonTimeExtra> dtTimeColl(nMuons);
   std::vector<reco::MuonTimeExtra> cscTimeColl(nMuons);
   std::vector<reco::MuonTimeExtra> combinedTimeColl(nMuons);


   event.getByLabel(theMuonsCollectionLabel.label(),"combined",timeMapCmb);
   event.getByLabel(theMuonsCollectionLabel.label(),"dt",timeMapDT);
   event.getByLabel(theMuonsCollectionLabel.label(),"csc",timeMapCSC);
   

   std::vector<reco::IsoDeposit> trackDepColl(nMuons);
   std::vector<reco::IsoDeposit> ecalDepColl(nMuons);
   std::vector<reco::IsoDeposit> hcalDepColl(nMuons);
   std::vector<reco::IsoDeposit> hoDepColl(nMuons);
   std::vector<reco::IsoDeposit> jetDepColl(nMuons);


   edm::Handle<reco::IsoDepositMap> trackIsoDepMap;
   edm::Handle<reco::IsoDepositMap> ecalIsoDepMap;
   edm::Handle<reco::IsoDepositMap> hcalIsoDepMap;
   edm::Handle<reco::IsoDepositMap> hoIsoDepMap;
   edm::Handle<reco::IsoDepositMap> jetIsoDepMap;


   event.getByLabel(theTrackDepositName,trackIsoDepMap);
   event.getByLabel(theEcalDepositName,ecalIsoDepMap);
   event.getByLabel(theHcalDepositName,hcalIsoDepMap);
   event.getByLabel(theHoDepositName,hoIsoDepMap);
   event.getByLabel(theJetDepositName,jetIsoDepMap);

   
   std::vector<edm::Handle<edm::ValueMap<bool> > >  selectorMaps(fillSelectors_ ? theSelectorMapNames.size() : 0); 
   std::vector<std::vector<bool> > selectorMapResults(fillSelectors_ ? theSelectorMapNames.size() : 0);
   if(fillSelectors_){
     unsigned int s=0;
     for(InputTags::const_iterator tag = theSelectorMapNames.begin(); tag != theSelectorMapNames.end(); ++tag, ++s){
       event.getByLabel(*tag,selectorMaps[s]);
       selectorMapResults[s].resize(nMuons);
     }
   }

   edm::Handle<reco::MuonShowerMap> showerInfoMap;
   event.getByLabel(theShowerMapName,showerInfoMap);

   std::vector<reco::MuonShower> showerInfoColl(nMuons);

   edm::Handle<edm::ValueMap<unsigned int> > cosmicIdMap;
   if(fillCosmicsIdMap_) event.getByLabel(theCosmicCompMapName,cosmicIdMap);
   std::vector<unsigned int> cosmicIdColl(fillCosmicsIdMap_ ? nMuons : 0);
   
   edm::Handle<edm::ValueMap<reco::MuonCosmicCompatibility> > cosmicCompMap;
   if(fillCosmicsIdMap_) event.getByLabel(theCosmicCompMapName,cosmicCompMap);
   std::vector<reco::MuonCosmicCompatibility> cosmicCompColl(fillCosmicsIdMap_ ? nMuons : 0);


   std::vector<reco::MuonRef> muonRefColl(nMuons);



   if(inputMuons->empty()) {
     edm::OrphanHandle<reco::MuonCollection> muonHandle = event.put(outputMuons);
     
     fillMuonMap<reco::MuonTimeExtra>(event, muonHandle, combinedTimeColl,"combined");
     fillMuonMap<reco::MuonTimeExtra>(event, muonHandle, dtTimeColl,"dt");
     fillMuonMap<reco::MuonTimeExtra>(event, muonHandle, cscTimeColl,"csc");
     
     fillMuonMap<reco::IsoDeposit>(event, muonHandle, trackDepColl, theTrackDepositName.label());
     fillMuonMap<reco::IsoDeposit>(event, muonHandle, jetDepColl,   theJetDepositName.label());
     fillMuonMap<reco::IsoDeposit>(event, muonHandle, ecalDepColl,  theEcalDepositName.instance());
     fillMuonMap<reco::IsoDeposit>(event, muonHandle, hcalDepColl,  theHcalDepositName.instance());
     fillMuonMap<reco::IsoDeposit>(event, muonHandle, hoDepColl,    theHoDepositName.instance());
     

     if(fillSelectors_){
       unsigned int s = 0;
       for(InputTags::const_iterator tag = theSelectorMapNames.begin(); 
	   tag != theSelectorMapNames.end(); ++tag, ++s){
	 fillMuonMap<bool>(event, muonHandle, selectorMapResults[s], tag->label());
       }
     }

     fillMuonMap<reco::MuonShower>(event, muonHandle, showerInfoColl, theShowerMapName.label());

     if(fillCosmicsIdMap_){
       fillMuonMap<unsigned int>(event, muonHandle, cosmicIdColl, theCosmicCompMapName.label());
       fillMuonMap<reco::MuonCosmicCompatibility>(event, muonHandle, cosmicCompColl, theCosmicCompMapName.label());
     }

     fillMuonMap<reco::MuonRef>(event,inputMuonsOH, muonRefColl, theMuToMuMapName);

     return;
   }
   
   // FIXME: add the option to swith off the Muon-PF "info association".
   

   MuToPFMap muToPFMap;

   dout << "Number of PFCandidates: " << pfCandidates->size() << endl;
   foreach(const reco::PFCandidate &pfCand, *pfCandidates)
     if(abs(pfCand.pdgId()) == 13){
       muToPFMap[pfCand.muonRef()] = pfCand.p4();     
       dout << "MuonRef: " << pfCand.muonRef().id() << " " << pfCand.muonRef().key() << " PF p4: " << pfCand.p4() << endl;
     }


   dout << "Number of PFMuons: " << muToPFMap.size() << endl;
   dout << "Number of Muons in the original collection: " << inputMuons->size() << endl;


   reco::MuonRef::key_type muIndex = 0;
   unsigned int i = 0;
   foreach(const reco::Muon &inMuon, *inputMuons){
     
     reco::MuonRef muRef(inputMuons, muIndex);
     muonRefColl[i] = reco::MuonRef(outputMuonsRefProd, muIndex++);

     // Copy the muon 
     reco::Muon outMuon = inMuon;
     
     // search for the corresponding pf candidate
     MuToPFMap::iterator iter =  muToPFMap.find(muRef);
     if(iter != muToPFMap.end()){
       outMuon.setPFP4(iter->second);
       muToPFMap.erase(iter);
       dout << "MuonRef: " << muRef.id() << " " << muRef.key() 
	    << " Is it PF? " << outMuon.isPFMuon() 
	 // << " PF p4: " << outMuon.isPFMuon() ? outMuon.pfP4() : 0 << endl;
	    << " PF p4: " << outMuon.pfP4() << endl;
     }
     
     
     dout << "MuonRef: " << muRef.id() << " " << muRef.key() 
	  << " Is it PF? " << outMuon.isPFMuon() << endl;
     
     dout << "GLB "  << outMuon.isGlobalMuon()
	  << " TM "  << outMuon.isTrackerMuon()
	  << " STA " << outMuon.isStandAloneMuon() 
	  << " p4 "  << outMuon.p4() << endl;

     // Add PF isolation info
     thePFIsoHelper->embedPFIsolation(outMuon,muRef);


     
     // Fill timing information   
     
     combinedTimeColl[i] = (*timeMapCmb)[muRef];
     dtTimeColl[i] = (*timeMapDT)[muRef];
     cscTimeColl[i] = (*timeMapCSC)[muRef];
          


     // FIXME: Fill iso quantities too!
     
     trackDepColl[i] = (*trackIsoDepMap)[muRef];
     ecalDepColl[i]  = (*ecalIsoDepMap)[muRef];
     hcalDepColl[i]  = (*hcalIsoDepMap)[muRef];
     hoDepColl[i]    = (*hoIsoDepMap)[muRef];
     jetDepColl[i]   = (*jetIsoDepMap)[muRef];;

     if(fillSelectors_){
       unsigned int s = 0;
       for(InputTags::const_iterator tag = theSelectorMapNames.begin(); 
	   tag != theSelectorMapNames.end(); ++tag, ++s)
	 selectorMapResults[s][i] = (*selectorMaps[s])[muRef];
     }

     // Fill the Showering Info
     showerInfoColl[i] = (*showerInfoMap)[muRef];

     if(fillCosmicsIdMap_){
       cosmicIdColl[i] = (*cosmicIdMap)[muRef];
       cosmicCompColl[i] = (*cosmicCompMap)[muRef];
     }

     outputMuons->push_back(outMuon); 
     ++i;
   }

   dout << "Number of Muons in the new muon collection: " << outputMuons->size() << endl;
   edm::OrphanHandle<reco::MuonCollection> muonHandle = event.put(outputMuons);

   fillMuonMap<reco::MuonTimeExtra>(event, muonHandle, combinedTimeColl,"combined");
   fillMuonMap<reco::MuonTimeExtra>(event, muonHandle, dtTimeColl,"dt");
   fillMuonMap<reco::MuonTimeExtra>(event, muonHandle, cscTimeColl,"csc");


   fillMuonMap<reco::IsoDeposit>(event, muonHandle, trackDepColl, theTrackDepositName.label());
   fillMuonMap<reco::IsoDeposit>(event, muonHandle, jetDepColl,   theJetDepositName.label());
   fillMuonMap<reco::IsoDeposit>(event, muonHandle, ecalDepColl,  theEcalDepositName.instance());
   fillMuonMap<reco::IsoDeposit>(event, muonHandle, hcalDepColl,  theHcalDepositName.instance());
   fillMuonMap<reco::IsoDeposit>(event, muonHandle, hoDepColl,    theHoDepositName.instance());

   if(fillSelectors_){
     unsigned int s = 0;
     for(InputTags::const_iterator tag = theSelectorMapNames.begin(); 
	 tag != theSelectorMapNames.end(); ++tag, ++s)
       fillMuonMap<bool>(event, muonHandle, selectorMapResults[s], tag->label());
   }

   fillMuonMap<reco::MuonShower>(event, muonHandle, showerInfoColl, theShowerMapName.label());

   if(fillCosmicsIdMap_){
     fillMuonMap<unsigned int>(event, muonHandle, cosmicIdColl, theCosmicCompMapName.label());
     fillMuonMap<reco::MuonCosmicCompatibility>(event, muonHandle, cosmicCompColl, theCosmicCompMapName.label());
   }

   fillMuonMap<reco::MuonRef>(event,inputMuonsOH, muonRefColl, theMuToMuMapName);
   


}

template<typename TYPE>
void MuonProducer::fillMuonMap(edm::Event& event,
			       const edm::OrphanHandle<reco::MuonCollection>& muonHandle,
			       const std::vector<TYPE>& muonExtra,
			       const std::string& label){
 
  typedef typename edm::ValueMap<TYPE>::Filler FILLER; 

  std::auto_ptr<edm::ValueMap<TYPE> > muonMap(new edm::ValueMap<TYPE>());
  if(!muonExtra.empty()){
    FILLER filler(*muonMap);
    filler.insert(muonHandle, muonExtra.begin(), muonExtra.end());
    filler.fill();
  }
  event.put(muonMap,label);
}


