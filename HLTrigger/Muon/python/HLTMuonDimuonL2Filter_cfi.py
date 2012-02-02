import FWCore.ParameterSet.Config as cms

HLTMuonDimuonL2Filter = cms.EDFilter( "HLTMuonDimuonL2Filter",
    BeamSpotTag = cms.InputTag( "hltOfflineBeamSpot" ),
    CandTag = cms.InputTag( "hltL2MuonCandidates" ),
    PreviousCandTag = cms.InputTag( '' ),
    SeedMapTag = cms.InputTag( "hltL2Muons" ),                            
    FastAccept = cms.bool( False ),
    MaxEta = cms.double( 2.5 ),
    MinNhits = cms.int32( 0 ),
    MinNstations = cms.int32( 0 ),
    MinNchambers = cms.int32( 2 ),
    MaxDr = cms.double( 100.0 ),
    MaxDz = cms.double( 9999.0 ),
    ChargeOpt = cms.int32( 0 ),
    MinPtPair = cms.double( 0.0 ),
    MinPtMax = cms.double( 3.0 ),
    MinPtMin = cms.double( 3.0 ),
    MinInvMass = cms.double( 1.6 ),
    MaxInvMass = cms.double( 5.6 ),
    MinAcop = cms.double( -1.0 ),
    MaxAcop = cms.double( 3.15 ),
    MinAngle = cms.double( -999.0 ),
    MaxAngle = cms.double( 2.5 ),
    MinPtBalance = cms.double( -1.0 ),
    MaxPtBalance = cms.double( 999999.0 ),
    NSigmaPt = cms.double( 0.0 ),
    saveTags = cms.bool( False )
)

