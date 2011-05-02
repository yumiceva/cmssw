import FWCore.ParameterSet.Config as cms
import copy

import TauAnalysis.Configuration.plotterProcessDefinitions_cfi as plotter
import TauAnalysis.DQMTools.plotterStyleDefinitions_cfi as styles

# List of samples to run in the analysis
SAMPLES_TO_ANALYZE = [
    'data_TauPlusX_Run2011A_PR_v1',
    'data_TauPlusX_Run2011A_PR_v2',
    #'data_TauPlusElec_Run2011A_PR_T3',
    'data_TauPlusX_Run2011A_PR_nonGolden',
    #'Ztautau_powheg',
    'Ztautau_powheg_T3',
    #'DYtautauM10to20_powheg',
    'DYtautauM10to20_powheg_T3',
    'qqZll',
    #'Zee_powheg',
    'Zee_powheg_T3',
    #'DYeeM10to20_pythia',
    'DYeeM10to20_pythia_T3',
    #'PhotonPlusJets_Pt15to30','PhotonPlusJets_Pt30to50','PhotonPlusJets_Pt50to80',
	'PhotonPlusJets_Pt15to30_T3','PhotonPlusJets_Pt30to50_T3','PhotonPlusJets_Pt50to80_T3',
    #'QCD_BCtoE_Pt20to30','QCD_BCtoE_Pt30to80','QCD_BCtoE_Pt80to170',
	'QCD_BCtoE_Pt20to30_T3','QCD_BCtoE_Pt30to80_T3','QCD_BCtoE_Pt80to170_T3',
    #'QCD_EM_Pt20to30','QCD_EM_Pt30to80','QCD_EM_Pt80to170',
	'QCD_EM_Pt20to30_T3','QCD_EM_Pt30to80_T3','QCD_EM_Pt80to170_T3',
    #'TTplusJets_madgraph',
    'TTplusJets_madgraph_T3',
    #'WplusJets_madgraph',
    'WplusJets_madgraph_T3',
    #'WW','WZ','ZZ',
    'WW_T3','WZ_T3','ZZ_T3',
] 

# List of samples to include in the final level plots.  May include selections
# from the MERGE_SAMPLES defined at the bottom.
SAMPLES_TO_PLOT = [
    'data', 
    'qcdSum', 
    'photonPlusJetsSum',
    'WplusJets_madgraph_T3',
    #'TTplusJets_madgraph',
    'ZeeSum',
    'ZtautauSum',
    #'VVsum',
    #'Ztautau_powheg',
    #'Zee_powheg',
]

SAMPLES_TO_PRINT = copy.copy(SAMPLES_TO_PLOT)
#SAMPLES_TO_PRINT.append('smBgSum')
#SAMPLES_TO_PRINT.append('smSum')

SAMPLE_DEFAULTS = {
	'dbs_url' : "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
    'conditions' : 'START311_V2::All',
    'genPhaseSpaceCut' : '',
    'factorize' : False,
    'enableSysUncertainties' : True,
    'lumi_mask' : '',
    'runselection' : '',
    'inputFileType' : 'AOD',
    'hlt_paths' : [ 'HLT_IsoEle12_PFTau15_v3','HLT_Ele12_SW_TighterEleId_L1R_v2'],
    'SE_white_list' : '',
    'SE_black_list' : 'ccsrm.in2p3.fr',
	'disableDuplicateCheck' : True,
    'applyZrecoilCorrection' : True,
	'applyElectronTriggerEfficiencyCorrection' : True,
	'applyElectronIsolationEfficiencyCorrection' : True,
	'applyMuonTriggerEfficiencyCorrection' : False,
	'applyVertexMultiplicityReweighting' : False,
	'hlt' : cms.InputTag("TriggerResults", "", "HLT")
}

# Conversions to pico barns
_millibarns = 1.0e+9
_microbarns = 1.0e+6
_nanobarns  = 1.0e+3
_picobarns =  1.0
_femtobarns = 1.0e-3

# Integrated luminosity to normalize
#TARGET_LUMI = (200.0)/_picobarns
TARGET_LUMI = (55.3)/_picobarns 

#--------------------------------------------------------------------------------
# NOTE: cross-sections for W and Z production are scaled to next-to-leading order values
#       documented on the wiki
#        https://alcaraz.web.cern.ch/alcaraz/CROSS_SECTIONS.txt
#--------------------------------------------------------------------------------

RECO_SAMPLES = {
    'data_TauPlusElec_Run2011A_PR_T3' : {   # xxx/pb
        'datasetpath' : '/TauPlusX/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER',
        'dbs_url' : "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'conditions' : 'GR_P_V16::All',
        'number_of_jobs' : 20,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'				: '160431:MIN-161016:MAX'
        }
    },
    'data_TauPlusX_Run2011A_PR_v1' : {  # 14.5/pb  160431-161312
        'datasetpath' : '/TauPlusX/Run2011A-PromptReco-v1/AOD', 
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-163369_7TeV_PromptReco_Collisions11_JSON.txt',
        'conditions' : 'GR_P_V17::All',
        'number_of_jobs' : 50,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'     : '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'     : '161217:MIN-163261:MAX'
            }
        },
    'data_TauPlusX_Run2011A_PR_v2' : {  # 28.9/pb  162718-163369
        'datasetpath' : '/TauPlusX/Run2011A-PromptReco-v2/AOD', 
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_160404-163369_7TeV_PromptReco_Collisions11_JSON.txt',
        'conditions' : 'GR_P_V17::All',
        'number_of_jobs' : 100,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'     : '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'     : '161217:MIN-163261:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v4'     : '163269:MIN-163402:MAX'
            #'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v*' : '160431:MIN-163402:MAX'
            }
        },
    'data_TauPlusX_Run2011A_PR_nonGolden' : { # 11.9/pb 161217-161312
        'datasetpath' : '/TauPlusX/Run2011A-PromptReco-v1/AOD',
        'lumi_mask' : '/afs/cern.ch/cms/CAF/CMSCOMM/COMM_DQM/certification/Collisions11/7TeV/Prompt/Cert_161079-161352_7TeV_PromptReco_Collisions11_JSON_noESpbl_v2.txt',
        'conditions' : 'GR_P_V16::All',
        'number_of_jobs' : 100,
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data,
        'enableSysUncertainties' : False,
        'applyElectronTriggerEfficiencyCorrection' : False,
        'applyElectronIsolationEfficiencyCorrection' : False,
        'applyZrecoilCorrection' : False,
        'hlt_paths' : {
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v1'				: '160431:MIN-161016:MAX',
            'HLT_Ele15_CaloIdVT_CaloIsoT_TrkIdT_TrkIsoT_LooseIsoPFTau15_v2'             : '161217:MIN-161312:MAX'
        }
    },
    'DYtautauM10to20_powheg' : {
        'datasetpath' : "/DYToTauTau_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Spring11-PU_S1_START311_V1G1-v2/AODSIM",
        'events_processed' : 1991426,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'DYtautauM10to20_powheg_T3' : {
        'datasetpath' : "/DYToTauTau_M-10To20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 2,
        'events_processed' : 1991426,
        'skim_eff' : 775./1991426,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'Ztautau_powheg' : {
        'datasetpath' : "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/Spring11-PU_S1_START311_V1G1-v2/AODSIM",
        'events_processed' : 1994719,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'Ztautau_powheg_T3' : {
        'datasetpath' : "/DYToTauTau_M-20_CT10_TuneZ2_7TeV-powheg-pythia-tauola/jkolb-skimElecTau_413_v2-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 1994719,
        'skim_eff' : 89799./1994719,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'qqZll' : {
        'datasetpath' : "/VQQJetsToLL_TuneD6T_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 738233,
        'skim_eff' : 1.0,
        'x_sec' : 36.*_picobarns,
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Ztautau.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Ztautau,
        'applyZrecoilCorrection' : True,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    }, 
    'DYeeM10to20_pythia' : {
        'datasetpath' : "/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1933000,
        'skim_eff' : 1.0,
        'x_sec' : 1.282*2659*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282 (k-factor for mMuMu > 20 GeV)
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'applyZrecoilCorrection' : True,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'DYeeM10to20_pythia_T3' : {
        'datasetpath' : "/DYToEE_M-10To20_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1933000,
        'skim_eff' : 31175./1933000,
        'x_sec' : 1.282*2659*_picobarns, # Z + jets correction factor for NLO/LO cross-sections = 1.282 (k-factor for mMuMu > 20 GeV)
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'applyZrecoilCorrection' : True,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'Zee_powheg' : {
        'datasetpath' : "/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1992276,
        'skim_eff' : 1.0,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'applyZrecoilCorrection' : True,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'Zee_powheg_T3' : {
        'datasetpath' : "/DYToEE_M-20_CT10_TuneZ2_7TeV-powheg-pythia/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 1992276,
        'skim_eff' : 819311.0/1992276,
        'x_sec' : 1666*_picobarns,
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_Zee.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_Zee,
        'applyZrecoilCorrection' : True,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WplusJets_madgraph' : {
        'datasetpath' : "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 15110974,
        'skim_eff' : 1.0,
        'x_sec' : 31314*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WplusJets_madgraph_T3' : {
        'datasetpath' : "/WJetsToLNu_TuneZ2_7TeV-madgraph-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 100,
        'events_processed' : 15110974,
        'skim_eff' : 738862./15110974,
        'x_sec' : 31314*_picobarns,
        'legendEntry' : plotter.process_WplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_WplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_WplusJets,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WW' : {
        'datasetpath' : "/WWtoAnything_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2061760,
        'skim_eff' : 1.0,
        'x_sec' : 43.0*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'WW_T3' : {
        'datasetpath' : "/WWtoAnything_TuneZ2_7TeV-pythia6-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 2061760,
        'skim_eff' : 193655./2061760,
        'x_sec' : 43.0*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
  'WZ' : {
        'datasetpath' : "/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2108416,
        'skim_eff' : 1.0,
        'x_sec' : 18.2*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
  'WZ_T3' : {
        'datasetpath' : "/WZtoAnything_TuneZ2_7TeV-pythia6-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 2108416,
        'skim_eff' : 152293./2108416,
        'x_sec' : 18.2*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'ZZ' : {
        'datasetpath' : "/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 2108608,
        'skim_eff' : 1.0,
        'x_sec' : 5.9*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'ZZ_T3' : {
        'datasetpath' : "/ZZtoAnything_TuneZ2_7TeV-pythia6-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 2108608,
        'skim_eff' : 151802./2108608,
        'x_sec' : 5.9*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_VV.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_VV.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_VV,
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'TTplusJets_madgraph' : {
        'datasetpath' : "/TTJets_TuneZ2_7TeV-madgraph-tauola/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'dbs_url' :  "http://cmsdbsprod.cern.ch/cms_dbs_prod_global/servlet/DBSServlet",
        'events_processed' : 1164208,
        'skim_eff' : 1.0,
        'x_sec' : 157.5*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'TTplusJets_madgraph_T3' : {
        'datasetpath' : "/TTJets_TuneZ2_7TeV-madgraph-tauola/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 1164208,
        'skim_eff' : 172976./1164208,
        'x_sec' : 157.5*_picobarns, # NLO cross-section from https://twiki.cern.ch/twiki/bin/viewauth/CMS/StandardModelCrossSections
        'legendEntry' : plotter.process_TTplusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_TTplusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_TTplusJets,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt15to30' : {
        'datasetpath' : "/G_Pt_15to30_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1025840,
        'skim_eff' : 1.0,
		'factorize' : True,
        'x_sec' : 171700*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt15to30_T3' : {
        'datasetpath' : "/G_Pt_15to30_TuneZ2_7TeV_pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1025840,
        'skim_eff' : 22531./1025840,
        'factorize' : True,
        'x_sec' : 171700*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt30to50' : {
        'datasetpath' : "/G_Pt_30to50_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1025480,
        'skim_eff' : 1.0,
		'factorize' : True,
        'x_sec' : 16690*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt30to50_T3' : {
        'datasetpath' : "/G_Pt_30to50_TuneZ2_7TeV_pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1025480,
        'skim_eff' : 34061./1025480,
        'factorize' : True,
        'x_sec' : 16690*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt50to80' : {
        'datasetpath' : "/G_Pt_50to80_TuneZ2_7TeV_pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1024608,
        'skim_eff' : 1.0,
		'factorize' : True,
        'x_sec' : 2722*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'PhotonPlusJets_Pt50to80_T3' : {
        'datasetpath' : "/G_Pt_50to80_TuneZ2_7TeV_pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1025840,
        'skim_eff' : 36691./1025840,
        'factorize' : True,
        'x_sec' : 2722*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_gammaPlusJets.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_gammaPlusJets,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt20to30' : {
        'datasetpath' : "/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 2243439,
        'skim_eff' : 1,
		'factorize' : True,
        'x_sec' : 236000000*0.00056*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'legendEntry' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt20to30_T3' : {
        'datasetpath' : "/QCD_Pt-20to30_BCtoE_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 2243439,
        'skim_eff' : 237477./2243439,
        'factorize' : True,
        'x_sec' : 236000000*0.00056*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'legendEntry' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt30to80' : {
        'datasetpath' : "/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1995502,
        'skim_eff' : 1,
		'factorize' : True,
        'x_sec' : 59480000*0.0023*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'legendEntry' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt30to80_T3' : {
        'datasetpath' : "/QCD_Pt-30to80_BCtoE_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 50,
        'events_processed' : 1995502,
        'skim_eff' : 228156./1995502,
        'factorize' : True,
        'x_sec' : 59480000*0.0023*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'legendEntry' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt80to170' : {
        'datasetpath' : "/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 1043390,
        'skim_eff' : 1.0,
		'factorize' : True,
        'x_sec' : 900000*0.0104*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'legendEntry' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_BCtoE_Pt80to170_T3' : {
        'datasetpath' : "/QCD_Pt-80to170_BCtoE_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 10,
        'events_processed' : 1043390,
        'skim_eff' : 58871./1043390,
        'factorize' : True,
        'x_sec' : 900000*0.0104*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'legendEntry' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_BCtoE_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt20to30' : {
        'datasetpath' : "/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 36136246,
        'skim_eff' : 1.0,
		'factorize' : True,
        'x_sec' : 236000000*0.0104*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'legendEntry' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt20to30_T3' : {
        'datasetpath' : "/QCD_Pt-20to30_EMEnriched_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 30,
        'events_processed' : 36136246,
        'skim_eff' : 434178./36136246,
        'factorize' : True,
        'x_sec' : 236000000*0.0104*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'legendEntry' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt20to30.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt30to80' : {
        'datasetpath' : "/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 70708892,
        'skim_eff' : 1.0,
		'factorize' : True,
        'x_sec' : 59480000*0.065*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'legendEntry' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt30to80_T3' : {
        'datasetpath' : "/QCD_Pt-30to80_EMEnriched_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 80,
        'events_processed' : 70708892,
        'skim_eff' : 736277./70708892,
        'factorize' : True,
        'x_sec' : 59480000*0.065*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'legendEntry' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt30to80.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt80to170' : {
        'datasetpath' : "/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/Spring11-PU_S1_START311_V1G1-v1/AODSIM",
        'events_processed' : 8069591,
        'skim_eff' : 1.0,
        'factorize' : True,
        'x_sec' : 900000*0.155*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : True,
        'legendEntry' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    },
    'QCD_EM_Pt80to170_T3' : {
        'datasetpath' : "/QCD_Pt-80to170_EMEnriched_TuneZ2_7TeV-pythia6/jkolb-skimElecTau_413_v1-eae887ba91c6c27e2f0c00f8aee7bf0a/USER",
        'dbs_url' :  "https://cmsdbsprod.cern.ch:8443/cms_dbs_ph_analysis_01_writer/servlet/DBSServlet",
        'number_of_jobs' : 20,
        'events_processed' : 8069591,
        'skim_eff' : 74205./8069591,
        'factorize' : True,
        'x_sec' : 900000*0.155*_picobarns, 
        'applyZrecoilCorrection' : False,
        'applyElectronTriggerEfficiencyCorrection' : True,
        'applyElectronIsolationEfficiencyCorrection' : True,
        'applyVertexMultiplicityReweighting' : False,
        'legendEntry' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.legendEntry.value(),
        'type' : plotter.process_QCD_EMenriched_Pt80to170.config_dqmHistPlotter.type.value(),
        'drawOption' : styles.drawOption_QCD,
        'hlt' : cms.InputTag("TriggerResults", "", "REDIGI311X")
    }
}

# Define samples that get merged together
MERGE_SAMPLES = {
    'data' : {
        'samples' : [
            'data_TauPlusX_Run2011A_PR_v1',
            'data_TauPlusX_Run2011A_PR_v2',
            'data_TauPlusX_Run2011A_PR_nonGolden'
        ],
        'legendEntry' : 'DATA',
        'type' : 'Data',
        'drawOption' : styles.drawOption_Data
    },
    'qcdSum' : {
        'samples' : [
			'QCD_BCtoE_Pt20to30_T3',
			'QCD_BCtoE_Pt30to80_T3',
			'QCD_BCtoE_Pt80to170_T3',
			'QCD_EM_Pt20to30_T3',
			'QCD_EM_Pt30to80_T3',
			'QCD_EM_Pt80to170_T3'
        ],
        'legendEntry' : 'QCD',
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_QCD,
    },
    'photonPlusJetsSum' : {
        'samples' : [
			'PhotonPlusJets_Pt15to30_T3',
			'PhotonPlusJets_Pt30to50_T3',
			'PhotonPlusJets_Pt50to80_T3'
        ],
        'legendEntry' : plotter.process_gammaPlusJets.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_gammaPlusJets,
    },
    'ZeeSum' : {
        'samples' : [
            'DYeeM10to20_pythia_T3',
            'Zee_powheg_T3'
        ],
        'legendEntry' : plotter.process_Zee.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_Zee,
    },
    'ZtautauSum' : {
        'samples' : [
            'DYtautauM10to20_powheg_T3',
            'Ztautau_powheg_T3',
            #'qqZll'
        ],
        'legendEntry' : plotter.process_Ztautau.config_dqmHistPlotter.legendEntry.value(),
        'type' : 'smMC', 
        'drawOption' : styles.drawOption_Ztautau,
    },
    'VVsum' : {
        'samples' : [
            'WW_T3',
            'WZ_T3',
            'ZZ_T3'
        ],
        'legendEntry' : 'WW/WZ/ZZ',
        'type' : 'smMC',
        'drawOption' : styles.drawOption_VV
    }
    #'smBgSum' : {
    #    'samples' : [
    #        'ZeeSum',
    #        'qcdSum',
    #        'VVsum',
    #		'photonPlusJetsSum',
    #        'WplusJets_madgraph',
    #        'TTplusJets_madgraph',
    #    ],
    #    'legendEntry' : 'SM',
    #    'type' : 'smMC',
    #    'drawOption' : styles.drawOption_QCD
    #},
    #'smSum' : {
    #    'samples' : [
    #        'ZtautauSum',
    #        'smBgSum'
    #    ],
    #    'legendEntry' : 'SM',
    #    'type' : 'smSumMC',
    #    'drawOption' : styles.drawOption_QCD
    #}
}

# List of all subsamples used in any plot job.
# i.e. if qcdSum is included in samples to plot
FLATTENED_SAMPLES_TO_PLOT = []
for sample in SAMPLES_TO_PLOT:
    if sample in MERGE_SAMPLES:
        for subsample in MERGE_SAMPLES[sample]['samples']:
            FLATTENED_SAMPLES_TO_PLOT.append(subsample)
    else:
        FLATTENED_SAMPLES_TO_PLOT.append(sample)

ALL_SAMPLES = {}
# Update to use the defaults if necessary
for sample in RECO_SAMPLES.keys():
    defaults = copy.copy(SAMPLE_DEFAULTS)
    defaults.update(RECO_SAMPLES[sample])
    RECO_SAMPLES[sample] = defaults
    # Combine MERGE and RECO samples in ALL samples
    # for simple access 
    ALL_SAMPLES.update(MERGE_SAMPLES)
    ALL_SAMPLES.update(RECO_SAMPLES)

recoSampleDefinitionsZtoElecTau = {
    'SAMPLES_TO_ANALYZE' : SAMPLES_TO_ANALYZE,
    'SAMPLES_TO_PLOT' : SAMPLES_TO_PLOT,
    'SAMPLES_TO_PRINT' : SAMPLES_TO_PRINT,
    'SAMPLE_DEFAULTS' : SAMPLE_DEFAULTS,
    'TARGET_LUMI' : TARGET_LUMI,
    'RECO_SAMPLES' : RECO_SAMPLES,
    'MERGE_SAMPLES' : MERGE_SAMPLES,
    'FLATTENED_SAMPLES_TO_PLOT' : FLATTENED_SAMPLES_TO_PLOT,
    'ALL_SAMPLES' : ALL_SAMPLES
}

