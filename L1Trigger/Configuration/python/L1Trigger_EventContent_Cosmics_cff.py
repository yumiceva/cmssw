import FWCore.ParameterSet.Config as cms

# RAW content 
L1TriggerRAW = cms.PSet(
    outputCommands = cms.untracked.vstring(
    'keep *_cscTriggerPrimitiveDigis_*_*', 
    'keep *_dtTriggerPrimitiveDigis_*_*', 
    'keep *_rpcTriggerDigis_*_*', 
    'keep *_rctDigis_*_*', 
    'keep *_csctfDigis_*_*', 
    'keep *_csctfTrackDigis_*_*', 
    'keep *_dttfDigis_*_*', 
    'keep *_gctDigis_*_*', 
    'keep *_gmtDigis_*_*', 
    'keep *_gtDigis_*_*', 
    'keep *_gtEvmDigis_*_*', 
    'keep *_valCscTriggerPrimitiveDigis_*_*', 
    'keep *_valDtTriggerPrimitiveDigis_*_*', 
    'keep *_valRpcTriggerDigis_*_*', 
    'keep *_valRctDigis_*_*', 
    'keep *_valCsctfDigis_*_*', 
    'keep *_valCsctfTrackDigis_*_*', 
    'keep *_valDttfDigis_*_*', 
    'keep *_valGctDigis_*_*', 
    'keep *_valGmtDigis_*_*', 
    'keep *_valGtDigis_*_*', 
    'keep *_l1GtRecord_*_*', 
    'keep *_l1GtObjectMap_*_*', 
    'keep *_l1extraParticles_*_*')
)

# RECO content
L1TriggerRECO = cms.PSet(
    outputCommands = cms.untracked.vstring(
    'keep *_cscTriggerPrimitiveDigis_*_*', 
    'keep *_dtTriggerPrimitiveDigis_*_*', 
    'keep *_rpcTriggerDigis_*_*', 
    'keep *_rctDigis_*_*', 
    'keep *_csctfDigis_*_*', 
    'keep *_csctfTrackDigis_*_*', 
    'keep *_dttfDigis_*_*', 
    'keep *_gctDigis_*_*', 
    'keep *_gmtDigis_*_*', 
    'keep *_gtDigis_*_*', 
    'keep *_gtEvmDigis_*_*', 
    'keep *_valCscTriggerPrimitiveDigis_*_*', 
    'keep *_valDtTriggerPrimitiveDigis_*_*', 
    'keep *_valRpcTriggerDigis_*_*', 
    'keep *_valRctDigis_*_*', 
    'keep *_valCsctfDigis_*_*', 
    'keep *_valCsctfTrackDigis_*_*', 
    'keep *_valDttfDigis_*_*', 
    'keep *_valGctDigis_*_*', 
    'keep *_valGmtDigis_*_*', 
    'keep *_valGtDigis_*_*', 
    'keep *_l1GtRecord_*_*', 
    'keep *_l1GtObjectMap_*_*', 
    'keep *_l1extraParticles_*_*')
)

# AOD content
L1TriggerAOD = cms.PSet(
    outputCommands = cms.untracked.vstring(
    'keep *_gtDigis_*_*', 
    'keep *_l1GtRecord_*_*', 
    'keep *_l1GtObjectMap_*_*', 
    'keep *_l1extraParticles_*_*')
)

L1TriggerFEVTDEBUG = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_simCscTriggerPrimitiveDigis_*_*', 
        'keep *_simDtTriggerPrimitiveDigis_*_*', 
        'keep *_simRpcTriggerDigis_*_*', 
        'keep *_simRctDigis_*_*', 
        'keep *_simCsctfDigis_*_*', 
        'keep *_simCsctfTrackDigis_*_*', 
        'keep *_simDttfDigis_*_*', 
        'keep *_simGctDigis_*_*', 
        'keep *_simGmtDigis_*_*', 
        'keep *_simGtDigis_*_*', 
        'keep *_cscTriggerPrimitiveDigis_*_*', 
        'keep *_dtTriggerPrimitiveDigis_*_*', 
        'keep *_rpcTriggerDigis_*_*', 
        'keep *_rctDigis_*_*', 
        'keep *_csctfDigis_*_*', 
        'keep *_csctfTrackDigis_*_*', 
        'keep *_dttfDigis_*_*', 
        'keep *_gctDigis_*_*', 
        'keep *_gmtDigis_*_*', 
        'keep *_gtDigis_*_*',
        'keep *_gtEvmDigis_*_*')
)
