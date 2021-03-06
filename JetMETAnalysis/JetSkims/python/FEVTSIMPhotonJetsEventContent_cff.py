import FWCore.ParameterSet.Config as cms

from Configuration.EventContent.EventContent_cff import *
from JetMETAnalysis.JetSkims.photonjets_EventContent_cff import *
FEVTSIMPhotonJetsEventContent = cms.PSet(
    outputCommands = cms.untracked.vstring()
)
FEVTSIMPhotonJetsEventContent.outputCommands.extend(FEVTSIMEventContent.outputCommands)
FEVTSIMPhotonJetsEventContent.outputCommands.extend(photonjetsEventContent.outputCommands)

