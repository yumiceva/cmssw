# The following comments couldn't be translated into the new config version:

#Electrons

#Photons

#Electrons

#Photons

#Electrons

#Photons

import FWCore.ParameterSet.Config as cms

# Full Event content 
RecoEgammaFEVT = cms.PSet(
    outputCommands = cms.untracked.vstring('keep *_pixelMatchGsfElectrons_*_*', 
        'keep *_pixelMatchGsfFit_*_*', 
        'keep *_electronPixelSeeds_*_*',
        'keep *_eidRobustLoose_*_*',
        'keep *_eidRobustTight_*_*',
        'keep *_eidRobustHighEnergy_*_*',
        'keep *_eidLoose_*_*',
        'keep *_eidTight_*_*',
        'keep *_conversions_*_*', 
        'keep *_photons_*_*', 
        'keep *_ckfOutInTracksFromConversions_*_*', 
        'keep *_ckfInOutTracksFromConversions_*_*',
        'keep *_PhotonIDProd_*_*'
 )                                                                 
            
)
# RECO content
RecoEgammaRECO = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoGsfElectrons_pixelMatchGsfElectrons_*_*', 
        'keep recoGsfTracks_pixelMatchGsfFit_*_*', 
        'keep recoGsfTrackExtras_pixelMatchGsfFit_*_*', 
        'keep recoTrackExtras_pixelMatchGsfFit_*_*', 
        'keep TrackingRecHitsOwned_pixelMatchGsfFit_*_*', 
        'keep recoElectronPixelSeeds_electronPixelSeeds_*_*',
        'keep floatedmValueMap_eidRobustLoose_*_*',
        'keep floatedmValueMap_eidRobustTight_*_*',
        'keep floatedmValueMap_eidRobustHighEnergy_*_*',
        'keep floatedmValueMap_eidLoose_*_*',
        'keep floatedmValueMap_eidTight_*_*',
        'keep recoPhotons_photons_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoTracks_ckfOutInTracksFromConversions_*_*', 
        'keep recoTracks_ckfInOutTracksFromConversions_*_*', 
        'keep recoTrackExtras_ckfOutInTracksFromConversions_*_*', 
        'keep recoTrackExtras_ckfInOutTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_ckfOutInTracksFromConversions_*_*', 
        'keep TrackingRecHitsOwned_ckfInOutTracksFromConversions_*_*',
        'keep *_PhotonIDProd_*_*'
                                           
 )                                                                 

)
# AOD content
RecoEgammaAOD = cms.PSet(
    outputCommands = cms.untracked.vstring('keep recoGsfElectrons_pixelMatchGsfElectrons_*_*', 
        'keep recoGsfTracks_pixelMatchGsfFit_*_*',
        'keep floatedmValueMap_eidRobustLoose_*_*',
        'keep floatedmValueMap_eidRobustTight_*_*',
        'keep floatedmValueMap_eidRobustHighEnergy_*_*',
        'keep floatedmValueMap_eidLoose_*_*',
        'keep floatedmValueMap_eidTight_*_*',
        'keep recoPhotons_photons_*_*', 
        'keep recoConversions_conversions_*_*', 
        'keep recoTracks_ckfOutInTracksFromConversions_*_*', 
        'keep recoTracks_ckfInOutTracksFromConversions_*_*',
        'keep *_photonCutBasedIDLooseLabel_*_*',
        'keep *_photonCutBasedIDTightLabel_*_*'
        'keep *_PhotonIDProd_*_*'
  )                                                                 
                                  
)

