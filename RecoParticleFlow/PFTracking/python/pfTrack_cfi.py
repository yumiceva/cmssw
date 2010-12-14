import FWCore.ParameterSet.Config as cms

pfTrack = cms.EDProducer("PFTrackProducer",
    TrackQuality = cms.string('highPurity'),
    UseQuality = cms.bool(True),
    GsfTrackModuleLabel = cms.InputTag("electronGsfTracks"),
    TkColList = cms.VInputTag(cms.InputTag("generalTracks")),
    MuColl = cms.InputTag("muons"),
    TrajInEvents = cms.bool(False)
)


