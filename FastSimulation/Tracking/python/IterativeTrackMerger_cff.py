import FWCore.ParameterSet.Config as cms

from FastSimulation.Tracking.IterativeFirstTrackMerger_cfi import *
from FastSimulation.Tracking.IterativeSecondTrackMerger_cfi import *
from FastSimulation.Tracking.IterativeThirdTrackMerger_cfi import *
iterativeTrackMerging = cms.Sequence(iterativeFirstTrackMerging+iterativeSecondTrackMerging+iterativeThirdTrackMerging)

