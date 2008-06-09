import FWCore.ParameterSet.Config as cms

cscRecHitValidation = cms.EDFilter("CSCRecHitValidation",
    simHitsTag = cms.InputTag("g4SimHitsMuonCSCHits"),
    outputFile = cms.string('CSCRecHitValidation.root'),
    recHitLabel = cms.InputTag("csc2DRecHits"),
    segmentLabel = cms.InputTag("cscSegments")
)


