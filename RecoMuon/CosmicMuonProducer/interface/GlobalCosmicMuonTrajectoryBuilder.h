#ifndef RecoMuon_CosmicMuonProducer_GlobalCosmicMuonTrajectoryBuilder_H
#define RecoMuon_CosmicMuonProducer_GlobalCosmicMuonTrajectoryBuilder_H

/** \file GlobalCosmicMuonTrajectoryBuilder
 *  class to build combined trajectory from cosmic tracks in tk and mu
 *
 *  $Date: 2008/11/03 18:23:49 $
 *  $Revision: 1.10 $
 *  \author Chang Liu  -  Purdue University
 */

#include "RecoMuon/TrackingTools/interface/MuonTrajectoryBuilder.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "TrackingTools/DetLayers/interface/DetLayer.h"
#include "RecoMuon/TrackingTools/interface/MuonServiceProxy.h"
#include "RecoMuon/TransientTrackingRecHit/interface/MuonTransientTrackingRecHit.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHit.h"
#include "RecoMuon/CosmicMuonProducer/interface/CosmicMuonSmoother.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHitBuilder.h"
#include "RecoMuon/GlobalTrackingTools/interface/GlobalMuonTrackMatcher.h"

namespace edm {class ParameterSet; class Event; class EventSetup;}

class Trajectory;
class TrajectoryMeasurement;
class CosmicMuonUtilities;

class GlobalCosmicMuonTrajectoryBuilder : public MuonTrajectoryBuilder{

public:
  typedef TransientTrackingRecHit::RecHitContainer RecHitContainer;
  typedef TransientTrackingRecHit::ConstRecHitContainer ConstRecHitContainer;
  typedef TransientTrackingRecHit::RecHitPointer RecHitPointer;
  typedef TransientTrackingRecHit::ConstRecHitPointer ConstRecHitPointer;

  typedef MuonTransientTrackingRecHit::MuonRecHitPointer MuonRecHitPointer;
  typedef MuonTransientTrackingRecHit::ConstMuonRecHitPointer ConstMuonRecHitPointer;
  typedef MuonTransientTrackingRecHit::MuonRecHitContainer MuonRecHitContainer;
  typedef MuonTransientTrackingRecHit::ConstMuonRecHitContainer ConstMuonRecHitContainer;

  /// Constructor
  GlobalCosmicMuonTrajectoryBuilder(const edm::ParameterSet&,const MuonServiceProxy* service);

  /// Destructor
  virtual ~GlobalCosmicMuonTrajectoryBuilder();

  /// dummy implementation, unused in this class
  std::vector<Trajectory*> trajectories(const TrajectorySeed&) {return std::vector<Trajectory*>();}

  /// choose tk Track and build combined trajectories
  virtual CandidateContainer trajectories(const TrackCand&);


  virtual void setEvent(const edm::Event&);

private:

  void sortHits(ConstRecHitContainer&, ConstRecHitContainer&, ConstRecHitContainer&);

  ConstRecHitContainer getTransientRecHits(const reco::Track&) const;

  CosmicMuonSmoother* smoother() const {return theSmoother;}

  CosmicMuonUtilities* utilities() const {return smoother()->utilities();}

  const MuonServiceProxy *theService;

  CosmicMuonSmoother* theSmoother;

  GlobalMuonTrackMatcher* theTrackMatcher;

  std::string thePropagatorName;
  std::string theTkTrackLabel;

  std::string theTrackerRecHitBuilderName;
  edm::ESHandle<TransientTrackingRecHitBuilder> theTrackerRecHitBuilder;
  
  std::string theMuonRecHitBuilderName;
  edm::ESHandle<TransientTrackingRecHitBuilder> theMuonRecHitBuilder;

  edm::Handle<reco::TrackCollection> theTrackerTracks;

  bool tkTrajsAvailable;

  const std::vector<Trajectory>* allTrackerTrajs;
  
};
#endif
