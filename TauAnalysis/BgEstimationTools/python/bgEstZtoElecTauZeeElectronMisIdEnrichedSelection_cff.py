import FWCore.ParameterSet.Config as cms
import copy

from TauAnalysis.CandidateTools.tools.objSelConfigurator import *
from TauAnalysis.RecoTools.tools.eventSelFlagProdConfigurator import *

#--------------------------------------------------------------------------------
# select Z --> e+ e- background enriched event sample
#--------------------------------------------------------------------------------

from TauAnalysis.BgEstimationTools.bgEstZtoElecTauZtautauEnrichedSelection_cff import *

#--------------------------------------------------------------------------------  
# produce collection of pat::Electrons
#--------------------------------------------------------------------------------

# require electron candidate to pass the eidRobustTight electron id. criteria
electronsBgEstZeeElectronMisIdEnrichedId = copy.deepcopy(electronsBgEstZtautauEnrichedId)

# require electron candidate to not be within eta-crack
# between Barrel and Encap ECAL calorimeter
electronsBgEstZeeElectronMisIdEnrichedAntiCrackCut = copy.deepcopy(electronsBgEstZtautauEnrichedAntiCrackCut)

# require electron candidate to be within geometric acceptance of electron trigger
electronsBgEstZeeElectronMisIdEnrichedEta = copy.deepcopy(electronsBgEstZtautauEnrichedEta)

# require electron candidate to have transverse momentum above threshold
electronsBgEstZeeElectronMisIdEnrichedPt = copy.deepcopy(electronsBgEstZtautauEnrichedPt)

electronsBgEstZeeElectronMisIdEnrichedIso = copy.deepcopy(electronsBgEstZtautauEnrichedIso)

# require electron to not be from a photon conversion
electronsBgEstZeeElectronMisIdEnrichedConversionVeto = copy.deepcopy(electronsBgEstZtautauEnrichedConversionVeto)

electronsBgEstZeeElectronMisIdEnrichedTrkIP = copy.deepcopy(electronsBgEstZtautauEnrichedTrkIP)


electronSelConfiguratorBgEstZeeElectronMisIdEnriched = objSelConfigurator(
    [ electronsBgEstZeeElectronMisIdEnrichedId,
      electronsBgEstZeeElectronMisIdEnrichedAntiCrackCut,
      electronsBgEstZeeElectronMisIdEnrichedEta,
      electronsBgEstZeeElectronMisIdEnrichedPt,
      electronsBgEstZeeElectronMisIdEnrichedIso,
      electronsBgEstZeeElectronMisIdEnrichedConversionVeto,
      electronsBgEstZeeElectronMisIdEnrichedTrkIP
      ],
    src = "cleanPatElectrons",
    pyModuleName = __name__,
    doSelIndividual = False
)


selectElectronsBgEstZeeElectronMisIdEnriched = electronSelConfiguratorBgEstZeeElectronMisIdEnriched.configure(pyNameSpace = locals())

   
#--------------------------------------------------------------------------------  
# produce collection of pat::Taus
#--------------------------------------------------------------------------------

# require tau candidate not to overlap with selected electrons
# (in order to avoid double-counting one and the same physical particle
#  as electron and as tau candidate)
tausBgEstZeeElectronMisIdEnrichedAntiOverlapWithElectronsVeto = copy.deepcopy(tausBgEstZtautauEnrichedAntiOverlapWithElectronsVeto)
tausBgEstZeeElectronMisIdEnrichedAntiOverlapWithElectronsVeto.srcNotToBeFiltered = cms.VInputTag("electronsBgEstZeeElectronMisIdEnrichedPtCumulative")

# require tau candidate to be within geometric acceptance of Pixel + SiTracker detectors
tausBgEstZeeElectronMisIdEnrichedEta = copy.deepcopy(tausBgEstZtautauEnrichedEta)

# require tau candidate to have transverse energy above threshold
tausBgEstZeeElectronMisIdEnrichedPt = copy.deepcopy(tausBgEstZtautauEnrichedPt)

# require leading track of tau candidate to have Pt > 5. GeV
tausBgEstZeeElectronMisIdEnrichedLeadTrkPt = copy.deepcopy(tausBgEstZtautauEnrichedLeadTrkPt)

# require tau candidate to pass TaNC discriminator
tausBgEstZeeElectronMisIdEnrichedIso = copy.deepcopy(tausBgEstZtautauEnrichedIso)

# require tau candidate to fail electron veto  !!!!! inverted cut
tausBgEstZeeElectronMisIdEnrichedElectronVeto = copy.deepcopy(tausBgEstZtautauEnrichedElectronVeto)
tausBgEstZeeElectronMisIdEnrichedElectronVeto.cut = cms.string('leadPFCand().mva_e_pi() > -0.1')


# require tau candidate to pass muon veto
tausBgEstZeeElectronMisIdEnrichedMuonVeto = copy.deepcopy(tausBgEstZtautauEnrichedMuonVeto)



tauSelConfiguratorBgEstZeeElectronMisIdEnriched = objSelConfigurator(
    [ tausBgEstZeeElectronMisIdEnrichedAntiOverlapWithElectronsVeto,
      tausBgEstZeeElectronMisIdEnrichedEta,
      tausBgEstZeeElectronMisIdEnrichedPt,
      tausBgEstZeeElectronMisIdEnrichedLeadTrkPt,
      tausBgEstZeeElectronMisIdEnrichedIso,
      tausBgEstZeeElectronMisIdEnrichedElectronVeto,
      tausBgEstZeeElectronMisIdEnrichedMuonVeto ],
    src = "cleanPatTaus",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectTausBgEstZeeElectronMisIdEnriched = tauSelConfiguratorBgEstZeeElectronMisIdEnriched.configure(pyNameSpace = locals())



#--------------------------------------------------------------------------------  
# produce collection of electron + tau-jet combinations
#--------------------------------------------------------------------------------

### production


elecTauPairsBgEstZeeElectronMisIdEnriched = copy.deepcopy(elecTauPairsBgEstZtautauEnriched)
elecTauPairsBgEstZeeElectronMisIdEnriched.srcLeg1 = cms.InputTag('electronsBgEstZeeElectronMisIdEnrichedTrkIPcumulative')
elecTauPairsBgEstZeeElectronMisIdEnriched.srcLeg2 = cms.InputTag('tausBgEstZeeElectronMisIdEnrichedMuonVetoCumulative')

produceElecTauPairsBgEstZeeElectronMisIdEnriched = cms.Sequence(elecTauPairsBgEstZeeElectronMisIdEnriched)

### selection

elecTauPairsBgEstZeeElectronMisIdEnrichedAntiOverlapVeto = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedAntiOverlapVeto)

#### !!!!! turned off
elecTauPairsBgEstZeeElectronMisIdEnrichedMt1MET = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedMt1MET)
elecTauPairsBgEstZeeElectronMisIdEnrichedMt1MET.cut = cms.string('mt1MET < 500.')

#### !!!!! turned off
elecTauPairsBgEstZeeElectronMisIdEnrichedPzetaDiff = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedPzetaDiff)
elecTauPairsBgEstZeeElectronMisIdEnrichedPzetaDiff.cut = cms.string('(pZeta - 1.5*pZetaVis) > -2000.')

elecTauPairsBgEstZeeElectronMisIdEnrichedZeroCharge = copy.deepcopy(elecTauPairsBgEstZtautauEnrichedZeroCharge)

elecTauPairSelConfiguratorBgEstZeeElectronMisIdEnriched = objSelConfigurator(
    [ elecTauPairsBgEstZeeElectronMisIdEnrichedAntiOverlapVeto,
      elecTauPairsBgEstZeeElectronMisIdEnrichedMt1MET,
      elecTauPairsBgEstZeeElectronMisIdEnrichedPzetaDiff,
      elecTauPairsBgEstZeeElectronMisIdEnrichedZeroCharge
      ],
    src = "elecTauPairsBgEstZeeElectronMisIdEnriched",
    pyModuleName = __name__,
    doSelIndividual = False
)

selectElecTauPairsBgEstZeeElectronMisIdEnriched = elecTauPairSelConfiguratorBgEstZeeElectronMisIdEnriched.configure(pyNameSpace = locals())

####### anti Zee Cut #######



### production

allDiElecPairBgEstZeeElectronMisIdEnrichedZeeHypothesesByLooseIsolation = copy.deepcopy(allDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
allDiElecPairBgEstZeeElectronMisIdEnrichedZeeHypothesesByLooseIsolation.srcLeg1 = cms.InputTag("electronsBgEstZeeElectronMisIdEnrichedTrkIPcumulative")

### selection

selectedDiElecPairBgEstZeeElectronMisIdEnrichedZeeHypothesesByLooseIsolation = copy.deepcopy(selectedDiElecPairBgEstZtautauEnrichedZeeHypothesesByLooseIsolation)
selectedDiElecPairBgEstZeeElectronMisIdEnrichedZeeHypothesesByLooseIsolation.src = cms.InputTag("allDiElecPairBgEstZeeElectronMisIdEnrichedZeeHypothesesByLooseIsolation")

produceElecTauPairZeeHypothesesBgEstZeeElectronMisIdEnriched = cms.Sequence(
	selectedPatElectronsForZeeHypotheses * 
	allDiElecPairBgEstZeeElectronMisIdEnrichedZeeHypothesesByLooseIsolation *
	selectedDiElecPairBgEstZeeElectronMisIdEnrichedZeeHypothesesByLooseIsolation
)



#jet selection

jetsBgEstZeeElectronMisIdEnrichedEta = copy.deepcopy(jetsBgEstZtautauEnrichedEta)
jetsBgEstZeeElectronMisIdEnrichedEt = copy.deepcopy(jetsBgEstZtautauEnrichedEt)
jetsBgEstZeeElectronMisIdEnrichedAntiOverlapWithLeptonsVeto = copy.deepcopy(jetsBgEstZtautauEnrichedAntiOverlapWithLeptonsVeto)
jetsBgEstZeeElectronMisIdEnrichedJetTag = copy.deepcopy(jetsBgEstZtautauEnrichedJetTag)
jetsBgEstZeeElectronMisIdEnrichedJetBtag = copy.deepcopy(jetsBgEstZtautauEnrichedJetBtag)

patJetSelConfiguratorBgEstZeeElectronMisIdEnrichedBtag = objSelConfigurator(
    [ jetsBgEstZeeElectronMisIdEnrichedEta,
      jetsBgEstZeeElectronMisIdEnrichedEt,
      jetsBgEstZeeElectronMisIdEnrichedAntiOverlapWithLeptonsVeto,
      jetsBgEstZeeElectronMisIdEnrichedJetBtag ],
    src = "patJets",
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsBgEstZeeElectronMisIdEnrichedBtag = patJetSelConfiguratorBgEstZeeElectronMisIdEnrichedBtag.configure(pyNameSpace = locals())

patJetSelConfiguratorJetTag = objSelConfigurator(
    [ jetsBgEstZeeElectronMisIdEnrichedJetTag ],
    src = 'jetsBgEstZeeElectronMisIdEnrichedAntiOverlapWithLeptonsVetoCumulative',
    pyModuleName = __name__,
    doSelIndividual = True
)

selectPatJetsBgEstZeeElectronMisIdEnrichedJetTag = patJetSelConfiguratorJetTag.configure(pyNameSpace = locals())

selectPatJetsBgEstZeeElectronMisIdEnriched = cms.Sequence(selectPatJetsBgEstZeeElectronMisIdEnrichedBtag + selectPatJetsBgEstZeeElectronMisIdEnrichedJetTag)


#--------------------------------------------------------------------------------  
# produce boolean event selection flags
#--------------------------------------------------------------------------------


# electron cuts

cfgElectronIdCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgElectronIdCutBgEstZtautauEnriched)
cfgElectronIdCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('electronIdCutBgEstZeeElectronMisIdEnriched')
cfgElectronIdCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeElectronMisIdEnrichedIdCumulative')

cfgElectronAntiCrackCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgElectronAntiCrackCutBgEstZtautauEnriched)
cfgElectronAntiCrackCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('electronAntiCrackCutBgEstZeeElectronMisIdEnriched')
cfgElectronAntiCrackCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeElectronMisIdEnrichedAntiCrackCutCumulative')

cfgElectronEtaCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgElectronEtaCutBgEstZtautauEnriched)
cfgElectronEtaCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('electronEtaCutBgEstZeeElectronMisIdEnriched')
cfgElectronEtaCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeElectronMisIdEnrichedEtaCumulative')

cfgElectronPtCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgElectronPtCutBgEstZtautauEnriched)
cfgElectronPtCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('electronPtCutBgEstZeeElectronMisIdEnriched')
cfgElectronPtCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeElectronMisIdEnrichedPtCumulative')

cfgElectronIsoCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgElectronIsoCutBgEstZtautauEnriched)
cfgElectronIsoCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('electronIsoCutBgEstZeeElectronMisIdEnriched')
cfgElectronIsoCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeElectronMisIdEnrichedIsoCumulative')

cfgElectronConversionVetoBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgElectronConversionVetoBgEstZtautauEnriched)
cfgElectronConversionVetoBgEstZeeElectronMisIdEnriched.pluginName = cms.string('electronConversionVetoBgEstZeeElectronMisIdEnriched')
cfgElectronConversionVetoBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeElectronMisIdEnrichedConversionVetoCumulative')

cfgElectronTrkIPcutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgElectronTrkIPcutBgEstZtautauEnriched)
cfgElectronTrkIPcutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('electronTrkIPcutBgEstZeeElectronMisIdEnriched')
cfgElectronTrkIPcutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('electronsBgEstZeeElectronMisIdEnrichedTrkIPcumulative')


# tau cuts

cfgTauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgTauAntiOverlapWithElectronsVetoBgEstZtautauEnriched)
cfgTauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched.pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched')
cfgTauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeElectronMisIdEnrichedAntiOverlapWithElectronsVetoCumulative')

cfgTauEtaCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgTauEtaCutBgEstZtautauEnriched)
cfgTauEtaCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('tauEtaCutBgEstZeeElectronMisIdEnriched')
cfgTauEtaCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeElectronMisIdEnrichedEtaCumulative')

cfgTauPtCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgTauPtCutBgEstZtautauEnriched)
cfgTauPtCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('tauPtCutBgEstZeeElectronMisIdEnriched')
cfgTauPtCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeElectronMisIdEnrichedPtCumulative')

cfgTauLeadTrkPtCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgTauLeadTrkPtCutBgEstZtautauEnriched)
cfgTauLeadTrkPtCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched')
cfgTauLeadTrkPtCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeElectronMisIdEnrichedLeadTrkPtCumulative')

cfgTauIsoCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgTauIsoCutBgEstZtautauEnriched)
cfgTauIsoCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('tauIsoCutBgEstZeeElectronMisIdEnriched')
cfgTauIsoCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeElectronMisIdEnrichedIsoCumulative')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgTauElectronVetoBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgTauElectronVetoBgEstZtautauEnriched)
cfgTauElectronVetoBgEstZeeElectronMisIdEnriched.pluginName = cms.string('tauElectronVetoBgEstZeeElectronMisIdEnriched')
cfgTauElectronVetoBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeElectronMisIdEnrichedElectronVetoCumulative')

cfgTauMuonVetoBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgTauMuonVetoBgEstZtautauEnriched)
cfgTauMuonVetoBgEstZeeElectronMisIdEnriched.pluginName = cms.string('tauMuonVetoBgEstZeeElectronMisIdEnriched')
cfgTauMuonVetoBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('tausBgEstZeeElectronMisIdEnrichedMuonVetoCumulative')

# di-tau candidate selection

cfgDiTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgDiTauCandidateAntiOverlapVetoBgEstZtautauEnriched)
cfgDiTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched.pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched')
cfgDiTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstZeeElectronMisIdEnrichedAntiOverlapVetoCumulative')

cfgDiTauCandidateForElecTauMt1METCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauMt1METCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauMt1METCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched')
cfgDiTauCandidateForElecTauMt1METCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstZeeElectronMisIdEnrichedMt1METcumulative')

cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched')
cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstZeeElectronMisIdEnrichedPzetaDiffCumulative')

cfgDiTauCandidateForElecTauZeroChargeCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgDiTauCandidateForElecTauZeroChargeCutBgEstZtautauEnriched)
cfgDiTauCandidateForElecTauZeroChargeCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched')
cfgDiTauCandidateForElecTauZeroChargeCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('elecTauPairsBgEstZeeElectronMisIdEnrichedZeroChargeCumulative')

#!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
cfgElecTauPairZeeHypothesisVetoBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgElecTauPairZeeHypothesisVetoBgEstZtautauEnriched)
cfgElecTauPairZeeHypothesisVetoBgEstZeeElectronMisIdEnriched.pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched')
cfgElecTauPairZeeHypothesisVetoBgEstZeeElectronMisIdEnriched.pluginType = cms.string('PATCandViewMinEventSelector')
cfgElecTauPairZeeHypothesisVetoBgEstZeeElectronMisIdEnriched.src = cms.InputTag('selectedDiElecPairBgEstZeeElectronMisIdEnrichedZeeHypothesesByLooseIsolation')
cfgElecTauPairZeeHypothesisVetoBgEstZeeElectronMisIdEnriched.minNumber = cms.uint32(1)


# jet veto/b-jet candidate selection
cfgJetEtCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgJetEtCutBgEstZtautauEnriched)
cfgJetEtCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('jetEtCutBgEstZeeElectronMisIdEnriched')
cfgJetEtCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('jetsBgEstZeeElectronMisIdEnrichedJetTagCumulative')
cfgJetEtCutBgEstZeeElectronMisIdEnriched.src_individual = cms.InputTag('jetsBgEstZeeElectronMisIdEnrichedJetTagIndividual')

cfgBtagVetoBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgBtagVetoBgEstZtautauEnriched)
cfgBtagVetoBgEstZeeElectronMisIdEnriched.pluginName = cms.string('jetBtagVetoBgEstZeeElectronMisIdEnriched')
cfgBtagVetoBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('jetsBgEstZeeElectronMisIdEnrichedJetBtagCumulative')
cfgBtagVetoBgEstZeeElectronMisIdEnriched.src_individual = cms.InputTag('jetsBgEstZeeElectronMisIdEnrichedJetBtagIndividual')

cfgBtagCutBgEstZeeElectronMisIdEnriched = copy.deepcopy(cfgBtagCutBgEstZtautauEnriched)
cfgBtagCutBgEstZeeElectronMisIdEnriched.pluginName = cms.string('jetBtagCutBgEstZeeElectronMisIdEnriched')
cfgBtagCutBgEstZeeElectronMisIdEnriched.src_cumulative = cms.InputTag('jetsBgEstZeeElectronMisIdEnrichedJetBtagCumulative')
cfgBtagCutBgEstZeeElectronMisIdEnriched.src_individual = cms.InputTag('jetsBgEstZeeElectronMisIdEnrichedJetBtagIndividual')





evtSelConfiguratorBgEstZeeElectronMisIdEnriched = eventSelFlagProdConfigurator(
    [ cfgElectronIdCutBgEstZeeElectronMisIdEnriched,
      cfgElectronAntiCrackCutBgEstZeeElectronMisIdEnriched,
      cfgElectronEtaCutBgEstZeeElectronMisIdEnriched,
      cfgElectronPtCutBgEstZeeElectronMisIdEnriched,
      cfgElectronIsoCutBgEstZeeElectronMisIdEnriched,
      cfgElectronConversionVetoBgEstZeeElectronMisIdEnriched,
      cfgElectronTrkIPcutBgEstZeeElectronMisIdEnriched,
      cfgTauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched,
      cfgTauEtaCutBgEstZeeElectronMisIdEnriched,
      cfgTauPtCutBgEstZeeElectronMisIdEnriched,
      cfgTauLeadTrkPtCutBgEstZeeElectronMisIdEnriched,
      cfgTauIsoCutBgEstZeeElectronMisIdEnriched,
      cfgTauElectronVetoBgEstZeeElectronMisIdEnriched,
      cfgTauMuonVetoBgEstZeeElectronMisIdEnriched,
      cfgDiTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched,
      cfgDiTauCandidateForElecTauMt1METCutBgEstZeeElectronMisIdEnriched,
      cfgDiTauCandidateForElecTauPzetaDiffCutBgEstZeeElectronMisIdEnriched,
      cfgDiTauCandidateForElecTauZeroChargeCutBgEstZeeElectronMisIdEnriched,
      cfgElecTauPairZeeHypothesisVetoBgEstZeeElectronMisIdEnriched,
      cfgJetEtCutBgEstZeeElectronMisIdEnriched,
      cfgBtagVetoBgEstZeeElectronMisIdEnriched,
      cfgBtagCutBgEstZeeElectronMisIdEnriched           
      ],
    boolEventSelFlagProducer = "BoolEventSelFlagProducer",
    pyModuleName = __name__
)

selectEventsBgEstZeeElectronMisIdEnriched = evtSelConfiguratorBgEstZeeElectronMisIdEnriched.configure()


#--------------------------------------------------------------------------------  
# apply event selection criteria; fill histograms
#--------------------------------------------------------------------------------

from TauAnalysis.Configuration.analyzeZtoElecTau_cff import *


diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched = copy.deepcopy(diTauCandidateHistManagerForElecTauBgEstZtautauEnriched)
diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched.pluginName = cms.string('diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched')
diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched.diTauCandidateSource = cms.InputTag('elecTauPairsBgEstZeeElectronMisIdEnriched')

electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched = copy.deepcopy(electronHistManagerForElecTauBgEstZtautauEnriched)
electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.pluginName = cms.string('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched')

tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched = copy.deepcopy(tauHistManagerForElecTauBgEstZtautauEnriched)
tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.pluginName = cms.string('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched')
tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.jetSource = cms.InputTag('cleanPatJets')

diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched = copy.deepcopy(diTauCandidateNSVfitHistManagerForElecTauBgEstZtautauEnriched)
diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched.pluginName = cms.string('diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched')
diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched.diTauCandidateSource = cms.InputTag('elecTauPairsBgEstZeeElectronMisIdEnriched')


analyzeEventsBgEstZeeElectronMisIdEnriched = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_ZeeElectronMisIdEnriched'), 

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,        
        evtSelPrimaryEventVertexHighestPtTrackSum,

        #start electron cuts
        
        cms.PSet(
            pluginName = cms.string('electronIdCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIdCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronAntiCrackCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronAntiCrackCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronEtaCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEtaCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronPtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronPtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronIsoCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIsoCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronConversionVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronConversionVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronTrkIPcutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIPcutBgEstZeeElectronMisIdEnriched','cumulative')
        ),


        #start tau cuts
        
        cms.PSet(
            pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauEtaCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEtaCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauPtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauPtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauIsoCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauIsoCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauElectronVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauElectronVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ), 
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),

        #start ditau cuts
        
        cms.PSet(
            pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched')
        )
    ),
  
    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
         diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
         electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
         tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
    ),

    eventDumps = cms.VPSet(),
   
    analysisSequence = cms.VPSet(
    
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
        cms.PSet(
            filter = cms.string('genPhaseSpaceCut'),
            title = cms.string('gen. Phase-Space')
        ),
        cms.PSet(
            filter = cms.string('evtSelTrigger'),
            title = cms.string('Trigger')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexQuality'),
            title = cms.string('Valid vertex with #dof >= 4')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexPosition'),
            title = cms.string('Vertex abs(z) < 24 & Rho < 2')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexHighestPtTrackSum'),
            title = cms.string('Highest Pt Vertex')
        ),
        cms.PSet(
            filter = cms.string('electronIdCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron ID'),
        ),
        cms.PSet(
            filter = cms.string('electronAntiCrackCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('electronEtaCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('-2.5 < eta(Electron) < +2.5'),
        ),
        cms.PSet(
            filter = cms.string('electronPtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Pt(Electron) > 20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau not overlapping with Elec.'),
        ),
        cms.PSet(
            filter = cms.string('tauEtaCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('-2.3 < eta(Tau) < +2.3'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedPtCumulative',
                                  'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedPtCumulative'
                                  )
        ),           
        cms.PSet(
            filter = cms.string('electronIsoCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron Isolation'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedIsoCumulative')
        ),          
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron Track conversion veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedConversionVetoCumulative')
        ),          
        cms.PSet(
            filter = cms.string('electronTrkIPcutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron Track IP'),
        ),        
        cms.PSet(
            filter = cms.string('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau lead. Track Pt'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedTrkIPcumulative',
                                  'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedLeadTrkPtCumulative')
        ),
        cms.PSet(
            filter = cms.string('tauIsoCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau ID byHPSloose'),            
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedIsoCumulative')
        ),         
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau e-Veto (inverted)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedElectronVetoCumulative')
        ),              
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau mu-Veto'),
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('dR(Electron-Tau) > 0.5'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedMuonVetoCumulative',
                                  'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched.diTauCandidateSource = elecTauPairsBgEstZeeElectronMisIdEnrichedAntiOverlapVetoCumulative',
                                  'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched.diTauCandidateSource = elecTauPairsBgEstZeeElectronMisIdEnrichedAntiOverlapVetoCumulative'
                                  )
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('M_{T}(Electron-MET) < 40 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Pzeta-1.5*Pzeta(vis) > -20 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),            
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Charge(Electron+Tau) = 0'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('no 2nd OS, loosely-isolated e- (inverted)')
         ),

        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Charge(Electron+Tau) = 0'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),        

    )

)







analyzeEventsBgEstZeeElectronMisIdEnrichedBtag = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_ZeeElectronMisIdEnrichedBtag'), 

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,        
        evtSelPrimaryEventVertexHighestPtTrackSum,

        #start electron cuts
        
        cms.PSet(
            pluginName = cms.string('electronIdCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIdCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronAntiCrackCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronAntiCrackCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronEtaCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEtaCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronPtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronPtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronIsoCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIsoCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronConversionVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronConversionVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronTrkIPcutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIPcutBgEstZeeElectronMisIdEnriched','cumulative')
        ),


        #start tau cuts
        
        cms.PSet(
            pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauEtaCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEtaCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauPtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauPtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauIsoCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauIsoCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauElectronVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauElectronVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ), 
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),

        #start ditau cuts
        
        cms.PSet(
            pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('jetEtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetEtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('jetBtagCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetBtagCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),


        
    ),
  
    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
         diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
         electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
         tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
    ),

    eventDumps = cms.VPSet(),
   
    analysisSequence = cms.VPSet(
    
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
        cms.PSet(
            filter = cms.string('genPhaseSpaceCut'),
            title = cms.string('gen. Phase-Space')
        ),
        cms.PSet(
            filter = cms.string('evtSelTrigger'),
            title = cms.string('Trigger')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexQuality'),
            title = cms.string('Valid vertex with #dof >= 4')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexPosition'),
            title = cms.string('Vertex abs(z) < 24 & Rho < 2')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexHighestPtTrackSum'),
            title = cms.string('Highest Pt Vertex')
        ),
        cms.PSet(
            filter = cms.string('electronIdCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron ID'),
        ),
        cms.PSet(
            filter = cms.string('electronAntiCrackCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('electronEtaCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('-2.5 < eta(Electron) < +2.5'),
        ),
        cms.PSet(
            filter = cms.string('electronPtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Pt(Electron) > 20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau not overlapping with Elec.'),
        ),
        cms.PSet(
            filter = cms.string('tauEtaCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('-2.3 < eta(Tau) < +2.3'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedPtCumulative',
                                  'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedPtCumulative'
                                  )
        ),           
        cms.PSet(
            filter = cms.string('electronIsoCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron Isolation'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedIsoCumulative')
        ),          
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron Track conversion veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedConversionVetoCumulative')
        ),          
        cms.PSet(
            filter = cms.string('electronTrkIPcutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron Track IP'),
        ),        
        cms.PSet(
            filter = cms.string('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau lead. Track Pt'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedTrkIPcumulative',
                                  'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedLeadTrkPtCumulative')
        ),
        cms.PSet(
            filter = cms.string('tauIsoCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau ID byHPSloose'),            
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedIsoCumulative')
        ),         
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau e-Veto (inverted)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedElectronVetoCumulative')
        ),              
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau mu-Veto'),
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('dR(Electron-Tau) > 0.5'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedMuonVetoCumulative',
                                  'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched.diTauCandidateSource = elecTauPairsBgEstZeeElectronMisIdEnrichedAntiOverlapVetoCumulative',
                                  'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched.diTauCandidateSource = elecTauPairsBgEstZeeElectronMisIdEnrichedAntiOverlapVetoCumulative'
                                  )
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('M_{T}(Electron-MET) < 40 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Pzeta-1.5*Pzeta(vis) > -20 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),            
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('no 2nd OS, loosely-isolated e- (inverted)')
         ),

        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),
        cms.PSet(
            filter = cms.string('jetEtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('N(jets with E_{T} > 30) < 2'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
        ),            
        cms.PSet(
            filter = cms.string('jetBtagCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('E_{T} > 20 GeV jet with b-Tag'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            
        ), 
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Charge(Electron+Tau) = 0'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),        

    )

)








analyzeEventsBgEstZeeElectronMisIdEnrichedBtagVeto = cms.EDAnalyzer("GenericAnalyzer",

    name = cms.string('BgEstTemplateAnalyzer_ZeeElectronMisIdEnrichedBtagVeto'), 

    filters = cms.VPSet(
        evtSelGenPhaseSpace,
        evtSelTrigger,
        evtSelPrimaryEventVertexQuality,
        evtSelPrimaryEventVertexPosition,        
        evtSelPrimaryEventVertexHighestPtTrackSum,

        #start electron cuts
        
        cms.PSet(
            pluginName = cms.string('electronIdCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIdCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronAntiCrackCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronAntiCrackCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronEtaCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronEtaCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronPtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronPtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('electronIsoCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronIsoCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronConversionVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronConversionVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('electronTrkIPcutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('electronTrkIPcutBgEstZeeElectronMisIdEnriched','cumulative')
        ),


        #start tau cuts
        
        cms.PSet(
            pluginName = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauEtaCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauEtaCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauPtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauPtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),  
        cms.PSet(
            pluginName = cms.string('tauIsoCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauIsoCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('tauElectronVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauElectronVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ), 
        cms.PSet(
            pluginName = cms.string('tauMuonVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('tauMuonVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),

        #start ditau cuts
        
        cms.PSet(
            pluginName = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),        
        cms.PSet(
            pluginName = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched')
        ),
        cms.PSet(
            pluginName = cms.string('jetEtCutBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetEtCutBgEstZeeElectronMisIdEnriched','cumulative')
        ),
        cms.PSet(
            pluginName = cms.string('jetBtagVetoBgEstZeeElectronMisIdEnriched'),
            pluginType = cms.string('BoolEventSelector'),
            src = cms.InputTag('jetBtagVetoBgEstZeeElectronMisIdEnriched','cumulative')
        ),


        
    ),
  
    analyzers = cms.VPSet(
         diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
         diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
         electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
         tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched,
    ),

    eventDumps = cms.VPSet(),
   
    analysisSequence = cms.VPSet(
    
        # generator level phase-space selection
        # (NOTE: (1) to be used in case of Monte Carlo samples
        #            overlapping in simulated phase-space only !!
        #        (2) genPhaseSpaceCut needs to be **always** the first entry in the list of cuts
        #           - otherwise the script submitToBatch.csh for submission of cmsRun jobs
        #            to the CERN batch system will not work !!)
        cms.PSet(
            filter = cms.string('genPhaseSpaceCut'),
            title = cms.string('gen. Phase-Space')
        ),
        cms.PSet(
            filter = cms.string('evtSelTrigger'),
            title = cms.string('Trigger')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexQuality'),
            title = cms.string('Valid vertex with #dof >= 4')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexPosition'),
            title = cms.string('Vertex abs(z) < 24 & Rho < 2')
        ),
        cms.PSet(
            filter = cms.string('evtSelPrimaryEventVertexHighestPtTrackSum'),
            title = cms.string('Highest Pt Vertex')
        ),
        cms.PSet(
            filter = cms.string('electronIdCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron ID'),
        ),
        cms.PSet(
            filter = cms.string('electronAntiCrackCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron crack-Veto'),
        ),
        cms.PSet(
            filter = cms.string('electronEtaCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('-2.5 < eta(Electron) < +2.5'),
        ),
        cms.PSet(
            filter = cms.string('electronPtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Pt(Electron) > 20 GeV'),
        ),
        cms.PSet(
            filter = cms.string('tauAntiOverlapWithElectronsVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau not overlapping with Elec.'),
        ),
        cms.PSet(
            filter = cms.string('tauEtaCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('-2.3 < eta(Tau) < +2.3'),
        ),
        cms.PSet(
            filter = cms.string('tauPtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Pt(Tau) > 20 GeV'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedPtCumulative',
                                  'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedPtCumulative'
                                  )
        ),           
        cms.PSet(
            filter = cms.string('electronIsoCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron Isolation'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedIsoCumulative')
        ),          
        cms.PSet(
            filter = cms.string('electronConversionVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron Track conversion veto'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedConversionVetoCumulative')
        ),          
        cms.PSet(
            filter = cms.string('electronTrkIPcutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Electron Track IP'),
        ),        
        cms.PSet(
            filter = cms.string('tauLeadTrkPtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau lead. Track Pt'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched.electronSource = electronsBgEstZeeElectronMisIdEnrichedTrkIPcumulative',
                                  'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedLeadTrkPtCumulative')
        ),
        cms.PSet(
            filter = cms.string('tauIsoCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau ID byHPSloose'),            
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedIsoCumulative')
        ),         
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('tauElectronVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau e-Veto (inverted)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedElectronVetoCumulative')
        ),              
        cms.PSet(
            filter = cms.string('tauMuonVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Tau mu-Veto'),
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidateAntiOverlapVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('dR(Electron-Tau) > 0.5'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    ),
            replace = cms.vstring('tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched.tauSource = tausBgEstZeeElectronMisIdEnrichedMuonVetoCumulative',
                                  'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched.diTauCandidateSource = elecTauPairsBgEstZeeElectronMisIdEnrichedAntiOverlapVetoCumulative',
                                  'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched.diTauCandidateSource = elecTauPairsBgEstZeeElectronMisIdEnrichedAntiOverlapVetoCumulative'
                                  )
        ),
        cms.PSet(
            filter = cms.string('diTauCandidateMt1METCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('M_{T}(Electron-MET) < 40 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
        ),        
        cms.PSet(
            filter = cms.string('diTauCandidatePzetaDiffCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Pzeta-1.5*Pzeta(vis) > -20 GeV (off)'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),            
        #!!!!!!!!!!!!!!!!!!!!!!!!!!!!! inverted cut
        cms.PSet(
            filter = cms.string('diTauCandidateZeeHypothesisVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('no 2nd OS, loosely-isolated e- (inverted)')
         ),

        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),
        cms.PSet(
            filter = cms.string('jetEtCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('N(jets with E_{T} > 30) < 2'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
        ),            
        cms.PSet(
            filter = cms.string('jetBtagVetoBgEstZeeElectronMisIdEnriched'),
            title = cms.string('no E_{T} > 20 GeV jet with b-Tag'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    ),
            
        ), 
        cms.PSet(
            filter = cms.string('diTauCandidateZeroChargeCutBgEstZeeElectronMisIdEnriched'),
            title = cms.string('Charge(Electron+Tau) = 0'),
        ),
        cms.PSet(
            analyzers = cms.vstring('electronHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'tauHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateHistManagerForElecTauBgEstZeeElectronMisIdEnriched',
                                    'diTauCandidateNSVfitHistManagerForElecTauBgEstZeeElectronMisIdEnriched',                                    
                                    
                                    ),
        ),        

    )

)




#--------------------------------------------------------------------------------  
# define (final) analysis sequence
#--------------------------------------------------------------------------------

bgEstZeeElectronMisIdEnrichedAnalysisSequence = cms.Sequence(


      selectElectronsBgEstZeeElectronMisIdEnriched
    + selectTausBgEstZeeElectronMisIdEnriched
    + produceElecTauPairsBgEstZeeElectronMisIdEnriched
    + selectElecTauPairsBgEstZeeElectronMisIdEnriched
    + produceElecTauPairZeeHypothesesBgEstZeeElectronMisIdEnriched
    + selectPatJetsBgEstZeeElectronMisIdEnriched
    + selectEventsBgEstZeeElectronMisIdEnriched 
    #+ analyzeEventsBgEstZeeElectronMisIdEnriched
    + analyzeEventsBgEstZeeElectronMisIdEnrichedBtag
    + analyzeEventsBgEstZeeElectronMisIdEnrichedBtagVeto      

)
