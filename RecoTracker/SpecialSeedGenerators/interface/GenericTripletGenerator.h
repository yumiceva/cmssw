#ifndef SpecialSeedGenerators_GenericTripletGenerator_h
#define SpecialSeedGenerators_GenericTripletGenerator_h
//FWK
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "RecoTracker/TkTrackingRegions/interface/OrderedHitsGenerator.h"
#include "RecoTracker/TkTrackingRegions/interface/TrackingRegion.h"
#include "RecoPixelVertexing/PixelTriplets/interface/OrderedHitTriplets.h"
#include "RecoTracker/TkSeedingLayers/interface/SeedingLayerSets.h"
#include "RecoTracker/TkSeedingLayers/interface/OrderedSeedingHits.h"

class GenericTripletGenerator : public OrderedHitsGenerator {
	public:
	GenericTripletGenerator(const edm::ParameterSet& conf);
	virtual ~GenericTripletGenerator(){};
	virtual const OrderedSeedingHits& run(const TrackingRegion& region, 
					      const edm::Event & ev, 
					      const edm::EventSetup& es);
	private:
	ctfseeding::SeedingLayerSets init(const edm::EventSetup& es);
	edm::ParameterSet conf_;
	OrderedHitTriplets hitTriplets;
};


#endif
