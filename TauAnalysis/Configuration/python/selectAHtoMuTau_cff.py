import FWCore.ParameterSet.Config as cms

from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *
from TauAnalysis.CandidateTools.sysErrDefinitions_cfi import *

#--------------------------------------------------------------------------------
# define event selection criteria for A/H --> mu + tau-jet channel
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.selectZtoMuTau_cff import *

# di-tau candidate selection
cfgDiTauCandidateForAHtoMuTauAntiOverlapVeto = cfgDiTauCandidateForMuTauAntiOverlapVeto.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauAntiOverlapVeto'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauAntiOverlapVetoCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauAntiOverlapVetoIndividual')
)
cfgDiTauCandidateForAHtoMuTauMt1METcut = cfgDiTauCandidateForMuTauMt1METcut.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauMt1METcut'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauMt1METcumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauMt1METindividual')
)
cfgDiTauCandidateForAHtoMuTauPzetaDiffCut = cfgDiTauCandidateForMuTauPzetaDiffCut.clone(
    pluginName = cms.string('diTauCandidateForAHtoMuTauPzetaDiffCut'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauPzetaDiffIndividual')
)

# "final" selection of di-tau candidates for "OppositeSign" signal region
cfgDiTauCandidateForAHtoMuTauZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForAHtoMuTauZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauZeroChargeIndividual'),
    systematics = cms.vstring(muTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)

# "final" selection of di-tau candidates for "SameSign" background dominated control region
cfgDiTauCandidateForAHtoMuTauNonZeroChargeCut = cms.PSet(
    pluginName = cms.string('diTauCandidateForAHtoMuTauNonZeroChargeCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedMuTauPairsForAHtoMuTauNonZeroChargeCumulative'),
    src_individual = cms.InputTag('selectedMuTauPairsForAHtoMuTauNonZeroChargeIndividual'),
    systematics = cms.vstring(muTauPairSystematics.keys()),
    minNumber = cms.uint32(1)
)

# central jet veto/b-jet candidate selection
cfgCentralJetEt20bTagVeto = cms.PSet(
    pluginName = cms.string('centralJetEt20bTagVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoMuTauBtagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoMuTauBtagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    maxNumber = cms.uint32(0)
)
cfgCentralJetEt20Cut = cms.PSet(
    pluginName = cms.string('centralJetEt20Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoMuTauAntiOverlapWithLeptonsVetoIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgCentralJetEt20bTagCut = cms.PSet(
    pluginName = cms.string('centralJetEt20bTagCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedPatJetsForAHtoMuTauBtagCumulative'),
    src_individual = cms.InputTag('selectedPatJetsForAHtoMuTauBtagIndividual'),
    systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)

# VBF event selection
cfgVBFEventTag = cms.PSet(
    pluginName = cms.string('vbfTagCut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphereCumulative'),
    src_individual = cms.InputTag('selectedVBFEventHypothesesForAHtoMuTauTagJetOpposHemisphereIndividual'),
    #systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgVBFEventDEta35 = cms.PSet(
    pluginName = cms.string('vbfDEta35Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35Cumulative'),
    src_individual = cms.InputTag('selectedVBFEventHypothesesForAHtoMuTauTagJetDEta35Individual'),
    #systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgVBFEventMass350 = cms.PSet(
    pluginName = cms.string('vbfMass350Cut'),
    pluginType = cms.string('PATCandViewMinEventSelector'),
    src_cumulative = cms.InputTag('selectedVBFEventHypothesesForAHtoMuTauTagJetMass350Cumulative'),
    src_individual = cms.InputTag('selectedVBFEventHypothesesForAHtoMuTauTagJetMass350Individual'),
    #systematics = cms.vstring(jetSystematics.keys()),
    minNumber = cms.uint32(1)
)
cfgVBF3rdTagJetVeto = cms.PSet(
    pluginName = cms.string('vbf3rdTagJetVeto'),
    pluginType = cms.string('PATCandViewMaxEventSelector'),
    src = cms.InputTag('selectedPatTagJetsForVBFEt30Cumulative'),
    #systematics = cms.vstring(jetSystematics.keys()),
    maxNumber = cms.uint32(2)
)

ahToMuTauEventSelConfiguratorOS = eventSelFlagProdConfigurator(
    [ cfgGenPhaseSpaceCut,
      cfgTrigger,
      cfgGlobalMuonCut,
      cfgMuonEtaCut,
      cfgMuonPtCut,
      cfgTauAntiOverlapWithMuonsVeto,
      cfgTauEtaCut,
      cfgTauPtCut,
      cfgMuonVbTfIdCut,
      cfgMuonPFRelIsoCut,
      cfgMuonTrkIPcut,
      cfgTauLeadTrkCut,
      cfgTauLeadTrkPtCut,
      cfgTauTaNCdiscrCut,
      cfgTauProngCut,
      cfgTauChargeCut,
      cfgTauMuonVeto,
      cfgTauElectronVeto,
      cfgDiTauCandidateForAHtoMuTauAntiOverlapVeto,
      cfgDiTauCandidateForAHtoMuTauMt1METcut,
      cfgDiTauCandidateForAHtoMuTauPzetaDiffCut,
      cfgDiTauCandidateForAHtoMuTauZeroChargeCut,
      cfgPrimaryEventVertexForMuTau,
      cfgPrimaryEventVertexQualityForMuTau,
      cfgPrimaryEventVertexPositionForMuTau,
      cfgDiMuPairZmumuHypothesisVetoByLooseIsolation,
      cfgDiMuPairDYmumuHypothesisVeto,
      cfgCentralJetEt20bTagVeto,
      cfgCentralJetEt20Cut,
      cfgCentralJetEt20bTagCut,
      cfgVBFEventTag,
      cfgVBFEventDEta35,
      cfgVBFEventMass350,
      cfgVBF3rdTagJetVeto ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsAHtoMuTauOS = ahToMuTauEventSelConfiguratorOS.configure()

ahToMuTauEventSelConfiguratorSS = eventSelFlagProdConfigurator(
    [ cfgDiTauCandidateForAHtoMuTauNonZeroChargeCut ],
     boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

produceEventSelFlagsAHtoMuTauSS = ahToMuTauEventSelConfiguratorSS.configure()

produceEventSelFlagsAHtoMuTau = cms.Sequence(produceEventSelFlagsAHtoMuTauOS * produceEventSelFlagsAHtoMuTauSS)

isRecAHtoMuTauCentralJetVeto = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoMuTauCentralJetVeto'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauElectronVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCut', 'cumulative'),
        cms.InputTag('primaryEventVertexPositionForMuTau'),                                           
        cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation'),
        cms.InputTag('diMuPairDYmumuHypothesisVeto'),                           
        cms.InputTag('centralJetEt20bTagVeto', 'cumulative')
    )
)

isRecAHtoMuTauCentralJetBtag = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoMuTauCentralJetBtag'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauElectronVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCut', 'cumulative'),
        cms.InputTag('primaryEventVertexPositionForMuTau'),                                            
        cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation'),
        cms.InputTag('diMuPairDYmumuHypothesisVeto'),                           
        cms.InputTag('centralJetEt20bTagCut', 'cumulative')
    )
)

isRecAHtoMuTauVBFtag = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoMuTauVBFtag'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauElectronVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCut', 'cumulative'),
        cms.InputTag('primaryEventVertexPositionForMuTau'),                                            
        cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation'),
        cms.InputTag('diMuPairDYmumuHypothesisVeto'),                           
        cms.InputTag('vbfMass350Cut', 'cumulative')
    )
)

isRecAHtoMuTau = cms.EDProducer("BoolEventSelFlagProducer",
    pluginName = cms.string('isRecAHtoMuTauCentralJetBtag'),
    pluginType = cms.string('MultiBoolEventSelFlagSelector'),
    flags = cms.VInputTag(
        cms.InputTag('Trigger'),
        cms.InputTag('genPhaseSpaceCut'),
        cms.InputTag('muonTrkIPcut', 'cumulative'),
        cms.InputTag('tauElectronVeto', 'cumulative'),
        cms.InputTag('diTauCandidateForAHtoMuTauZeroChargeCut', 'cumulative'),
        cms.InputTag('primaryEventVertexPositionForMuTau'),                              
        cms.InputTag('diMuPairZmumuHypothesisVetoByLooseIsolation'),
        cms.InputTag('diMuPairDYmumuHypothesisVeto')                           
    )
)

selectAHtoMuTauEvents = cms.Sequence(
    produceEventSelFlagsAHtoMuTau
   * isRecAHtoMuTauCentralJetVeto
   * isRecAHtoMuTauCentralJetBtag
   * isRecAHtoMuTauVBFtag
   * isRecAHtoMuTau
)
