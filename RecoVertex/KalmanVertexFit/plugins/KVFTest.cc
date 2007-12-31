#include "RecoVertex/KalmanVertexFit/plugins/KVFTest.h"

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/Common/interface/EDProduct.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "TrackingTools/TransientTrack/interface/TransientTrackBuilder.h"
#include "TrackingTools/Records/interface/TransientTrackRecord.h"
#include "TrackingTools/TransientTrack/interface/TransientTrack.h"
#include "RecoVertex/VertexPrimitives/interface/TransientVertex.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertError.h"
#include "RecoVertex/KalmanVertexFit/interface/KalmanVertexFitter.h"
#include "SimTracker/Records/interface/TrackAssociatorRecord.h"

#include <iostream>

using namespace reco;
using namespace edm;
using namespace std;

KVFTest::KVFTest(const edm::ParameterSet& iConfig)
  : theConfig(iConfig)
{
  trackLabel_ = iConfig.getParameter<std::string>("TrackLabel");
  outputFile_ = iConfig.getUntrackedParameter<std::string>("outputFile");
  kvfPSet = iConfig.getParameter<edm::ParameterSet>("KVFParameters");
  rootFile_ = TFile::Open(outputFile_.c_str(),"RECREATE"); 
  edm::LogInfo("RecoVertex/KVFTest") 
    << "Initializing KVF TEST analyser  - Output file: " << outputFile_ <<"\n";
}


KVFTest::~KVFTest() {
  delete rootFile_;
}

void KVFTest::beginJob(edm::EventSetup const& setup){
  edm::ESHandle<TrackAssociatorBase> theAssociatorForParamAtPca;
  setup.get<TrackAssociatorRecord>().get("TrackAssociatorByChi2",theAssociatorForParamAtPca);
  associatorForParamAtPca = (TrackAssociatorByChi2 *) theAssociatorForParamAtPca.product();

  tree = new SimpleVertexTree("VertexFitter", associatorForParamAtPca);
}


void KVFTest::endJob() {
  delete tree;
}

//
// member functions
//

void
KVFTest::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  edm::LogInfo("RecoVertex/KVFTest") 
    << "Reconstructing event number: " << iEvent.id() << "\n";
    
  // get RECO tracks from the event
  // `tks` can be used as a ptr to a reco::TrackCollection
  edm::Handle<reco::TrackCollection> tks;
  iEvent.getByLabel(trackLabel_, tks);
  if (!tks.isValid()) {
    edm::LogInfo("RecoVertex/KVFTest") 
      << "Exception during event number: " << iEvent.id()
      << "\n";
  } else {
    edm::LogInfo("RecoVertex/KVFTest") 
      << "Found: " << (*tks).size() << " reconstructed tracks" << "\n";
    cout << "got " << (*tks).size() << " tracks " << endl;
    
    // Transform Track to TransientTrack

    //get the builder:
    edm::ESHandle<TransientTrackBuilder> theB;
    iSetup.get<TransientTrackRecord>().get("TransientTrackBuilder",theB);
    //do the conversion:
    vector<TransientTrack> t_tks = (*theB).build(tks);

    edm::LogInfo("RecoVertex/KVFTest") 
      << "Found: " << t_tks.size() << " reconstructed tracks" << "\n";
    
    // Call the KalmanVertexFitter if more than 1 track
    if (t_tks.size() > 1) {
      //      KalmanVertexFitter kvf(kvfPSet);
      KalmanVertexFitter kvf(true);
      TransientVertex tv = kvf.vertex(t_tks);

      std::cout << "Position: " << Vertex::Point(tv.position()) << "\n";

      // For the analysis: compare to your SimVertex
      TrackingVertex sv = getSimVertex(iEvent);
      edm::Handle<TrackingParticleCollection>  TPCollectionH ;
      iEvent.getByLabel("trackingtruth","TrackTruth",TPCollectionH);
      if (!TPCollectionH.isValid()) {
	edm::LogInfo("RecoVertex/KVFTest") 
	  << "Exception during event number: " << iEvent.id() 
	  << "\n";
      } else {
	const TrackingParticleCollection tPC = *(TPCollectionH.product());
	reco::RecoToSimCollection recSimColl=associatorForParamAtPca->associateRecoToSim(tks,
											 TPCollectionH,
											 &iEvent);    
	tree->fill(tv, &sv, &recSimColl);
      }
    }
  }  
}

//Returns the first vertex in the list.

TrackingVertex KVFTest::getSimVertex(const edm::Event& iEvent) const
{
   // get the simulated vertices
  edm::Handle<TrackingVertexCollection>  TVCollectionH ;
  iEvent.getByLabel("trackingtruth","VertexTruth",TVCollectionH);
  const TrackingVertexCollection tPC = *(TVCollectionH.product());

//    Handle<edm::SimVertexContainer> simVtcs;
//    iEvent.getByLabel("g4SimHits", simVtcs);
//    std::cout << "SimVertex " << simVtcs->size() << std::endl;
//    for(edm::SimVertexContainer::const_iterator v=simVtcs->begin();
//        v!=simVtcs->end(); ++v){
//      std::cout << "simvtx "
// 	       << v->position().x() << " "
// 	       << v->position().y() << " "
// 	       << v->position().z() << " "
// 	       << v->parentIndex() << " "
// 	       << v->noParent() << " "
//               << std::endl;
//    }
   return *(tPC.begin());
}
