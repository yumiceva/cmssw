import FWCore.ParameterSet.Config as cms

OutputCommands = cms.untracked.vstring(
    "drop *",
    "keep *_*_*_sampleProducer",
    "keep *_TriggerResults_*_HLT",
    "keep *_*_Tau_HLT",
    "keep recoCaloJets_*_*_HLT",
    "keep recoIsolatedTauTagInfos_*_*_HLT",
    "keep recoJetedmRefToBaseProdTofloatsAssociationVector_*_*_HLT",
    "keep *_hltCtfWithMaterialTracksL3SingleTau_*_HLT",
    "keep *_htlPixelTracks_*_HLT",
    "keep recoCaloTaus_*_*_RECO",
    "keep recoCaloTauTagInfos_*_*_RECO",
    "keep recoCaloTauDiscriminatorByIsolation_*_*_RECO",
    "keep recoPFTaus_*_*_RECO",
    "keep recoPFTauTagInfos_*_*_RECO",
    "keep recoPFTauDiscriminatorByIsolation_*_*_RECO",
    "keep recoTracks_pixelTracks_*_RECO",
    "keep recoVertexs_*_*_*",
    "drop *_TriggerResults_*_sampleProducer",
    "drop *_hltBLifetimeHighestEtJets_*_*",
    "drop *_hltBLifetimeL25Jets_*_*",
    "drop *_hltBLifetimeL3Jets_*_*",
    "drop *_hltBSoftmuonHighestEtJets_*_*",
    "drop *_hltBSoftmuonL25Jets_*_*",
    "drop *_hltBLifetimeL25BJetTags_*_*",
    "drop *_hltBLifetimeL3BJetTags_*_*",
    "drop *_hltBSoftmuonL25BJetTags_*_*",
    "drop *_hltBSoftmuonL3BJetTagsByDR_*_*",
    "drop *_hltBSoftmuonL3BJetTags_*_*",
    "keep *_source_*_*"
)

