import FWCore.ParameterSet.Config as cms

process = cms.Process("GeometryXMLWriter")
process.load("CondCore.DBCommon.CondDBCommon_cfi")
process.load('Configuration.Geometry.GeometryExtendedPostLS2_cff')
process.trackerNumberingGeometry.fromDDD = cms.bool( True )
process.trackerNumberingGeometry.layerNumberPXB = cms.uint32(18)
process.trackerNumberingGeometry.totalBlade = cms.uint32(56)

process.source = cms.Source("EmptyIOVSource",
                            lastValue = cms.uint64(1),
                            timetype = cms.string('runnumber'),
                            firstValue = cms.uint64(1),
                            interval = cms.uint64(1)
                            )

process.BigXMLWriter = cms.EDAnalyzer("OutputDDToDDL",
                              rotNumSeed = cms.int32(0),
                              fileName = cms.untracked.string("./geSingleBigFile.xml")
                              )


process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1)
    )

process.p1 = cms.Path(process.BigXMLWriter)

