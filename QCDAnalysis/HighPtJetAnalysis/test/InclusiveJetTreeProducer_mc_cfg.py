import FWCore.ParameterSet.Config as cms

process = cms.Process("Ana")
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.StandardSequences.MagneticField_38T_cff')
process.load('RecoJets.JetAssociationProducers.ak7JTA_cff') 
process.load('RecoJets.JetAssociationProducers.ak5JTA_cff')
process.load('RecoJets.JetAssociationProducers.iterativeCone5JTA_cff')
process.load('Configuration/StandardSequences/FrontierConditions_GlobalTag_cff')
#############   Set the number of events #############
process.maxEvents = cms.untracked.PSet(
     input = cms.untracked.int32(-1)
)
#############   Define the source file ###############
process.source = cms.Source("PoolSource",
    fileNames = cms.untracked.vstring('/store/mc/Summer09/MinBias/GEN-SIM-RECO/V16D_900GeV-v1/0000/584D6949-F116-DF11-803F-00E081791787.root')
)
#############   inclusive jet tree producer ##################
process.TFileService = cms.Service("TFileService",fileName = cms.string('InclusiveJetTreeMC.root'))

# Other statements
process.GlobalTag.globaltag = 'START3X_V16D::All'


process.ak5calo = cms.EDAnalyzer('InclusiveJetTreeProducer',
    jets                   = cms.string('ak5CaloJets'), 
    jetsID                 = cms.string('ak5JetID'),
    jetExtender            = cms.string('ak5JetExtender'),
    met                    = cms.string('met'),
    metNoHF                = cms.string('metNoHF'),
    hcalNoiseTag           = cms.InputTag('hcalnoise'),
    minJetPt               = cms.double(3.0), 
    jetTriggerNames        = cms.vstring(),
#    jetTriggerNames        = cms.vstring('HLT_MinBiasBSC','HLT_L1Jet6U','HLT_Jet15U','HLT_Jet30U','HLT_Jet50U','HLT_DiJetAve15U_8E29','HLT_DiJetAve30U_8E29','HLT_QuadJet15U','HLT_CSCBeamHalo','HLT_SplashBSC','HLT_ForwardBSC','HLT_BackwardBSC','HLT_HighMultiplicityBSC','HLT_GlobalRunHPDNoise','HLT_PhysicsDeclared'),
    triggerProcessName     = cms.string('HLT'), 
    triggerResultsTag      = cms.InputTag('TriggerResults','','HLT'),
    l1TriggerNames         = cms.vstring(),
#    A pile of BeamCommisioning09 trigger paths
#    l1TriggerNames         = cms.vstring('L1Tech_BSC_minBias_threshold1.v0','L1Tech_BSC_minBias_threshold2.v0','L1Tech_BSC_splash_beam1.v0','L1Tech_BSC_splash_beam2.v0','L1_SingleJet6','L1_SingleJet10','L1_SingleJet20','L1_SingleJet30','L1_SingleJet40','L1_SingleJet50','L1_SingleJet60'),     
    L1GTReadoutRcdSource   = cms.InputTag('gtDigis'),
    L1GTObjectMapRcdSource = cms.InputTag('hltL1GtObjectMap','','HLT'),
    ########### Only in MC ################################
    isMCarlo               = cms.untracked.bool(True),
    genjets                = cms.untracked.string('ak5GenJets')
)

process.ak7calo = process.ak5calo.clone(jets = 'ak7CaloJets', jetsID = 'ak7JetID', jetExtender = 'ak7JetExtender', genjets='ak7GenJets')
#process.iterativeCone = process.ak5calo.clone(jets = 'iterativeCone5CaloJets', jetsID = 'ic5JetID', jetExtender = 'iterativeCone5JetExtender',genjets = 'iterativeCone5GenJets')

#process.p = cms.Path(process.ak5JTA * process.ak5calo * process.ak7JTA * process.ak7calo * process.iterativeCone5JTA * process.iterativeCone)
process.p = cms.Path(process.ak5JTA * process.ak5calo * process.ak7JTA * process.ak7calo)
#############   Format MessageLogger #################
process.MessageLogger.cerr.FwkReport.reportEvery = 100

