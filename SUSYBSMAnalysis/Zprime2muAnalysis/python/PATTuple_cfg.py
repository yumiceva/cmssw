#!/usr/bin/env python

import FWCore.ParameterSet.Config as cms

process = cms.Process('PAT')

process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(-1))
process.options = cms.untracked.PSet(wantSummary = cms.untracked.bool(True))

process.source = cms.Source('PoolSource',
                            fileNames = cms.untracked.vstring('file:PlaceHolder.root')
                            )

process.load('FWCore.MessageLogger.MessageLogger_cfi')
process.MessageLogger.cerr.FwkReport.reportEvery = 1000
process.MessageLogger.cerr.threshold = 'INFO'
process.MessageLogger.categories.append('PATSummaryTables')
process.MessageLogger.cerr.PATSummaryTables = cms.untracked.PSet(limit = cms.untracked.int32(-1))

process.load('Configuration.StandardSequences.Geometry_cff')
process.load('Configuration.StandardSequences.MagneticField_cff')
process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_cff')
process.GlobalTag.globaltag = cms.string('PlaceHolder::All')

process.load('PhysicsTools.PatAlgos.patSequences_cff')

# For muon and electron MC matching, want to be able to match to
# decays-in-flight produced by GEANT, so make some GenParticles out of
# the simTracks.
process.simParticles = cms.EDProducer('PATGenCandsFromSimTracksProducer',
                                      src = cms.InputTag('g4SimHits'),
                                      setStatus = cms.int32(1),
                                      filter = cms.string('(abs(pdgId) == 13 || abs(pdgId) == 11) && pt > 2')
                                      )

process.genSimParticles = cms.EDProducer('GenParticleMerger',
                                         src = cms.VInputTag(cms.InputTag('genParticles'), cms.InputTag('simParticles'))
                                         )

# Before we prepend the above two modules to the patDefaultSequence,
# save a copy of the old one to go back to later if the user calls
# configure() to remove MC use.
process.patDefaultSequenceBeforeZp2mu = process.patDefaultSequence
process.patDefaultSequence = cms.Sequence(process.simParticles * process.genSimParticles * process.patDefaultSequence._seq)

for x in (process.muonMatch, process.electronMatch):
    # Use the new gen + sim MC particles created above.
    x.matched = cms.InputTag('genSimParticles')

    # PAT muon/electron-MC matching requires, in addition to deltaR < 0.5,
    # the MC and reconstructed leptons to have the same charge, and (reco
    # pt - gen pt)/gen pt < 0.5. Disable these two cuts.
    x.checkCharge = False
    x.maxDPtRel = 1e6

from PhysicsTools.PatAlgos.patEventContent_cff import patEventContent
process.out = cms.OutputModule('PoolOutputModule',
                               fileName = cms.untracked.string('pat.root'),
                               SelectEvents   = cms.untracked.PSet(SelectEvents = cms.vstring('p')),
                               outputCommands = cms.untracked.vstring('drop *', *patEventContent ) 
                               )
process.outpath = cms.EndPath(process.out)

# Keep some extra stuff not defined in the patEventContent.
process.out.outputCommands += [
    'keep recoGenParticles_genParticles_*_*',
    'keep GenEventInfoProduct_*_*_*',
    'keep GenRunInfoProduct_*_*_*',
    'keep *_offlineBeamSpot_*_*',
    'keep *_offlinePrimaryVertices_*_*',
    'keep edmTriggerResults_TriggerResults__HLT*',
    'keep L1GlobalTriggerObjectMapRecord_hltL1GtObjectMap__HLT*',
    'keep *_hltTriggerSummaryAOD__HLT*',
    ]

# Embed the tracker tracks (by default, every other track is embedded).
process.patMuons.embedTrack = True

# Filter out events with no muons.
process.countPatMuons.minNumber = 1

# Define simple quality cuts (analysis cuts to be done at the analysis level).
process.selectedPatMuons.cut = 'isGlobalMuon'
#process.selectedPatMuons.cut = 'isTrackerMuon && pt > 1 && p > 2.5 && innerTrack.hitPattern.numberOfValidTrackerHits > 12 && innerTrack.normalizedChi2 < 5 && abs(dB) < 0.5 && abs(dZ) < 5 && muonID("TMLastStationAngTight")'
#process.selectedPatMuons.cut = 'muonID("GlobalMuonPromptTight") && muonID("TMOneStationLoose") && (globalTrack.hitPattern.numberOfValidMuonCSCHits + globalTrack.hitPattern.numberOfValidMuonDTHits) >= 1 && innerTrack.hitPattern.trackerLayersWithMeasurement >= 6'
