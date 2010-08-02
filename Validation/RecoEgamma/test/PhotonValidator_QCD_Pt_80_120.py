
import FWCore.ParameterSet.Config as cms

process = cms.Process("TestPhotonValidator")
process.load('Configuration/StandardSequences/GeometryPilot2_cff')
process.load("Configuration.StandardSequences.MagneticField_38T_cff")
process.load("Geometry.TrackerGeometryBuilder.trackerGeometry_cfi")
process.load("RecoTracker.GeometryESProducer.TrackerRecoGeometryESProducer_cfi")
process.load("Geometry.TrackerNumberingBuilder.trackerNumberingGeometry_cfi")
process.load("RecoTracker.MeasurementDet.MeasurementTrackerESProducer_cfi")
process.load("SimGeneral.MixingModule.mixNoPU_cfi")
process.load("TrackingTools.TransientTrack.TransientTrackBuilder_cfi")
process.load("DQMServices.Components.MEtoEDMConverter_cfi")
process.load("Validation.RecoEgamma.photonValidationSequence_cff")
process.load("Validation.RecoEgamma.photonPostprocessing_cfi")
process.load("Configuration.StandardSequences.FrontierConditions_GlobalTag_cff")
process.GlobalTag.globaltag = 'START38_V7::All'


process.DQMStore = cms.Service("DQMStore");
process.load("DQMServices.Components.DQMStoreStats_cfi")
from DQMServices.Components.DQMStoreStats_cfi import *
dqmStoreStats.runOnEndJob = cms.untracked.bool(True)



process.maxEvents = cms.untracked.PSet(
#input = cms.untracked.int32(10)
)



from Validation.RecoEgamma.photonValidationSequence_cff import *
from Validation.RecoEgamma.photonPostprocessing_cfi import *
photonValidation.OutputMEsInRootFile = True
photonValidation.OutputFileName = 'PhotonValidationRelVal380_QCD_Pt_80_120.root'

photonPostprocessing.batch = cms.bool(True)
photonPostprocessing.InputFileName = photonValidation.OutputFileName

process.source = cms.Source("PoolSource",
noEventSort = cms.untracked.bool(True),
duplicateCheckMode = cms.untracked.string('noDuplicateCheck'),
                            
    fileNames = cms.untracked.vstring(
# official RelVal 380 QCD_Pt_80_120
    '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/FEB511DA-6F96-DF11-88D2-002618943970.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/F86770D9-6F96-DF11-A32A-00261894389E.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/E00940DA-6F96-DF11-AA2A-00261894391B.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/DC46BDD8-6F96-DF11-B13D-003048678BEA.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/DAC485D5-6F96-DF11-8065-00248C0BE01E.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/C2E785D9-6F96-DF11-A95C-0018F3D095EA.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/B04D9DD2-6F96-DF11-B0F0-002354EF3BDF.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/AAC58ED3-6F96-DF11-A754-0030486792B8.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/A85596D9-6F96-DF11-A4B0-00261894389E.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/8C3B62DA-6F96-DF11-9B6A-00261894387A.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/86CBC0D8-6F96-DF11-A3B0-00248C0BE014.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/821194DC-6F96-DF11-ADDC-003048678F8A.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/7E355BD7-6F96-DF11-BC77-00248C0BE012.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/1AAF5CC3-7196-DF11-AE9F-003048679182.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/18DBEDDC-6F96-DF11-8523-003048678F8A.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/12E0D4D8-6F96-DF11-9F91-0018F3D09636.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/127F6ED7-6F96-DF11-9781-0026189438ED.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0007/006ABFD5-6F96-DF11-8B90-003048678ED2.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0005/DA0670F1-3795-DF11-AC60-001A92811736.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0004/E819E689-0895-DF11-8F3C-001A92971BBE.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0004/B880128C-0295-DF11-9101-00304867C29C.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0004/9C499798-0595-DF11-882F-00261894388D.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0004/8ACBD540-0495-DF11-B26D-003048678FF8.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0004/4ED13383-0395-DF11-935B-003048678C62.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0004/2CA47C7E-0495-DF11-8897-003048D15D22.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-RECO/START38_V7-v1/0004/100F9BF2-0995-DF11-87D8-003048679220.root'

 
     ),
    
    secondaryFileNames = cms.untracked.vstring(
# official RelVal 380 QCD_Pt_80_120
'/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/FCB17BD5-6F96-DF11-87C7-002618943832.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/FA096BD6-6F96-DF11-BB9B-001A92971B7E.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/EE8FDCD1-6F96-DF11-AAD0-00261894389D.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/EE4EFBD5-6F96-DF11-8B79-00261894393B.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/EC6185D1-6F96-DF11-AD0B-002354EF3BDB.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/E8EBF3D7-6F96-DF11-90CC-0026189438FE.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/DAF2BADB-6F96-DF11-9CB1-003048678F8A.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/D88FDED1-6F96-DF11-8DBB-0026189438ED.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/D81D5AD7-6F96-DF11-9998-002618943896.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/D200F9D8-6F96-DF11-9079-00304867BF18.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/D0F208D9-6F96-DF11-823B-0018F3D095FE.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/C40A82DB-6F96-DF11-AB23-003048678FA6.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/C0854FC2-7196-DF11-B666-003048678BAC.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/B02F44D6-6F96-DF11-AE37-003048D3C010.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/A6F757D7-6F96-DF11-9227-00261894380A.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/A4F070D6-6F96-DF11-B28F-00304867BED8.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/92210BD7-6F96-DF11-82AF-0026189438BF.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/9099F1D4-6F96-DF11-A97B-00261894389E.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/8C906CDA-6F96-DF11-958B-003048679162.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/8A7870D7-6F96-DF11-AA60-0026189438F2.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/8845CFD5-6F96-DF11-BC1D-002618943826.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/86C4D8DB-6F96-DF11-BC1D-001A92971B94.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/74B179D8-6F96-DF11-9D47-00248C0BE01E.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/7042FFD4-6F96-DF11-B67A-003048679012.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/6A9701D4-6F96-DF11-A348-002618FDA28E.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/6811FAD1-6F96-DF11-9513-00261894391B.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/5ED21CDA-6F96-DF11-B9F1-003048678AC0.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/56EFC1D4-6F96-DF11-B995-002354EF3BD0.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/541AD9D5-6F96-DF11-88C9-003048678B30.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/526F8CD9-6F96-DF11-AC7F-003048679296.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/50DD90D7-6F96-DF11-AD25-0026189438FC.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/4C5F84D9-6F96-DF11-9003-0018F3D09626.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/4864B0D4-6F96-DF11-B691-002354EF3BE1.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/3AA76FD6-6F96-DF11-B166-002618943964.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/320B12CE-6F96-DF11-9484-003048679182.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/304DD6D8-6F96-DF11-8E58-002354EF3BE1.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/2A3883D8-6F96-DF11-9667-0018F3D0961A.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/20D02FDA-6F96-DF11-9F17-001A92971B88.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/204DD8D7-6F96-DF11-877C-0030486790A6.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/0EE36ED6-6F96-DF11-9635-00304867BFAA.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/086CDDD5-6F96-DF11-9D5D-003048679076.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0007/06BF22D6-6F96-DF11-89C0-00261894398D.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0005/EA4926ED-3795-DF11-B5F9-001A9281171C.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/F4DD7E7A-0995-DF11-B61C-001A928116C8.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/DA3960A3-0895-DF11-AD5C-0018F3D096E4.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/D23D7839-0495-DF11-B46A-00261894388D.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/CE636AF2-0995-DF11-A031-002618943940.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/CA5CB608-0395-DF11-9424-0018F3D095FA.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/B2108083-0495-DF11-9F87-00304867BEE4.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/A28B4587-0295-DF11-A281-001A928116E0.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/6CEACBFD-0495-DF11-954F-0026189438B9.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/64B3518E-0595-DF11-8D86-003048679220.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/647E8579-0795-DF11-AA90-002618943973.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/5ECC1829-0495-DF11-A50D-003048D15CC0.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/54432587-0495-DF11-BB1F-003048678AFA.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/40AF0DF7-0895-DF11-8CE4-002618943973.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/40A0BA04-0395-DF11-99A0-001A928116E2.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/3861F101-0295-DF11-A068-0018F3D096E8.root',
        '/store/relval/CMSSW_3_8_0/RelValQCD_Pt_80_120/GEN-SIM-DIGI-RAW-HLTDEBUG/START38_V7-v1/0004/2A1F3092-0595-DF11-AAB5-003048678B72.root'
 
        )
 )


photonPostprocessing.rBin = 48
## For gam Jet and higgs
photonValidation.eMax  = 500
photonValidation.etMax = 500
photonPostprocessing.eMax  = 500
photonPostprocessing.etMax = 500




process.FEVT = cms.OutputModule("PoolOutputModule",
    outputCommands = cms.untracked.vstring("keep *_MEtoEDMConverter_*_*"),
    fileName = cms.untracked.string('pippo.root')
)


process.p1 = cms.Path(process.tpSelection*process.photonValidationSequence*process.photonPostprocessing*process.dqmStoreStats)
process.schedule = cms.Schedule(process.p1)


