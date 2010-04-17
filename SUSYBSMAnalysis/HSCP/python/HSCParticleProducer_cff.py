import FWCore.ParameterSet.Config as cms


####################################################################################
#   BEAMSPOT + TRAJECTORY BUILDERS
####################################################################################

from RecoVertex.BeamSpotProducer.BeamSpot_cff import *
from RecoTracker.TrackProducer.TrackRefitters_cff import *

####################################################################################
#   DEDX ESTIMATORS 
####################################################################################


dedxHarm2 = cms.EDProducer("DeDxEstimatorProducer",
    tracks                     = cms.InputTag("TrackRefitter"),
    trajectoryTrackAssociation = cms.InputTag("TrackRefitter"),

    estimator      = cms.string('generic'),
    exponent       = cms.double(-2.0),

    UseStrip       = cms.bool(True),
    UsePixel       = cms.bool(True),
    MeVperADCStrip = cms.double(3.61e-06*250),
    MeVperADCPixel = cms.double(3.61e-06),

    MisCalib_Mean      = cms.untracked.double(1.0),
    MisCalib_Sigma     = cms.untracked.double(0.00),

    UseCalibration  = cms.bool(False),
    calibrationPath = cms.string(""),
)

dedxNPHarm2             = dedxHarm2.clone()
dedxNPHarm2.UsePixel    = cms.bool(False)

dedxCHarm2                = dedxHarm2.clone()
dedxCHarm2.UseCalibration = cms.bool(True)
dedxCHarm2.calibrationPath = cms.string("file:Gains.root")

dedxCNPHarm2                = dedxNPHarm2.clone()
dedxCNPHarm2.UseCalibration = cms.bool(True)
dedxCNPHarm2.calibrationPath = cms.string("file:Gains.root")

####################################################################################
#   DEDX DISCRIMINATORS
####################################################################################

dedxProd               = cms.EDProducer("DeDxDiscriminatorProducer",
    tracks                     = cms.InputTag("TrackRefitter"),
    trajectoryTrackAssociation = cms.InputTag("TrackRefitter"),

    Reccord            = cms.untracked.string("SiStripDeDxMip_3D_Rcd"),
    Formula            = cms.untracked.uint32(0),
#    ProbabilityMode    = cms.untracked.string("Integral"),
    ProbabilityMode    = cms.untracked.string("Accumulation"),


    UseStrip           = cms.bool(True),
    UsePixel           = cms.bool(True),
    MeVperADCStrip     = cms.double(3.61e-06*250),
    MeVperADCPixel     = cms.double(3.61e-06),

    MisCalib_Mean      = cms.untracked.double(1.0),
    MisCalib_Sigma     = cms.untracked.double(0.00),

    UseCalibration  = cms.bool(True),
    calibrationPath = cms.string("file:Gains.root"),

    MaxNrStrips        = cms.untracked.uint32(255)
)

dedxSmi = dedxProd.clone()
dedxSmi.Formula = cms.untracked.uint32(2)

dedxASmi = dedxProd.clone()
dedxASmi.Formula = cms.untracked.uint32(3)


####################################################################################
#   MUON TIMING
####################################################################################

from RecoMuon.MuonIdentification.muonTiming_cfi import *
muontiming.MuonCollection = cms.InputTag("muons")

####################################################################################
#   HSCParticle Producer
####################################################################################

#ALL THIS IS NEEDED BY ECAL BETA CALCULATOR (TrackAssociator)
from TrackingTools.TrackAssociator.DetIdAssociatorESProducer_cff import *
from TrackingTools.TrackAssociator.default_cfi import * 
from TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAlong_cfi import *
from TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorOpposite_cfi import *
from TrackPropagation.SteppingHelixPropagator.SteppingHelixPropagatorAny_cfi import *
from Geometry.CMSCommonData.cmsIdealGeometryXML_cfi import *
from Geometry.CaloEventSetup.CaloGeometry_cff import *
from Geometry.CaloEventSetup.CaloTopology_cfi import *
from Geometry.CaloEventSetup.EcalTrigTowerConstituents_cfi import *
from Geometry.TrackerGeometryBuilder.trackerGeometry_cfi import *
from Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi import *
from Geometry.MuonNumbering.muonNumberingInitialization_cfi import *
from Geometry.DTGeometry.dtGeometry_cfi import *
from Geometry.RPCGeometry.rpcGeometry_cfi import *
from Geometry.CSCGeometry.cscGeometry_cfi import *
from Geometry.CommonDetUnit.bareGlobalTrackingGeometry_cfi import *



from SUSYBSMAnalysis.HSCP.HSCPSelections_cff import *

HSCParticleProducer = cms.EDProducer("HSCParticleProducer",
   TrackAssociatorParameterBlock, #Needed for ECAL/Track Matching

   #WHAT (BETA) INFORMATION TO COMPUTE
   useBetaFromTk      = cms.bool(True),
   useBetaFromMuon    = cms.bool(True),
   useBetaFromRpc     = cms.bool(True),
   useBetaFromEcal    = cms.bool(False),

   #TAG OF THE REQUIRED INPUT COLLECTION (ONLY ACTIVATED CALCULATOR)
   tracks             = cms.InputTag("TrackRefitter"),
   muons              = cms.InputTag("muons"),
   dedxEstimator      = cms.InputTag("dedxNPHarm2"),
   dedxDiscriminator  = cms.InputTag("dedxNPHarm2"),
   muontimingDt       = cms.InputTag("muontiming:dt"),
   muontimingCsc      = cms.InputTag("muontiming:csc"),
   muontimingCombined = cms.InputTag("muontiming:combined"),

   #TRACK SELECTION FOR THE HSCP SEED
   minMuP             = cms.double(2),
   minTkP             = cms.double(2),
   maxTkChi2          = cms.double(5),
   minTkHits          = cms.uint32(9),

   #MUON/TRACK MATCHING THRESHOLDS (ONLY IF NO MUON INNER TRACK)
   minDR              = cms.double(0.1),
   maxInvPtDiff       = cms.double(0.005),

   #SELECTION ON THE PRODUCED HSCP CANDIDATES (WILL STORE ONLY INTERESTING CANDIDATES)
   SelectionParameters = cms.VPSet(
      HSCPSelectionDefault,
   ),
)


####################################################################################
#   HSCParticle Selector  (Just an Example of what we can do)
####################################################################################

HSCParticleSelector = cms.EDProducer("HSCParticleSelector",
   source = cms.InputTag("HSCParticleProducer"),

   SelectionParameters = cms.VPSet(
      HSCPSelectionHighdEdx, #THE OR OF THE TWO SELECTION WILL BE APPLIED
      HSCPSelectionHighTOF,
   ),
)

####################################################################################
#   HSCP Candidate Sequence
####################################################################################

HSCParticleProducerSeq = cms.Sequence(offlineBeamSpot + TrackRefitter + dedxNPHarm2 + muontiming + HSCParticleProducer + HSCParticleSelector)


