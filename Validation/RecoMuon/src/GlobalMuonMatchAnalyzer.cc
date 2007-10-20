/**
 * Class: GlobalMuonMatchAnalyzer
 *
 *
 * $Date: $
 * $Revision: $
 *
 * Authors :
 * \author Adam Everett - Purdue University
 *
 */

#include "Validation/RecoMuon/src/GlobalMuonMatchAnalyzer.h"

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "SimTracker/Records/interface/TrackAssociatorRecord.h"
#include "SimTracker/TrackAssociation/interface/TrackAssociatorBase.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"

#include "SimDataFormats/TrackingAnalysis/interface/TrackingParticle.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/MuonReco/interface/MuonTrackLinks.h"
#include "DataFormats/MuonReco/interface/MuonFwd.h"

#include "DQMServices/Core/interface/DaqMonitorBEInterface.h"
#include "DQMServices/Daemon/interface/MonitorDaemon.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include <TH2.h>


GlobalMuonMatchAnalyzer::GlobalMuonMatchAnalyzer(const edm::ParameterSet& iConfig)

{
  //now do what ever initialization is needed
  tkAssociatorName_ = iConfig.getUntrackedParameter<std::string>("tkAssociator");
  muAssociatorName_ = iConfig.getUntrackedParameter<std::string>("muAssociator");

  tpName_ = iConfig.getUntrackedParameter<edm::InputTag>("tpLabel");
  tkName_ = iConfig.getUntrackedParameter<edm::InputTag>("tkLabel");
  staName_ = iConfig.getUntrackedParameter<edm::InputTag>("muLabel");
  glbName_ = iConfig.getUntrackedParameter<edm::InputTag>("glbLabel");

  out = iConfig.getUntrackedParameter<std::string>("out");
  dbe_ = edm::Service<DaqMonitorBEInterface>().operator->();
}


GlobalMuonMatchAnalyzer::~GlobalMuonMatchAnalyzer()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to for each event  ------------
void
GlobalMuonMatchAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;

   Handle<TrackingParticleCollection> tpHandle;
   iEvent.getByLabel(tpName_,tpHandle);
   const TrackingParticleCollection tpColl = *(tpHandle.product());

   Handle<reco::MuonTrackLinksCollection> muHandle;
   iEvent.getByLabel(glbName_,muHandle);
   const reco::MuonTrackLinksCollection muColl = *(muHandle.product());

   Handle<reco::TrackCollection> staHandle;
   iEvent.getByLabel(staName_,staHandle);
   const reco::TrackCollection staColl = *(staHandle.product());

   Handle<reco::TrackCollection> glbHandle;
   iEvent.getByLabel(glbName_,glbHandle);
   const reco::TrackCollection glbColl = *(glbHandle.product());

   Handle<reco::TrackCollection> tkHandle;
   iEvent.getByLabel(tkName_,tkHandle);
   const reco::TrackCollection mtkColl = *(tkHandle.product());

   reco::RecoToSimCollection tkrecoToSimCollection;
   reco::SimToRecoCollection tksimToRecoCollection;
   tkrecoToSimCollection = tkAssociator_->associateRecoToSim(tkHandle,tpHandle,&iEvent);
   tksimToRecoCollection = tkAssociator_->associateSimToReco(tkHandle,tpHandle,&iEvent);

   reco::RecoToSimCollection starecoToSimCollection;
   reco::SimToRecoCollection stasimToRecoCollection;
   starecoToSimCollection = muAssociator_->associateRecoToSim(staHandle,tpHandle,&iEvent);
   stasimToRecoCollection = muAssociator_->associateSimToReco(staHandle,tpHandle,&iEvent);

   reco::RecoToSimCollection glbrecoToSimCollection;
   reco::SimToRecoCollection glbsimToRecoCollection;
   glbrecoToSimCollection = muAssociator_->associateRecoToSim(glbHandle,tpHandle,&iEvent);
   glbsimToRecoCollection = muAssociator_->associateSimToReco(glbHandle,tpHandle,&iEvent);


   for (TrackingParticleCollection::size_type i=0; i<tpColl.size(); ++i){
     TrackingParticleRef tp(tpHandle,i);

     std::vector<std::pair<reco::TrackRef, double> > rvGlb;
     reco::TrackRef rGlb;
     if(glbsimToRecoCollection.find(tp) != glbsimToRecoCollection.end()){
       rvGlb = glbsimToRecoCollection[tp];
       if(rvGlb.size() != 0) {
	 rGlb = rvGlb.begin()->first;
       }
     }

     std::vector<std::pair<reco::TrackRef, double> > rvSta;
     reco::TrackRef rSta;
     if(stasimToRecoCollection.find(tp) != stasimToRecoCollection.end()){
       rvSta = stasimToRecoCollection[tp];
       if(rvSta.size() != 0) {
	 rSta = rvSta.begin()->first;
       }
     }

     std::vector<std::pair<reco::TrackRef, double> > rvTk;
     reco::TrackRef rTk;
     if(tksimToRecoCollection.find(tp) != tksimToRecoCollection.end()){
       rvTk = tksimToRecoCollection[tp];
       if(rvTk.size() != 0) {
	 rTk = rvTk.begin()->first;
       }
     }
     
     if( rvSta.size() != 0 && rvTk.size() != 0 ){
       //should have matched
       h_shouldMatch->Fill(rTk->eta(),rTk->pt());
     }

     for ( reco::MuonTrackLinksCollection::const_iterator links = muHandle->begin(); links != muHandle->end(); ++links ) {
       if( rGlb == links->globalTrack() ) {
	 if( links->trackerTrack() == rTk && 
	     links->standAloneTrack() == rSta ) {
	   //goodMatch
	   h_goodMatchSim->Fill(rGlb->eta(),rGlb->pt());
	 } 
	 if ( links->trackerTrack() == rTk &&
	      links->standAloneTrack() != rSta ) {
	   //tkOnlyMatch
	   h_tkOnlySim->Fill(rGlb->eta(),rGlb->pt());
	 } 
	 if ( links->standAloneTrack() == rSta &&
	      links->trackerTrack() != rTk ) {
	   //staOnlyMatch
	   h_staOnlySim->Fill(rGlb->eta(),rGlb->pt());
	 }
       }
     }

   }
   
   ////////
   
   for ( reco::MuonTrackLinksCollection::const_iterator links = muHandle->begin(); links != muHandle->end(); ++links ) {
     reco::TrackRef glbRef = links->globalTrack();
     reco::TrackRef staRef = links->standAloneTrack();
     reco::TrackRef tkRef = links->trackerTrack();
     
     std::vector<std::pair<TrackingParticleRef, double> > tp1;
     TrackingParticleRef tp1r;
     if(glbrecoToSimCollection.find(glbRef) != glbrecoToSimCollection.end()){
       tp1 = glbrecoToSimCollection[glbRef];
       if(tp1.size() != 0) {
	 tp1r = tp1.begin()->first;
       }
     }
     
     std::vector<std::pair<TrackingParticleRef, double> > tp2;
     TrackingParticleRef tp2r;
     if(starecoToSimCollection.find(staRef) != starecoToSimCollection.end()){
       tp2 = starecoToSimCollection[staRef];
       if(tp2.size() != 0) {
	 tp2r = tp2.begin()->first;
       }
     }
     
     std::vector<std::pair<TrackingParticleRef, double> > tp3;
     TrackingParticleRef tp3r;
     if(tkrecoToSimCollection.find(tkRef) != tkrecoToSimCollection.end()){
       tp3 = tkrecoToSimCollection[tkRef];
       if(tp3.size() != 0) {
	 tp3r = tp3.begin()->first;
       }
     }
     
     
     if(tp1.size() != 0) {
       //was reconstructed
       h_totReco->Fill(glbRef->eta(),glbRef->pt());
       if(tp2r == tp3r) { // && tp1r == tp3r) {
	 //came from same TP
	 h_goodMatch->Fill(glbRef->eta(),glbRef->pt());
       } else {
	 //mis-match
	 h_fakeMatch->Fill(glbRef->eta(),glbRef->pt());
       }
     }
     
   }   

   
}


// ------------ method called once each job just before starting event loop  ------------
void 
GlobalMuonMatchAnalyzer::beginJob(const edm::EventSetup& setup)
{
  // Tk Associator
  edm::ESHandle<TrackAssociatorBase> tkassociatorHandle;
  setup.get<TrackAssociatorRecord>().get(tkAssociatorName_,tkassociatorHandle);
  tkAssociator_ = tkassociatorHandle.product();

  // Mu Associator
  edm::ESHandle<TrackAssociatorBase> muassociatorHandle;
  setup.get<TrackAssociatorRecord>().get(muAssociatorName_,muassociatorHandle);
  muAssociator_ = muassociatorHandle.product();

  dbe_->cd();
  std::string dirName="Matcher/";
  dbe_->setCurrentFolder(dirName.c_str());

  h_shouldMatch = dbe_->book2D("h_shouldMatch","SIM associated to Tk and Sta",50,-2.5,2.5,100,0.,500.);
  h_goodMatchSim = dbe_->book2D("h_goodMatchSim","SIM associated to Glb Sta Tk",50,-2.5,2.5,100,0.,500.);
  h_tkOnlySim = dbe_->book2D("h_tkOnlySim","SIM associated to Glb Tk",50,-2.5,2.5,100,0.,500.);
  h_staOnlySim = dbe_->book2D("h_staOnlySim","SIM associated to Glb Sta",50,-2.5,2.5,100,0.,500.);

  h_totReco = dbe_->book2D("h_totReco","Total Glb Reconstructed",50,-2.5,2.5,100,0.,500.);
  h_goodMatch = dbe_->book2D("h_goodMatch","Sta and Tk from same SIM",50,-2.5,2.5,100, 0., 500.);
  h_fakeMatch = dbe_->book2D("h_fakeMatch","Sta and Tk not from same SIM",50,-2.5,2.5,100,0.,500.);

  h_effic = dbe_->book1D("h_effic","Efficiency vs #eta",50,-2.5,2.5);
  h_efficPt = dbe_->book1D("h_efficPt","Efficiency vs p_{T}",100,0.,100.);

  h_fake = dbe_->book1D("h_fake","Fake fraction vs #eta",50,-2.5,2.5);
  h_fakePt = dbe_->book1D("h_fakePt","Fake fraction vs p_{T}",100,0.,100.);
}

// ------------ method called once each job just after ending the event loop  ------------
void 
GlobalMuonMatchAnalyzer::endJob() {
  computeEfficiencyEta(h_effic,h_goodMatchSim,h_shouldMatch);
  computeEfficiencyPt(h_efficPt,h_goodMatchSim,h_shouldMatch);

  computeEfficiencyEta(h_fake,h_fakeMatch,h_totReco);
  computeEfficiencyPt(h_fakePt,h_fakeMatch,h_totReco);

  if( out.size() != 0 && dbe_ ) dbe_->save(out);
}

void GlobalMuonMatchAnalyzer::computeEfficiencyEta(MonitorElement *effHist, MonitorElement *recoTH2, MonitorElement *simTH2){
  TH2F * h1 =dynamic_cast<TH2F*>(&(**((MonitorElementRootH2 *)recoTH2)));
  TH1D* reco = h1->ProjectionX();

  TH2F * h2 =dynamic_cast<TH2F*>(&(**((MonitorElementRootH2 *)simTH2)));
  TH1D* sim  = h2 ->ProjectionX();

    
  TH1F *hEff = (TH1F*) reco->Clone();  
  
  hEff->Divide(sim);
  
  hEff->SetName("tmp_"+TString(reco->GetName()));
  
  // Set the error accordingly to binomial statistics
  int nBinsEta = hEff->GetNbinsX();
  for(int bin = 1; bin <=  nBinsEta; bin++) {
    float nSimHit = sim->GetBinContent(bin);
    float eff = hEff->GetBinContent(bin);
    float error = 0;
    if(nSimHit != 0 && eff <= 1) {
      error = sqrt(eff*(1-eff)/nSimHit);
    }
    hEff->SetBinError(bin, error);
    effHist->setBinContent(bin,eff);
    effHist->setBinError(bin,error);
  }
  
}

void GlobalMuonMatchAnalyzer::computeEfficiencyPt(MonitorElement *effHist, MonitorElement *recoTH2, MonitorElement *simTH2){
  TH2F * h1 =dynamic_cast<TH2F*>(&(**((MonitorElementRootH2 *)recoTH2)));
  TH1D* reco = h1->ProjectionY();

  TH2F * h2 =dynamic_cast<TH2F*>(&(**((MonitorElementRootH2 *)simTH2)));
  TH1D* sim  = h2 ->ProjectionY();

    
  TH1F *hEff = (TH1F*) reco->Clone();  
  
  hEff->Divide(sim);
  
  hEff->SetName("tmp_"+TString(reco->GetName()));
  
  // Set the error accordingly to binomial statistics
  int nBinsPt = hEff->GetNbinsX();
  for(int bin = 1; bin <=  nBinsPt; bin++) {
    float nSimHit = sim->GetBinContent(bin);
    float eff = hEff->GetBinContent(bin);
    float error = 0;
    if(nSimHit != 0 && eff <= 1) {
      error = sqrt(eff*(1-eff)/nSimHit);
    }
    hEff->SetBinError(bin, error);
    effHist->setBinContent(bin,eff);
    effHist->setBinError(bin,error);
  }
  
}

