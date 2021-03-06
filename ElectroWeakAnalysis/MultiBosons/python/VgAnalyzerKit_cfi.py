import FWCore.ParameterSet.Config as cms
from ElectroWeakAnalysis.MultiBosons.kinAxis_cfi  import *

VgAnalyzerKit = cms.EDProducer("VgAnalyzerKit",
                               ntuplize = cms.string('none'),
                               outputTextName = cms.string('VgAnalyzerKit_output.txt'),
                               enable = cms.string(''),
                               disable = cms.string(''),
                               doMuon         = cms.bool(True),
                               doElectron     = cms.bool(True),
                               doTau          = cms.bool(True),
                               doJet          = cms.bool(True),
                               doMET          = cms.bool(True),
                               doPhoton       = cms.bool(True),
                               doTrack        = cms.bool(True),
                               doGenParticles = cms.bool(True),
                               doStoreJets    = cms.bool(False),
                               doJetHLTMatch  = cms.bool(False),
                               doSkim         = cms.bool(True),
                               doZmumu        = cms.bool(False),
                               electronSrc = cms.InputTag("cleanPatElectrons"),
                               tauSrc = cms.InputTag("cleanPatTaus"),
                               muonSrc = cms.InputTag("cleanPatMuons"),
                               jetSrc = cms.InputTag("cleanPatJets"),
                               photonSrc = cms.InputTag("cleanPatPhotons"),
                               METSrc = cms.InputTag("patMETs"),
                               trackSrc = cms.InputTag("allTrackCandidates"),
                               genParticleSrc = cms.InputTag("genParticles"),
                               zmumuSrc = cms.InputTag("dimuonsGlobal"),
                               L1GTReadoutRecord =cms.InputTag("hltGtDigis"),
                               triggerResults = cms.InputTag("TriggerResults::HLT"),
                               triggerEvent = cms.InputTag("patTriggerEvent"),
                               GTDigiLabel = cms.InputTag("gtDigis"),
                               VtxLabel = cms.InputTag("offlinePrimaryVertices"),
                               CaloTowerLabel = cms.InputTag("towerMaker"),
                               TrackLabel = cms.InputTag("generalTracks"),
                               #tcMETLabel = cms.InputTag("tcMet"),
                               #pfMETLabel = cms.InputTag("pfMet"),
                               tcMETLabel = cms.InputTag("patMETsTC"),
                               pfMETLabel = cms.InputTag("patMETsPF"),
                               TypeIpfMETLabel = cms.InputTag("pfType1CorrectedMet"),
                               SmearedTypeIpfMETLabel = cms.InputTag("smearedPFType1CorrectedMet"),
                               PFCandLabel = cms.InputTag("particleFlow"),
                               puInfoLabel = cms.InputTag("addPileupInfo"),
                               rhoLabel = cms.InputTag("kt6PFJets", "rho"),
                               sigmaLabel = cms.InputTag("kt6PFJets", "sigma"),
                               rho2011Label = cms.InputTag("kt6PFJetsForIsolation", "rho"),
                               rho2012Label = cms.InputTag("kt6PFJets", "rho"),
                               LeadingElePtCut = cms.double(15),
                               LeadingMuPtCut = cms.double(15),
                               LeadingPhoPtCut = cms.double(10),
                               ebReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEB"),
                               eeReducedRecHitCollection = cms.InputTag("reducedEcalRecHitsEE"),
                               BeamSpotCollection = cms.InputTag("offlineBeamSpot"),
                               IsoDepElectron = cms.VInputTag(cms.InputTag('elPFIsoDepositChargedPFIso'),
                                                              cms.InputTag('elPFIsoDepositNeutralPFIso'),
                                                              cms.InputTag('elPFIsoDepositGammaPFIso')),
                               IsoValElectronPF = cms.VInputTag(cms.InputTag('elPFIsoValueCharged03PFIdPFIso'),
                                                                cms.InputTag('elPFIsoValueNeutral03PFIdPFIso'),
                                                                cms.InputTag('elPFIsoValueGamma03PFIdPFIso'),
								cms.InputTag('elPFIsoValueCharged04PFIdPFIso'),
								cms.InputTag('elPFIsoValueNeutral04PFIdPFIso'),
								cms.InputTag('elPFIsoValueGamma04PFIdPFIso')),
                               IsoDepPhoton = cms.VInputTag(cms.InputTag('phPFIsoDepositChargedPFIso'),
                                                            cms.InputTag('phPFIsoDepositNeutralPFIso'),
                                                            cms.InputTag('phPFIsoDepositGammaPFIso')),
                               IsoValPhoton = cms.VInputTag(cms.InputTag('phPFIsoValueCharged03PFIdPFIso'),
                                                            cms.InputTag('phPFIsoValueNeutral03PFIdPFIso'),
                                                            cms.InputTag('phPFIsoValueGamma03PFIdPFIso')),
                               muonAxis     = kinAxis(0, 200, 0, 200),
                               electronAxis = kinAxis(0, 200, 0, 200),
                               tauAxis      = kinAxis(0, 200, 0, 200),
                               jetAxis      = kinAxis(0, 200, 0, 200),
                               METAxis      = kinAxis(0, 200, 0, 200),
                               photonAxis   = kinAxis(0, 200, 0, 200),
                               trackAxis    = kinAxis(0, 200, 0, 200),
                               genParticleAxis = kinAxis(0, 200, 0, 200)
                               )


