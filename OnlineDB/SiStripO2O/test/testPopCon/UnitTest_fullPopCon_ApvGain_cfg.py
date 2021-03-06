import FWCore.ParameterSet.Config as cms

process = cms.Process("o2o")

process.MessageLogger = cms.Service("MessageLogger",
    destinations = cms.untracked.vstring('cout')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
)
process.source = cms.Source("EmptySource",
    numberEventsInRun = cms.untracked.uint32(1),
    firstRun = cms.untracked.uint32(1)
)


process.load("OnlineDB.SiStripConfigDb.SiStripConfigDb_cfi")
process.SiStripConfigDb.UsingDb = True
process.SiStripConfigDb.ConfDb = ''
process.SiStripConfigDb.Partitions = cms.untracked.PSet(
    PartTIBD = cms.untracked.PSet(
                ForceCurrentState = cms.untracked.bool(False),
                ForceVersions = cms.untracked.bool(True),
                PartitionName = cms.untracked.string('TI_27-JAN-2010_2'),
                RunNumber = cms.untracked.uint32(127631),
                CablingVersion = cms.untracked.vuint32(74, 0),
                FecVersion = cms.untracked.vuint32(630, 0),
                FedVersion = cms.untracked.vuint32(825, 0),
                DcuDetIdsVersion = cms.untracked.vuint32(9, 10),
                DcuPsuMapVersion = cms.untracked.vuint32(0, 0),
                MaskVersion      = cms.untracked.vuint32(94, 0),
                ApvTimingVersion   = cms.untracked.vuint32(9, 0)
                )
    )
process.SiStripConfigDb.TNS_ADMIN = ''

process.SiStripDetInfoFileReader = cms.Service("SiStripDetInfoFileReader")

process.load("OnlineDB.SiStripO2O.SiStripO2OCalibrationFactors_cfi")
process.SiStripCondObjBuilderFromDb = cms.Service("SiStripCondObjBuilderFromDb",
    process.SiStripO2OCalibrationFactors
)
process.SiStripCondObjBuilderFromDb.UseAnalysis = True

process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.CondDBCommon.connect = 'sqlite_file:dbfile.db'
process.CondDBCommon.DBParameters.messageLevel = 4
process.CondDBCommon.DBParameters.authenticationPath = '/afs/cern.ch/cms/DB/conddb'
process.PoolDBOutputService = cms.Service("PoolDBOutputService",
    process.CondDBCommon,
    BlobStreamerName = cms.untracked.string('TBufferBlobStreamingService'),
    logconnect = cms.untracked.string('sqlite_file:log.db'),
    timetype = cms.untracked.string('runnumber'),
    toPut = cms.VPSet(cms.PSet(
        record = cms.string('SiStripApvGainRcd'),
        tag = cms.string('SiStripApvGain_test')
    ))
)

process.CommonSiStripPopConParams = cms.PSet(
    SinceAppendMode = cms.bool(True),
    Source = cms.PSet(
        since = cms.untracked.uint32(1),
        name = cms.untracked.string('default'),
        debug = cms.untracked.bool(True)
    ),
    loggingOn = cms.untracked.bool(True)
)

process.siStripPopConApvGain = cms.EDAnalyzer("SiStripPopConApvGain",
    process.CommonSiStripPopConParams,
    record = cms.string('SiStripApvGainRcd')
)
process.siStripPopConApvGain.Source.name = 'siStripPopConApvGain'


process.pped = cms.Path(process.siStripPopConApvGain)


