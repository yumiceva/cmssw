import FWCore.ParameterSet.Config as cms

from RecoHI.HiEgammaAlgos.HiCaloIsolation_cff import *
from RecoHI.HiEgammaAlgos.HiTrackerIsolation_cff import *

from RecoEgamma.EgammaIsolationAlgos.gamIsolationSequence_cff import *
from RecoEgamma.PhotonIdentification.photonId_cff import *

from RecoEcal.EgammaClusterProducers.reducedRecHitsSequence_cff import *

interestingEcalDetIdEB.basicClustersLabel = cms.InputTag("islandBasicClusters","islandBarrelBasicClusters")
interestingEcalDetIdEE.basicClustersLabel = cms.InputTag("islandBasicClusters","islandEndcapBasicClusters")

reducedEcalRecHitsEB.interestingDetIdCollections = cms.VInputTag(
             # ecal
             cms.InputTag("interestingEcalDetIdEB"),
             cms.InputTag("interestingEcalDetIdEBU"),
             # egamma
             #cms.InputTag("interestingEleIsoDetIdEB"),
             #cms.InputTag("interestingGamIsoDetIdEB"),
             # tau
             #cms.InputTag("caloRecoTauProducer"),
             #pf
             #cms.InputTag("pfElectronInterestingEcalDetIdEB"),
             #cms.InputTag("pfPhotonInterestingEcalDetIdEB"),
             # muons
             #cms.InputTag("muonEcalDetIds"),
             # high pt tracks
             #cms.InputTag("interestingTrackEcalDetIds")
             )

reducedEcalRecHitsEE.interestingDetIdCollections = cms.VInputTag(
             # ecal
             cms.InputTag("interestingEcalDetIdEB"),
             cms.InputTag("interestingEcalDetIdEBU"),
             # egamma
             #cms.InputTag("interestingEleIsoDetIdEB"),
             #cms.InputTag("interestingGamIsoDetIdEB"),
             # tau
             #cms.InputTag("caloRecoTauProducer"),
             #pf
             #cms.InputTag("pfElectronInterestingEcalDetIdEB"),
             #cms.InputTag("pfPhotonInterestingEcalDetIdEB"),
             # muons
             #cms.InputTag("muonEcalDetIds"),
             # high pt tracks
             #cms.InputTag("interestingTrackEcalDetIds")
             )


gamIsoDepositEcalFromHits.ExtractorPSet.barrelEcalHits = cms.InputTag("reducedEcalRecHitsEB")
gamIsoDepositEcalFromHits.ExtractorPSet.endcapEcalHits = cms.InputTag("reducedEcalRecHitsEE")

gamIsoDepositTk.ExtractorPSet.inputTrackCollection = isolationInputParameters.track

hiEgammaIsolationSequenceAll = cms.Sequence(hiCaloIsolationAll+hiTrackerIsolation)
hiEgammaIsolationSequence = cms.Sequence(hiCaloIsolationBckSubtracted+hiTrackerIsolation+photonIDSequence+reducedEcalRecHitsSequence*gamIsolationSequence)


