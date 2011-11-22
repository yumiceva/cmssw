import FWCore.ParameterSet.Config as cms

process = cms.Process("myprocess")
process.load("CondCore.DBCommon.CondDBCommon_cfi")

process.maxEvents = cms.untracked.PSet(
        input = cms.untracked.int32(1)
        )

process.source = cms.Source("EmptySource")

process.PoolDBESSource = cms.ESSource("PoolDBESSource",
      DBParameters = cms.PSet(
        messageLevel = cms.untracked.int32(0)
        ),
      timetype = cms.string('runnumber'),
      toGet = cms.VPSet(
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec10V1_AK5Calo'),
            label  = cms.untracked.string('AK5CaloLocal')
            ),
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec10V1_AK5PF'),
            label  = cms.untracked.string('AK5PFLocal')
            ),
      cms.PSet(
            record = cms.string('JetCorrectionsRecord'),
            tag    = cms.string('JetCorrectorParametersCollection_Jec10V1_AK5JPT'),
            label  = cms.untracked.string('AK5JPTLocal')
            ),                                                                                
       ),
      connect = cms.string('sqlite:Jec10V1.db')
)


process.demo1 = cms.EDAnalyzer('JetCorrectorDBReader', 
        payloadName    = cms.untracked.string('AK5CaloLocal'),
        printScreen    = cms.untracked.bool(False),
        createTextFile = cms.untracked.bool(True),
        globalTag      = cms.untracked.string('Jec10V1')
)


process.demo2 = cms.EDAnalyzer('JetCorrectorDBReader', 
        payloadName    = cms.untracked.string('AK5PFLocal'),
        printScreen    = cms.untracked.bool(False),
        createTextFile = cms.untracked.bool(False),
        globalTag      = cms.untracked.string('Jec10V1')
)

process.demo3 = cms.EDAnalyzer('JetCorrectorDBReader', 
        payloadName    = cms.untracked.string('AK5JPTLocal'),
        printScreen    = cms.untracked.bool(False),
        createTextFile = cms.untracked.bool(False),
        globalTag      = cms.untracked.string('Jec10V1')                               
)

process.p = cms.Path(process.demo1 * process.demo2 * process.demo3)
