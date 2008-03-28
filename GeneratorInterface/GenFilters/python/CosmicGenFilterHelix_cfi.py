import FWCore.ParameterSet.Config as cms

cosmicInTracker = cms.EDFilter("CosmicGenFilterHelix",
    maxZ = cms.double(212.0), ## dito

    src = cms.InputTag("source"),
    ignoreMaterial = cms.bool(False), ## Should SteppingHelixPropagator take into account material?

    minPt = cms.double(0.0),
    charges = cms.vint32(1, -1), ## needs to be parallel to pdgIds

    pdgIds = cms.vint32(-13, 13), ## only generated particles of these IDs are considered

    minZ = cms.double(-212.0), ## i.e. at least four TEC discs

    # defines dimensions of target cylinder in cm (full tracker: r=112, z=+/- 270)
    radius = cms.double(80.0),
    doMonitor = cms.untracked.bool(False), ## Fill monitoring histograms? Needs TFileService, cf. below.

    # momentum cuts after propagation:
    minP = cms.double(0.0)
)


