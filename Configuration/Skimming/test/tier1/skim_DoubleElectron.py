# Auto generated configuration file
# using: 
# Revision: 1.372.2.4 
# Source: /local/reps/CMSSW/CMSSW/Configuration/PyReleaseValidation/python/ConfigBuilder.py,v 
# with command line options: skims -s SKIM:@DoubleElectron --data --no_exec --dbs find file,file.parent where dataset=/DoubleElectron/Run2012B-PromptReco-v1/RECO and run=194050 and lumi=500 -n 1000 --conditions auto:com10 --python_filename=skim_DoubleElectron.py
import FWCore.ParameterSet.Config as cms

process = cms.Process('SKIM')

# import of standard configurations
process.load('Configuration.StandardSequences.Services_cff')
process.load('SimGeneral.HepPDTESSource.pythiapdt_cfi')
process.load('FWCore.MessageService.MessageLogger_cfi')
process.load('Configuration.EventContent.EventContent_cff')
process.load('Configuration.StandardSequences.GeometryDB_cff')
process.load('Configuration.StandardSequences.MagneticField_AutoFromDBCurrent_cff')
process.load('Configuration.StandardSequences.Skims_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')

process.maxEvents = cms.untracked.PSet(
    input = cms.untracked.int32(1000)
)

# Input source
process.source = cms.Source("PoolSource",
    secondaryFileNames = cms.untracked.vstring('/store/data/Run2012B/DoubleElectron/RAW/v1/000/194/050/661CAF77-1D9C-E111-B6C3-5404A640A639.root'),
    fileNames = cms.untracked.vstring('/store/data/Run2012B/DoubleElectron/RECO/PromptReco-v1/000/194/050/144D7C11-309E-E111-A887-5404A638869E.root')
)

process.options = cms.untracked.PSet(

)

# Production Info
process.configurationMetadata = cms.untracked.PSet(
    version = cms.untracked.string('$Revision: 1.372.2.4 $'),
    annotation = cms.untracked.string('skims nevts:1000'),
    name = cms.untracked.string('PyReleaseValidation')
)

# Output definition

# Additional output definition
process.SKIMStreamDiTau = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('tauSkimBy2Path')
    ),
    outputCommands = (cms.untracked.vstring('drop *', 'drop *', 'keep  FEDRawDataCollection_rawDataCollector_*_*', 'keep  FEDRawDataCollection_source_*_*', 'keep  FEDRawDataCollection_rawDataCollector_*_*', 'keep  FEDRawDataCollection_source_*_*', 'drop *_hlt*_*_*', 'keep *_hltL1GtObjectMap_*_*', 'keep FEDRawDataCollection_rawDataCollector_*_*', 'keep FEDRawDataCollection_source_*_*', 'keep edmTriggerResults_*_*_*', 'keep triggerTriggerEvent_*_*_*', 'keep DetIdedmEDCollection_siStripDigis_*_*', 'keep DetIdedmEDCollection_siPixelDigis_*_*', 'keep *_siPixelClusters_*_*', 'keep *_siStripClusters_*_*', 'keep *_dt1DRecHits_*_*', 'keep *_dt4DSegments_*_*', 'keep *_dt1DCosmicRecHits_*_*', 'keep *_dt4DCosmicSegments_*_*', 'keep *_csc2DRecHits_*_*', 'keep *_cscSegments_*_*', 'keep *_rpcRecHits_*_*', 'keep *_hbhereco_*_*', 'keep *_hfreco_*_*', 'keep *_horeco_*_*', 'keep HBHERecHitsSorted_hbherecoMB_*_*', 'keep HORecHitsSorted_horecoMB_*_*', 'keep HFRecHitsSorted_hfrecoMB_*_*', 'keep ZDCDataFramesSorted_*Digis_*_*', 'keep ZDCRecHitsSorted_*_*_*', 'keep *_reducedHcalRecHits_*_*', 'keep *_castorreco_*_*', 'keep *_ecalPreshowerRecHit_*_*', 'keep *_ecalRecHit_*_*', 'keep *_ecalCompactTrigPrim_*_*', 'keep *_ecalTPSkim_*_*', 'keep *_selectDigi_*_*', 'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 'keep *_hybridSuperClusters_*_*', 'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 'keep *_multi5x5SuperClusters_*_*', 'keep recoSuperClusters_multi5x5SuperClusters_*_*', 'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 'drop recoClusterShapes_*_*_*', 'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 'keep *_CkfElectronCandidates_*_*', 'keep *_GsfGlobalElectronTest_*_*', 'keep *_electronMergedSeeds_*_*', 'keep recoGsfTracks_electronGsfTracks_*_*', 'keep recoGsfTrackExtras_electronGsfTracks_*_*', 'keep recoTrackExtras_electronGsfTracks_*_*', 'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 'keep recoTracks_generalTracks_*_*', 'keep recoTrackExtras_generalTracks_*_*', 'keep TrackingRecHitsOwned_generalTracks_*_*', 'keep recoTracks_beamhaloTracks_*_*', 'keep recoTrackExtras_beamhaloTracks_*_*', 'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 'keep recoTracks_regionalCosmicTracks_*_*', 'keep recoTrackExtras_regionalCosmicTracks_*_*', 'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 'keep recoTracks_rsWithMaterialTracks_*_*', 'keep recoTrackExtras_rsWithMaterialTracks_*_*', 'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 'keep recoTracks_conversionStepTracks_*_*', 'keep recoTrackExtras_conversionStepTracks_*_*', 'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 'keep *_ctfPixelLess_*_*', 'keep *_dedxTruncated40_*_*', 'keep *_dedxDiscrimASmi_*_*', 'keep *_dedxHarmonic2_*_*', 'keep *_trackExtrapolator_*_*', 'keep *_kt4CaloJets_*_*', 'keep *_kt6CaloJets_*_*', 'keep *_ak5CaloJets_*_*', 'keep *_ak7CaloJets_*_*', 'keep *_iterativeCone5CaloJets_*_*', 'keep *_iterativeCone15CaloJets_*_*', 'keep *_kt4PFJets_*_*', 'keep *_kt6PFJets_*_*', 'keep *_ak5PFJets_*_*', 'keep *_ak7PFJets_*_*', 'keep *_iterativeCone5PFJets_*_*', 'keep *_JetPlusTrackZSPCorJetAntiKt5_*_*', 'keep *_ak5TrackJets_*_*', 'keep *_kt4TrackJets_*_*', 'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 'keep *_caloTowers_*_*', 'keep *_towerMaker_*_*', 'keep *_CastorTowerReco_*_*', 'keep *_ic5JetTracksAssociatorAtVertex_*_*', 'keep *_iterativeCone5JetTracksAssociatorAtVertex_*_*', 'keep *_iterativeCone5JetTracksAssociatorAtCaloFace_*_*', 'keep *_iterativeCone5JetExtender_*_*', 'keep *_kt4JetTracksAssociatorAtVertex_*_*', 'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 'keep *_kt4JetExtender_*_*', 'keep *_ak5JetTracksAssociatorAtVertex_*_*', 'keep *_ak5JetTracksAssociatorAtCaloFace_*_*', 'keep *_ak5JetExtender_*_*', 'keep *_ak7JetTracksAssociatorAtVertex_*_*', 'keep *_ak7JetTracksAssociatorAtCaloFace_*_*', 'keep *_ak7JetExtender_*_*', 'keep *_ak5JetID_*_*', 'keep *_ak7JetID_*_*', 'keep *_ic5JetID_*_*', 'keep *_kt4JetID_*_*', 'keep *_kt6JetID_*_*', 'keep *_ak7BasicJets_*_*', 'keep *_ak7CastorJetID_*_*', 'keep double_kt6CaloJetsCentral_*_*', 'keep double_kt6PFJetsCentralChargedPileUp_*_*', 'keep double_kt6PFJetsCentralNeutral_*_*', 'keep double_kt6PFJetsCentralNeutralTight_*_*', 'keep *_fixedGridRho*_*_*', 'keep recoCaloMETs_met_*_*', 'keep recoCaloMETs_metNoHF_*_*', 'keep recoCaloMETs_metHO_*_*', 'keep recoCaloMETs_corMetGlobalMuons_*_*', 'keep recoCaloMETs_metNoHFHO_*_*', 'keep recoCaloMETs_metOptHO_*_*', 'keep recoCaloMETs_metOpt_*_*', 'keep recoCaloMETs_metOptNoHFHO_*_*', 'keep recoCaloMETs_metOptNoHF_*_*', 'keep recoMETs_htMetAK5_*_*', 'keep recoMETs_htMetAK7_*_*', 'keep recoMETs_htMetIC5_*_*', 'keep recoMETs_htMetKT4_*_*', 'keep recoMETs_htMetKT6_*_*', 'keep recoMETs_tcMet_*_*', 'keep recoMETs_tcMetWithPFclusters_*_*', 'keep recoPFMETs_pfMet_*_*', 'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 'keep recoMuonMETCorrectionDataedmValueMap_muonTCMETValueMapProducer_*_*', 'keep recoHcalNoiseRBXs_hcalnoise_*_*', 'keep HcalNoiseSummary_hcalnoise_*_*', 'keep *HaloData_*_*_*', 'keep *BeamHaloSummary_BeamHaloSummary_*_*', 'keep *_MuonSeed_*_*', 'keep *_ancientMuonSeed_*_*', 'keep *_mergedStandAloneMuonSeeds_*_*', 'keep TrackingRecHitsOwned_globalMuons_*_*', 'keep TrackingRecHitsOwned_tevMuons_*_*', 'keep recoCaloMuons_calomuons_*_*', 'keep *_CosmicMuonSeed_*_*', 'keep recoTrackExtras_cosmicMuons_*_*', 'keep TrackingRecHitsOwned_cosmicMuons_*_*', 'keep recoTrackExtras_globalCosmicMuons_*_*', 'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 'keep recoTrackExtras_cosmicMuons1Leg_*_*', 'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 'keep recoTracks_cosmicsVetoTracks_*_*', 'keep *_SETMuonSeed_*_*', 'keep recoTracks_standAloneSETMuons_*_*', 'keep recoTrackExtras_standAloneSETMuons_*_*', 'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 'keep recoTracks_globalSETMuons_*_*', 'keep recoTrackExtras_globalSETMuons_*_*', 'keep TrackingRecHitsOwned_globalSETMuons_*_*', 'keep recoMuons_muonsWithSET_*_*', 'keep *_muons_*_*', 'keep *_*_muons_*', 'drop *_muons_muons1stStep2muonsMap_*', 'keep recoTracks_standAloneMuons_*_*', 'keep recoTrackExtras_standAloneMuons_*_*', 'keep TrackingRecHitsOwned_standAloneMuons_*_*', 'keep recoTracks_globalMuons_*_*', 'keep recoTrackExtras_globalMuons_*_*', 'keep recoTracks_tevMuons_*_*', 'keep recoTrackExtras_tevMuons_*_*', 'keep recoTracks_generalTracks_*_*', 'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 'keep recoTracks_cosmicMuons_*_*', 'keep recoTracks_globalCosmicMuons_*_*', 'keep recoMuons_muonsFromCosmics_*_*', 'keep recoTracks_cosmicMuons1Leg_*_*', 'keep recoTracks_globalCosmicMuons1Leg_*_*', 'keep recoMuons_muonsFromCosmics1Leg_*_*', 'keep recoTracks_refittedStandAloneMuons_*_*', 'keep recoTrackExtras_refittedStandAloneMuons_*_*', 'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 'keep *_muIsoDepositTk_*_*', 'keep *_muIsoDepositCalByAssociatorTowers_*_*', 'keep *_muIsoDepositCalByAssociatorHits_*_*', 'keep *_muIsoDepositJets_*_*', 'keep *_muGlobalIsoDepositCtfTk_*_*', 'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 'keep *_muGlobalIsoDepositJets_*_*', 'keep *_impactParameterTagInfos_*_*', 'keep *_trackCountingHighEffBJetTags_*_*', 'keep *_trackCountingHighPurBJetTags_*_*', 'keep *_jetProbabilityBJetTags_*_*', 'keep *_jetBProbabilityBJetTags_*_*', 'keep *_secondaryVertexTagInfos_*_*', 'keep *_ghostTrackVertexTagInfos_*_*', 'keep *_simpleSecondaryVertexBJetTags_*_*', 'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 'keep *_combinedSecondaryVertexBJetTags_*_*', 'keep *_combinedSecondaryVertexMVABJetTags_*_*', 'keep *_ghostTrackBJetTags_*_*', 'keep *_btagSoftElectrons_*_*', 'keep *_softElectronCands_*_*', 'keep *_softPFElectrons_*_*', 'keep *_softElectronTagInfos_*_*', 'keep *_softElectronBJetTags_*_*', 'keep *_softElectronByIP3dBJetTags_*_*', 'keep *_softElectronByPtBJetTags_*_*', 'keep *_softMuonTagInfos_*_*', 'keep *_softMuonBJetTags_*_*', 'keep *_softMuonByIP3dBJetTags_*_*', 'keep *_softMuonByPtBJetTags_*_*', 'keep *_combinedMVABJetTags_*_*', 'keep *_ak5PFJetsRecoTauPiZeros_*_*', 'keep *_hpsPFTauProducer_*_*', 'keep *_hpsPFTauDiscrimination*_*_*', 'keep *_shrinkingConePFTauProducer_*_*', 'keep *_shrinkingConePFTauDiscrimination*_*_*', 'keep *_hpsTancTaus_*_*', 'keep *_hpsTancTausDiscrimination*_*_*', 'keep *_TCTauJetPlusTrackZSPCorJetAntiKt5_*_*', 'keep *_caloRecoTauTagInfoProducer_*_*', 'keep recoCaloTaus_caloRecoTauProducer*_*_*', 'keep *_caloRecoTauDiscrimination*_*_*', 'keep  *_offlinePrimaryVertices__*', 'keep  *_offlinePrimaryVerticesWithBS_*_*', 'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 'keep  *_nuclearInteractionMaker_*_*', 'keep *_generalV0Candidates_*_*', 'keep recoGsfElectronCores_gsfElectronCores_*_*', 'keep recoGsfElectrons_gsfElectrons_*_*', 'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 'keep floatedmValueMap_eidRobustLoose_*_*', 'keep floatedmValueMap_eidRobustTight_*_*', 'keep floatedmValueMap_eidRobustHighEnergy_*_*', 'keep floatedmValueMap_eidLoose_*_*', 'keep floatedmValueMap_eidTight_*_*', 'keep recoPhotons_photons_*_*', 'keep recoPhotonCores_photonCore_*_*', 'keep recoConversions_conversions_*_*', 'drop *_conversions_uncleanedConversions_*', 'keep recoConversions_allConversions_*_*', 'keep recoTracks_ckfOutInTracksFromConversions_*_*')+cms.untracked.vstring('keep recoTracks_ckfInOutTracksFromConversions_*_*', 'keep recoTrackExtras_ckfOutInTracksFromConversions_*_*', 'keep recoTrackExtras_ckfInOutTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_ckfOutInTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_ckfInOutTracksFromConversions_*_*', 'keep recoConversions_uncleanedOnlyAllConversions_*_*', 'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 'keep *_PhotonIDProd_*_*', 'keep *_hfRecoEcalCandidate_*_*', 'keep *_hfEMClusters_*_*', 'keep *_pixelTracks_*_*', 'keep *_pixelVertices_*_*', 'drop CaloTowersSorted_towerMakerPF_*_*', 'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 'keep recoPFClusters_particleFlowClusterECAL_*_*', 'keep recoPFClusters_particleFlowClusterHCAL_*_*', 'keep recoPFClusters_particleFlowClusterHO_*_*', 'keep recoPFClusters_particleFlowClusterPS_*_*', 'keep recoPFBlocks_particleFlowBlock_*_*', 'keep recoPFCandidates_particleFlow_*_*', 'keep recoPFCandidates_particleFlowTmp_electrons_*', 'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 'keep *_pfElectronTranslator_*_*', 'keep *_pfPhotonTranslator_*_*', 'keep *_particleFlow_electrons_*', 'keep *_particleFlow_photons_*', 'keep *_trackerDrivenElectronSeeds_preid_*', 'keep *_offlineBeamSpot_*_*', 'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 'keep *_l1GtRecord_*_*', 'keep *_l1GtTriggerMenuLite_*_*', 'keep *_conditionsInEdm_*_*', 'keep *_l1extraParticles_*_*', 'keep *_l1L1GtObjectMap_*_*', 'keep L1MuGMTReadoutCollection_gtDigis_*_*', 'keep L1GctEmCand*_gctDigis_*_*', 'keep L1GctJetCand*_gctDigis_*_*', 'keep L1GctEtHad*_gctDigis_*_*', 'keep L1GctEtMiss*_gctDigis_*_*', 'keep L1GctEtTotal*_gctDigis_*_*', 'keep L1GctHtMiss*_gctDigis_*_*', 'keep L1GctJetCounts*_gctDigis_*_*', 'keep L1GctHFRingEtSums*_gctDigis_*_*', 'keep L1GctHFBitCounts*_gctDigis_*_*', 'keep LumiDetails_lumiProducer_*_*', 'keep LumiSummary_lumiProducer_*_*', 'drop *_hlt*_*_*', 'keep *_hltL1GtObjectMap_*_*', 'keep edmTriggerResults_*_*_*', 'keep triggerTriggerEvent_*_*_*', 'keep L1AcceptBunchCrossings_*_*_*', 'keep L1TriggerScalerss_*_*_*', 'keep Level1TriggerScalerss_*_*_*', 'keep LumiScalerss_*_*_*', 'keep BeamSpotOnlines_*_*_*', 'keep DcsStatuss_*_*_*', 'keep *_logErrorHarvester_*_*', 'drop *_MEtoEDMConverter_*_*', 'drop *_*_*_SKIM')),
    fileName = cms.untracked.string('DiTau.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('DiTau'),
        dataTier = cms.untracked.string('RAW-RECO')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880)
)
process.SKIMStreamHZZ = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('HZZ4ePath', 
            'HZZ2e2mPath', 
            'HZZ2m2ePath', 
            'HZZ4mPath', 
            'HZZem2ePath', 
            'HZZem2mPath')
    ),
    outputCommands = cms.untracked.vstring('drop *', 
        'keep *_castorreco_*_*', 
        'keep *_reducedHcalRecHits_*_*', 
        'keep *_selectDigi_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 
        'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 
        'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 
        'keep recoCaloClusters_hybridSuperClusters_*_*', 
        'keep recoSuperClusters_hybridSuperClusters_uncleanOnlyHybridSuperClusters_*', 
        'keep recoCaloClusters_multi5x5SuperClusters_multi5x5EndcapBasicClusters_*', 
        'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 
        'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 
        'keep recoTracks_GsfGlobalElectronTest_*_*', 
        'keep recoGsfTracks_electronGsfTracks_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracks_rsWithMaterialTracks_*_*', 
        'keep recoTracks_conversionStepTracks_*_*', 
        'keep recoTracks_beamhaloTracks_*_*', 
        'keep recoTracks_regionalCosmicTracks_*_*', 
        'keep recoTracks_ctfPixelLess_*_*', 
        'keep *_dedxHarmonic2_*_*', 
        'keep *_dedxDiscrimASmi_*_*', 
        'keep *_trackExtrapolator_*_*', 
        'keep *_kt4CaloJets_*_*', 
        'keep *_kt6CaloJets_*_*', 
        'keep *_ak5CaloJets_*_*', 
        'keep *_ak7CaloJets_*_*', 
        'keep *_kt4PFJets_*_*', 
        'keep *_kt6PFJets_*_*', 
        'keep *_ak5PFJets_*_*', 
        'keep *_ak7PFJets_*_*', 
        'keep *_JetPlusTrackZSPCorJetAntiKt5_*_*', 
        'keep *_ak5TrackJets_*_*', 
        'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 
        'keep *_caloTowers_*_*', 
        'keep *_towerMaker_*_*', 
        'keep *_CastorTowerReco_*_*', 
        'keep *_ak5JetTracksAssociatorAtVertex_*_*', 
        'keep *_ak7JetTracksAssociatorAtVertex_*_*', 
        'keep *_kt4JetExtender_*_*', 
        'keep *_ak5JetExtender_*_*', 
        'keep *_ak7JetExtender_*_*', 
        'keep *_ak5JetID_*_*', 
        'keep *_ak7JetID_*_*', 
        'keep *_kt4JetID_*_*', 
        'keep *_kt6JetID_*_*', 
        'keep *_ak7BasicJets_*_*', 
        'keep *_ak7CastorJetID_*_*', 
        'keep double_kt6CaloJetsCentral_*_*', 
        'keep double_kt6PFJetsCentralChargedPileUp_*_*', 
        'keep double_kt6PFJetsCentralNeutral_*_*', 
        'keep double_kt6PFJetsCentralNeutralTight_*_*', 
        'keep *_fixedGridRho*_*_*', 
        'keep recoCaloMETs_met_*_*', 
        'keep recoCaloMETs_metNoHF_*_*', 
        'keep recoCaloMETs_metHO_*_*', 
        'keep recoCaloMETs_corMetGlobalMuons_*_*', 
        'keep recoMETs_htMetAK5_*_*', 
        'keep recoMETs_htMetAK7_*_*', 
        'keep recoMETs_htMetIC5_*_*', 
        'keep recoMETs_htMetKT4_*_*', 
        'keep recoMETs_htMetKT6_*_*', 
        'keep recoMETs_tcMet_*_*', 
        'keep recoMETs_tcMetWithPFclusters_*_*', 
        'keep recoPFMETs_pfMet_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 
        'keep recoMuonMETCorrectionDataedmValueMap_muonTCMETValueMapProducer_*_*', 
        'drop recoHcalNoiseRBXs_*_*_*', 
        'keep HcalNoiseSummary_hcalnoise_*_*', 
        'keep *GlobalHaloData_*_*_*', 
        'keep *BeamHaloSummary_BeamHaloSummary_*_*', 
        'keep *_muons_*_*', 
        'keep *_*_muons_*', 
        'drop *_muons_muons1stStep2muonsMap_*', 
        'keep recoTracks_standAloneMuons_*_*', 
        'keep recoTrackExtras_standAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_standAloneMuons_*_*', 
        'keep recoTracks_globalMuons_*_*', 
        'keep recoTrackExtras_globalMuons_*_*', 
        'keep recoTracks_tevMuons_*_*', 
        'keep recoTrackExtras_tevMuons_*_*', 
        'keep recoTracks_generalTracks_*_*', 
        'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 
        'keep recoTracks_cosmicMuons_*_*', 
        'keep recoTracks_globalCosmicMuons_*_*', 
        'keep recoMuons_muonsFromCosmics_*_*', 
        'keep recoTracks_cosmicMuons1Leg_*_*', 
        'keep recoTracks_globalCosmicMuons1Leg_*_*', 
        'keep recoMuons_muonsFromCosmics1Leg_*_*', 
        'keep recoTracks_refittedStandAloneMuons_*_*', 
        'keep recoTrackExtras_refittedStandAloneMuons_*_*', 
        'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 
        'keep *_muIsoDepositTk_*_*', 
        'keep *_muIsoDepositCalByAssociatorTowers_*_*', 
        'keep *_muIsoDepositCalByAssociatorHits_*_*', 
        'keep *_muIsoDepositJets_*_*', 
        'keep *_trackCountingHighEffBJetTags_*_*', 
        'keep *_trackCountingHighPurBJetTags_*_*', 
        'keep *_jetProbabilityBJetTags_*_*', 
        'keep *_jetBProbabilityBJetTags_*_*', 
        'keep *_simpleSecondaryVertexBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 
        'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 
        'keep *_combinedSecondaryVertexBJetTags_*_*', 
        'keep *_combinedSecondaryVertexMVABJetTags_*_*', 
        'keep *_ghostTrackBJetTags_*_*', 
        'keep *_softElectronBJetTags_*_*', 
        'keep *_softElectronByIP3dBJetTags_*_*', 
        'keep *_softElectronByPtBJetTags_*_*', 
        'keep *_softMuonBJetTags_*_*', 
        'keep *_softMuonByIP3dBJetTags_*_*', 
        'keep *_softMuonByPtBJetTags_*_*', 
        'keep *_combinedMVABJetTags_*_*', 
        'keep *_ak5PFJetsRecoTauPiZeros_*_*', 
        'keep *_hpsPFTauProducer_*_*', 
        'keep *_hpsPFTauDiscrimination*_*_*', 
        'keep *_shrinkingConePFTauProducer_*_*', 
        'keep *_shrinkingConePFTauDiscrimination*_*_*', 
        'keep *_hpsTancTaus_*_*', 
        'keep *_hpsTancTausDiscrimination*_*_*', 
        'keep *_TCTauJetPlusTrackZSPCorJetAntiKt5_*_*', 
        'keep *_caloRecoTauTagInfoProducer_*_*', 
        'keep recoCaloTaus_caloRecoTauProducer*_*_*', 
        'keep *_caloRecoTauDiscrimination*_*_*', 
        'keep  *_offlinePrimaryVertices__*', 
        'keep  *_offlinePrimaryVerticesWithBS_*_*', 
        'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 
        'keep  *_nuclearInteractionMaker_*_*', 
        'keep *_generalV0Candidates_*_*', 
        'keep recoGsfElectronCores_gsfElectronCores_*_*', 
        'keep recoGsfElectrons_gsfElectrons_*_*', 
        'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 
        'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 
        'keep floatedmValueMap_eidRobustLoose_*_*', 
        'keep floatedmValueMap_eidRobustTight_*_*', 
        'keep floatedmValueMap_eidRobustHighEnergy_*_*', 
        'keep floatedmValueMap_eidLoose_*_*', 
        'keep floatedmValueMap_eidTight_*_*', 
        'keep recoPhotonCores_photonCore_*_*', 
        'keep recoPhotons_photons_*_*', 
        'keep recoConversions_conversions_*_*', 
        'drop *_conversions_uncleanedConversions_*', 
        'keep recoConversions_allConversions_*_*', 
        'keep recoTracks_ckfOutInTracksFromConversions_*_*', 
        'keep recoTracks_ckfInOutTracksFromConversions_*_*', 
        'keep recoConversions_uncleanedOnlyAllConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 
        'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 
        'keep *_PhotonIDProd_*_*', 
        'keep *_hfRecoEcalCandidate_*_*', 
        'keep *_hfEMClusters_*_*', 
        'drop CaloTowersSorted_towerMakerPF_*_*', 
        'drop *_pfElectronTranslator_*_*', 
        'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 
        'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 
        'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 
        'keep recoPFCandidates_particleFlow_*_*', 
        'keep *_particleFlow_electrons_*', 
        'keep *_particleFlow_photons_*', 
        'keep recoCaloClusters_pfElectronTranslator_*_*', 
        'keep recoPreshowerClusters_pfElectronTranslator_*_*', 
        'keep recoSuperClusters_pfElectronTranslator_*_*', 
        'keep recoCaloClusters_pfPhotonTranslator_*_*', 
        'keep recoPreshowerClusters_pfPhotonTranslator_*_*', 
        'keep recoSuperClusters_pfPhotonTranslator_*_*', 
        'keep recoPhotons_pfPhotonTranslator_*_*', 
        'keep recoPhotonCores_pfPhotonTranslator_*_*', 
        'keep recoConversions_pfPhotonTranslator_*_*', 
        'keep *_offlineBeamSpot_*_*', 
        'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 
        'keep *_l1GtRecord_*_*', 
        'keep *_l1GtTriggerMenuLite_*_*', 
        'keep *_conditionsInEdm_*_*', 
        'keep *_l1extraParticles_*_*', 
        'keep *_l1L1GtObjectMap_*_*', 
        'keep LumiSummary_lumiProducer_*_*', 
        'drop *_hlt*_*_*', 
        'keep *_hltL1GtObjectMap_*_*', 
        'drop L1GlobalTriggerObjectMapRecord_hltL1GtObjectMap_*_*', 
        'keep edmTriggerResults_*_*_*', 
        'keep triggerTriggerEvent_*_*_*', 
        'keep L1AcceptBunchCrossings_*_*_*', 
        'keep L1TriggerScalerss_*_*_*', 
        'keep Level1TriggerScalerss_*_*_*', 
        'keep LumiScalerss_*_*_*', 
        'keep BeamSpotOnlines_*_*_*', 
        'keep DcsStatuss_*_*_*', 
        'keep *_logErrorHarvester_*_*', 
        'keep PileupSummaryInfos_*_*_*', 
        'drop *_MEtoEDMConverter_*_*', 
        'drop *_*_*_SKIM'),
    fileName = cms.untracked.string('HZZ.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('HZZ'),
        dataTier = cms.untracked.string('AOD')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880)
)
process.SKIMStreamLogError = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathlogerror')
    ),
    outputCommands = (cms.untracked.vstring('drop *', 'drop *', 'keep  FEDRawDataCollection_rawDataCollector_*_*', 'keep  FEDRawDataCollection_source_*_*', 'keep  FEDRawDataCollection_rawDataCollector_*_*', 'keep  FEDRawDataCollection_source_*_*', 'drop *_hlt*_*_*', 'keep *_hltL1GtObjectMap_*_*', 'keep FEDRawDataCollection_rawDataCollector_*_*', 'keep FEDRawDataCollection_source_*_*', 'keep edmTriggerResults_*_*_*', 'keep triggerTriggerEvent_*_*_*', 'keep DetIdedmEDCollection_siStripDigis_*_*', 'keep DetIdedmEDCollection_siPixelDigis_*_*', 'keep *_siPixelClusters_*_*', 'keep *_siStripClusters_*_*', 'keep *_dt1DRecHits_*_*', 'keep *_dt4DSegments_*_*', 'keep *_dt1DCosmicRecHits_*_*', 'keep *_dt4DCosmicSegments_*_*', 'keep *_csc2DRecHits_*_*', 'keep *_cscSegments_*_*', 'keep *_rpcRecHits_*_*', 'keep *_hbhereco_*_*', 'keep *_hfreco_*_*', 'keep *_horeco_*_*', 'keep HBHERecHitsSorted_hbherecoMB_*_*', 'keep HORecHitsSorted_horecoMB_*_*', 'keep HFRecHitsSorted_hfrecoMB_*_*', 'keep ZDCDataFramesSorted_*Digis_*_*', 'keep ZDCRecHitsSorted_*_*_*', 'keep *_reducedHcalRecHits_*_*', 'keep *_castorreco_*_*', 'keep *_ecalPreshowerRecHit_*_*', 'keep *_ecalRecHit_*_*', 'keep *_ecalCompactTrigPrim_*_*', 'keep *_ecalTPSkim_*_*', 'keep *_selectDigi_*_*', 'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 'keep *_hybridSuperClusters_*_*', 'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 'keep *_multi5x5SuperClusters_*_*', 'keep recoSuperClusters_multi5x5SuperClusters_*_*', 'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 'drop recoClusterShapes_*_*_*', 'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 'keep *_CkfElectronCandidates_*_*', 'keep *_GsfGlobalElectronTest_*_*', 'keep *_electronMergedSeeds_*_*', 'keep recoGsfTracks_electronGsfTracks_*_*', 'keep recoGsfTrackExtras_electronGsfTracks_*_*', 'keep recoTrackExtras_electronGsfTracks_*_*', 'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 'keep recoTracks_generalTracks_*_*', 'keep recoTrackExtras_generalTracks_*_*', 'keep TrackingRecHitsOwned_generalTracks_*_*', 'keep recoTracks_beamhaloTracks_*_*', 'keep recoTrackExtras_beamhaloTracks_*_*', 'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 'keep recoTracks_regionalCosmicTracks_*_*', 'keep recoTrackExtras_regionalCosmicTracks_*_*', 'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 'keep recoTracks_rsWithMaterialTracks_*_*', 'keep recoTrackExtras_rsWithMaterialTracks_*_*', 'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 'keep recoTracks_conversionStepTracks_*_*', 'keep recoTrackExtras_conversionStepTracks_*_*', 'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 'keep *_ctfPixelLess_*_*', 'keep *_dedxTruncated40_*_*', 'keep *_dedxDiscrimASmi_*_*', 'keep *_dedxHarmonic2_*_*', 'keep *_trackExtrapolator_*_*', 'keep *_kt4CaloJets_*_*', 'keep *_kt6CaloJets_*_*', 'keep *_ak5CaloJets_*_*', 'keep *_ak7CaloJets_*_*', 'keep *_iterativeCone5CaloJets_*_*', 'keep *_iterativeCone15CaloJets_*_*', 'keep *_kt4PFJets_*_*', 'keep *_kt6PFJets_*_*', 'keep *_ak5PFJets_*_*', 'keep *_ak7PFJets_*_*', 'keep *_iterativeCone5PFJets_*_*', 'keep *_JetPlusTrackZSPCorJetAntiKt5_*_*', 'keep *_ak5TrackJets_*_*', 'keep *_kt4TrackJets_*_*', 'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 'keep *_caloTowers_*_*', 'keep *_towerMaker_*_*', 'keep *_CastorTowerReco_*_*', 'keep *_ic5JetTracksAssociatorAtVertex_*_*', 'keep *_iterativeCone5JetTracksAssociatorAtVertex_*_*', 'keep *_iterativeCone5JetTracksAssociatorAtCaloFace_*_*', 'keep *_iterativeCone5JetExtender_*_*', 'keep *_kt4JetTracksAssociatorAtVertex_*_*', 'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 'keep *_kt4JetExtender_*_*', 'keep *_ak5JetTracksAssociatorAtVertex_*_*', 'keep *_ak5JetTracksAssociatorAtCaloFace_*_*', 'keep *_ak5JetExtender_*_*', 'keep *_ak7JetTracksAssociatorAtVertex_*_*', 'keep *_ak7JetTracksAssociatorAtCaloFace_*_*', 'keep *_ak7JetExtender_*_*', 'keep *_ak5JetID_*_*', 'keep *_ak7JetID_*_*', 'keep *_ic5JetID_*_*', 'keep *_kt4JetID_*_*', 'keep *_kt6JetID_*_*', 'keep *_ak7BasicJets_*_*', 'keep *_ak7CastorJetID_*_*', 'keep double_kt6CaloJetsCentral_*_*', 'keep double_kt6PFJetsCentralChargedPileUp_*_*', 'keep double_kt6PFJetsCentralNeutral_*_*', 'keep double_kt6PFJetsCentralNeutralTight_*_*', 'keep *_fixedGridRho*_*_*', 'keep recoCaloMETs_met_*_*', 'keep recoCaloMETs_metNoHF_*_*', 'keep recoCaloMETs_metHO_*_*', 'keep recoCaloMETs_corMetGlobalMuons_*_*', 'keep recoCaloMETs_metNoHFHO_*_*', 'keep recoCaloMETs_metOptHO_*_*', 'keep recoCaloMETs_metOpt_*_*', 'keep recoCaloMETs_metOptNoHFHO_*_*', 'keep recoCaloMETs_metOptNoHF_*_*', 'keep recoMETs_htMetAK5_*_*', 'keep recoMETs_htMetAK7_*_*', 'keep recoMETs_htMetIC5_*_*', 'keep recoMETs_htMetKT4_*_*', 'keep recoMETs_htMetKT6_*_*', 'keep recoMETs_tcMet_*_*', 'keep recoMETs_tcMetWithPFclusters_*_*', 'keep recoPFMETs_pfMet_*_*', 'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 'keep recoMuonMETCorrectionDataedmValueMap_muonTCMETValueMapProducer_*_*', 'keep recoHcalNoiseRBXs_hcalnoise_*_*', 'keep HcalNoiseSummary_hcalnoise_*_*', 'keep *HaloData_*_*_*', 'keep *BeamHaloSummary_BeamHaloSummary_*_*', 'keep *_MuonSeed_*_*', 'keep *_ancientMuonSeed_*_*', 'keep *_mergedStandAloneMuonSeeds_*_*', 'keep TrackingRecHitsOwned_globalMuons_*_*', 'keep TrackingRecHitsOwned_tevMuons_*_*', 'keep recoCaloMuons_calomuons_*_*', 'keep *_CosmicMuonSeed_*_*', 'keep recoTrackExtras_cosmicMuons_*_*', 'keep TrackingRecHitsOwned_cosmicMuons_*_*', 'keep recoTrackExtras_globalCosmicMuons_*_*', 'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 'keep recoTrackExtras_cosmicMuons1Leg_*_*', 'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 'keep recoTracks_cosmicsVetoTracks_*_*', 'keep *_SETMuonSeed_*_*', 'keep recoTracks_standAloneSETMuons_*_*', 'keep recoTrackExtras_standAloneSETMuons_*_*', 'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 'keep recoTracks_globalSETMuons_*_*', 'keep recoTrackExtras_globalSETMuons_*_*', 'keep TrackingRecHitsOwned_globalSETMuons_*_*', 'keep recoMuons_muonsWithSET_*_*', 'keep *_muons_*_*', 'keep *_*_muons_*', 'drop *_muons_muons1stStep2muonsMap_*', 'keep recoTracks_standAloneMuons_*_*', 'keep recoTrackExtras_standAloneMuons_*_*', 'keep TrackingRecHitsOwned_standAloneMuons_*_*', 'keep recoTracks_globalMuons_*_*', 'keep recoTrackExtras_globalMuons_*_*', 'keep recoTracks_tevMuons_*_*', 'keep recoTrackExtras_tevMuons_*_*', 'keep recoTracks_generalTracks_*_*', 'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 'keep recoTracks_cosmicMuons_*_*', 'keep recoTracks_globalCosmicMuons_*_*', 'keep recoMuons_muonsFromCosmics_*_*', 'keep recoTracks_cosmicMuons1Leg_*_*', 'keep recoTracks_globalCosmicMuons1Leg_*_*', 'keep recoMuons_muonsFromCosmics1Leg_*_*', 'keep recoTracks_refittedStandAloneMuons_*_*', 'keep recoTrackExtras_refittedStandAloneMuons_*_*', 'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 'keep *_muIsoDepositTk_*_*', 'keep *_muIsoDepositCalByAssociatorTowers_*_*', 'keep *_muIsoDepositCalByAssociatorHits_*_*', 'keep *_muIsoDepositJets_*_*', 'keep *_muGlobalIsoDepositCtfTk_*_*', 'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 'keep *_muGlobalIsoDepositJets_*_*', 'keep *_impactParameterTagInfos_*_*', 'keep *_trackCountingHighEffBJetTags_*_*', 'keep *_trackCountingHighPurBJetTags_*_*', 'keep *_jetProbabilityBJetTags_*_*', 'keep *_jetBProbabilityBJetTags_*_*', 'keep *_secondaryVertexTagInfos_*_*', 'keep *_ghostTrackVertexTagInfos_*_*', 'keep *_simpleSecondaryVertexBJetTags_*_*', 'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 'keep *_combinedSecondaryVertexBJetTags_*_*', 'keep *_combinedSecondaryVertexMVABJetTags_*_*', 'keep *_ghostTrackBJetTags_*_*', 'keep *_btagSoftElectrons_*_*', 'keep *_softElectronCands_*_*', 'keep *_softPFElectrons_*_*', 'keep *_softElectronTagInfos_*_*', 'keep *_softElectronBJetTags_*_*', 'keep *_softElectronByIP3dBJetTags_*_*', 'keep *_softElectronByPtBJetTags_*_*', 'keep *_softMuonTagInfos_*_*', 'keep *_softMuonBJetTags_*_*', 'keep *_softMuonByIP3dBJetTags_*_*', 'keep *_softMuonByPtBJetTags_*_*', 'keep *_combinedMVABJetTags_*_*', 'keep *_ak5PFJetsRecoTauPiZeros_*_*', 'keep *_hpsPFTauProducer_*_*', 'keep *_hpsPFTauDiscrimination*_*_*', 'keep *_shrinkingConePFTauProducer_*_*', 'keep *_shrinkingConePFTauDiscrimination*_*_*', 'keep *_hpsTancTaus_*_*', 'keep *_hpsTancTausDiscrimination*_*_*', 'keep *_TCTauJetPlusTrackZSPCorJetAntiKt5_*_*', 'keep *_caloRecoTauTagInfoProducer_*_*', 'keep recoCaloTaus_caloRecoTauProducer*_*_*', 'keep *_caloRecoTauDiscrimination*_*_*', 'keep  *_offlinePrimaryVertices__*', 'keep  *_offlinePrimaryVerticesWithBS_*_*', 'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 'keep  *_nuclearInteractionMaker_*_*', 'keep *_generalV0Candidates_*_*', 'keep recoGsfElectronCores_gsfElectronCores_*_*', 'keep recoGsfElectrons_gsfElectrons_*_*', 'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 'keep floatedmValueMap_eidRobustLoose_*_*', 'keep floatedmValueMap_eidRobustTight_*_*', 'keep floatedmValueMap_eidRobustHighEnergy_*_*', 'keep floatedmValueMap_eidLoose_*_*', 'keep floatedmValueMap_eidTight_*_*', 'keep recoPhotons_photons_*_*', 'keep recoPhotonCores_photonCore_*_*', 'keep recoConversions_conversions_*_*', 'drop *_conversions_uncleanedConversions_*', 'keep recoConversions_allConversions_*_*', 'keep recoTracks_ckfOutInTracksFromConversions_*_*')+cms.untracked.vstring('keep recoTracks_ckfInOutTracksFromConversions_*_*', 'keep recoTrackExtras_ckfOutInTracksFromConversions_*_*', 'keep recoTrackExtras_ckfInOutTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_ckfOutInTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_ckfInOutTracksFromConversions_*_*', 'keep recoConversions_uncleanedOnlyAllConversions_*_*', 'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 'keep *_PhotonIDProd_*_*', 'keep *_hfRecoEcalCandidate_*_*', 'keep *_hfEMClusters_*_*', 'keep *_pixelTracks_*_*', 'keep *_pixelVertices_*_*', 'drop CaloTowersSorted_towerMakerPF_*_*', 'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 'keep recoPFClusters_particleFlowClusterECAL_*_*', 'keep recoPFClusters_particleFlowClusterHCAL_*_*', 'keep recoPFClusters_particleFlowClusterHO_*_*', 'keep recoPFClusters_particleFlowClusterPS_*_*', 'keep recoPFBlocks_particleFlowBlock_*_*', 'keep recoPFCandidates_particleFlow_*_*', 'keep recoPFCandidates_particleFlowTmp_electrons_*', 'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 'keep *_pfElectronTranslator_*_*', 'keep *_pfPhotonTranslator_*_*', 'keep *_particleFlow_electrons_*', 'keep *_particleFlow_photons_*', 'keep *_trackerDrivenElectronSeeds_preid_*', 'keep *_offlineBeamSpot_*_*', 'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 'keep *_l1GtRecord_*_*', 'keep *_l1GtTriggerMenuLite_*_*', 'keep *_conditionsInEdm_*_*', 'keep *_l1extraParticles_*_*', 'keep *_l1L1GtObjectMap_*_*', 'keep L1MuGMTReadoutCollection_gtDigis_*_*', 'keep L1GctEmCand*_gctDigis_*_*', 'keep L1GctJetCand*_gctDigis_*_*', 'keep L1GctEtHad*_gctDigis_*_*', 'keep L1GctEtMiss*_gctDigis_*_*', 'keep L1GctEtTotal*_gctDigis_*_*', 'keep L1GctHtMiss*_gctDigis_*_*', 'keep L1GctJetCounts*_gctDigis_*_*', 'keep L1GctHFRingEtSums*_gctDigis_*_*', 'keep L1GctHFBitCounts*_gctDigis_*_*', 'keep LumiDetails_lumiProducer_*_*', 'keep LumiSummary_lumiProducer_*_*', 'drop *_hlt*_*_*', 'keep *_hltL1GtObjectMap_*_*', 'keep edmTriggerResults_*_*_*', 'keep triggerTriggerEvent_*_*_*', 'keep L1AcceptBunchCrossings_*_*_*', 'keep L1TriggerScalerss_*_*_*', 'keep Level1TriggerScalerss_*_*_*', 'keep LumiScalerss_*_*_*', 'keep BeamSpotOnlines_*_*_*', 'keep DcsStatuss_*_*_*', 'keep *_logErrorHarvester_*_*', 'drop *_MEtoEDMConverter_*_*', 'drop *_*_*_SKIM')),
    fileName = cms.untracked.string('LogError.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('LogError'),
        dataTier = cms.untracked.string('RAW-RECO')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880)
)
process.SKIMStreamLogErrorMonitor = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('pathlogerrormonitor')
    ),
    outputCommands = cms.untracked.vstring('drop *_*_*_*', 
        'keep edmErrorSummaryEntrys_*_*_*'),
    fileName = cms.untracked.string('LogErrorMonitor.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('LogErrorMonitor'),
        dataTier = cms.untracked.string('USER')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880)
)
process.SKIMStreamZElectron = cms.OutputModule("PoolOutputModule",
    SelectEvents = cms.untracked.PSet(
        SelectEvents = cms.vstring('ZElectronPathGsf')
    ),
    outputCommands = (cms.untracked.vstring('drop *', 'drop *', 'keep  FEDRawDataCollection_rawDataCollector_*_*', 'keep  FEDRawDataCollection_source_*_*', 'keep  FEDRawDataCollection_rawDataCollector_*_*', 'keep  FEDRawDataCollection_source_*_*', 'drop *_hlt*_*_*', 'keep *_hltL1GtObjectMap_*_*', 'keep FEDRawDataCollection_rawDataCollector_*_*', 'keep FEDRawDataCollection_source_*_*', 'keep edmTriggerResults_*_*_*', 'keep triggerTriggerEvent_*_*_*', 'keep DetIdedmEDCollection_siStripDigis_*_*', 'keep DetIdedmEDCollection_siPixelDigis_*_*', 'keep *_siPixelClusters_*_*', 'keep *_siStripClusters_*_*', 'keep *_dt1DRecHits_*_*', 'keep *_dt4DSegments_*_*', 'keep *_dt1DCosmicRecHits_*_*', 'keep *_dt4DCosmicSegments_*_*', 'keep *_csc2DRecHits_*_*', 'keep *_cscSegments_*_*', 'keep *_rpcRecHits_*_*', 'keep *_hbhereco_*_*', 'keep *_hfreco_*_*', 'keep *_horeco_*_*', 'keep HBHERecHitsSorted_hbherecoMB_*_*', 'keep HORecHitsSorted_horecoMB_*_*', 'keep HFRecHitsSorted_hfrecoMB_*_*', 'keep ZDCDataFramesSorted_*Digis_*_*', 'keep ZDCRecHitsSorted_*_*_*', 'keep *_reducedHcalRecHits_*_*', 'keep *_castorreco_*_*', 'keep *_ecalPreshowerRecHit_*_*', 'keep *_ecalRecHit_*_*', 'keep *_ecalCompactTrigPrim_*_*', 'keep *_ecalTPSkim_*_*', 'keep *_selectDigi_*_*', 'keep EcalRecHitsSorted_reducedEcalRecHitsEE_*_*', 'keep EcalRecHitsSorted_reducedEcalRecHitsEB_*_*', 'keep EcalRecHitsSorted_reducedEcalRecHitsES_*_*', 'keep *_hybridSuperClusters_*_*', 'keep recoSuperClusters_correctedHybridSuperClusters_*_*', 'keep *_multi5x5SuperClusters_*_*', 'keep recoSuperClusters_multi5x5SuperClusters_*_*', 'keep recoSuperClusters_multi5x5SuperClustersWithPreshower_*_*', 'keep recoSuperClusters_correctedMulti5x5SuperClustersWithPreshower_*_*', 'keep recoPreshowerClusters_multi5x5SuperClustersWithPreshower_*_*', 'keep recoPreshowerClusterShapes_multi5x5PreshowerClusterShape_*_*', 'drop recoClusterShapes_*_*_*', 'drop recoBasicClustersToOnerecoClusterShapesAssociation_*_*_*', 'drop recoBasicClusters_multi5x5BasicClusters_multi5x5BarrelBasicClusters_*', 'drop recoSuperClusters_multi5x5SuperClusters_multi5x5BarrelSuperClusters_*', 'keep *_CkfElectronCandidates_*_*', 'keep *_GsfGlobalElectronTest_*_*', 'keep *_electronMergedSeeds_*_*', 'keep recoGsfTracks_electronGsfTracks_*_*', 'keep recoGsfTrackExtras_electronGsfTracks_*_*', 'keep recoTrackExtras_electronGsfTracks_*_*', 'keep TrackingRecHitsOwned_electronGsfTracks_*_*', 'keep recoTracks_generalTracks_*_*', 'keep recoTrackExtras_generalTracks_*_*', 'keep TrackingRecHitsOwned_generalTracks_*_*', 'keep recoTracks_beamhaloTracks_*_*', 'keep recoTrackExtras_beamhaloTracks_*_*', 'keep TrackingRecHitsOwned_beamhaloTracks_*_*', 'keep recoTracks_regionalCosmicTracks_*_*', 'keep recoTrackExtras_regionalCosmicTracks_*_*', 'keep TrackingRecHitsOwned_regionalCosmicTracks_*_*', 'keep recoTracks_rsWithMaterialTracks_*_*', 'keep recoTrackExtras_rsWithMaterialTracks_*_*', 'keep TrackingRecHitsOwned_rsWithMaterialTracks_*_*', 'keep recoTracks_conversionStepTracks_*_*', 'keep recoTrackExtras_conversionStepTracks_*_*', 'keep TrackingRecHitsOwned_conversionStepTracks_*_*', 'keep *_ctfPixelLess_*_*', 'keep *_dedxTruncated40_*_*', 'keep *_dedxDiscrimASmi_*_*', 'keep *_dedxHarmonic2_*_*', 'keep *_trackExtrapolator_*_*', 'keep *_kt4CaloJets_*_*', 'keep *_kt6CaloJets_*_*', 'keep *_ak5CaloJets_*_*', 'keep *_ak7CaloJets_*_*', 'keep *_iterativeCone5CaloJets_*_*', 'keep *_iterativeCone15CaloJets_*_*', 'keep *_kt4PFJets_*_*', 'keep *_kt6PFJets_*_*', 'keep *_ak5PFJets_*_*', 'keep *_ak7PFJets_*_*', 'keep *_iterativeCone5PFJets_*_*', 'keep *_JetPlusTrackZSPCorJetAntiKt5_*_*', 'keep *_ak5TrackJets_*_*', 'keep *_kt4TrackJets_*_*', 'keep recoRecoChargedRefCandidates_trackRefsForJets_*_*', 'keep *_caloTowers_*_*', 'keep *_towerMaker_*_*', 'keep *_CastorTowerReco_*_*', 'keep *_ic5JetTracksAssociatorAtVertex_*_*', 'keep *_iterativeCone5JetTracksAssociatorAtVertex_*_*', 'keep *_iterativeCone5JetTracksAssociatorAtCaloFace_*_*', 'keep *_iterativeCone5JetExtender_*_*', 'keep *_kt4JetTracksAssociatorAtVertex_*_*', 'keep *_kt4JetTracksAssociatorAtCaloFace_*_*', 'keep *_kt4JetExtender_*_*', 'keep *_ak5JetTracksAssociatorAtVertex_*_*', 'keep *_ak5JetTracksAssociatorAtCaloFace_*_*', 'keep *_ak5JetExtender_*_*', 'keep *_ak7JetTracksAssociatorAtVertex_*_*', 'keep *_ak7JetTracksAssociatorAtCaloFace_*_*', 'keep *_ak7JetExtender_*_*', 'keep *_ak5JetID_*_*', 'keep *_ak7JetID_*_*', 'keep *_ic5JetID_*_*', 'keep *_kt4JetID_*_*', 'keep *_kt6JetID_*_*', 'keep *_ak7BasicJets_*_*', 'keep *_ak7CastorJetID_*_*', 'keep double_kt6CaloJetsCentral_*_*', 'keep double_kt6PFJetsCentralChargedPileUp_*_*', 'keep double_kt6PFJetsCentralNeutral_*_*', 'keep double_kt6PFJetsCentralNeutralTight_*_*', 'keep *_fixedGridRho*_*_*', 'keep recoCaloMETs_met_*_*', 'keep recoCaloMETs_metNoHF_*_*', 'keep recoCaloMETs_metHO_*_*', 'keep recoCaloMETs_corMetGlobalMuons_*_*', 'keep recoCaloMETs_metNoHFHO_*_*', 'keep recoCaloMETs_metOptHO_*_*', 'keep recoCaloMETs_metOpt_*_*', 'keep recoCaloMETs_metOptNoHFHO_*_*', 'keep recoCaloMETs_metOptNoHF_*_*', 'keep recoMETs_htMetAK5_*_*', 'keep recoMETs_htMetAK7_*_*', 'keep recoMETs_htMetIC5_*_*', 'keep recoMETs_htMetKT4_*_*', 'keep recoMETs_htMetKT6_*_*', 'keep recoMETs_tcMet_*_*', 'keep recoMETs_tcMetWithPFclusters_*_*', 'keep recoPFMETs_pfMet_*_*', 'keep recoMuonMETCorrectionDataedmValueMap_muonMETValueMapProducer_*_*', 'keep recoMuonMETCorrectionDataedmValueMap_muonTCMETValueMapProducer_*_*', 'keep recoHcalNoiseRBXs_hcalnoise_*_*', 'keep HcalNoiseSummary_hcalnoise_*_*', 'keep *HaloData_*_*_*', 'keep *BeamHaloSummary_BeamHaloSummary_*_*', 'keep *_MuonSeed_*_*', 'keep *_ancientMuonSeed_*_*', 'keep *_mergedStandAloneMuonSeeds_*_*', 'keep TrackingRecHitsOwned_globalMuons_*_*', 'keep TrackingRecHitsOwned_tevMuons_*_*', 'keep recoCaloMuons_calomuons_*_*', 'keep *_CosmicMuonSeed_*_*', 'keep recoTrackExtras_cosmicMuons_*_*', 'keep TrackingRecHitsOwned_cosmicMuons_*_*', 'keep recoTrackExtras_globalCosmicMuons_*_*', 'keep TrackingRecHitsOwned_globalCosmicMuons_*_*', 'keep recoTrackExtras_cosmicMuons1Leg_*_*', 'keep TrackingRecHitsOwned_cosmicMuons1Leg_*_*', 'keep recoTrackExtras_globalCosmicMuons1Leg_*_*', 'keep TrackingRecHitsOwned_globalCosmicMuons1Leg_*_*', 'keep recoTracks_cosmicsVetoTracks_*_*', 'keep *_SETMuonSeed_*_*', 'keep recoTracks_standAloneSETMuons_*_*', 'keep recoTrackExtras_standAloneSETMuons_*_*', 'keep TrackingRecHitsOwned_standAloneSETMuons_*_*', 'keep recoTracks_globalSETMuons_*_*', 'keep recoTrackExtras_globalSETMuons_*_*', 'keep TrackingRecHitsOwned_globalSETMuons_*_*', 'keep recoMuons_muonsWithSET_*_*', 'keep *_muons_*_*', 'keep *_*_muons_*', 'drop *_muons_muons1stStep2muonsMap_*', 'keep recoTracks_standAloneMuons_*_*', 'keep recoTrackExtras_standAloneMuons_*_*', 'keep TrackingRecHitsOwned_standAloneMuons_*_*', 'keep recoTracks_globalMuons_*_*', 'keep recoTrackExtras_globalMuons_*_*', 'keep recoTracks_tevMuons_*_*', 'keep recoTrackExtras_tevMuons_*_*', 'keep recoTracks_generalTracks_*_*', 'keep recoTracksToOnerecoTracksAssociation_tevMuons_*_*', 'keep recoTracks_cosmicMuons_*_*', 'keep recoTracks_globalCosmicMuons_*_*', 'keep recoMuons_muonsFromCosmics_*_*', 'keep recoTracks_cosmicMuons1Leg_*_*', 'keep recoTracks_globalCosmicMuons1Leg_*_*', 'keep recoMuons_muonsFromCosmics1Leg_*_*', 'keep recoTracks_refittedStandAloneMuons_*_*', 'keep recoTrackExtras_refittedStandAloneMuons_*_*', 'keep TrackingRecHitsOwned_refittedStandAloneMuons_*_*', 'keep *_muIsoDepositTk_*_*', 'keep *_muIsoDepositCalByAssociatorTowers_*_*', 'keep *_muIsoDepositCalByAssociatorHits_*_*', 'keep *_muIsoDepositJets_*_*', 'keep *_muGlobalIsoDepositCtfTk_*_*', 'keep *_muGlobalIsoDepositCalByAssociatorTowers_*_*', 'keep *_muGlobalIsoDepositCalByAssociatorHits_*_*', 'keep *_muGlobalIsoDepositJets_*_*', 'keep *_impactParameterTagInfos_*_*', 'keep *_trackCountingHighEffBJetTags_*_*', 'keep *_trackCountingHighPurBJetTags_*_*', 'keep *_jetProbabilityBJetTags_*_*', 'keep *_jetBProbabilityBJetTags_*_*', 'keep *_secondaryVertexTagInfos_*_*', 'keep *_ghostTrackVertexTagInfos_*_*', 'keep *_simpleSecondaryVertexBJetTags_*_*', 'keep *_simpleSecondaryVertexHighEffBJetTags_*_*', 'keep *_simpleSecondaryVertexHighPurBJetTags_*_*', 'keep *_combinedSecondaryVertexBJetTags_*_*', 'keep *_combinedSecondaryVertexMVABJetTags_*_*', 'keep *_ghostTrackBJetTags_*_*', 'keep *_btagSoftElectrons_*_*', 'keep *_softElectronCands_*_*', 'keep *_softPFElectrons_*_*', 'keep *_softElectronTagInfos_*_*', 'keep *_softElectronBJetTags_*_*', 'keep *_softElectronByIP3dBJetTags_*_*', 'keep *_softElectronByPtBJetTags_*_*', 'keep *_softMuonTagInfos_*_*', 'keep *_softMuonBJetTags_*_*', 'keep *_softMuonByIP3dBJetTags_*_*', 'keep *_softMuonByPtBJetTags_*_*', 'keep *_combinedMVABJetTags_*_*', 'keep *_ak5PFJetsRecoTauPiZeros_*_*', 'keep *_hpsPFTauProducer_*_*', 'keep *_hpsPFTauDiscrimination*_*_*', 'keep *_shrinkingConePFTauProducer_*_*', 'keep *_shrinkingConePFTauDiscrimination*_*_*', 'keep *_hpsTancTaus_*_*', 'keep *_hpsTancTausDiscrimination*_*_*', 'keep *_TCTauJetPlusTrackZSPCorJetAntiKt5_*_*', 'keep *_caloRecoTauTagInfoProducer_*_*', 'keep recoCaloTaus_caloRecoTauProducer*_*_*', 'keep *_caloRecoTauDiscrimination*_*_*', 'keep  *_offlinePrimaryVertices__*', 'keep  *_offlinePrimaryVerticesWithBS_*_*', 'keep  *_offlinePrimaryVerticesFromCosmicTracks_*_*', 'keep  *_nuclearInteractionMaker_*_*', 'keep *_generalV0Candidates_*_*', 'keep recoGsfElectronCores_gsfElectronCores_*_*', 'keep recoGsfElectrons_gsfElectrons_*_*', 'keep recoGsfElectronCores_uncleanedOnlyGsfElectronCores_*_*', 'keep recoGsfElectrons_uncleanedOnlyGsfElectrons_*_*', 'keep floatedmValueMap_eidRobustLoose_*_*', 'keep floatedmValueMap_eidRobustTight_*_*', 'keep floatedmValueMap_eidRobustHighEnergy_*_*', 'keep floatedmValueMap_eidLoose_*_*', 'keep floatedmValueMap_eidTight_*_*', 'keep recoPhotons_photons_*_*', 'keep recoPhotonCores_photonCore_*_*', 'keep recoConversions_conversions_*_*', 'drop *_conversions_uncleanedConversions_*', 'keep recoConversions_allConversions_*_*', 'keep recoTracks_ckfOutInTracksFromConversions_*_*')+cms.untracked.vstring('keep recoTracks_ckfInOutTracksFromConversions_*_*', 'keep recoTrackExtras_ckfOutInTracksFromConversions_*_*', 'keep recoTrackExtras_ckfInOutTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_ckfOutInTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_ckfInOutTracksFromConversions_*_*', 'keep recoConversions_uncleanedOnlyAllConversions_*_*', 'keep recoTracks_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 'keep recoTracks_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 'keep recoTrackExtras_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 'keep recoTrackExtras_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_uncleanedOnlyCkfOutInTracksFromConversions_*_*', 'keep TrackingRecHitsOwned_uncleanedOnlyCkfInOutTracksFromConversions_*_*', 'keep *_PhotonIDProd_*_*', 'keep *_hfRecoEcalCandidate_*_*', 'keep *_hfEMClusters_*_*', 'keep *_pixelTracks_*_*', 'keep *_pixelVertices_*_*', 'drop CaloTowersSorted_towerMakerPF_*_*', 'keep recoPFRecHits_particleFlowClusterECAL_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHCAL_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHO_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHFEM_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterHFHAD_Cleaned_*', 'keep recoPFRecHits_particleFlowClusterPS_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitECAL_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitHCAL_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitHO_Cleaned_*', 'keep recoPFRecHits_particleFlowRecHitPS_Cleaned_*', 'keep recoPFClusters_particleFlowClusterECAL_*_*', 'keep recoPFClusters_particleFlowClusterHCAL_*_*', 'keep recoPFClusters_particleFlowClusterHO_*_*', 'keep recoPFClusters_particleFlowClusterPS_*_*', 'keep recoPFBlocks_particleFlowBlock_*_*', 'keep recoPFCandidates_particleFlow_*_*', 'keep recoPFCandidates_particleFlowTmp_electrons_*', 'keep recoPFDisplacedVertexs_particleFlowDisplacedVertex_*_*', 'keep *_pfElectronTranslator_*_*', 'keep *_pfPhotonTranslator_*_*', 'keep *_particleFlow_electrons_*', 'keep *_particleFlow_photons_*', 'keep *_trackerDrivenElectronSeeds_preid_*', 'keep *_offlineBeamSpot_*_*', 'keep L1GlobalTriggerReadoutRecord_gtDigis_*_*', 'keep *_l1GtRecord_*_*', 'keep *_l1GtTriggerMenuLite_*_*', 'keep *_conditionsInEdm_*_*', 'keep *_l1extraParticles_*_*', 'keep *_l1L1GtObjectMap_*_*', 'keep L1MuGMTReadoutCollection_gtDigis_*_*', 'keep L1GctEmCand*_gctDigis_*_*', 'keep L1GctJetCand*_gctDigis_*_*', 'keep L1GctEtHad*_gctDigis_*_*', 'keep L1GctEtMiss*_gctDigis_*_*', 'keep L1GctEtTotal*_gctDigis_*_*', 'keep L1GctHtMiss*_gctDigis_*_*', 'keep L1GctJetCounts*_gctDigis_*_*', 'keep L1GctHFRingEtSums*_gctDigis_*_*', 'keep L1GctHFBitCounts*_gctDigis_*_*', 'keep LumiDetails_lumiProducer_*_*', 'keep LumiSummary_lumiProducer_*_*', 'drop *_hlt*_*_*', 'keep *_hltL1GtObjectMap_*_*', 'keep edmTriggerResults_*_*_*', 'keep triggerTriggerEvent_*_*_*', 'keep L1AcceptBunchCrossings_*_*_*', 'keep L1TriggerScalerss_*_*_*', 'keep Level1TriggerScalerss_*_*_*', 'keep LumiScalerss_*_*_*', 'keep BeamSpotOnlines_*_*_*', 'keep DcsStatuss_*_*_*', 'keep *_logErrorHarvester_*_*', 'drop *_MEtoEDMConverter_*_*', 'drop *_*_*_SKIM')),
    fileName = cms.untracked.string('ZElectron.root'),
    dataset = cms.untracked.PSet(
        filterName = cms.untracked.string('ZElectron'),
        dataTier = cms.untracked.string('RAW-RECO')
    ),
    eventAutoFlushCompressedSize = cms.untracked.int32(5242880)
)

# Other statements
process.GlobalTag.globaltag = 'GR_R_52_V7::All'

# Path and EndPath definitions
process.SKIMStreamDiTauOutPath = cms.EndPath(process.SKIMStreamDiTau)
process.SKIMStreamHZZOutPath = cms.EndPath(process.SKIMStreamHZZ)
process.SKIMStreamLogErrorOutPath = cms.EndPath(process.SKIMStreamLogError)
process.SKIMStreamLogErrorMonitorOutPath = cms.EndPath(process.SKIMStreamLogErrorMonitor)
process.SKIMStreamZElectronOutPath = cms.EndPath(process.SKIMStreamZElectron)

# Schedule definition
process.schedule = cms.Schedule(process.tauSkimBy2Path,process.pathlogerror,process.HZZ4ePath,process.HZZ2e2mPath,process.HZZ2m2ePath,process.HZZ4mPath,process.HZZem2ePath,process.HZZem2mPath,process.ZElectronPathGsf,process.pathlogerrormonitor,process.SKIMStreamDiTauOutPath,process.SKIMStreamHZZOutPath,process.SKIMStreamLogErrorOutPath,process.SKIMStreamLogErrorMonitorOutPath,process.SKIMStreamZElectronOutPath)

process.options = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
