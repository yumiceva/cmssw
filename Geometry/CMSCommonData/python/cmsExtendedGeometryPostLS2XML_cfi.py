import FWCore.ParameterSet.Config as cms

XMLIdealGeometryESSource = cms.ESSource("XMLIdealGeometryESSource",
    geomXMLFiles = cms.vstring('SLHCUpgradeSimulations/Geometry/data/PhaseI/materials.xml',
        'Geometry/CMSCommonData/data/rotations.xml',
        'Geometry/CMSCommonData/data/extend/cmsextent.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/cms.xml',
        'Geometry/CMSCommonData/data/cmsMother.xml',
        'Geometry/CMSCommonData/data/cmsTracker.xml',
        'Geometry/CMSCommonData/data/caloBase.xml',
        'Geometry/CMSCommonData/data/cmsCalo.xml',
        'Geometry/CMSCommonData/data/muonBase.xml',
        'Geometry/CMSCommonData/data/cmsMuon.xml',
        'Geometry/CMSCommonData/data/mgnt.xml',
        'Geometry/CMSCommonData/data/PhaseI/beampipe.xml',
        'Geometry/CMSCommonData/data/cmsBeam.xml',
        'Geometry/CMSCommonData/data/muonMB.xml',
        'Geometry/CMSCommonData/data/muonMagnet.xml',
        'Geometry/CMSCommonData/data/cavern.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdMaterials.xml',
        'Geometry/TrackerCommonData/data/pixfwdCommon.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdCylinder.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwd.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdDisks.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdInnerDisk1.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdInnerDisk2.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdInnerDisk3.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdOuterDisk1.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdOuterDisk2.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdOuterDisk3.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdblade1.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdblade2.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixfwdblade3.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarmaterial.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladder.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladderfull0.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladderfull1.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladderfull2.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarladderfull3.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarlayer.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarlayer0.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarlayer1.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarlayer2.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbarlayer3.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/pixbar.xml', 
        'Geometry/TrackerCommonData/data/tibtidcommonmaterial.xml',
        'Geometry/TrackerCommonData/data/tibmaterial.xml',
        'Geometry/TrackerCommonData/data/tibmodpar.xml',
        'Geometry/TrackerCommonData/data/tibmodule0.xml',
        'Geometry/TrackerCommonData/data/tibmodule0a.xml',
        'Geometry/TrackerCommonData/data/tibmodule0b.xml',
        'Geometry/TrackerCommonData/data/tibmodule2.xml',
        'Geometry/TrackerCommonData/data/tibstringpar.xml',
        'Geometry/TrackerCommonData/data/tibstring0ll.xml',
        'Geometry/TrackerCommonData/data/tibstring0lr.xml',
        'Geometry/TrackerCommonData/data/tibstring0ul.xml',
        'Geometry/TrackerCommonData/data/tibstring0ur.xml',
        'Geometry/TrackerCommonData/data/tibstring0.xml',
        'Geometry/TrackerCommonData/data/tibstring1ll.xml',
        'Geometry/TrackerCommonData/data/tibstring1lr.xml',
        'Geometry/TrackerCommonData/data/tibstring1ul.xml',
        'Geometry/TrackerCommonData/data/tibstring1ur.xml',
        'Geometry/TrackerCommonData/data/tibstring1.xml',
        'Geometry/TrackerCommonData/data/tibstring2ll.xml',
        'Geometry/TrackerCommonData/data/tibstring2lr.xml',
        'Geometry/TrackerCommonData/data/tibstring2ul.xml',
        'Geometry/TrackerCommonData/data/tibstring2ur.xml',
        'Geometry/TrackerCommonData/data/tibstring2.xml',
        'Geometry/TrackerCommonData/data/tibstring3ll.xml',
        'Geometry/TrackerCommonData/data/tibstring3lr.xml',
        'Geometry/TrackerCommonData/data/tibstring3ul.xml',
        'Geometry/TrackerCommonData/data/tibstring3ur.xml',
        'Geometry/TrackerCommonData/data/tibstring3.xml',
        'Geometry/TrackerCommonData/data/tiblayerpar.xml',
        'Geometry/TrackerCommonData/data/tiblayer0.xml',
        'Geometry/TrackerCommonData/data/tiblayer1.xml',
        'Geometry/TrackerCommonData/data/tiblayer2.xml',
        'Geometry/TrackerCommonData/data/tiblayer3.xml',
        'Geometry/TrackerCommonData/data/tib.xml',
        'Geometry/TrackerCommonData/data/tidmaterial.xml',
        'Geometry/TrackerCommonData/data/tidmodpar.xml',
        'Geometry/TrackerCommonData/data/tidmodule0.xml',
        'Geometry/TrackerCommonData/data/tidmodule0r.xml',
        'Geometry/TrackerCommonData/data/tidmodule0l.xml',
        'Geometry/TrackerCommonData/data/tidmodule1.xml',
        'Geometry/TrackerCommonData/data/tidmodule1r.xml',
        'Geometry/TrackerCommonData/data/tidmodule1l.xml',
        'Geometry/TrackerCommonData/data/tidmodule2.xml',
        'Geometry/TrackerCommonData/data/tidringpar.xml',
        'Geometry/TrackerCommonData/data/tidring0.xml',
        'Geometry/TrackerCommonData/data/tidring0f.xml',
        'Geometry/TrackerCommonData/data/tidring0b.xml',
        'Geometry/TrackerCommonData/data/tidring1.xml',
        'Geometry/TrackerCommonData/data/tidring1f.xml',
        'Geometry/TrackerCommonData/data/tidring1b.xml',
        'Geometry/TrackerCommonData/data/tidring2.xml',
        'Geometry/TrackerCommonData/data/tid.xml',
        'Geometry/TrackerCommonData/data/tidf.xml',
        'Geometry/TrackerCommonData/data/tidb.xml',
        'Geometry/TrackerCommonData/data/tibtidservices.xml',
        'Geometry/TrackerCommonData/data/tibtidservicesf.xml',
        'Geometry/TrackerCommonData/data/tibtidservicesb.xml',
        'Geometry/TrackerCommonData/data/tobmaterial.xml',
        'Geometry/TrackerCommonData/data/tobmodpar.xml',
        'Geometry/TrackerCommonData/data/tobmodule0.xml',
        'Geometry/TrackerCommonData/data/tobmodule2.xml',
        'Geometry/TrackerCommonData/data/tobmodule4.xml',
        'Geometry/TrackerCommonData/data/tobrodpar.xml',
        'Geometry/TrackerCommonData/data/tobrod0c.xml',
        'Geometry/TrackerCommonData/data/tobrod0l.xml',
        'Geometry/TrackerCommonData/data/tobrod0h.xml',
        'Geometry/TrackerCommonData/data/tobrod0.xml',
        'Geometry/TrackerCommonData/data/tobrod1l.xml',
        'Geometry/TrackerCommonData/data/tobrod1h.xml',
        'Geometry/TrackerCommonData/data/tobrod1.xml',
        'Geometry/TrackerCommonData/data/tobrod2c.xml',
        'Geometry/TrackerCommonData/data/tobrod2l.xml',
        'Geometry/TrackerCommonData/data/tobrod2h.xml',
        'Geometry/TrackerCommonData/data/tobrod2.xml',
        'Geometry/TrackerCommonData/data/tobrod3l.xml',
        'Geometry/TrackerCommonData/data/tobrod3h.xml',
        'Geometry/TrackerCommonData/data/tobrod3.xml',
        'Geometry/TrackerCommonData/data/tobrod4c.xml',
        'Geometry/TrackerCommonData/data/tobrod4l.xml',
        'Geometry/TrackerCommonData/data/tobrod4h.xml',
        'Geometry/TrackerCommonData/data/tobrod4.xml',
        'Geometry/TrackerCommonData/data/tobrod5l.xml',
        'Geometry/TrackerCommonData/data/tobrod5h.xml',
        'Geometry/TrackerCommonData/data/tobrod5.xml',
        'Geometry/TrackerCommonData/data/tob.xml',
        'Geometry/TrackerCommonData/data/tecmaterial.xml',
        'Geometry/TrackerCommonData/data/tecmodpar.xml',
        'Geometry/TrackerCommonData/data/tecmodule0.xml',
        'Geometry/TrackerCommonData/data/tecmodule0r.xml',
        'Geometry/TrackerCommonData/data/tecmodule0s.xml',
        'Geometry/TrackerCommonData/data/tecmodule1.xml',
        'Geometry/TrackerCommonData/data/tecmodule1r.xml',
        'Geometry/TrackerCommonData/data/tecmodule1s.xml',
        'Geometry/TrackerCommonData/data/tecmodule2.xml',
        'Geometry/TrackerCommonData/data/tecmodule3.xml',
        'Geometry/TrackerCommonData/data/tecmodule4.xml',
        'Geometry/TrackerCommonData/data/tecmodule4r.xml',
        'Geometry/TrackerCommonData/data/tecmodule4s.xml',
        'Geometry/TrackerCommonData/data/tecmodule5.xml',
        'Geometry/TrackerCommonData/data/tecmodule6.xml',
        'Geometry/TrackerCommonData/data/tecpetpar.xml',
        'Geometry/TrackerCommonData/data/tecring0.xml',
        'Geometry/TrackerCommonData/data/tecring1.xml',
        'Geometry/TrackerCommonData/data/tecring2.xml',
        'Geometry/TrackerCommonData/data/tecring3.xml',
        'Geometry/TrackerCommonData/data/tecring4.xml',
        'Geometry/TrackerCommonData/data/tecring5.xml',
        'Geometry/TrackerCommonData/data/tecring6.xml',
        'Geometry/TrackerCommonData/data/tecring0f.xml',
        'Geometry/TrackerCommonData/data/tecring1f.xml',
        'Geometry/TrackerCommonData/data/tecring2f.xml',
        'Geometry/TrackerCommonData/data/tecring3f.xml',
        'Geometry/TrackerCommonData/data/tecring4f.xml',
        'Geometry/TrackerCommonData/data/tecring5f.xml',
        'Geometry/TrackerCommonData/data/tecring6f.xml',
        'Geometry/TrackerCommonData/data/tecring0b.xml',
        'Geometry/TrackerCommonData/data/tecring1b.xml',
        'Geometry/TrackerCommonData/data/tecring2b.xml',
        'Geometry/TrackerCommonData/data/tecring3b.xml',
        'Geometry/TrackerCommonData/data/tecring4b.xml',
        'Geometry/TrackerCommonData/data/tecring5b.xml',
        'Geometry/TrackerCommonData/data/tecring6b.xml',
        'Geometry/TrackerCommonData/data/tecpetalf.xml',
        'Geometry/TrackerCommonData/data/tecpetalb.xml',
        'Geometry/TrackerCommonData/data/tecpetal0.xml',
        'Geometry/TrackerCommonData/data/tecpetal0f.xml',
        'Geometry/TrackerCommonData/data/tecpetal0b.xml',
        'Geometry/TrackerCommonData/data/tecpetal3.xml',
        'Geometry/TrackerCommonData/data/tecpetal3f.xml',
        'Geometry/TrackerCommonData/data/tecpetal3b.xml',
        'Geometry/TrackerCommonData/data/tecpetal6f.xml',
        'Geometry/TrackerCommonData/data/tecpetal6b.xml',
        'Geometry/TrackerCommonData/data/tecpetal8f.xml',
        'Geometry/TrackerCommonData/data/tecpetal8b.xml',
        'Geometry/TrackerCommonData/data/tecwheel.xml',
        'Geometry/TrackerCommonData/data/tecwheela.xml',
        'Geometry/TrackerCommonData/data/tecwheelb.xml',
        'Geometry/TrackerCommonData/data/tecwheelc.xml',
        'Geometry/TrackerCommonData/data/tecwheeld.xml',
        'Geometry/TrackerCommonData/data/tecwheel6.xml',
        'Geometry/TrackerCommonData/data/tecservices.xml',
        'Geometry/TrackerCommonData/data/tecbackplate.xml',
        'Geometry/TrackerCommonData/data/tec.xml',
        'Geometry/TrackerCommonData/data/trackermaterial.xml',
        'Geometry/TrackerCommonData/data/tracker.xml',
        'Geometry/TrackerCommonData/data/trackerpixbar.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/trackerpixfwd.xml', 
        'Geometry/TrackerCommonData/data/trackertibtidservices.xml',
        'Geometry/TrackerCommonData/data/trackertib.xml',
        'Geometry/TrackerCommonData/data/trackertid.xml',
        'Geometry/TrackerCommonData/data/trackertob.xml',
        'Geometry/TrackerCommonData/data/trackertec.xml',
        'Geometry/TrackerCommonData/data/trackerbulkhead.xml',
        'Geometry/TrackerCommonData/data/trackerother.xml',
        'Geometry/EcalCommonData/data/eregalgo.xml',
        'Geometry/EcalCommonData/data/ebalgo.xml',
        'Geometry/EcalCommonData/data/ebcon.xml',
        'Geometry/EcalCommonData/data/ebrot.xml',
        'Geometry/EcalCommonData/data/eecon.xml',
        'Geometry/EcalCommonData/data/eefixed.xml',
        'Geometry/EcalCommonData/data/eehier.xml',
        'Geometry/EcalCommonData/data/eealgo.xml',
        'Geometry/EcalCommonData/data/escon.xml',
        'Geometry/EcalCommonData/data/esalgo.xml',
        'Geometry/EcalCommonData/data/eeF.xml',
        'Geometry/EcalCommonData/data/eeB.xml',
        'Geometry/HcalCommonData/data/hcalrotations.xml',
        'Geometry/HcalCommonData/data/hcalalgo.xml',
        'Geometry/HcalCommonData/data/hcalbarrelalgo.xml',
        'Geometry/HcalCommonData/data/hcalendcapalgo.xml',
        'Geometry/HcalCommonData/data/hcalouteralgo.xml',
        'Geometry/HcalCommonData/data/hcalforwardalgo.xml',
        'Geometry/HcalCommonData/data/average/hcalforwardmaterial.xml',
        'Geometry/MuonCommonData/data/v1/mbCommon.xml',
        'Geometry/MuonCommonData/data/v1/mb1.xml',
        'Geometry/MuonCommonData/data/v1/mb2.xml',
        'Geometry/MuonCommonData/data/v1/mb3.xml',
        'Geometry/MuonCommonData/data/v1/mb4.xml',
        'Geometry/MuonCommonData/data/design/muonYoke.xml',
        'Geometry/MuonCommonData/data/v2/mf.xml',
        'Geometry/MuonCommonData/data/v2/rpcf.xml',
        'Geometry/MuonCommonData/data/v2/csc.xml',
        'Geometry/MuonCommonData/data/v2/mfshield.xml',
        'Geometry/ForwardCommonData/data/forward.xml',
        'Geometry/ForwardCommonData/data/v2/forwardshield.xml',
        'Geometry/ForwardCommonData/data/brmrotations.xml',
        'Geometry/ForwardCommonData/data/brm.xml',
        'Geometry/ForwardCommonData/data/totemMaterials.xml',
        'Geometry/ForwardCommonData/data/totemRotations.xml',
        'Geometry/ForwardCommonData/data/totemt1.xml',
        'Geometry/ForwardCommonData/data/totemt2.xml',
        'Geometry/ForwardCommonData/data/ionpump.xml',
        'Geometry/ForwardCommonData/data/castor.xml',
        'Geometry/ForwardCommonData/data/zdcmaterials.xml',
        'Geometry/ForwardCommonData/data/lumimaterials.xml',
        'Geometry/ForwardCommonData/data/zdcrotations.xml',
        'Geometry/ForwardCommonData/data/lumirotations.xml',
        'Geometry/ForwardCommonData/data/zdc.xml',
        'Geometry/ForwardCommonData/data/zdclumi.xml',
        'Geometry/ForwardCommonData/data/cmszdc.xml')+cms.vstring(
        'Geometry/MuonCommonData/data/v2/muonNumbering.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/trackerStructureTopology.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/trackersens.xml', 
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/trackerRecoMaterial.xml', 
        'Geometry/EcalSimData/data/ecalsens.xml',
        'Geometry/HcalCommonData/data/hcalsenspmf.xml',
        'Geometry/HcalSimData/data/hf.xml',
        'Geometry/HcalSimData/data/hfpmt.xml',
        'Geometry/HcalSimData/data/hffibrebundle.xml',
        'Geometry/HcalSimData/data/CaloUtil.xml',
        'Geometry/MuonSimData/data/muonSens.xml',
        'Geometry/DTGeometryBuilder/data/dtSpecsFilter.xml',
        'Geometry/CSCGeometryBuilder/data/cscSpecsFilter.xml',
        'Geometry/CSCGeometryBuilder/data/cscSpecs.xml',
        'Geometry/RPCGeometryBuilder/data/RPCSpecs.xml',
        'Geometry/ForwardCommonData/data/brmsens.xml',
        'Geometry/ForwardSimData/data/castorsens.xml',
        'Geometry/ForwardSimData/data/zdcsens.xml',
        'Geometry/HcalSimData/data/HcalProdCuts.xml',
        'Geometry/EcalSimData/data/EcalProdCuts.xml',
        'Geometry/EcalSimData/data/ESProdCuts.xml',
        'SLHCUpgradeSimulations/Geometry/data/PhaseI/trackerProdCuts.xml', 
        'Geometry/TrackerSimData/data/trackerProdCutsBEAM.xml',
        'Geometry/MuonSimData/data/muonProdCuts.xml',
        'Geometry/ForwardSimData/data/CastorProdCuts.xml',
        'Geometry/ForwardSimData/data/zdcProdCuts.xml',
        'Geometry/ForwardSimData/data/ForwardShieldProdCuts.xml',
        'Geometry/CMSCommonData/data/FieldParameters.xml'),
    rootNodeName = cms.string('cms:OCMS')
)


