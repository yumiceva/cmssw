#ifndef _PixelVZeroFinder_h_
#define _PixelVZeroFinder_h_

#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"

#include "DataFormats/VZero/interface/VZero.h"
#include "DataFormats/VZero/interface/VZeroFwd.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "TrackingTools/TrajectoryState/interface/FreeTrajectoryState.h"

#include "MagneticField/Engine/interface/MagneticField.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"

#include "TrackingTools/PatternTools/interface/ClosestApproachInRPhi.h"

class PixelVZeroFinder
{
 public:
   PixelVZeroFinder(const edm::EventSetup& es,
                    const edm::ParameterSet& pset);
   ~PixelVZeroFinder();

   bool checkTrackPair(const reco::Track& posTrack,
                       const reco::Track& negTrack,
                       reco::VZeroData& data);

 private:
   FreeTrajectoryState getTrajectory(const reco::Track& track);

   ClosestApproachInRPhi * theApproach;

   float maxDcaR,
         maxDcaZ,
         minCrossingRadius,
         maxCrossingRadius,
         maxImpactMother; 

   const MagneticField * theMagField;
};

#endif

