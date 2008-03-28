import FWCore.ParameterSet.Config as cms

# Calo geometry service model
from Geometry.CaloEventSetup.CaloGeometry_cff import *
#ECAL conditions
from RecoLocalCalo.EcalRecProducers.getEcalConditions_frontier_cff import *
#ECAL reconstruction
from RecoLocalCalo.EcalRecProducers.ecalWeightUncalibRecHit_cfi import *
from RecoLocalCalo.EcalRecProducers.ecalRecHit_cfi import *
from RecoLocalCalo.EcalRecProducers.ecalPreshowerRecHit_cfi import *
ecalLocalRecoSequence = cms.Sequence(ecalWeightUncalibRecHit*ecalRecHit*ecalPreshowerRecHit)

