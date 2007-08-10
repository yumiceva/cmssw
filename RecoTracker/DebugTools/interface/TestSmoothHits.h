#ifndef TESTSMOOTHHITS_H
#define TESTSMOOTHHITS_H


// -*- C++ -*-
//
// Package:    TestSmoothHits
// Class:      TestSmoothHits
// 
/**\class TestSmoothHits TestSmoothHits.cc RecoTracker/DebugTools/src/TestSmoothHits.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Giuseppe Cerati
//         Created:  Tue Feb 13 17:29:10 CET 2007
// $Id: TestSmoothHits.h,v 1.2 2007/06/27 18:13:43 cerati Exp $
//
//
#include <memory>
#include <vector>
#include <string>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "SimTracker/TrackerHitAssociation/interface/TrackerHitAssociator.h"
#include "MagneticField/Engine/interface/MagneticField.h"
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h" 
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h" 
#include "TrackingTools/Records/interface/TransientRecHitRecord.h" 
#include "TrackingTools/PatternTools/interface/TrajectoryStateUpdator.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
#include "TrackingTools/GeomPropagators/interface/Propagator.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "RecoTracker/TransientTrackingRecHit/interface/TkTransientTrackingRecHitBuilder.h"
#include "TrackingTools/PatternTools/interface/MeasurementExtractor.h"
#include "TrackingTools/TrackFitters/interface/KFTrajectoryFitter.h"
#include "TrackingTools/TrackFitters/interface/KFTrajectorySmoother.h"
#include <sstream>
#include <TFile.h>
#include <TH1F.h>
#include <TH2F.h>

class TestSmoothHits : public edm::EDAnalyzer {
public:
  explicit TestSmoothHits(const edm::ParameterSet&);
  ~TestSmoothHits();

private:
  virtual void beginJob(const edm::EventSetup&) ;
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob() ;
/*   template<unsigned int D>  */
/*     double computeChi2Increment(MeasurementExtractor, TransientTrackingRecHit::ConstRecHitPointer); */
/*   double computeChi2Increment(MeasurementExtractor me, TransientTrackingRecHit::ConstRecHitPointer hit) { */
/*     switch (hit->dimension()) { */
/*     case 1: return computeChi2Increment<1>(me,hit); */
/*     case 2: return computeChi2Increment<2>(me,hit); */
/*     case 3: return computeChi2Increment<3>(me,hit); */
/*     case 4: return computeChi2Increment<4>(me,hit); */
/*     case 5: return computeChi2Increment<5>(me,hit); */
/*     } */
/*     throw cms::Exception("CkfDebugger error: rechit of dimension not 1,2,3,4,5"); */
/*   } */

  const edm::ParameterSet conf_;
  TrackerHitAssociator * hitAssociator;

  double mineta, maxeta;

  std::string propagatorName;
  std::string builderName;
  std::string srcName;
  std::string updatorName;
  std::string fname;
  std::string sname;

  edm::ESHandle<TrackerGeometry> theG;
  edm::ESHandle<MagneticField> theMF;
  edm::ESHandle<Propagator> thePropagator;
  edm::ESHandle<TransientTrackingRecHitBuilder> theBuilder;
  edm::ESHandle<TrajectoryStateUpdator> theUpdator;
  edm::Handle<TrackCandidateCollection> theTCCollection;
  edm::ESHandle<TrajectoryFitter> fit;
  edm::ESHandle<TrajectorySmoother> smooth;
  
  TFile* file;
  std::stringstream title;
  std::map<std::string,TH1F*> hPullGP_X_ts;
  std::map<std::string,TH1F*> hPullGP_Y_ts;
  std::map<std::string,TH1F*> hPullGP_Z_ts;
  std::map<std::string,TH1F*> hPullGM_X_ts;
  std::map<std::string,TH1F*> hPullGM_Y_ts;
  std::map<std::string,TH1F*> hPullGM_Z_ts;
  std::map<std::string,TH1F*> hPullGP_X_rs;
  std::map<std::string,TH1F*> hPullGP_Y_rs;
  std::map<std::string,TH1F*> hPullGP_Z_rs;
  std::map<std::string,TH1F*> hPullGP_X_tr;
  std::map<std::string,TH1F*> hPullGP_Y_tr;
  std::map<std::string,TH1F*> hPullGP_Z_tr;
  std::map<std::string,TH1F*> hChi2Increment;
  TH1F * hTotChi2Increment;
  TH2F * hChi2_vs_Process, *hChi2_vs_clsize;

  std::map<std::string,TH1F*> hPullGP_X_ts_mono;
  std::map<std::string,TH1F*> hPullGP_Y_ts_mono;
  std::map<std::string,TH1F*> hPullGP_Z_ts_mono;
  std::map<std::string,TH1F*> hPullGM_X_ts_mono;
  std::map<std::string,TH1F*> hPullGM_Y_ts_mono;
  std::map<std::string,TH1F*> hPullGM_Z_ts_mono;
  std::map<std::string,TH1F*> hPullGP_X_rs_mono;
  std::map<std::string,TH1F*> hPullGP_Y_rs_mono;
  std::map<std::string,TH1F*> hPullGP_Z_rs_mono;
  std::map<std::string,TH1F*> hPullGP_X_tr_mono;
  std::map<std::string,TH1F*> hPullGP_Y_tr_mono;
  std::map<std::string,TH1F*> hPullGP_Z_tr_mono;

  std::map<std::string,TH1F*> hPullGP_X_ts_stereo;
  std::map<std::string,TH1F*> hPullGP_Y_ts_stereo;
  std::map<std::string,TH1F*> hPullGP_Z_ts_stereo;
  std::map<std::string,TH1F*> hPullGM_X_ts_stereo;
  std::map<std::string,TH1F*> hPullGM_Y_ts_stereo;
  std::map<std::string,TH1F*> hPullGM_Z_ts_stereo;
  std::map<std::string,TH1F*> hPullGP_X_rs_stereo;
  std::map<std::string,TH1F*> hPullGP_Y_rs_stereo;
  std::map<std::string,TH1F*> hPullGP_Z_rs_stereo;
  std::map<std::string,TH1F*> hPullGP_X_tr_stereo;
  std::map<std::string,TH1F*> hPullGP_Y_tr_stereo;
  std::map<std::string,TH1F*> hPullGP_Z_tr_stereo;
};

#endif
