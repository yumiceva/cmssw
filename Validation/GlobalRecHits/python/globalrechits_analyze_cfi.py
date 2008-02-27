import FWCore.ParameterSet.Config as cms

globalrechitsanalyze = cms.EDAnalyzer("GlobalRecHitsAnalyzer",
    MuDTSrc = cms.InputTag("dt1DRecHits","",""),
    SiPxlSrc = cms.InputTag("siPixelRecHits","",""),
    VtxUnit = cms.untracked.int32(1),
    associateRecoTracks = cms.bool(False),
    MuDTSimSrc = cms.InputTag("g4SimHits","MuonDTHits",""),
    associatePixel = cms.bool(True),
    ROUList = cms.vstring('TrackerHitsTIBLowTof','TrackerHitsTIBHighTof','TrackerHitsTIDLowTof','TrackerHitsTIDHighTof','TrackerHitsTOBLowTof','TrackerHitsTOBHighTof','TrackerHitsTECLowTof','TrackerHitsTECHighTof','TrackerHitsPixelBarrelLowTof','TrackerHitsPixelBarrelHighTof','TrackerHitsPixelEndcapLowTof','TrackerHitsPixelEndcapHighTof'),
    ECalEESrc = cms.InputTag("ecalRecHit","EcalRecHitsEE",""),
    MuRPCSimSrc = cms.InputTag("g4SimHits","MuonRPCHits",""),
    SiStripSrc = cms.InputTag("siStripMatchedRecHits","matchedRecHit",""),
    HCalSrc = cms.InputTag("g4SimHits","HcalHits",""),
    ECalESSrc = cms.InputTag("ecalPreshowerRecHit","EcalRecHitsES",""),
    outputFile = cms.string('GlobalRecHitsHistogramsAnalyze.root'),
    ECalUncalEESrc = cms.InputTag("ecalWeightUncalibRecHit","EcalUncalibRecHitsEE",""),
    Name = cms.untracked.string('GlobalRecHitsAnalyzer'),
    Verbosity = cms.untracked.int32(0),
    associateStrip = cms.bool(True),
    MuRPCSrc = cms.InputTag("rpcRecHits","",""),
    ECalUncalEBSrc = cms.InputTag("ecalWeightUncalibRecHit","EcalUncalibRecHitsEB",""),
    MuCSCSrc = cms.InputTag("csc2DRecHits","",""),
    ProvenanceLookup = cms.PSet(
        PrintProvenanceInfo = cms.untracked.bool(False),
        GetAllProvenances = cms.untracked.bool(False)
    ),
    Frequency = cms.untracked.int32(50),
    ECalEBSrc = cms.InputTag("ecalRecHit","EcalRecHitsEB","")
)




