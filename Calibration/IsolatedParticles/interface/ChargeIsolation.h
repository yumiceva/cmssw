// -*- C++ -*
/* 
Functions to define isolation with respect to charge particles

Authors:  Seema Sharma, Sunanda Banerjee
Created: August 2009
*/

#ifndef CalibrationIsolatedParticleseChargeIsolation_h
#define CalibrationIsolatedParticleseChargeIsolation_h

// system include files
#include <memory>
#include <cmath>
#include <string>
#include <map>
#include <vector>

// user include files
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "Geometry/CaloGeometry/interface/CaloGeometry.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloTopology/interface/HcalTopology.h"

#include "TrackingTools/TrackAssociator/interface/TrackDetectorAssociator.h"
#include "RecoCaloTools/Navigation/interface/CaloNavigator.h"

namespace spr{

  double chargeIsolation(const edm::Event& iEvent, const edm::EventSetup& iSetup, CaloNavigator<DetId>& navigator, reco::TrackCollection::const_iterator trkItr, edm::Handle<reco::TrackCollection> trkCollection, const CaloSubdetectorGeometry* gEB, const CaloSubdetectorGeometry* gEE, TrackDetectorAssociator& associator, TrackAssociatorParameters& parameters_, int ieta, int iphi, std::string& theTrackQuality, bool debug=false);
  bool   chargeIsolation(const DetId anyCell, CaloNavigator<DetId>& navigator, int deta, int dphi);

  double chargeIsolationEcal(const edm::Event& iEvent, const edm::EventSetup& iSetup, const DetId& coreDet, reco::TrackCollection::const_iterator trkItr, edm::Handle<reco::TrackCollection> trkCollection, const CaloGeometry* geo, const CaloTopology* caloTopology, TrackDetectorAssociator& associator, TrackAssociatorParameters& parameters_, int ieta, int iphi, std::string& theTrackQuality, bool debug=false);

  double chargeIsolationHcal(const edm::Event& iEvent, const edm::EventSetup& iSetup, reco::TrackCollection::const_iterator trkItr, edm::Handle<reco::TrackCollection> trkCollection, const DetId ClosestCell, const HcalTopology* topology, const CaloSubdetectorGeometry* gHB, TrackDetectorAssociator& associator, TrackAssociatorParameters& parameters_, int ieta, int iphi, std::string& theTrackQuality, bool debug=false);

  bool chargeIsolation(const DetId anyCell, std::vector<DetId>& vdets) ;
}

#endif
