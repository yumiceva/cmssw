import FWCore.ParameterSet.Config as cms

topDiLeptonDQM = cms.EDAnalyzer("TopDiLeptonDQM",

    moduleName = cms.untracked.string('Physics/Top/DiLepton'),
    outputFile = cms.untracked.string('DimuonEvents.txt'),
    ### 
    TriggerResults = cms.InputTag('TriggerResults','','HLT'),
    hltPaths       = cms.vstring('HLT_Mu3','HLT_IsoMu3','HLT_Mu5','HLT_Mu9','HLT_Mu15','HLT_IsoMu9','HLT_DoubleMu3',
                                 'HLT_Ele15_SW_L1R','HLT_Ele20_SW_L1R'),
    ### 
    hltPaths_sig   = cms.vstring('HLT_IsoMu9', 'HLT_Mu15', 'HLT_DoubleMu3', 'HLT_Mu9'),
    hltPaths_trig  = cms.vstring('HLT_Mu9',    'HLT_Mu9',  'HLT_Mu9',       'HLT_Mu5'),
    ### 
    vertexCollection = cms.InputTag('offlinePrimaryVertices'),
    vertex_X_cut     = cms.double(  1.0 ),
    vertex_Y_cut     = cms.double(  1.0 ),
    vertex_Z_cut     = cms.double( 20.0 ),
    ### 
    muonCollection = cms.InputTag('muons'),
    muon_pT_cut    = cms.double( 1.0 ),
    muon_eta_cut   = cms.double( 5.0 ),
    muon_iso_cut   = cms.double( 0.2 ),
    ### 
    elecCollection = cms.InputTag('gsfElectrons'),
    elec_pT_cut    = cms.double( 1.0 ),
    elec_eta_cut   = cms.double( 5.0 ),
    elec_iso_cut   = cms.double( 0.2 ),
    ### 
    MassWindow_up   = cms.double( 120. ),
    MassWindow_down = cms.double(  60. )

)

topDiLeptonAnalyzer = cms.Sequence(topDiLeptonDQM)
