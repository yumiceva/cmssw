import FWCore.ParameterSet.Config as cms

# reorganization of Z->mumu categories sequence, to run after the ZMuMu(Sub)Skim (i.d. supposing dimuons, dimuonsGlobal, dimuonsOneTrack and dimuonsOneStndAloneMuon categories has been built)


### parameter set to be overloaded in the configuration file 


from ElectroWeakAnalysis.ZMuMu.goodZToMuMu_cfi import *
from ElectroWeakAnalysis.ZMuMu.goodZToMuMuSameCharge_cfi import *
from ElectroWeakAnalysis.ZMuMu.nonIsolatedZToMuMu_cfi import *
from ElectroWeakAnalysis.ZMuMu.goodZToMuMuOneTrack_cfi import *
from ElectroWeakAnalysis.ZMuMu.goodZToMuMuOneStandAloneMuon_cfi import *

### for zmusta modelling...

zmumuSaMassHistogram = cms.EDAnalyzer(
    "ZMuMuSaMassHistogram",
    src_m = cms.InputTag("goodZToMuMu"),
    min = cms.untracked.double(0.0),
    max = cms.untracked.double(200.0),
    nbin = cms.untracked.int32(200)
   # name = cms.untracked.string("zMass")    
    )


### paths for loose cuts, not notIso ones, not 1HLT and 2HLT: only ZGolden, zMuSta, zMuTk and ZGoldenSameChar...

goodZToMuMuPathLoose = cms.Path(
    goodZToMuMuLoose +
    goodZToMuMuAtLeast1HLTLoose
    )

goodZToMuMu2HLTPathLoose = cms.Path(
    goodZToMuMuLoose +
    goodZToMuMu2HLTLoose
    )

goodZToMuMu1HLTPathLoose = cms.Path(
    goodZToMuMuLoose +
    goodZToMuMu1HLTLoose
    )


goodZToMuMuSameChargePathLoose = cms.Path(
    dimuonsGlobalSameChargeLoose+
    goodZToMuMuSameChargeLoose +
    goodZToMuMuSameChargeAtLeast1HLTLoose
    )


## goodZToMuMuSameCharge2HLTPathLoose = cms.Path(
##     dimuonsGlobalSameChargeLoose+
##     goodZToMuMuSameChargeLoose +
##     goodZToMuMuSameCharge2HLTLoose
##     )


## goodZToMuMuSameCharge1HLTPathLoose = cms.Path(
##     dimuonsGlobalSameChargeLoose+
##     goodZToMuMuSameChargeLoose +
##     goodZToMuMuSameCharge1HLTLoose
##     )



goodZToMuMuOneStandAloneMuonPathLoose = cms.Path(
    ~goodZToMuMuLoose + 
    zToMuMuOneStandAloneMuonLoose + 
    goodZToMuMuOneStandAloneMuonLoose +
    goodZToMuMuOneStandAloneMuonFirstHLTLoose 
    )


goodZToMuMuOneTrackPathLoose=cms.Path(
    ~goodZToMuMuLoose +
    ~zToMuMuOneStandAloneMuonLoose +
    zToMuGlobalMuOneTrack +
    zToMuMuOneTrackLoose +
    goodZToMuMuOneTrackLoose +
    goodZToMuMuOneTrackFirstHLTLoose 
    )





### sequences and path for tight cuts...

initialGoodZToMuMuPath = cms.Path( 
    goodZToMuMuLoose +
    goodZToMuMu +
    zmumuSaMassHistogram     
)


goodZToMuMuPath = cms.Path(
    goodZToMuMuLoose +
    goodZToMuMuAtLeast1HLTLoose + 
    goodZToMuMu +
    goodZToMuMuAtLeast1HLT
    )



goodZToMuMu2HLTPath = cms.Path(
    goodZToMuMuLoose +
    goodZToMuMu2HLTLoose +
    goodZToMuMu +
    goodZToMuMu2HLT
    )


goodZToMuMu1HLTPath = cms.Path(
    goodZToMuMuLoose +
    goodZToMuMu1HLTLoose +
    goodZToMuMu +
    goodZToMuMu1HLT
    )



goodZToMuMuSameChargePath = cms.Path(
    dimuonsGlobalSameChargeLoose+
    goodZToMuMuSameChargeLoose +
    goodZToMuMuSameChargeAtLeast1HLTLoose +
    goodZToMuMuSameCharge +
    goodZToMuMuSameChargeAtLeast1HLT
    )


goodZToMuMuSameCharge2HLTPath = cms.Path(
    dimuonsGlobalSameChargeLoose+
    goodZToMuMuSameChargeLoose +
    goodZToMuMuSameChargeAtLeast1HLTLoose +
    goodZToMuMuSameCharge +
    goodZToMuMuSameCharge2HLT
    )



goodZToMuMuSameCharge1HLTPath = cms.Path(
    dimuonsGlobalSameChargeLoose+
    goodZToMuMuSameChargeLoose +
    goodZToMuMuSameChargeAtLeast1HLTLoose +
    goodZToMuMuSameCharge +
    goodZToMuMuSameCharge1HLT
    )



nonIsolatedZToMuMuPath = cms.Path (
    nonIsolatedZToMuMu +
    nonIsolatedZToMuMuAtLeast1HLT 
)


oneNonIsolatedZToMuMuPath  = cms.Path(
    nonIsolatedZToMuMu  +
    oneNonIsolatedZToMuMu  +
    oneNonIsolatedZToMuMuAtLeast1HLT  
)


twoNonIsolatedZToMuMuPath  = cms.Path(
    nonIsolatedZToMuMu  +
    twoNonIsolatedZToMuMu  +
    twoNonIsolatedZToMuMuAtLeast1HLT  
)


goodZToMuMuOneStandAloneMuonPath = cms.Path(
    ~goodZToMuMu +
    zToMuMuOneStandAloneMuon + 
    goodZToMuMuOneStandAloneMuon +
    goodZToMuMuOneStandAloneMuonFirstHLT 
    )




goodZToMuMuOneTrackPath=cms.Path(
    ~goodZToMuMu +
    ~zToMuMuOneStandAloneMuon +
    zToMuGlobalMuOneTrack +
    zToMuMuOneTrack +
    goodZToMuMuOneTrack +
    goodZToMuMuOneTrackFirstHLT 
    )

###### endPath




eventInfo = cms.OutputModule (
    "AsciiOutputModule"
)

eventInfo.setLabel("eventInfo")

NtuplesOut = cms.Sequence(
    eventInfo
    )


VtxedNtuplesOut = cms.Sequence(
    eventInfo
    )



endPath = cms.EndPath( 
   NtuplesOut +
   VtxedNtuplesOut 
)


