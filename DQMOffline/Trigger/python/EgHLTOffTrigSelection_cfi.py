import FWCore.ParameterSet.Config as cms


from DQMOffline.Trigger.EgHLTOffEleSelection_cfi import *
from DQMOffline.Trigger.EgHLTOffPhoSelection_cfi import *

egHLTOffBaseEleTrigCuts = cms.PSet (
    trigName = cms.string("default"),                
    barrel = cms.PSet(egHLTOffEleBarrelCuts),
    endcap = cms.PSet(egHLTOffEleEndcapCuts)
)

egHLTOffBasePhoTrigCuts = cms.PSet (
    trigName = cms.string("default"),                
    barrel = cms.PSet(egHLTOffPhoBarrelCuts),
    endcap = cms.PSet(egHLTOffPhoEndcapCuts)
)

#8E29
egHLTOffEleEt10LWCuts = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffEleEt10LWCuts.trigName = "hltL1NonIsoHLTNonIsoSingleElectronLWEt10PixelMatchFilter"
egHLTOffEleEt10LWCuts.barrel.minEt = 10.
egHLTOffEleEt10LWCuts.barrel.cuts = "et"
egHLTOffEleEt10LWCuts.endcap.minEt = 10.
egHLTOffEleEt10LWCuts.endcap.cuts = "et"

egHLTOffEleEt15LWCuts = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffEleEt15LWCuts.trigName = "hltL1NonIsoHLTNonIsoSingleElectronLWEt15PixelMatchFilter"
egHLTOffEleEt15LWCuts.barrel.minEt = 15.
egHLTOffEleEt15LWCuts.barrel.cuts = "et"
egHLTOffEleEt15LWCuts.endcap.minEt = 15.
egHLTOffEleEt15LWCuts.endcap.cuts = "et"

egHLTOffEleEt10LWEleIdCuts = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffEleEt10LWEleIdCuts.trigName = "hltL1NonIsoHLTNonIsoSingleElectronLWEt10EleIdDphiFilter"
egHLTOffEleEt10LWEleIdCuts.barrel.minEt = 15.
egHLTOffEleEt10LWEleIdCuts.barrel.maxSigmaEtaEta = 0.015
egHLTOffEleEt10LWEleIdCuts.barrel.maxDEtaIn = 0.008
egHLTOffEleEt10LWEleIdCuts.barrel.maxDPhiIn = 0.1
egHLTOffEleEt10LWEleIdCuts.barrel.cuts = "et:hltDEtaIn:hltDPhiIn:sigmaEtaEta:ctfTrack"
egHLTOffEleEt10LWEleIdCuts.endcap.minEt = 15.
egHLTOffEleEt10LWEleIdCuts.endcap.maxSigmaEtaEta = 0.04
egHLTOffEleEt10LWEleIdCuts.endcap.maxDEtaIn = 0.008
egHLTOffEleEt10LWEleIdCuts.endcap.maxDPhiIn = 0.1
egHLTOffEleEt10LWEleIdCuts.endcap.cuts = "et:hltDEtaIn:hltDPhiIn:sigmaEtaEta:ctfTrack"

egHLTOffDoubleEleEt5Cuts = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffDoubleEleEt5Cuts.trigName = "hltL1NonIsoHLTNonIsoDoubleElectronEt5PixelMatchFilter"
egHLTOffDoubleEleEt5Cuts.barrel.minEt = 5.
egHLTOffDoubleEleEt5Cuts.barrel.cuts = "et"
egHLTOffDoubleEleEt5Cuts.endcap.minEt = 5.
egHLTOffDoubleEleEt5Cuts.endcap.cuts = "et"



#1E31
egHLTOffEleEt10SWCuts = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffEleEt10SWCuts.trigName = "hltL1NonIsoHLTNonIsoSingleElectronEt10PixelMatchFilter"
egHLTOffEleEt10SWCuts.barrel.minEt = 10.
egHLTOffEleEt10SWCuts.barrel.cuts = "et"
egHLTOffEleEt10SWCuts.endcap.minEt = 10.
egHLTOffEleEt10SWCuts.endcap.cuts = "et"

egHLTOffEleEt15SWCuts = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffEleEt15SWCuts.trigName = "hltL1NonIsoHLTNonIsoSingleElectronEt15PixelMatchFilter"
egHLTOffEleEt15SWCuts.barrel.minEt = 15.
egHLTOffEleEt15SWCuts.barrel.cuts = "et"
egHLTOffEleEt15SWCuts.endcap.minEt = 15.
egHLTOffEleEt15SWCuts.endcap.cuts = "et"

egHLTOffEleEt20SWCuts  = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffEleEt20SWCuts.trigName = "hltL1NonIsoHLTNonIsoSingleElectronEt20PixelMatchFilter"
egHLTOffEleEt20SWCuts.barrel.minEt = 20.
egHLTOffEleEt20SWCuts.barrel.cuts = "et"
egHLTOffEleEt20SWCuts.endcap.minEt = 20.
egHLTOffEleEt20SWCuts.endcap.cuts = "et"

egHLTOffEleEt15SWEleIdCuts = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffEleEt15SWEleIdCuts.trigName = "hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdDphiFilter"
egHLTOffEleEt15SWEleIdCuts.barrel.minEt = 15.
egHLTOffEleEt15SWEleIdCuts.barrel.maxSigmaEtaEta = 0.015
egHLTOffEleEt15SWEleIdCuts.barrel.maxHLTDEtaIn = 0.008
egHLTOffEleEt15SWEleIdCuts.barrel.maxHLTDPhiIn = 0.1
egHLTOffEleEt15SWEleIdCuts.barrel.cuts = "et:hltDEtaIn:hltDPhiIn:sigmaEtaEta:ctfTrack"
egHLTOffEleEt15SWEleIdCuts.endcap.minEt = 15.
egHLTOffEleEt15SWEleIdCuts.endcap.maxSigmaEtaEta = 0.04
egHLTOffEleEt15SWEleIdCuts.endcap.maxHLTDEtaIn = 0.008
egHLTOffEleEt15SWEleIdCuts.endcap.maxHLTDPhiIn = 0.1
egHLTOffEleEt15SWEleIdCuts.endcap.cuts = "et:hltDEtaIn:hltDPhiIn:sigmaEtaEta:ctfTrack"


egHLTOffEleEt15SWEleIdLTICuts = cms.PSet(egHLTOffEleEt15SWEleIdCuts)
egHLTOffEleEt15SWEleIdLTICuts.trigName = "hltL1NonIsoHLTNonIsoSingleElectronEt15EleIdTrackIsolFilterESet25LTI"
egHLTOffEleEt15SWEleIdLTICuts.barrel.minEt = 25.
egHLTOffEleEt15SWEleIdLTICuts.barrel.maxHLTIsolTrksEleOverPt=0.5
egHLTOffEleEt15SWEleIdLTICuts.barrel.maxHLTIsolTrksEle=8
egHLTOffEleEt15SWEleIdLTICuts.barrel.cuts = "et:dEtaIn:dPhiIn:sigmaIEtaIEta:hltIsolTrksEle:ctfTrack"
egHLTOffEleEt15SWEleIdLTICuts.endcap.minEt = 25.
egHLTOffEleEt15SWEleIdLTICuts.endcap.maxHLTIsolTrksEleOverPt=0.5
egHLTOffEleEt15SWEleIdLTICuts.endcap.maxHLTIsolTrksEle=8
egHLTOffEleEt15SWEleIdLTICuts.endcap.cuts = "et:dEtaIn:dPhiIn:sigmaIEtaIEta:hltIsolTrksEle:ctfTrack"

egHLTOffEleEt15SWLTICuts = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffEleEt15SWLTICuts.trigName = "hltL1NonIsoHLTNonIsoSingleElectronEt15LTITrackIsolFilter"
egHLTOffEleEt15SWLTICuts.barrel.minEt = 15.
egHLTOffEleEt15SWLTICuts.barrel.maxHLTIsolTrksEleOverPt=0.5
egHLTOffEleEt15SWLTICuts.barrel.maxHLTIsolTrksEle=8
egHLTOffEleEt15SWLTICuts.barrel.cuts = "et:hltIsolTrksEle:ctfTrack"
egHLTOffEleEt15SWLTICuts.endcap.minEt = 15.
egHLTOffEleEt15SWLTICuts.endcap.maxHLTIsolTrksEleOverPt=0.5
egHLTOffEleEt15SWLTICuts.endcap.maxHLTIsolTrksEle=8
egHLTOffEleEt15SWLTICuts.endcap.cuts = "et:hltIsolTrksEle:ctfTrack"

egHLTOffDoubleEleEt10SWCuts = cms.PSet(egHLTOffBaseEleTrigCuts)
egHLTOffDoubleEleEt10SWCuts.trigName = "hltL1NonIsoHLTNonIsoDoubleElectronEt10PixelMatchFilter"
egHLTOffDoubleEleEt10SWCuts.barrel.minEt = 10.
egHLTOffDoubleEleEt10SWCuts.barrel.cuts = "et"
egHLTOffDoubleEleEt10SWCuts.endcap.minEt = 10.
egHLTOffDoubleEleEt10SWCuts.endcap.cuts = "et"

#photons
egHLTOffPhoEt10Cuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffPhoEt10Cuts.trigName = "hltL1NonIsoHLTNonIsoSinglePhotonEt10HcalIsolFilter"
egHLTOffPhoEt10Cuts.barrel.minEt = 10.
egHLTOffPhoEt10Cuts.barrel.cuts = "et"
egHLTOffPhoEt10Cuts.endcap.minEt = 10.
egHLTOffPhoEt10Cuts.endcap.cuts = "et"

egHLTOffPhoEt15Cuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffPhoEt15Cuts.trigName = "hltL1NonIsoHLTNonIsoSinglePhotonEt15HcalIsolFilter"
egHLTOffPhoEt15Cuts.barrel.minEt = 15.
egHLTOffPhoEt15Cuts.barrel.cuts = "et"
egHLTOffPhoEt15Cuts.endcap.minEt = 15.
egHLTOffPhoEt15Cuts.endcap.cuts = "et"

egHLTOffPhoEt15LEICuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffPhoEt15LEICuts.trigName = "hltL1NonIsoSinglePhotonEt15LEIHcalIsolFilter"
egHLTOffPhoEt15LEICuts.barrel.minEt = 15.
egHLTOffPhoEt15LEICuts.barrel.maxHLTIsolEm = 3.;
egHLTOffPhoEt15LEICuts.barrel.maxHLTIsolEmOverEt = .1;
egHLTOffPhoEt15LEICuts.barrel.maxHLTIsolEmOverEt2 = -1.;
egHLTOffPhoEt15LEICuts.barrel.cuts = "et:hltIsolEm"
egHLTOffPhoEt15LEICuts.endcap.minEt = 15.
egHLTOffPhoEt15LEICuts.endcap.maxHLTIsolEm = 3.;
egHLTOffPhoEt15LEICuts.endcap.maxHLTIsolEmOverEt = .1;
egHLTOffPhoEt15LEICuts.endcap.maxHLTIsolEmOverEt2 = -1.;
egHLTOffPhoEt15LEICuts.endcap.cuts = "et:hltIsolEm"


egHLTOffPhoEt15HTICuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffPhoEt15HTICuts.trigName = "hltL1NonIsoSinglePhotonEt15HTITrackIsolFilter"
egHLTOffPhoEt15HTICuts.barrel.minEt = 15.
egHLTOffPhoEt15HTICuts.barrel.maxHLTIsolTrksPho = 4.
egHLTOffPhoEt15HTICuts.barrel.maxHLTIsolTrksPhoOverPt = 0.05
egHLTOffPhoEt15HTICuts.barrel.maxHLTIsolTrksPhoOverPt2 = -1
egHLTOffPhoEt15HTICuts.barrel.cuts = "et:hltIsolTrksPho"
egHLTOffPhoEt15HTICuts.endcap.minEt = 15.
egHLTOffPhoEt15HTICuts.endcap.maxHLTIsolTrksPho = 4.
egHLTOffPhoEt15HTICuts.endcap.maxHLTIsolTrksPhoOverPt = 0.05
egHLTOffPhoEt15HTICuts.endcap.maxHLTIsolTrksPhoOverPt2 = -1
egHLTOffPhoEt15HTICuts.endcap.cuts = "et:hltIsolTrksPho"

egHLTOffPhoEt20Cuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffPhoEt20Cuts.trigName = "hltL1NonIsoHLTNonIsoSinglePhotonEt20HcalIsolFilter"
egHLTOffPhoEt20Cuts.barrel.minEt = 20.
egHLTOffPhoEt20Cuts.barrel.cuts = "et"
egHLTOffPhoEt20Cuts.endcap.minEt = 20.
egHLTOffPhoEt20Cuts.endcap.cuts = "et"



egHLTOffPhoEt10LEITICuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffPhoEt10LEITICuts.trigName = "hltL1NonIsoHLTLEITISinglePhotonEt10TrackIsolFilter"                                   
egHLTOffPhoEt10LEITICuts.barrel.minEt = 15.
egHLTOffPhoEt10LEITICuts.barrel.maxHLTIsolTrksPho = 4.
egHLTOffPhoEt10LEITICuts.barrel.maxHLTIsolTrksPhoOverPt = 0.05
egHLTOffPhoEt10LEITICuts.barrel.maxHLTIsolTrksPhoOverPt2 = -1
egHLTOffPhoEt10LEITICuts.barrel.maxHLTIsolEm = 3.;
egHLTOffPhoEt10LEITICuts.barrel.maxHLTIsolEmOverEt = .1;
egHLTOffPhoEt10LEITICuts.barrel.maxHLTIsolEmOverEt2 = -1.;
egHLTOffPhoEt10LEITICuts.barrel.cuts = "et:hltIsolTrksPho:hltIsolEm"
egHLTOffPhoEt10LEITICuts.endcap.minEt = 15.
egHLTOffPhoEt10LEITICuts.endcap.maxHLTIsolTrksPho = 4.
egHLTOffPhoEt10LEITICuts.endcap.maxHLTIsolTrksPhoOverPt = 0.05
egHLTOffPhoEt10LEITICuts.endcap.maxHLTIsolTrksPhoOverPt2 = -1
egHLTOffPhoEt10LEITICuts.endcap.maxHLTIsolEm = 3.;
egHLTOffPhoEt10LEITICuts.endcap.maxHLTIsolEmOverEt = .1;
egHLTOffPhoEt10LEITICuts.endcap.maxHLTIsolEmOverEt2 = -1.;
egHLTOffPhoEt10LEITICuts.endcap.cuts = "et:hltIsolTrksPho:hltIsolEm"


egHLTOffPhoEt20LEITICuts = cms.PSet(egHLTOffPhoEt10LEITICuts)
egHLTOffPhoEt20LEITICuts.trigName = "hltL1NonIsoHLTLEITISinglePhotonEt20TrackIsolFilter"
egHLTOffPhoEt20LEITICuts.barrel.minEt = 20.
egHLTOffPhoEt20LEITICuts.endcap.minEt = 20.


egHLTOffPhoEt25LEITICuts = cms.PSet(egHLTOffPhoEt10LEITICuts)
egHLTOffPhoEt25LEITICuts.trigName = "hltL1NonIsoHLTLEITISinglePhotonEt25TrackIsolFilter"
egHLTOffPhoEt25LEITICuts.barrel.minEt = 25.
egHLTOffPhoEt25LEITICuts.endcap.minEt = 25.


egHLTOffPhoEt25Cuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffPhoEt25Cuts.trigName = "hltL1NonIsoHLTNonIsoSinglePhotonEt25HcalIsolFilter"
egHLTOffPhoEt25Cuts.barrel.minEt = 25.
egHLTOffPhoEt25Cuts.barrel.cuts = "et"
egHLTOffPhoEt25Cuts.endcap.minEt = 25.
egHLTOffPhoEt25Cuts.endcap.cuts = "et"

egHLTOffPhoEt30Cuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffPhoEt30Cuts.trigName = "hltL1NonIsoHLTNonIsoSinglePhotonEt30HcalIsolFilter"
egHLTOffPhoEt30Cuts.barrel.minEt = 30.
egHLTOffPhoEt30Cuts.barrel.cuts = "et"
egHLTOffPhoEt30Cuts.endcap.minEt = 30.
egHLTOffPhoEt30Cuts.endcap.cuts = "et"


egHLTOffDoublePhoEt10Cuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffDoublePhoEt10Cuts.trigName = "hltL1NonIsoHLTNonIsoDoublePhotonEt10HcalIsolFilter"
egHLTOffDoublePhoEt10Cuts.barrel.minEt = 10.
egHLTOffDoublePhoEt10Cuts.barrel.cuts = "et"
egHLTOffDoublePhoEt10Cuts.endcap.minEt = 10.
egHLTOffDoublePhoEt10Cuts.endcap.cuts = "et"

egHLTOffDoublePhoEt15Cuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffDoublePhoEt15Cuts.trigName = "hltL1NonIsoHLTNonIsoDoublePhotonEt15HcalIsolFilter"
egHLTOffDoublePhoEt15Cuts.barrel.minEt = 15.
egHLTOffDoublePhoEt15Cuts.barrel.cuts = "et"
egHLTOffDoublePhoEt15Cuts.endcap.minEt = 15.
egHLTOffDoublePhoEt15Cuts.endcap.cuts = "et"

egHLTOffDoublePhoEt15VLEICuts = cms.PSet(egHLTOffBasePhoTrigCuts)
egHLTOffDoublePhoEt15VLEICuts.trigName = "hltL1NonIsoHLTVLEIDoublePhotonEt15HcalIsolFilter"
egHLTOffDoublePhoEt15VLEICuts.barrel.minEt = 15.
egHLTOffDoublePhoEt15VLEICuts.barrel.maxHLTIsolEm = 5.;
egHLTOffDoublePhoEt15VLEICuts.barrel.maxHLTIsolEmOverEt = .2;
egHLTOffDoublePhoEt15VLEICuts.barrel.maxHLTIsolEmOverEt2 = -1.;

egHLTOffDoublePhoEt15VLEICuts.barrel.cuts = "et:hltIsolEm"
egHLTOffDoublePhoEt15VLEICuts.endcap.minEt = 15.
egHLTOffDoublePhoEt15VLEICuts.endcap.maxHLTIsolEm = 5.;
egHLTOffDoublePhoEt15VLEICuts.endcap.maxHLTIsolEmOverEt = .2;
egHLTOffDoublePhoEt15VLEICuts.endcap.maxHLTIsolEmOverEt2 = -1.;
egHLTOffDoublePhoEt15VLEICuts.endcap.cuts = "et:hltIsolEm"

