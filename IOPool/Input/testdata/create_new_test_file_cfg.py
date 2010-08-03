import FWCore.ParameterSet.Config as cms
import os

process = cms.Process("TEST")

process.source = cms.Source("EmptySource",
                            numberEventsInLuminosityBlock = cms.untracked.uint32(3),
                            numberEventsInRun= cms.untracked.uint32(6))
process.maxEvents = cms.untracked.PSet(input=cms.untracked.int32(10))

process.i =cms.EDProducer("IntProducer", ivalue = cms.int32(4))

process.prd = cms.Path(process.i)
process.out = cms.OutputModule("PoolOutputModule",
                               fileName = cms.untracked.string("old_format_"+os.environ['CMSSW_VERSION']+".root"))
process.o = cms.EndPath(process.out)
