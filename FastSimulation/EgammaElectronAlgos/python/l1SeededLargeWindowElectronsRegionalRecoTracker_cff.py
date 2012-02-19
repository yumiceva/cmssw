import FWCore.ParameterSet.Config as cms

# Take all pixel tracks but the potential electrons
hltL1SeededLargeWindowElectronsRegionalCTFFinalFitWithMaterial = cms.EDProducer("FastTrackMerger",
    SaveTracksOnly = cms.untracked.bool(True),
    TrackProducers = cms.VInputTag(cms.InputTag("globalPixelWithMaterialTracksForElectrons"),
                                   cms.InputTag("globalPixelTrackCandidatesForElectrons"))
)

# The sequence
HLTL1SeededLargeWindowElectronsRegionalRecoTrackerSequence = cms.Sequence(cms.SequencePlaceholder("globalPixelTracking")+
                                                                          hltL1SeededLargeWindowElectronsRegionalCTFFinalFitWithMaterial)

