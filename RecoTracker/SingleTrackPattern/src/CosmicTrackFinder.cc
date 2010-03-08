// Package:    RecoTracker/SingleTrackPattern
// Class:      CosmicTrackFinder
// Original Author:  Michele Pioppi-INFN perugia
#include <memory>
#include <string>

#include "RecoTracker/SingleTrackPattern/interface/CosmicTrackFinder.h"
#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripMatchedRecHit2DCollection.h"
#include "DataFormats/TrackerRecHit2D/interface/SiStripRecHit2DCollection.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/TrackExtra.h"
#include "TrackingTools/PatternTools/interface/Trajectory.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "TrackingTools/PatternTools/interface/TrajTrackAssociation.h"


namespace cms
{

  CosmicTrackFinder::CosmicTrackFinder(edm::ParameterSet const& conf) : 
    cosmicTrajectoryBuilder_(conf) ,
    crackTrajectoryBuilder_(conf) ,
    conf_(conf)
  {
    geometry=conf_.getUntrackedParameter<std::string>("GeometricStructure","STANDARD");
    produces<TrackCandidateCollection>();
  }


  // Virtual destructor needed.
  CosmicTrackFinder::~CosmicTrackFinder() { }  

  // Functions that gets called by framework every event
  void CosmicTrackFinder::produce(edm::Event& e, const edm::EventSetup& es)
  {
    using namespace std  ;
    edm::InputTag matchedrecHitsTag = conf_.getParameter<edm::InputTag>("matchedRecHits");
    edm::InputTag rphirecHitsTag = conf_.getParameter<edm::InputTag>("rphirecHits");
    edm::InputTag stereorecHitsTag = conf_.getParameter<edm::InputTag>("stereorecHits");
    edm::InputTag pixelRecHitsTag = conf_.getParameter<edm::InputTag>("pixelRecHits");  


    edm::InputTag seedTag = conf_.getParameter<edm::InputTag>("cosmicSeeds");
    // retrieve seeds
    edm::Handle<TrajectorySeedCollection> seed;
    e.getByLabel(seedTag,seed);  

  //retrieve PixelRecHits
    static const SiPixelRecHitCollection s_empty;
    const SiPixelRecHitCollection *pixelHitCollection = &s_empty;
    edm::Handle<SiPixelRecHitCollection> pixelHits;
    if (geometry!="MTCC" && (geometry!="CRACK" )) {
      if( e.getByLabel(pixelRecHitsTag, pixelHits)) {
	pixelHitCollection = pixelHits.product();
      } else {
	edm::LogWarning("CosmicTrackFinder") << "Collection SiPixelRecHitCollection with InputTag " << pixelRecHitsTag << " cannot be found, using empty collection of same type.";
      }
    }
    
   


 //retrieve StripRecHits
    edm::Handle<SiStripMatchedRecHit2DCollection> matchedrecHits;
    e.getByLabel( matchedrecHitsTag ,matchedrecHits);
    edm::Handle<SiStripRecHit2DCollection> rphirecHits;
    e.getByLabel( rphirecHitsTag ,rphirecHits);
    edm::Handle<SiStripRecHit2DCollection> stereorecHits;
    e.getByLabel( stereorecHitsTag, stereorecHits);

    // Step B: create empty output collection
    std::auto_ptr<TrackCandidateCollection> output(new TrackCandidateCollection);

    edm::ESHandle<TrackerGeometry> tracker;
    es.get<TrackerDigiGeometryRecord>().get(tracker);
    edm::LogVerbatim("CosmicTrackFinder") << "========== Cosmic Track Finder Info ==========";
    edm::LogVerbatim("CosmicTrackFinder") << " Numbers of Seeds " << (*seed).size();
    if((*seed).size()>0){

      std::vector<Trajectory> trajoutput;
      
      if(geometry!="CRACK" ) {
        cosmicTrajectoryBuilder_.run(*seed,
                                   *stereorecHits,
                                   *rphirecHits,
                                   *matchedrecHits,
				   *pixelHitCollection,
                                   es,
                                   e,
                                   trajoutput);
      } else {
        crackTrajectoryBuilder_.run(*seed,
                                   *stereorecHits,
                                   *rphirecHits,
                                   *matchedrecHits,
				   *pixelHitCollection,
                                   es,
                                   e,
                                   trajoutput);
      }

      edm::LogVerbatim("CosmicTrackFinder") << " Numbers of Temp Trajectories " << trajoutput.size();
      edm::LogVerbatim("CosmicTrackFinder") << "========== END Info ==========";
      if(trajoutput.size()>0){
	std::vector<Trajectory*> tmpTraj;
	std::vector<Trajectory>::iterator itr;
	for (itr=trajoutput.begin();itr!=trajoutput.end();itr++)tmpTraj.push_back(&(*itr));

	//The best track is selected
	//FOR MTCC the criteria are:
	//1)# of layers,2) # of Hits,3)Chi2
	if (geometry=="MTCC")  stable_sort(tmpTraj.begin(),tmpTraj.end(),CompareTrajLay());
	else  stable_sort(tmpTraj.begin(),tmpTraj.end(),CompareTrajChi());



	const Trajectory  theTraj = *(*tmpTraj.begin());
	bool seedplus=(theTraj.seed().direction()==alongMomentum);
	//PropagationDirection seedDir =theTraj.seed().direction();

	if (seedplus)
	  LogDebug("CosmicTrackFinder")<<"Reconstruction along momentum ";
	else
	  LogDebug("CosmicTrackFinder")<<"Reconstruction opposite to momentum";

	/*
	// === the convention is to save always final tracks with hits sorted *along* momentum	
	*/

	Trajectory::RecHitContainer thits;
	//it->recHitsV(thits);
        theTraj.recHitsV(thits,true);
	edm::OwnVector<TrackingRecHit> recHits;
	recHits.reserve(thits.size());

	// reverse hit order
	for (Trajectory::RecHitContainer::const_iterator hitIt = thits.end()-1;
	     hitIt >= thits.begin(); hitIt--) {
	  recHits.push_back( (**hitIt).hit()->clone());
	}

	TSOS firstState;
	unsigned int firstId;

	firstState=theTraj.lastMeasurement().updatedState();
	firstId = theTraj.lastMeasurement().recHit()->geographicalId().rawId();

	/*
	cout << "firstState y, z: " << firstState.globalPosition().y() 
	     << " , " << firstState.globalPosition().z() <<  endl;

	*/

	AlgebraicSymMatrix55 C = AlgebraicMatrixID();
	TSOS startingState( firstState.localParameters(), LocalTrajectoryError(C),
			    firstState.surface(),
			    firstState.magneticField() );

	// protection againt invalid initial states
	if (! firstState.isValid()) {
	  edm::LogWarning("CosmicTrackFinder") << "invalid innerState, will not make TrackCandidate";
	  edm::OrphanHandle<TrackCandidateCollection> rTrackCand = e.put(output);  
	  return;
	}

	if(firstId != recHits.front().geographicalId().rawId()){
	  edm::LogWarning("CosmicTrackFinder") <<"Mismatch in DetID of first hit: firstID= " <<firstId
					       << "   DetId= " << recHits.front().geographicalId().rawId();
	  edm::OrphanHandle<TrackCandidateCollection> rTrackCand = e.put(output);  
	  return;
	}

	PTrajectoryStateOnDet* state = TrajectoryStateTransform().persistentState( startingState, firstId);
	
	
	output->push_back(TrackCandidate(recHits,theTraj.seed(),*state,theTraj.seedRef() ) );
	
	delete state;
      }

    }
    edm::OrphanHandle<TrackCandidateCollection> rTrackCand = e.put(output);  
  }
}
