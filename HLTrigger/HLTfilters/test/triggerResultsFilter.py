import FWCore.ParameterSet.Config as cms

process = cms.Process('TEST')

process.options = cms.untracked.PSet(
    wantSummary = cms.untracked.bool(True)
)
process.load('FWCore.MessageService.MessageLogger_cfi')
process.MessageLogger.cerr.INFO = cms.untracked.PSet(
    reportEvery = cms.untracked.int32(1), # every!
    limit = cms.untracked.int32(-1)       # no limit!
    )
process.MessageLogger.cerr.FwkReport.reportEvery = 10 # only report every 10th event start
process.MessageLogger.cerr_stats.threshold = 'INFO' # also info in statistics

# read back the trigger decisions
process.source = cms.Source('PoolSource',
    fileNames = cms.untracked.vstring('file:trigger.root')
)

import HLTrigger.HLTfilters.triggerResultsFilter_cfi as hlt

# accept if 'HLT_Path_1' succeeds
process.filter_1 = hlt.triggerResultsFilter.clone(
    triggerConditions = [ 'HLT_Path_1'],
    l1tResults = '',
    throw = False
    )

# accept if 'HLT_Path_2' succeeds
process.filter_2 = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_Path_2'],
    l1tResults = '',
    throw = False
    )

# accept if 'HLT_Path_3' succeeds
process.filter_3 = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_Path_3'],
    l1tResults = '',
    throw = False
    )

# accept if any path succeeds (explicit OR)
process.filter_any_or = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_Path_1 OR HLT_Path_2 OR HLT_Path_3'],
    l1tResults = '',
    throw = False
    )

# accept if any path succeeds (implicit via of paths)
process.filter_any_list = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_Path_1', 'HLT_Path_2', 'HLT_Path_3'],
    l1tResults = '',
    throw = False
    )

# accept if any path succeeds (wildcard, '*')
process.filter_any_star = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_*'],
    l1tResults = '',
    throw = False
    )

# accept if any path succeeds (wildcard, twice '*')
process.filter_any_doublestar = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_*_*'],
    l1tResults = '',
    throw = False
    )


# accept if any path succeeds (wildcard, '?')
process.filter_any_question = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_Path_?'],
    l1tResults = '',
    throw = False
    )

# accept if all path succeed (explicit AND)
process.filter_all_explicit = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_Path_1 AND HLT_Path_2 AND HLT_Path_3'],
    l1tResults = '',
    throw = False
)

# wrong path name (explicit)
process.filter_wrong_name = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_Wrong'],
    l1tResults = '',
    throw = False
)

# wrong path name (wildcard)
process.filter_wrong_pattern = hlt.triggerResultsFilter.clone(
    triggerConditions = ['HLT_*_Wrong'],
    l1tResults = '',
    throw = False
)

# empty path list
process.filter_empty_pattern = hlt.triggerResultsFilter.clone(
    triggerConditions = [''],
    l1tResults = '',
    throw = False
)


process.path_1 = cms.Path( process.filter_1 )
process.path_2 = cms.Path( process.filter_2 )
process.path_3 = cms.Path( process.filter_3 )

process.path_any_or   = cms.Path( process.filter_any_or )
process.path_any_list = cms.Path( process.filter_any_list )
process.path_any_star = cms.Path( process.filter_any_star )

process.path_any_doublestar      = cms.Path( process.filter_any_doublestar )
process.path_any_question        = cms.Path( process.filter_any_question )
process.path_all_explicit        = cms.Path( process.filter_all_explicit )
process.path_wrong_name          = cms.Path( process.filter_wrong_name )
process.path_wrong_pattern       = cms.Path( process.filter_wrong_pattern )
process.path_not_wrong_pattern   = cms.Path( ~ process.filter_wrong_pattern )
process.path_empty_pattern       = cms.Path( process.filter_empty_pattern )

# define an EndPath to analyze all other path results
process.hltTrigReport = cms.EDAnalyzer( 'HLTrigReport',
    HLTriggerResults = cms.InputTag( 'TriggerResults','','TEST' )
)
process.HLTAnalyzerEndpath = cms.EndPath( process.hltTrigReport )
