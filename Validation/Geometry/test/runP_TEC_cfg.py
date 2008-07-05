import FWCore.ParameterSet.Config as cms

process = cms.Process("PROD")
#Geometry
#
process.load("Geometry.CMSCommonData.cmsSimIdealGeometryXML_cfi")

process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")

#Magnetic Field
#
process.load("Configuration.StandardSequences.MagneticField_38T_cff")

# Output of events, etc...
#
# Explicit note : since some histos/tree might be dumped directly,
#                 better NOT use PoolOutputModule !
# Detector simulation (Geant4-based)
#
process.load("SimG4Core.Application.g4SimHits_cfi")

process.RandomNumberGeneratorService = cms.Service("RandomNumberGeneratorService",
    moduleSeeds = cms.PSet(
        g4SimHits = cms.untracked.uint32(9876)
    )
)

process.MessageLogger = cms.Service("MessageLogger",
    cout = cms.untracked.PSet(
        default = cms.untracked.PSet(
            limit = cms.untracked.int32(0)
        ),
        FwkJob = cms.untracked.PSet( ## but FwkJob category - those unlimitted

            limit = cms.untracked.int32(-1)
        )
    ),
    categories = cms.untracked.vstring('FwkJob'),
    destinations = cms.untracked.vstring('cout')
)

process.source = cms.Source("MCFileSource",
    # The HepMC test File
    fileNames = cms.untracked.vstring('file:single_neutrino.random.dat')
)

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(-1)
)
process.p1 = cms.Path(process.g4SimHits)
process.g4SimHits.Generator.HepMCProductLabel = 'source'
process.g4SimHits.UseMagneticField = False
process.g4SimHits.Physics.type = 'SimG4Core/Physics/DummyPhysics'
process.g4SimHits.Physics.DummyEMPhysics = True
process.g4SimHits.Physics.CutsPerRegion = False
process.g4SimHits.Watchers = cms.VPSet(cms.PSet(
    type = cms.string('MaterialBudgetAction'),
    MaterialBudgetAction = cms.PSet(
        HistosFile = cms.string('matbdg_TEC.root'),
        AllStepsToTree = cms.bool(True),
        HistogramList = cms.string('Tracker'),
        SelectedVolumes = cms.vstring('TEC'),
        TreeFile = cms.string('None'), ## is NOT requested

        StopAfterProcess = cms.string('None'),
        # string TextFile = "matbdg_TEC.txt"
        TextFile = cms.string('None')
    )
))


