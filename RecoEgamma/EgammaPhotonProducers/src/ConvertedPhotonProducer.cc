#include <iostream>
#include <vector>
#include <memory>

// Framework
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"
//
#include "DataFormats/EgammaReco/interface/BasicCluster.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaCandidates/interface/ConvertedPhoton.h"
#include "DataFormats/TrackCandidate/interface/TrackCandidateCollection.h"
//
#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
//
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
//  Abstract classes for the converion tracking components
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionSeedFinder.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/ConversionTrackFinder.h"
// Class header file
#include "RecoEgamma/EgammaPhotonProducers/interface/ConvertedPhotonProducer.h"
//
#include "RecoTracker/Record/interface/CkfComponentsRecord.h"
#include "RecoTracker/Record/interface/TrackerRecoGeometryRecord.h"

#include "RecoEgamma/EgammaPhotonAlgos/interface/OutInConversionSeedFinder.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/InOutConversionSeedFinder.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/OutInConversionTrackFinder.h"
#include "RecoEgamma/EgammaPhotonAlgos/interface/InOutConversionTrackFinder.h"

ConvertedPhotonProducer::ConvertedPhotonProducer(const edm::ParameterSet& config) : 
  conf_(config), 
  theNavigationSchool_(0), 
  theOutInSeedFinder_(0), 
  theOutInTrackFinder_(0), 
  theInOutSeedFinder_(0),
  theInOutTrackFinder_(0),
  isInitialized(0)

{

  std::cout << " ConvertedPhotonProducer CTOR " << std::endl;

  // use onfiguration file to setup input/output collection names
 
  bcProducer_             = conf_.getParameter<std::string>("bcProducer");
  bcBarrelCollection_     = conf_.getParameter<std::string>("bcBarrelCollection");
  bcEndcapCollection_     = conf_.getParameter<std::string>("bcEndcapCollection");
  
  scHybridBarrelProducer_       = conf_.getParameter<std::string>("scHybridBarrelProducer");
  scIslandEndcapProducer_       = conf_.getParameter<std::string>("scIslandEndcapProducer");
  
  scHybridBarrelCollection_     = conf_.getParameter<std::string>("scHybridBarrelCollection");
  scIslandEndcapCollection_     = conf_.getParameter<std::string>("scIslandEndcapCollection");
  

  ConvertedPhotonCollection_     = conf_.getParameter<std::string>("convertedPhotonCollection");


  // Register the product
  produces< reco::ConvertedPhotonCollection >(ConvertedPhotonCollection_);



}

ConvertedPhotonProducer::~ConvertedPhotonProducer() {

  //  delete theMeasurementTracker_;
  delete theOutInSeedFinder_; 
  delete theOutInTrackFinder_;
  delete theInOutSeedFinder_;  

}


void  ConvertedPhotonProducer::beginJob (edm::EventSetup const & theEventSetup) {

  //get magnetic field
  edm::LogInfo("ConvertedPhotonProducer") << "get magnetic field" << "\n";
  theEventSetup.get<IdealMagneticFieldRecord>().get(theMF_);  


  theEventSetup .get<TrackerRecoGeometryRecord>().get( theGeomSearchTracker_ );


  // get the measurement tracker   
  edm::ESHandle<MeasurementTracker> measurementTrackerHandle;
  theEventSetup.get<CkfComponentsRecord>().get(measurementTrackerHandle);
  theMeasurementTracker_ = measurementTrackerHandle.product();
  
  theLayerMeasurements_  = new LayerMeasurements(theMeasurementTracker_);
  theNavigationSchool_   = new SimpleNavigationSchool( &(*theGeomSearchTracker_)  , &(*theMF_));
  NavigationSetter setter( *theNavigationSchool_);
  
  // get the Out In Seed Finder  
  //  edm::LogInfo("ConvertedPhotonProducer") << "get the OutInSeedFinder" << "\n";
  //theOutInSeedFinder_ = new OutInConversionSeedFinder (   &(*theMF_) ,  theMeasurementTracker_ );
  
  // get the Out In Track Finder
  // edm::LogInfo("ConvertedPhotonProducer") << "get the OutInTrackFinder" << "\n";
  //theOutInTrackFinder_ = new OutInConversionTrackFinder ( theEventSetup, conf_, &(*theMF_),  theMeasurementTracker_  );
  
  
  
  
  // get the In Out Seed Finder  
  //edm::LogInfo("ConvertedPhotonProducer") << "get the InOutSeedFinder" << "\n";
  //theInOutSeedFinder_ = new InOutConversionSeedFinder (  &(*theMF_) ,  theMeasurementTracker_  );
  
  
  
  // get the In Out Track Finder
  //edm::LogInfo("ConvertedPhotonProducer") << "get the InOutTrackFinder" << "\n";
  //theInOutTrackFinder_ = new InOutConversionTrackFinder ( theEventSetup, conf_, &(*theMF_),  theMeasurementTracker_  );
  
  
}


void ConvertedPhotonProducer::produce(edm::Event& theEvent, const edm::EventSetup& theEventSetup) {
  
  using namespace edm;
  
  edm::LogInfo("ConvertedPhotonProducer") << "Analyzing event number: " << theEvent.id() << "\n";
  std::cout << "ConvertedPhotonProducer:Analyzing event number " <<   theEvent.id() << std::endl;
  
  // Update MeasurementTracker
  //  theMeasurementTracker_->update(theEvent);
  
  
  
  //
  // create empty output collections
  //
  //  Out In Track Candidates
  //std::auto_ptr<TrackCandidateCollection> outInTrackCandidate_p(new TrackCandidateCollection); 
  //  In Out  Track Candidates
  //std::auto_ptr<TrackCandidateCollection> inOutTrackCandidate_p(new TrackCandidateCollection); 

  // Converted photon candidates
  reco::ConvertedPhotonCollection outputConvPhotonCollection;
  std::auto_ptr< reco::ConvertedPhotonCollection > outputConvPhotonCollection_p(new reco::ConvertedPhotonCollection);
  std::cout << " Created empty ConvertedPhotonCollection size " <<   std::endl;



  
  // Get the basic cluster collection in the Barrel 
  //  edm::Handle<reco::BasicClusterCollection> bcBarrelHandle;
  //theEvent.getByLabel(bcProducer_, bcBarrelCollection_, bcBarrelHandle);
  //std::cout << " Trying to access basic cluster collection in the Barrel from my Producer " << std::endl;
  //reco::BasicClusterCollection clusterCollectionBarrel = *(bcBarrelHandle.product());
  //std::cout << " basic cluster collection size  " << clusterCollectionBarrel.size() << std::endl;



  // Get the basic cluster collection in the Endcap 
  //  edm::Handle<reco::BasicClusterCollection> bcEndcapHandle;
  // theEvent.getByLabel(bcProducer_, bcEndcapCollection_, bcEndcapHandle);
  // std::cout << " Trying to access basic cluster collection in the Endcap from my Producer " << std::endl;
  //reco::BasicClusterCollection clusterCollectionEndcap = *(bcEndcapHandle.product());
  //std::cout << " basic cluster collection size  " << clusterCollectionEndcap.size() << std::endl;


  // Get the Super Cluster collection in the Barrel
  Handle<reco::SuperClusterCollection> scBarrelHandle;
  theEvent.getByLabel(scHybridBarrelProducer_,scHybridBarrelCollection_,scBarrelHandle);
  std::cout << " Trying to access " << scHybridBarrelCollection_.c_str() << "  from my Producer " << std::endl;
  reco::SuperClusterCollection scBarrelCollection = *(scBarrelHandle.product());
  std::cout << "barrel  SC collection size  " << scBarrelCollection.size() << std::endl;

  // Get the Super Cluster collection in the Endcap
  Handle<reco::SuperClusterCollection> scEndcapHandle;
  theEvent.getByLabel(scIslandEndcapProducer_,scIslandEndcapCollection_,scEndcapHandle);
  std::cout << " Trying to access " <<scIslandEndcapCollection_.c_str() << "  from my Producer " << std::endl;
  reco::SuperClusterCollection scEndcapCollection = *(scEndcapHandle.product());
  std::cout << "Endcap SC collection size  " << scEndcapCollection.size() << std::endl;




  reco::ConvertedPhotonCollection myConvPhotons;
  const std::vector<TrajectorySeed> theOutInSeeds;



  //  Loop over SC in the barrel and reconstruct converted photons
  int myCands=0;
  int iSC=0; // index in photon collection
  int lSC=0; // local index on barrel
  reco::SuperClusterCollection::iterator aClus;
  for(aClus = scBarrelCollection.begin(); aClus != scBarrelCollection.end(); aClus++) {
  
    //    if ( abs( aClus->eta() ) > 0.9 ) return; 
    std::cout << "  ConvertedPhotonProducer SC eta " <<  aClus->eta() << " phi " <<  aClus->phi() << std::endl;

    //    theOutInSeedFinder_->setCandidate(*aClus);
    // theOutInSeedFinder_->makeSeeds(  clusterCollectionBarrel );



    //    std::vector<Trajectory> theOutInTracks= theOutInTrackFinder_->tracks(theOutInSeedFinder_->seeds(),  *outInTrackCandidate_p);    
  

    //    theInOutSeedFinder_->setCandidate(*aClus);
    //theInOutSeedFinder_->setTracks(  theOutInTracks );   
    //theInOutSeedFinder_->makeSeeds(  clusterCollectionBarrel);


    //    std::vector<Trajectory> theInOutTracks= theInOutTrackFinder_->tracks(theInOutSeedFinder_->seeds(),  *inOutTrackCandidate_p); 





    // Define candidates with tracks
    //    std::cout << "  ConvertedPhotonProducer theOutInTracks.size() " << theOutInTracks.size() << " theInOutTracks.size() " << theInOutTracks.size() << std::endl;

    //    for (std::vector<Trajectory>::const_iterator it = theOutInTracks.begin(); it !=  theOutInTracks.end(); it++) {
    // std::cout << " ConvertedPhotonProducer OutIn Tracks Number of hits " << (*it).foundHits() << std::endl; 
    // }

    //    for (std::vector<Trajectory>::const_iterator it = theInOutTracks.begin(); it !=  theInOutTracks.end(); it++) {
    // std::cout << " ConvertedPhotonProducer InOut Tracks Number of hits " << (*it).foundHits() << std::endl; 
    // }

    //    if ( theOutInTracks.size() ||  theOutInTracks.size() ) {

      // Track cleaning
      // Track pairing
      // vertex finding
      // final candidate 
      const reco::Particle::Point  vtx( 0, 0, 0 );
      // Possibly correct the vertex position from the tracks
      math::XYZVector direction =aClus->position() - vtx;
      math::XYZVector momentum = direction.unit() * aClus->energy();
      // Possibly compute the momentum from the tracks

      const reco::Particle::LorentzVector  p4(momentum.x(), momentum.y(), momentum.z(), aClus->energy() );

      reco::ConvertedPhoton  newCandidate(0, p4, vtx);
     
      outputConvPhotonCollection.push_back(newCandidate);

      reco::SuperClusterRef scRef(reco::SuperClusterRef(scBarrelHandle, lSC));
      outputConvPhotonCollection[iSC].setSuperCluster(scRef);
      lSC++;
      iSC++;      
      myCands++;
      std::cout << " Put the ConvertedPhotonCollection a candidate in the Barrel " << std::endl;

      //    }





  }


  //  Loop over SC in the Endcap and reconstruct converted photons


  lSC=0; // reset local index for endcap
  for(aClus = scEndcapCollection.begin(); aClus != scEndcapCollection.end(); aClus++) {
  
    //    if ( abs( aClus->eta() ) > 0.9 ) return; 
    std::cout << "  ConvertedPhotonProducer SC eta " <<  aClus->eta() << " phi " <<  aClus->phi() << std::endl;

    //    theOutInSeedFinder_->setCandidate(*aClus);
    // theOutInSeedFinder_->makeSeeds(  clusterCollectionEndcap );
  
    //    std::vector<Trajectory> theOutInTracks= theOutInTrackFinder_->tracks(theOutInSeedFinder_->seeds());     


    //    theInOutSeedFinder_->setCandidate(*aClus);
    //theInOutSeedFinder_->setTracks(  theOutInTracks );   
    //theInOutSeedFinder_->makeSeeds(  clusterCollectionEndcap );

    //    std::vector<Trajectory> theInOutTracks= theInOutTrackFinder_->tracks(theInOutSeedFinder_->seeds());     

    // Define candidates with tracks
    //    std::cout << "  ConvertedPhotonProducer theOutInTracks.size() " << theOutInTracks.size() << " theInOutTracks.size() " << theInOutTracks.size() << std::endl;

    //    for (std::vector<Trajectory>::const_iterator it = theOutInTracks.begin(); it !=  theOutInTracks.end(); it++) {
    // std::cout << " ConvertedPhotonProducer OutIn Tracks Number of hits " << (*it).foundHits() << std::endl; 
    // }

    //    for (std::vector<Trajectory>::const_iterator it = theInOutTracks.begin(); it !=  theInOutTracks.end(); it++) {
    // std::cout << " ConvertedPhotonProducer InOut Tracks Number of hits " << (*it).foundHits() << std::endl; 
      //}

    //    if ( theOutInTracks.size() ||  theOutInTracks.size() ) {

      // Track cleaning
      // Track pairing
      // vertex finding
      // final candidate 
      const reco::Particle::Point  vtx( 0, 0, 0 );
      // Possibly correct the vertex position from the tracks
      math::XYZVector direction =aClus->position() - vtx;
      math::XYZVector momentum = direction.unit() * aClus->energy();
      // Possibly compute the momentum from the tracks

      const reco::Particle::LorentzVector  p4(momentum.x(), momentum.y(), momentum.z(), aClus->energy() );

      reco::ConvertedPhoton  newCandidate(0, p4, vtx);
     
      outputConvPhotonCollection.push_back(newCandidate);

      reco::SuperClusterRef scRef(reco::SuperClusterRef(scEndcapHandle, lSC));
      outputConvPhotonCollection[iSC].setSuperCluster(scRef);
      lSC++;
      iSC++;      
      myCands++;
      std::cout << " Put the ConvertedPhotonCollection a candidate in the Endcap  " << std::endl;

      //    }

  }



  // put the product in the event

  //  std::cout << "  ConvertedPhotonProducer   Putting in the event " << (*outInTrackCandidate_p).size() << " Out In track Candidates " << std::endl;
  //theEvent.put( outInTrackCandidate_p, OutInTrackCandidateCollection_  );

  //  std::cout << "  ConvertedPhotonProducer Putting in the event  " << (*inOutTrackCandidate_p).size() << " In Out track Candidates " <<  std::endl;
  //theEvent.put( inOutTrackCandidate_p, InOutTrackCandidateCollection_ );


  
  outputConvPhotonCollection_p->assign(outputConvPhotonCollection.begin(),outputConvPhotonCollection.end());
  std::cout << "  ConvertedPhotonProducer Putting in the event  " << myCands << "  converted photon candidates " << (*outputConvPhotonCollection_p).size() << std::endl;  
  theEvent.put( outputConvPhotonCollection_p, ConvertedPhotonCollection_);





}
