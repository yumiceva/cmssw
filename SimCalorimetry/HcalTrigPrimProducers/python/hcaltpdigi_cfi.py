import FWCore.ParameterSet.Config as cms

simHcalTriggerPrimitiveDigis = cms.EDProducer("HcalTrigPrimDigiProducer",
    latency = cms.int32(1),
    weights = cms.vdouble(1.0, 1.0), ##hardware algo		

    #vdouble weights = { -1, -1, 1, 1} //low lumi algo
    peakFilter = cms.bool(True),
    # Input digi label (_must_ be without zero-suppression!)
    firstTPSample = cms.int32(2),
    TPSize = cms.int32(4),
    inputLabel = cms.VInputTag(cms.InputTag('simHcalUnsuppressedDigis'),cms.InputTag('simHcalUnsuppressedDigis')),
    FG_threshold = cms.uint32(12), ## threshold for setting fine grain bit
    ZS_threshold = cms.uint32(1) ## threshold for setting fine grain bit
)
