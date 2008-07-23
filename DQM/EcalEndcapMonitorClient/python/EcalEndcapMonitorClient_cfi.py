import FWCore.ParameterSet.Config as cms

ecalEndcapMonitorClient = cms.EDFilter("EcalEndcapMonitorClient",
    inputFile = cms.untracked.string(''),
    dbName = cms.untracked.string(''),
    dbHostName = cms.untracked.string(''),
    dbHostPort = cms.untracked.int32(1521),
    dbUserName = cms.untracked.string(''),
    dbPassword = cms.untracked.string(''),
    dbTagName = cms.untracked.string('CMSSW'),
    maskFile = cms.untracked.string('maskfile.dat'),
    mergeRuns = cms.untracked.bool(False),
    location = cms.untracked.string(''),
    baseHtmlDir = cms.untracked.string(''),
    cloneME = cms.untracked.bool(True),
    updateTime = cms.untracked.int32(0),
    dbUpdateTime = cms.untracked.int32(0),
    htmlUpdateTime = cms.untracked.int32(0),
    enabledClients = cms.untracked.vstring('Integrity', 
        'Occupancy', 
        'StatusFlags', 
        'PedestalOnline', 
        'Pedestal', 
        'TestPulse', 
        'Laser', 
        'Led', 
        'Timing', 
        'Cosmic', 
        'BeamCalo', 
        'BeamHodo', 
        'Summary'),
    enableMonitorDaemon = cms.untracked.bool(False),
    prefixME = cms.untracked.string('EcalEndcap'),
    enableCleanup = cms.untracked.bool(False),
    enableUpdate = cms.untracked.bool(False),
    clientName = cms.untracked.string('EcalEndcapMonitorClient'),
    hostName = cms.untracked.string('localhost'),
    hostPort = cms.untracked.int32(9090),
    superModules = cms.untracked.vint32(1, 2, 3, 4, 5, 
        6, 7, 8, 9, 10, 
        11, 12, 13, 14, 15, 
        16, 17, 18),
    verbose = cms.untracked.bool(True),
    debug = cms.untracked.bool(False),
    prescaleFactor = cms.untracked.int32(1)
)

