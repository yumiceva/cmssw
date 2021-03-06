import FWCore.ParameterSet.Config as cms

def RecoInput() : 
 return cms.Source("PoolSource",
                   debugVerbosity = cms.untracked.uint32(200),
                   debugFlag = cms.untracked.bool(True),
                   
                   fileNames = cms.untracked.vstring(
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/06EDBDB7-089B-DD11-BBAC-003048678CA2.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/081712B5-089B-DD11-AFB2-001A92971AEC.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/0A5D191C-F89A-DD11-8AC8-001A928116EE.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/0EADEA9F-039B-DD11-B5B7-001731AF68CF.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/147C3C7B-069B-DD11-A2AE-001A928116B0.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/1CB62299-019B-DD11-97C7-001A92810AAA.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/229B4D83-069B-DD11-B531-001731AF6BC1.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/26D0253C-009B-DD11-B405-003048D15DF6.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/2A6B2E98-019B-DD11-AA4A-001A928116C0.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/52DA8185-069B-DD11-872F-00304876A147.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/54FA17A0-019B-DD11-86D2-003048767D4F.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/6EAEA8B7-089B-DD11-B6D8-003048678FDE.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/70553DBB-089B-DD11-B6B3-0018F3D09652.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/7E8D23C1-089B-DD11-9046-0017312B55A3.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/801413AD-089B-DD11-BE95-003048678FDE.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/8CED1F8B-069B-DD11-A770-00304876A153.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/8EC86680-069B-DD11-96EA-0018F3D09642.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/AC35CAB7-089B-DD11-9727-001A92810ADC.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/BA7CC587-069B-DD11-A420-003048767EE5.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/BCC7B982-069B-DD11-870F-001731A28BE1.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/C2BAD6B5-089B-DD11-95B5-001A92971B0C.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/D857EC97-039B-DD11-ABD1-001A92971BC8.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/DADCFB80-069B-DD11-861B-001A92811714.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/E888CEBF-089B-DD11-A6CF-001731AF68AD.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/EA0DA641-009B-DD11-A649-001A92810AC6.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/EE9D0E99-019B-DD11-B40F-001A928116E8.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0001/FED1FEB2-089B-DD11-B1FE-001A92810AA2.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/24A12C65-0B9B-DD11-9AF8-0017312B56A7.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/3ACC423B-299B-DD11-8B55-001731AF67BF.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/563D6DD7-579B-DD11-9961-0018F3D096DC.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/866074B8-089B-DD11-A955-001A92811736.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/9C60FD39-429B-DD11-ADEB-003048767EFB.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/A410F839-4E9B-DD11-947F-0018F3D0962A.root',
     '/store/relval/CMSSW_2_1_10/RelValH200ZZ4L/GEN-SIM-DIGI-RAW-HLTDEBUG-RECO/STARTUP_V7_v1/0002/AE2BAF55-0B9B-DD11-BFB1-001A92971AD0.root'
     )
                   )
