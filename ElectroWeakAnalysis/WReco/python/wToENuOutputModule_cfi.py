import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
wToENuOutputModule = cms.OutputModule("PoolOutputModule",
    AODSIMEventContent,
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('wToENu_Filter'),
        dataTier = cms.untracked.string('USER')
    ),
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('wToENuHLTPath')
    ),
    fileName = cms.untracked.string('wToENu.root')
)


