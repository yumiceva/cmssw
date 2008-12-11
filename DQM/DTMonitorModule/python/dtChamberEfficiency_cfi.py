import FWCore.ParameterSet.Config as cms

from RecoMuon.TrackingTools.MuonServiceProxy_cff import * 
from RecoMuon.TrackingTools.MuonTrackLoader_cff import *

dtEfficiencyMonitor = cms.EDAnalyzer("DTChamberEfficiency",
    MuonServiceProxy,
    debug = cms.untracked.bool(True),
    TrackCollection = cms.InputTag("cosmicMuons1LegBarrelOnly"),                                 
    theMaxChi2 = cms.double(50.),
    theNSigma = cms.double(2.),
    theMinNrec = cms.double(1.),
    dt4DSegments = cms.InputTag("dt4DSegments"),
    theRPCRecHits = cms.InputTag("dummy"),
    cscSegments = cms.InputTag("dummy"),
    RPCLayers = cms.bool(False),
    NavigationType = cms.string("Direct")
)


