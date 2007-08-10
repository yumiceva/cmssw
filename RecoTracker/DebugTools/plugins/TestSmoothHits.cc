#include "RecoTracker/DebugTools/interface/TestSmoothHits.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TrajectoryState/interface/TrajectoryStateTransform.h"
#include "TrackingTools/PatternTools/interface/TransverseImpactPointExtrapolator.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoTracker/TrackProducer/interface/TrackingRecHitLessFromGlobalPosition.h"
#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"
#include "TrackingTools/TrackFitters/interface/TrajectoryStateCombiner.h"
#include <TDirectory.h>


typedef TrajectoryStateOnSurface TSOS;
typedef TransientTrackingRecHit::ConstRecHitPointer CTTRHp;
using namespace std;
using namespace edm;

TestSmoothHits::TestSmoothHits(const edm::ParameterSet& iConfig):
  conf_(iConfig){
  LogTrace("TestSmoothHits") << conf_<< std::endl;
  propagatorName = conf_.getParameter<std::string>("Propagator");   
  builderName = conf_.getParameter<std::string>("TTRHBuilder");   
  srcName = conf_.getParameter<std::string>("src");   
  updatorName = conf_.getParameter<std::string>("updator");
  fname = conf_.getParameter<std::string>("Fitter");
  sname = conf_.getParameter<std::string>("Smoother");
  mineta = conf_.getParameter<double>("mineta");
  maxeta = conf_.getParameter<double>("maxeta");
}

TestSmoothHits::~TestSmoothHits(){}

void TestSmoothHits::beginJob(const edm::EventSetup& iSetup)
{
 
  iSetup.get<TrackerDigiGeometryRecord>().get(theG);
  iSetup.get<IdealMagneticFieldRecord>().get(theMF);  
  iSetup.get<TrackingComponentsRecord>().get(propagatorName,thePropagator);
  iSetup.get<TransientRecHitRecord>().get(builderName,theBuilder);
  iSetup.get<TrackingComponentsRecord>().get(updatorName,theUpdator);
  iSetup.get<TrackingComponentsRecord>().get(fname, fit);
  iSetup.get<TrackingComponentsRecord>().get(sname, smooth);

  file = new TFile("testSmoothHits.root","recreate");
  for (int i=0; i!=6; i++)
    for (int j=0; j!=9; j++){
      if (i==0 && j>2) break;
      if (i==1 && j>1) break;
      if (i==2 && j>3) break;
      if (i==3 && j>2) break;
      if (i==4 && j>5) break;
      if (i==5 && j>8) break;
      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_X_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_Y_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_Z_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "Chi2Increment_" << i+1 << "-" << j+1;
      hChi2Increment[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,0,100);

      title.str("");
      title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_X_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_Y_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_Z_ts[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_X_tr[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_Y_tr[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_Z_tr[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_X_rs[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_Y_rs[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_Z_rs[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

      if ( ((i==2||i==4)&&(j==0||j==1)) || (i==3||i==5) ){
	//mono
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_X_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_Y_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_Z_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_X_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_Y_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_Z_ts_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_X_tr_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_Y_tr_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_Z_tr_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_X_rs_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_Y_rs_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_Z_rs_mono[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	//stereo
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_X_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_Y_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_Z_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_X_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_Y_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_Z_ts_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_X_tr_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_Y_tr_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_Z_tr_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);

	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_X_rs_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_Y_rs_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_Z_rs_stereo[title.str()] = new TH1F(title.str().c_str(),title.str().c_str(),1000,-50,50);
      }
    }
  hTotChi2Increment = new TH1F("TotChi2Increment","TotChi2Increment",1000,0,100);
  hChi2_vs_Process  = new TH2F("Chi2_vs_Process","Chi2_vs_Process",1000,0,100,17,-0.5,16.5);  
  hChi2_vs_clsize  = new TH2F("Chi2_vs_clsize","Chi2_vs_clsize",1000,0,100,17,-0.5,16.5);
}

#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

void TestSmoothHits::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  LogTrace("TestSmoothHits") << "new event" << std::endl;
  //cout << "new event" << std::endl;

  iEvent.getByLabel(srcName,theTCCollection ); 
  hitAssociator = new TrackerHitAssociator::TrackerHitAssociator(iEvent);

  TrajectoryStateCombiner combiner;

  for (TrackCandidateCollection::const_iterator i=theTCCollection->begin(); i!=theTCCollection->end();i++){

    LogTrace("TestSmoothHits") << "new candidate" << std::endl;
    //cout << "new candidate" << std::endl;
      
    const TrackCandidate * theTC = &(*i);
    PTrajectoryStateOnDet state = theTC->trajectoryStateOnDet();
    const TrackCandidate::range& recHitVec=theTC->recHits();

    //convert PTrajectoryStateOnDet to TrajectoryStateOnSurface
    TrajectoryStateTransform transformer;
    
    DetId  detId(state.detId());
    TrajectoryStateOnSurface theTSOS=
      transformer.transientState(state, &(theG->idToDet(detId)->surface()),theMF.product());

    if (theTSOS.globalMomentum().eta()>maxeta || theTSOS.globalMomentum().eta()<mineta) continue;
    
    //convert the TrackingRecHit vector to a TransientTrackingRecHit vector
    TransientTrackingRecHit::RecHitContainer hits;
    
    for (edm::OwnVector<TrackingRecHit>::const_iterator i=recHitVec.first;
	 i!=recHitVec.second; i++){
      hits.push_back(theBuilder->build(&(*i) ));
    }

    //call the fitter
    vector<Trajectory> fitted = fit->fit(theTC->seed(), hits, theTSOS);
    //call the smoother
    vector<Trajectory> result; 
    for(vector<Trajectory>::iterator it = fitted.begin(); it != fitted.end(); it++) {
      vector<Trajectory> smoothed = smooth->trajectories(*it);
      result.insert(result.end(), smoothed.begin(), smoothed.end());
    }
    if (result.size()==0) break;
    std::vector<TrajectoryMeasurement> vtm = result[0].measurements();

    TSOS lastState = theTSOS;
    for (std::vector<TrajectoryMeasurement>::iterator tm=vtm.begin(); tm!=vtm.end();tm++){

      TransientTrackingRecHit::ConstRecHitPointer rhit = tm->recHit();
      if ((rhit)->isValid()==0&&rhit->det()!=0) continue;
      LogTrace("TestSmoothHits") << "new hit" ;
      //cout << "new hit" << std::endl;
      int subdetId = rhit->det()->geographicalId().subdetId();
      int layerId  = 0;
      DetId id = rhit->det()->geographicalId();
      if (id.subdetId()==3) layerId = ((TIBDetId)(id)).layer();
      if (id.subdetId()==5) layerId = ((TOBDetId)(id)).layer();
      if (id.subdetId()==1) layerId = ((PXBDetId)(id)).layer();
      if (id.subdetId()==4) layerId = ((TIDDetId)(id)).wheel();
      if (id.subdetId()==6) layerId = ((TECDetId)(id)).wheel();
      if (id.subdetId()==2) layerId = ((PXFDetId)(id)).disk();
      LogTrace("TestSmoothHits") << "subdetId=" << subdetId << " layerId=" << layerId ;
      //cout << "subdetId=" << subdetId << " layerId=" << layerId << std::endl;

      vector<PSimHit> assSimHits = hitAssociator->associateHit(*(rhit->hit()));
      if (assSimHits.size()==0) continue;
      PSimHit shit=*(assSimHits.begin());
 
      TSOS currentState = combiner(tm->backwardPredictedState(), tm->forwardPredictedState());
      TSOS updatedState = tm->updatedState();
 
      //plot chi2 increment
      //MeasurementExtractor me(combTsos);
      //double chi2increment = computeChi2Increment(me,rhit);
      double chi2increment = tm->estimate();
      LogTrace("TestSmoothHits") << "tm->estimate()=" << tm->estimate();
      //cout << "tm->estimate()=" << tm->estimate()<<endl;
      title.str("");
      title << "Chi2Increment_" << subdetId << "-" << layerId;
      hChi2Increment[title.str()]->Fill( chi2increment );
      hTotChi2Increment->Fill( chi2increment );
      hChi2_vs_Process->Fill( chi2increment, shit.processType() );
      if (dynamic_cast<const SiPixelRecHit*>(rhit->hit()))	
	hChi2_vs_clsize->Fill( chi2increment, ((const SiPixelRecHit*)(rhit->hit()))->cluster()->size() );
      if (dynamic_cast<const SiStripRecHit2D*>(rhit->hit()))	
	hChi2_vs_clsize->Fill( chi2increment, ((const SiStripRecHit2D*)(rhit->hit()))->cluster()->amplitudes().size() );
     
      //#if 0    
      //test hits
      const Surface * surf = &( (rhit)->det()->surface() );
      LocalVector shitLMom = shit.momentumAtEntry();
      GlobalVector shitGMom = surf->toGlobal(shitLMom);
      LocalPoint shitLPos = shit.localPosition();
      GlobalPoint shitGPos = surf->toGlobal(shitLPos);

      GlobalVector tsosGMom = currentState.globalMomentum();
      GlobalError  tsosGMEr(currentState.cartesianError().matrix().Sub<AlgebraicSymMatrix33>(3,3));
      GlobalPoint  tsosGPos = currentState.globalPosition();
      GlobalError  tsosGPEr = currentState.cartesianError().position();

      GlobalPoint rhitGPos = (rhit)->globalPosition();
      GlobalError rhitGPEr = (rhit)->globalPositionError();

      double pullGPX_rs = (rhitGPos.x()-shitGPos.x())/sqrt(rhitGPEr.cxx());
      double pullGPY_rs = (rhitGPos.y()-shitGPos.y())/sqrt(rhitGPEr.cyy());
      double pullGPZ_rs = (rhitGPos.z()-shitGPos.z())/sqrt(rhitGPEr.czz());
      //double pullGPX_rs = (rhitGPos.x()-shitGPos.x());
      //double pullGPY_rs = (rhitGPos.y()-shitGPos.y());
      //double pullGPZ_rs = (rhitGPos.z()-shitGPos.z());
      
      LogTrace("TestSmoothHits") << "rs" << std::endl;

      title.str("");
      title << "PullGP_X_" << subdetId << "-" << layerId << "_rs";
      hPullGP_X_rs[title.str()]->Fill( pullGPX_rs );
      title.str("");
      title << "PullGP_Y_" << subdetId << "-" << layerId << "_rs";
      hPullGP_Y_rs[title.str()]->Fill( pullGPY_rs );
      title.str("");
      title << "PullGP_Z_" << subdetId << "-" << layerId << "_rs";
      hPullGP_Z_rs[title.str()]->Fill( pullGPZ_rs );

      double pullGPX_tr = (tsosGPos.x()-rhitGPos.x())/sqrt(tsosGPEr.cxx()+rhitGPEr.cxx());
      double pullGPY_tr = (tsosGPos.y()-rhitGPos.y())/sqrt(tsosGPEr.cyy()+rhitGPEr.cyy());
      double pullGPZ_tr = (tsosGPos.z()-rhitGPos.z())/sqrt(tsosGPEr.czz()+rhitGPEr.czz());
      //double pullGPX_tr = (tsosGPos.x()-rhitGPos.x());
      //double pullGPY_tr = (tsosGPos.y()-rhitGPos.y());
      //double pullGPZ_tr = (tsosGPos.z()-rhitGPos.z());

      LogTrace("TestSmoothHits") << "tr" << std::endl;

      title.str("");
      title << "PullGP_X_" << subdetId << "-" << layerId << "_tr";
      hPullGP_X_tr[title.str()]->Fill( pullGPX_tr );
      title.str("");
      title << "PullGP_Y_" << subdetId << "-" << layerId << "_tr";
      hPullGP_Y_tr[title.str()]->Fill( pullGPY_tr );
      title.str("");
      title << "PullGP_Z_" << subdetId << "-" << layerId << "_tr";
      hPullGP_Z_tr[title.str()]->Fill( pullGPZ_tr );

      double pullGPX_ts = (tsosGPos.x()-shitGPos.x())/sqrt(tsosGPEr.cxx());
      double pullGPY_ts = (tsosGPos.y()-shitGPos.y())/sqrt(tsosGPEr.cyy());
      double pullGPZ_ts = (tsosGPos.z()-shitGPos.z())/sqrt(tsosGPEr.czz());
      //double pullGPX_ts = (tsosGPos.x()-shitGPos.x());
      //double pullGPY_ts = (tsosGPos.y()-shitGPos.y());
      //double pullGPZ_ts = (tsosGPos.z()-shitGPos.z());

      LogTrace("TestSmoothHits") << "ts1" << std::endl;

      title.str("");
      title << "PullGP_X_" << subdetId << "-" << layerId << "_ts";
      hPullGP_X_ts[title.str()]->Fill( pullGPX_ts );
      title.str("");
      title << "PullGP_Y_" << subdetId << "-" << layerId << "_ts";
      hPullGP_Y_ts[title.str()]->Fill( pullGPY_ts );
      title.str("");
      title << "PullGP_Z_" << subdetId << "-" << layerId << "_ts";
      hPullGP_Z_ts[title.str()]->Fill( pullGPZ_ts );

      double pullGMX_ts = (tsosGMom.x()-shitGMom.x())/sqrt(tsosGMEr.cxx());
      double pullGMY_ts = (tsosGMom.y()-shitGMom.y())/sqrt(tsosGMEr.cyy());
      double pullGMZ_ts = (tsosGMom.z()-shitGMom.z())/sqrt(tsosGMEr.czz());
      //double pullGMX_ts = (tsosGMom.x()-shitGMom.x());
      //double pullGMY_ts = (tsosGMom.y()-shitGMom.y());
      //double pullGMZ_ts = (tsosGMom.z()-shitGMom.z());

      LogTrace("TestSmoothHits") << "ts2" << std::endl;

      title.str("");
      title << "PullGM_X_" << subdetId << "-" << layerId << "_ts";
      hPullGM_X_ts[title.str()]->Fill( pullGMX_ts );
      title.str("");
      title << "PullGM_Y_" << subdetId << "-" << layerId << "_ts";
      hPullGM_Y_ts[title.str()]->Fill( pullGMY_ts );
      title.str("");
      title << "PullGM_Z_" << subdetId << "-" << layerId << "_ts";
      hPullGM_Z_ts[title.str()]->Fill( pullGMZ_ts );

      if (dynamic_cast<const SiStripMatchedRecHit2D*>((rhit)->hit())) {
	//mono
	LogTrace("TestSmoothHits") << "MONO HIT" << endl;
	CTTRHp tMonoHit = 
	  theBuilder->build(dynamic_cast<const SiStripMatchedRecHit2D*>((rhit)->hit())->monoHit());
	if (tMonoHit==0) continue;
	vector<PSimHit> assMonoSimHits = hitAssociator->associateHit(*tMonoHit->hit());
	if (assMonoSimHits.size()==0) continue;
	const PSimHit sMonoHit = *(assSimHits.begin());
	const Surface * monoSurf = &( tMonoHit->det()->surface() );
	if (monoSurf==0) continue;
	TSOS monoState = thePropagator->propagate(lastState,*monoSurf);
	if (monoState.isValid()==0) continue;

	LocalVector monoShitLMom = sMonoHit.momentumAtEntry();
	GlobalVector monoShitGMom = monoSurf->toGlobal(monoShitLMom);
	LocalPoint monoShitLPos = sMonoHit.localPosition();
	GlobalPoint monoShitGPos = monoSurf->toGlobal(monoShitLPos);

	GlobalVector monoTsosGMom = monoState.globalMomentum();
	GlobalError  monoTsosGMEr(monoState.cartesianError().matrix().Sub<AlgebraicSymMatrix33>(3,3));
	GlobalPoint  monoTsosGPos = monoState.globalPosition();
	GlobalError  monoTsosGPEr = monoState.cartesianError().position();

	GlobalPoint monoRhitGPos = tMonoHit->globalPosition();
	GlobalError monoRhitGPEr = tMonoHit->globalPositionError();

	double pullGPX_rs_mono = (monoRhitGPos.x()-monoShitGPos.x())/sqrt(monoRhitGPEr.cxx());
	double pullGPY_rs_mono = (monoRhitGPos.y()-monoShitGPos.y())/sqrt(monoRhitGPEr.cyy());
	double pullGPZ_rs_mono = (monoRhitGPos.z()-monoShitGPos.z())/sqrt(monoRhitGPEr.czz());
	//double pullGPX_rs_mono = (monoRhitGPos.x()-monoShitGPos.x());
	//double pullGPY_rs_mono = (monoRhitGPos.y()-monoShitGPos.y());
	//double pullGPZ_rs_mono = (monoRhitGPos.z()-monoShitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_rs_mono";
	hPullGP_X_rs_mono[title.str()]->Fill( pullGPX_rs_mono );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_rs_mono";
	hPullGP_Y_rs_mono[title.str()]->Fill( pullGPY_rs_mono );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_rs_mono";
	hPullGP_Z_rs_mono[title.str()]->Fill( pullGPZ_rs_mono );

	double pullGPX_tr_mono = (monoTsosGPos.x()-monoRhitGPos.x())/sqrt(monoTsosGPEr.cxx()+monoRhitGPEr.cxx());
	double pullGPY_tr_mono = (monoTsosGPos.y()-monoRhitGPos.y())/sqrt(monoTsosGPEr.cyy()+monoRhitGPEr.cyy());
	double pullGPZ_tr_mono = (monoTsosGPos.z()-monoRhitGPos.z())/sqrt(monoTsosGPEr.czz()+monoRhitGPEr.czz());
	//double pullGPX_tr_mono = (monoTsosGPos.x()-monoRhitGPos.x());
	//double pullGPY_tr_mono = (monoTsosGPos.y()-monoRhitGPos.y());
	//double pullGPZ_tr_mono = (monoTsosGPos.z()-monoRhitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_tr_mono";
	hPullGP_X_tr_mono[title.str()]->Fill( pullGPX_tr_mono );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_tr_mono";
	hPullGP_Y_tr_mono[title.str()]->Fill( pullGPY_tr_mono );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_tr_mono";
	hPullGP_Z_tr_mono[title.str()]->Fill( pullGPZ_tr_mono );

	double pullGPX_ts_mono = (monoTsosGPos.x()-monoShitGPos.x())/sqrt(monoTsosGPEr.cxx());
	double pullGPY_ts_mono = (monoTsosGPos.y()-monoShitGPos.y())/sqrt(monoTsosGPEr.cyy());
	double pullGPZ_ts_mono = (monoTsosGPos.z()-monoShitGPos.z())/sqrt(monoTsosGPEr.czz());
	//double pullGPX_ts_mono = (monoTsosGPos.x()-monoShitGPos.x());
	//double pullGPY_ts_mono = (monoTsosGPos.y()-monoShitGPos.y());
	//double pullGPZ_ts_mono = (monoTsosGPos.z()-monoShitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGP_X_ts_mono[title.str()]->Fill( pullGPX_ts_mono );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGP_Y_ts_mono[title.str()]->Fill( pullGPY_ts_mono );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGP_Z_ts_mono[title.str()]->Fill( pullGPZ_ts_mono );

	double pullGMX_ts_mono = (monoTsosGMom.x()-monoShitGMom.x())/sqrt(monoTsosGMEr.cxx());
	double pullGMY_ts_mono = (monoTsosGMom.y()-monoShitGMom.y())/sqrt(monoTsosGMEr.cyy());
	double pullGMZ_ts_mono = (monoTsosGMom.z()-monoShitGMom.z())/sqrt(monoTsosGMEr.czz());
	//double pullGMX_ts_mono = (monoTsosGMom.x()-monoShitGMom.x());
	//double pullGMY_ts_mono = (monoTsosGMom.y()-monoShitGMom.y());
	//double pullGMZ_ts_mono = (monoTsosGMom.z()-monoShitGMom.z());

	title.str("");
	title << "PullGM_X_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGM_X_ts_mono[title.str()]->Fill( pullGMX_ts_mono );
	title.str("");
	title << "PullGM_Y_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGM_Y_ts_mono[title.str()]->Fill( pullGMY_ts_mono );
	title.str("");
	title << "PullGM_Z_" << subdetId << "-" << layerId << "_ts_mono";
	hPullGM_Z_ts_mono[title.str()]->Fill( pullGMZ_ts_mono );

	//stereo
	LogTrace("TestSmoothHits") << "STEREO HIT" << endl;
	CTTRHp tStereoHit = 
	  theBuilder->build(dynamic_cast<const SiStripMatchedRecHit2D*>((rhit)->hit())->stereoHit());
	if (tStereoHit==0) continue;
	vector<PSimHit> assStereoSimHits = hitAssociator->associateHit(*tStereoHit->hit());
	if (assStereoSimHits.size()==0) continue;
	const PSimHit sStereoHit = *(assSimHits.begin());
	const Surface * stereoSurf = &( tStereoHit->det()->surface() );
	if (stereoSurf==0) continue;
	TSOS stereoState = thePropagator->propagate(lastState,*stereoSurf);
	if (stereoState.isValid()==0) continue;

	LocalVector stereoShitLMom = sStereoHit.momentumAtEntry();
	GlobalVector stereoShitGMom = stereoSurf->toGlobal(stereoShitLMom);
	LocalPoint stereoShitLPos = sStereoHit.localPosition();
	GlobalPoint stereoShitGPos = stereoSurf->toGlobal(stereoShitLPos);

	GlobalVector stereoTsosGMom = stereoState.globalMomentum();
	GlobalError  stereoTsosGMEr(stereoState.cartesianError().matrix().Sub<AlgebraicSymMatrix33>(3,3));
	GlobalPoint  stereoTsosGPos = stereoState.globalPosition();
	GlobalError  stereoTsosGPEr = stereoState.cartesianError().position();

	GlobalPoint stereoRhitGPos = tStereoHit->globalPosition();
	GlobalError stereoRhitGPEr = tStereoHit->globalPositionError();

	double pullGPX_rs_stereo = (stereoRhitGPos.x()-stereoShitGPos.x())/sqrt(stereoRhitGPEr.cxx());
	double pullGPY_rs_stereo = (stereoRhitGPos.y()-stereoShitGPos.y())/sqrt(stereoRhitGPEr.cyy());
	double pullGPZ_rs_stereo = (stereoRhitGPos.z()-stereoShitGPos.z())/sqrt(stereoRhitGPEr.czz());
	//double pullGPX_rs_stereo = (stereoRhitGPos.x()-stereoShitGPos.x());
	//double pullGPY_rs_stereo = (stereoRhitGPos.y()-stereoShitGPos.y());
	//double pullGPZ_rs_stereo = (stereoRhitGPos.z()-stereoShitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_rs_stereo";
	hPullGP_X_rs_stereo[title.str()]->Fill( pullGPX_rs_stereo );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_rs_stereo";
	hPullGP_Y_rs_stereo[title.str()]->Fill( pullGPY_rs_stereo );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_rs_stereo";
	hPullGP_Z_rs_stereo[title.str()]->Fill( pullGPZ_rs_stereo );

	double pullGPX_tr_stereo = (stereoTsosGPos.x()-stereoRhitGPos.x())/sqrt(stereoTsosGPEr.cxx()+stereoRhitGPEr.cxx());
	double pullGPY_tr_stereo = (stereoTsosGPos.y()-stereoRhitGPos.y())/sqrt(stereoTsosGPEr.cyy()+stereoRhitGPEr.cyy());
	double pullGPZ_tr_stereo = (stereoTsosGPos.z()-stereoRhitGPos.z())/sqrt(stereoTsosGPEr.czz()+stereoRhitGPEr.czz());
	//double pullGPX_tr_stereo = (stereoTsosGPos.x()-stereoRhitGPos.x());
	//double pullGPY_tr_stereo = (stereoTsosGPos.y()-stereoRhitGPos.y());
	//double pullGPZ_tr_stereo = (stereoTsosGPos.z()-stereoRhitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_tr_stereo";
	hPullGP_X_tr_stereo[title.str()]->Fill( pullGPX_tr_stereo );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_tr_stereo";
	hPullGP_Y_tr_stereo[title.str()]->Fill( pullGPY_tr_stereo );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_tr_stereo";
	hPullGP_Z_tr_stereo[title.str()]->Fill( pullGPZ_tr_stereo );

	double pullGPX_ts_stereo = (stereoTsosGPos.x()-stereoShitGPos.x())/sqrt(stereoTsosGPEr.cxx());
	double pullGPY_ts_stereo = (stereoTsosGPos.y()-stereoShitGPos.y())/sqrt(stereoTsosGPEr.cyy());
	double pullGPZ_ts_stereo = (stereoTsosGPos.z()-stereoShitGPos.z())/sqrt(stereoTsosGPEr.czz());
	//double pullGPX_ts_stereo = (stereoTsosGPos.x()-stereoShitGPos.x());
	//double pullGPY_ts_stereo = (stereoTsosGPos.y()-stereoShitGPos.y());
	//double pullGPZ_ts_stereo = (stereoTsosGPos.z()-stereoShitGPos.z());

	title.str("");
	title << "PullGP_X_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGP_X_ts_stereo[title.str()]->Fill( pullGPX_ts_stereo );
	title.str("");
	title << "PullGP_Y_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGP_Y_ts_stereo[title.str()]->Fill( pullGPY_ts_stereo );
	title.str("");
	title << "PullGP_Z_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGP_Z_ts_stereo[title.str()]->Fill( pullGPZ_ts_stereo );

	double pullGMX_ts_stereo = (stereoTsosGMom.x()-stereoShitGMom.x())/sqrt(stereoTsosGMEr.cxx());
	double pullGMY_ts_stereo = (stereoTsosGMom.y()-stereoShitGMom.y())/sqrt(stereoTsosGMEr.cyy());
	double pullGMZ_ts_stereo = (stereoTsosGMom.z()-stereoShitGMom.z())/sqrt(stereoTsosGMEr.czz());
	//double pullGMX_ts_stereo = (stereoTsosGMom.x()-stereoShitGMom.x());
	//double pullGMY_ts_stereo = (stereoTsosGMom.y()-stereoShitGMom.y());
	//double pullGMZ_ts_stereo = (stereoTsosGMom.z()-stereoShitGMom.z());

	title.str("");
	title << "PullGM_X_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGM_X_ts_stereo[title.str()]->Fill( pullGMX_ts_stereo );
	title.str("");
	title << "PullGM_Y_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGM_Y_ts_stereo[title.str()]->Fill( pullGMY_ts_stereo );
	title.str("");
	title << "PullGM_Z_" << subdetId << "-" << layerId << "_ts_stereo";
	hPullGM_Z_ts_stereo[title.str()]->Fill( pullGMZ_ts_stereo );
      }    
      lastState = updatedState;
      //#endif
    }
  }
  delete hitAssociator;
  LogTrace("TestSmoothHits") << "end of event" << std::endl;
}

void TestSmoothHits::endJob() {
  //file->Write();
  TDirectory * chi2i = file->mkdir("Chi2_Increment");

  TDirectory * gp_ts = file->mkdir("GP_TSOS-SimHit");
  TDirectory * gm_ts = file->mkdir("GM_TSOS-SimHit");
  TDirectory * gp_tr = file->mkdir("GP_TSOS-RecHit");
  TDirectory * gp_rs = file->mkdir("GP_RecHit-SimHit");

  TDirectory * gp_tsx = gp_ts->mkdir("X");
  TDirectory * gp_tsy = gp_ts->mkdir("Y");
  TDirectory * gp_tsz = gp_ts->mkdir("Z");
  TDirectory * gm_tsx = gm_ts->mkdir("X");
  TDirectory * gm_tsy = gm_ts->mkdir("Y");
  TDirectory * gm_tsz = gm_ts->mkdir("Z");
  TDirectory * gp_trx = gp_tr->mkdir("X");
  TDirectory * gp_try = gp_tr->mkdir("Y");
  TDirectory * gp_trz = gp_tr->mkdir("Z");
  TDirectory * gp_rsx = gp_rs->mkdir("X");
  TDirectory * gp_rsy = gp_rs->mkdir("Y");
  TDirectory * gp_rsz = gp_rs->mkdir("Z");

  TDirectory * gp_tsx_mono = gp_ts->mkdir("MONOX");
  TDirectory * gp_tsy_mono = gp_ts->mkdir("MONOY");
  TDirectory * gp_tsz_mono = gp_ts->mkdir("MONOZ");
  TDirectory * gm_tsx_mono = gm_ts->mkdir("MONOX");
  TDirectory * gm_tsy_mono = gm_ts->mkdir("MONOY");
  TDirectory * gm_tsz_mono = gm_ts->mkdir("MONOZ");
  TDirectory * gp_trx_mono = gp_tr->mkdir("MONOX");
  TDirectory * gp_try_mono = gp_tr->mkdir("MONOY");
  TDirectory * gp_trz_mono = gp_tr->mkdir("MONOZ");
  TDirectory * gp_rsx_mono = gp_rs->mkdir("MONOX");
  TDirectory * gp_rsy_mono = gp_rs->mkdir("MONOY");
  TDirectory * gp_rsz_mono = gp_rs->mkdir("MONOZ");

  TDirectory * gp_tsx_stereo = gp_ts->mkdir("STEREOX");
  TDirectory * gp_tsy_stereo = gp_ts->mkdir("STEREOY");
  TDirectory * gp_tsz_stereo = gp_ts->mkdir("STEREOZ");
  TDirectory * gm_tsx_stereo = gm_ts->mkdir("STEREOX");
  TDirectory * gm_tsy_stereo = gm_ts->mkdir("STEREOY");
  TDirectory * gm_tsz_stereo = gm_ts->mkdir("STEREOZ");
  TDirectory * gp_trx_stereo = gp_tr->mkdir("STEREOX");
  TDirectory * gp_try_stereo = gp_tr->mkdir("STEREOY");
  TDirectory * gp_trz_stereo = gp_tr->mkdir("STEREOZ");
  TDirectory * gp_rsx_stereo = gp_rs->mkdir("STEREOX");
  TDirectory * gp_rsy_stereo = gp_rs->mkdir("STEREOY");
  TDirectory * gp_rsz_stereo = gp_rs->mkdir("STEREOZ");

  chi2i->cd();
  hTotChi2Increment->Write();
  hChi2_vs_Process->Write();
  hChi2_vs_clsize->Write();
  for (int i=0; i!=6; i++)
    for (int j=0; j!=9; j++){
      if (i==0 && j>2) break;
      if (i==1 && j>1) break;
      if (i==2 && j>3) break;
      if (i==3 && j>2) break;
      if (i==4 && j>5) break;
      if (i==5 && j>8) break;
      chi2i->cd();
      title.str("");
      title << "Chi2Increment_" << i+1 << "-" << j+1;
      hChi2Increment[title.str()]->Write();

      gp_ts->cd();
      gp_tsx->cd();
      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_X_ts[title.str()]->Write();
      gp_tsy->cd();
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_Y_ts[title.str()]->Write();
      gp_tsz->cd();
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts";
      hPullGP_Z_ts[title.str()]->Write();

      gm_ts->cd();
      gm_tsx->cd();
      title.str("");
      title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_X_ts[title.str()]->Write();
      gm_tsy->cd();
      title.str("");
      title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_Y_ts[title.str()]->Write();
      gm_tsz->cd();
      title.str("");
      title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts";
      hPullGM_Z_ts[title.str()]->Write();

      gp_tr->cd();
      gp_trx->cd();
      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_X_tr[title.str()]->Write();
      gp_try->cd();
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_Y_tr[title.str()]->Write();
      gp_trz->cd();
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr";
      hPullGP_Z_tr[title.str()]->Write();

      gp_rs->cd();
      gp_rsx->cd();
      title.str("");
      title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_X_rs[title.str()]->Write();
      gp_rsy->cd();
      title.str("");
      title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_Y_rs[title.str()]->Write();
      gp_rsz->cd();
      title.str("");
      title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs";
      hPullGP_Z_rs[title.str()]->Write();

      if ( ((i==2||i==4)&&(j==0||j==1)) || (i==3||i==5) ){
	//mono
	gp_ts->cd();
	gp_tsx_mono->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_X_ts_mono[title.str()]->Write();
	gp_tsy_mono->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_Y_ts_mono[title.str()]->Write();
	gp_tsz_mono->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGP_Z_ts_mono[title.str()]->Write();

	gm_ts->cd();
	gm_tsx_mono->cd();
	title.str("");
	title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_X_ts_mono[title.str()]->Write();
	gm_tsy_mono->cd();
	title.str("");
	title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_Y_ts_mono[title.str()]->Write();
	gm_tsz_mono->cd();
	title.str("");
	title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts_mono";
	hPullGM_Z_ts_mono[title.str()]->Write();

	gp_tr->cd();
	gp_trx_mono->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_X_tr_mono[title.str()]->Write();
	gp_try_mono->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_Y_tr_mono[title.str()]->Write();
	gp_trz_mono->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr_mono";
	hPullGP_Z_tr_mono[title.str()]->Write();

	gp_rs->cd();
	gp_rsx_mono->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_X_rs_mono[title.str()]->Write();
	gp_rsy_mono->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_Y_rs_mono[title.str()]->Write();
	gp_rsz_mono->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs_mono";
	hPullGP_Z_rs_mono[title.str()]->Write();

	//stereo
	gp_ts->cd();
	gp_tsx_stereo->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_X_ts_stereo[title.str()]->Write();
	gp_tsy_stereo->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_Y_ts_stereo[title.str()]->Write();
	gp_tsz_stereo->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGP_Z_ts_stereo[title.str()]->Write();

	gm_ts->cd();
	gm_tsx_stereo->cd();
	title.str("");
	title << "PullGM_X_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_X_ts_stereo[title.str()]->Write();
	gm_tsy_stereo->cd();
	title.str("");
	title << "PullGM_Y_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_Y_ts_stereo[title.str()]->Write();
	gm_tsz_stereo->cd();
	title.str("");
	title << "PullGM_Z_" << i+1 << "-" << j+1 << "_ts_stereo";
	hPullGM_Z_ts_stereo[title.str()]->Write();

	gp_tr->cd();
	gp_trx_stereo->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_X_tr_stereo[title.str()]->Write();
	gp_try_stereo->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_Y_tr_stereo[title.str()]->Write();
	gp_trz_stereo->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_tr_stereo";
	hPullGP_Z_tr_stereo[title.str()]->Write();

	gp_rs->cd();
	gp_rsx_stereo->cd();
	title.str("");
	title << "PullGP_X_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_X_rs_stereo[title.str()]->Write();
	gp_rsy_stereo->cd();
	title.str("");
	title << "PullGP_Y_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_Y_rs_stereo[title.str()]->Write();
	gp_rsz_stereo->cd();
	title.str("");
	title << "PullGP_Z_" << i+1 << "-" << j+1 << "_rs_stereo";
	hPullGP_Z_rs_stereo[title.str()]->Write();
      }
    }

  file->Close();
}

// template<unsigned int D> 
// double TestSmoothHits::computeChi2Increment(MeasurementExtractor me, 
// 				      TransientTrackingRecHit::ConstRecHitPointer rhit) {
//   typedef typename AlgebraicROOTObject<D>::Vector VecD;
//   typedef typename AlgebraicROOTObject<D,D>::SymMatrix SMatDD;
//   VecD r = asSVector<D>(rhit->parameters()) - me.measuredParameters<D>(rhit);
  
//   SMatDD R = asSMatrix<D>(rhit->parametersError()) + me.measuredError<D>(rhit);
//   R.Invert();
//   return ROOT::Math::Similarity(r,R) ;
// }

#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(TestSmoothHits);
