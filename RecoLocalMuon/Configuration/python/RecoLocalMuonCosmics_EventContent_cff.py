# The following comments couldn't be translated into the new config version:

# DT 

# no Drift algo

# CSC

# RPC

# DT

# no Drift algo

# CSC

# RPC

import FWCore.ParameterSet.Config as cms

# Full Event content 
RecoLocalMuonFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DRecHitsNoDrift_*_*', 
        'keep *_dt4DSegmentsNoDrift_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*')
)
# RECO content
RecoLocalMuonRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_dt1DRecHits_*_*', 
        'keep *_dt4DSegments_*_*', 
        'keep *_dt1DRecHitsNoDrift_*_*', 
        'keep *_dt4DSegmentsNoDrift_*_*', 
        'keep *_csc2DRecHits_*_*', 
        'keep *_cscSegments_*_*', 
        'keep *_rpcRecHits_*_*')
)
# AOD content
RecoLocalMuonAOD = cms.PSet(
    outputCommands = cms.untracked.vstring()
)


