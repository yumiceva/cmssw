import FWCore.ParameterSet.Config as cms

PhotonIDProd = cms.EDProducer("PhotonIDProducer",
    barrelbasicclusterProducer = cms.string('islandBasicClusters'),
    #cuts
    PhotonBCIso = cms.double(999.0),
    PhotonHcalRecHitIso = cms.double(999.0),
    DoEtaWidthCut = cms.bool(False),
    barrelEcalRecHitCollection = cms.string('EcalRecHitsEB'),
    PhotonEcalRecHitIso = cms.double(999.0),
    PhotonSolidTrk = cms.double(999.0),
    PhotonHollowNTrk = cms.int32(999),
    EcalRecHitInnerRadius = cms.double(0.1),
    photonLabel = cms.string(''),
    endcapsuperclusterCollection = cms.string(''),
    photonIDAssociationLabel = cms.string('PhotonAssociatedID'),
    photonProducer = cms.string('photons'),
    PhotonSolidNTrk = cms.int32(999),
    barrelbasiccluterCollection = cms.string('islandBarrelBasicClusters'),
    endcapbasicclusterProducer = cms.string('islandBasicClusters'),
    TrackConeInnerRadius = cms.double(0.1),
    HcalRecHitOuterRadius = cms.double(0.4),
    PhotonHadOverEM = cms.double(999.0),
    DoHollowConeNTrkCut = cms.bool(False),
    PhotonEtaWidth = cms.double(999.0),
    HcalRecHitCollection = cms.string(''),
    barrelislandsuperclusterCollection = cms.string(''),
    DoSolidConeTrackIsolationCut = cms.bool(False),
    RequireFiducial = cms.bool(False),
    isolationtrackThreshold = cms.double(0.0),
    endcapSuperClustersProducer = cms.string('correctedEndcapSuperClustersWithPreshower'),
    doCutBased = cms.bool(True),
    photonIDLabel = cms.string('PhotonIDCutBasedProducer'),
    trackProducer = cms.InputTag("generalTracks"),
    BasicClusterConeOuterRadius = cms.double(0.4),
    DoSolidConeNTrkCut = cms.bool(False),
    basicclusterProducer = cms.string('islandBasicClusters'),
    endcapEcalRecHitProducer = cms.string('ecalRecHit'),
    PhotonHollowTrk = cms.double(999.0),
    EcalRecThresh = cms.double(0.0),
    TrackConeOuterRadius = cms.double(0.4),
    HcalRecHitInnerRadius = cms.double(0.1),
    DoHollowConeTrackIsolationCut = cms.bool(False),
    isolationbasicclusterThreshold = cms.double(0.0),
    RequireNotElectron = cms.bool(False),
    barrelEcalRecHitProducer = cms.string('ecalRecHit'),
    EcalRecHitOuterRadius = cms.double(0.4),
    GsfRecoCollection = cms.InputTag("pixelMatchGsfElectrons"),
    DoHadOverEMCut = cms.bool(False),
    endcapEcalRecHitCollection = cms.string('EcalRecHitsEE'),
    HcalRecHitThresh = cms.double(0.0),
    BasicClusterConeInnerRadius = cms.double(0.0),
    endcapbasicclusterCollection = cms.string('islandEndcapBasicClusters'),
    DoEcalRecHitIsolationCut = cms.bool(False),
    #select cuts to do.
    DoBasicClusterIsolationCut = cms.bool(False),
    #end cuts
    #Algo required inputs:
    barrelislandsuperclusterProducer = cms.string('correctedIslandBarrelSuperClusters'),
    HcalRecHitProducer = cms.string('hbhereco'),
    DoHcalRecHitIsolationCut = cms.bool(False)
)


