#!/usr/bin/env python
from optparse import OptionParser, OptionGroup

## set up the option parser
parser = OptionParser(usage="usage: %prog [options] ARG1 ARG2 ARG3 ...", description="This is a script for job submission via crab or batch on a large scale or for the submission. In some cases, which require a more complex configuration it can also be used to execute a single interactive job with a pre-defined default configuration as it is generally used for analysis. The corresponding analysis processes are called and configured via command line options. The arguments ARG1, ARG2, ARG3, ... correspond to the job directories that have been prepared for submission. These directories should contain all datacards and input files needed for the execution of the job. They are supposed to end with a floating point number or with an integer number corresponding to the mass, for which the process is to be applied. There is a few exception to this common rule as explained in this parameter description. A small set of options and sub-options can still be passed on to this script. More dedicated options that should act on the scripts that are to be executed within this script can be piped through via the common option --options, enbraced by '...'. More explanation are given in section COMMON OPTIONS of this parameter description. This is meant to facilitate expert use if necessary.")
##
## MAIN OPTIONS
##
agroup = OptionGroup(parser, "MAIN OPTIONS", "These are the command line options that list all available processes that can be executed via this script. Each process is run with a default configuration that has been recently used for analysis. Depending on the process the execution of the script will result in the submission of a pre-defined set of batch jobs via lxb (lxq) or in a submission of a pre-defined set of jobs via crab. The options that will lead submission to lxb (lxq) are: --max-likelihood, --likelihood-scan, --multidim-fit, --asymptotic, --tanb+. The options that will lead to crab submission are: --significance, --CLs, --bayesian, --tanb. The latter requires the proper setup of a glite and a crab environment. Note that this is the case even if the submission would only take place via lxb (lxq), as the grid environment is used internally by crab. All options are explained in the following. All command line options in this section are exclusive. For the options --likelihood-scan, --asymptotic, --tanb+ it is possible to force interactive running. If run in batch mode the jobs will be split per mass.")
agroup.add_option("--max-likelihood", dest="optMLFit", default=False, action="store_true",
                  help="Perform a maximum likelihood fit scan to determine the signal strength from the datacards in the directory/ies corresponding to ARGs. This fit will be applied to the SM with single signal modifier. The pre-configuration corresponds to --stable, --rMin -5, --rMax 5. This pre-configuration will be applied irrespective of the mass, for which the process should be executed. The process will be executed via lxb (lxq), split by each single mass point that is part of ARGs or as a single interactive job when using the option --interactive. When submitting to lxb (lxq) you can configure the queue to which the jobs will be submitted as described in section BATCH OPTIONS of this parameter description. [Default: False]")

agroup.add_option("--likelihood-scan", dest="optNLLScan", default=False, action="store_true",
                  help="Perform a maximum likelihood scan to determine the signal strength from the datacards in the directory/ies corresponding to ARGs. This scan will be applied to the SM with single signal modifier. The pre-configuration corresponds to --points 100, --rMin -2, --rMax 2. This pre-configuration will be applied irrespective of the mass, for which the process should be executed. The process will be executed via lxb (lxq), split by each single mass point that is part of ARGs or as a single interactive job when using the option --interactive. When submitting to lxb (lxq) you can configure the queue to which the jobs will be submitted as described in section BATCH OPTIONS of this parameter description. When running in batch mode you can go one level up in the expected directory structure as described in the head of this section. [Default: False]")
agroup.add_option("--multidim-fit", dest="optMDFit", default=False, action="store_true",
                  help="Perform a maximum likelihood fit in two dimensions to determine the signal strength from the datacards in the directory/ies corresponding to ARGs. This option requires the configuration of a dedicated physics model as described in section MODEL OPTIONS of this parameter description. The process will be executed via lxb (lxq). [Default: False]")
agroup.add_option("--significance", dest="optSig", default=False, action="store_true",
                  help="Calculate the expected significance from likelihood profiling. The expected significance and its uncertainties are based on toys. This script will submit toys to a batch system or to the grid using crab. This action will require a grid certificate. You can configure this script to submit to the grid or to submit to lxb (lxq) as described in section SIGNIFICANCE OPTIONS in this parameter description. You can monitor and receive the results of your jobs once finished using the script limit.py using the CRAB OPTIONS as explained in the parameter description, there. [Default: False]")
agroup.add_option("--asymptotic", dest="optAsym", default=False, action="store_true",
                  help="Calculate asymptotic CLs limits from the datacards in the directory/ise corresponding to ARGs. The process will be executed via lxb (lxq), split by each single mass point that is part of ARGs or as a single interactive job when using the option --interactive. When submitting to lxb (lxq) you can configure the queue to which the jobs will be submitted as described in section BATCH OPTIONS of this parameter description. When running in batch mode you can go one level up in the expected directory structure as described in the head of this section. [Default: False]")
agroup.add_option("--injected", dest="optInject", default=False, action="store_true",
                  help="Calculate asymptotic CLs limits with a SM signal injected from the datacards in the directory/ise corresponding to ARGs. This process is fully toy based and will require a large numnber of toys, which will be submitted via lxb (lxq). For each toy a pseudo-dataset will be created and an observed limit will be calculated. It is possible to give an input file from which the pulls of the nuisance parameters will be taken, when running the limits. The median and quantiles of the tossed toys define the expected limit with signal injected and the uncertainties. This script internally calls the script lxb-injected.py. [Default: False]")
agroup.add_option("--CLs", dest="optCLs", default=False, action="store_true",
                  help="Calculate the observed and expected full CLs limits. This method is completely toy based. This script will submit toys to the grid using crab. This action will require a grid certificate. As this operation is very computing intensive there is no pre-defined option to submit to lxb (lxq). You can monitor and receive the results of your jobs once finished using the script limit.py using the CRAB OPTIONS as explained in the parameter description, there. [Default: False]")
agroup.add_option("--bayesian", dest="optBayes", default=False, action="store_true",
                  help="Calculate the observed and expected bayesian limits. This method is completely toy based. This script will submit toys, which are needed for the markov chain integration to the grid using crab. This action will require a grid certificate. As this operation is very computing intensive there is no pre-defined option to submit to lxb (lxq). You can monitor and receive the results of your jobs once finished using the script limit.py using the CRAB OPTIONS as explained in the parameter description, there. [Default: False]")
agroup.add_option("--tanb", dest="optTanb", default=False, action="store_true",
                  help="Calculate the observed and expected limits directly in the MSSM mA-tanb plane based on full CLs limits. This method is completely toy based. This script will prepare the directory structure to do these calculations and submit toys to the grid using crab. This action will require a grid certificate. As this operation is very computing intensive there is no pre-defined option to submit to lxb (lxq). You can monitor and receive the results of your jobs once finished using the script limit.py using the CRAB OPTIONS as explained in the parameter description, there. [Default: False]")
agroup.add_option("--tanb+", dest="optTanbPlus", default=False, action="store_true",
                  help="Calculate the observed and expected limits directly in the MSSM mA-tanb plane based on asymptotic CLs limits. This script will prepare the directory structure to do these calculations and submit and submit the required asymptotic limit calculation for each corresponding point in mA to lxb (lxq). The process will be executed via lxb (lxq), split by each single mass point that is part of ARGs or as a single interactive job when using the option --interactive. When submitting to lxb (lxq) you can configure the queue to which the jobs will be submitted as described in section BATCH OPTIONS of this parameter description. When running in batch mode you can go one level up in the expected directory structure as described in the head of this section. [Default: False]")
parser.add_option_group(agroup)
##
## COMMON OPTIONS
##
bgroup = OptionGroup(parser, "COMMON OPTIONS", "These are command line options that are not specific to one or the other main option. Where such restrictions exist they are specified in the corresponding parameter description below.")
bgroup.add_option("--printOnly", dest="printOnly", default=False, action="store_true",
                  help="Only print the command line option(s) to the screen. Do not execute them. This can be used if you only want to know the command line option to use it elsewhere in a slightly modified way. [Default: False]")
bgroup.add_option("--cycles", dest="cycles", default=1, type="int",
                  help="Number of submission cycles for crab job submission. This option only applies to the main options --CLs and --tanb. For all other main options it has no effect. One submission cycle consists of 50 crab jobs. [Default: 1]")
bgroup.add_option("--options", dest="opt", default="", type="string",
                  help="You can use this string for additional options that can be passed on to the scripts that are executed within this script. NB: these options should be enclosed by \"...\". [Default: \"\"]")
parser.add_option_group(bgroup)
##
## BATCH OPTIONS
##
cgroup = OptionGroup(parser, "BATCH OPTIONS", "These are the command line options that can be used to configure lxb (lxq) batch job submission via the script lxb_submit.py. Batch job submission of this type is applicable for the following options: --likelihood-scan, --asymptotic, --tanb+. When using batch job submission in one of these main options it is possible to go up by one level in the directory structure when specifying ARGs as described in the head of this parameter description. Note: when runnign in batch mode batch jobs are split by each mass directory that can be found in the directory structure.")
cgroup.add_option("--toys", dest="toys", default="5000", type="string",
                  help="Set the number toys that you want to use to calculate the toy based expectations. This is valid for the calculation of the expected significance (--significance) and for the calculation of the expected 95% CL upper asymptotic CLs limit with a SM signal injected (--injected). [Default: 5000]")
cgroup.add_option("-q", "--queue", dest="queue", default="-q 8nh", type="string",
                  help="The queue, to which to submit the jobs. [Default: \"-q 8nh\"]")
cgroup.add_option("--interactive", dest="interactive", default=False, action="store_true",
                  help="Force interactive running. Do not run in batch mode. This will lead to one large execution. [Default: False]")
cgroup.add_option("--lxq", dest="lxq", default=False, action="store_true",
                  help="Specify this option when running on lxq instead of lxb for simple batch job submissions. [Default: False]")
cgroup.add_option("--condor", dest="condor", default=False, action="store_true",
                  help="Specify this option when running on condor instead of lxb for simple batch job submissions. [Default: False]")
parser.add_option_group(cgroup)
##
## MODEL OPTIONS
##
dgroup = OptionGroup(parser, "MODEL OPTIONS", "These are the command line options that can be used to configure the submission of multi-dimensional fits or asymptotic limits that do require specific models. Specific models can be used for option --multidim-fit and for option --asymptotic. Possible model options for option --multidim-fit are: ggH-bbH (MSSM), ggH-qqH (SM) and cV-cF (SM). Possible model options for option --asymptotic are: \"\" (SM), ggH (MSSM) and bbH (MSSM).")
dgroup.add_option("--physics-model", dest="fitModel", default="", type="choice", choices=["ggH-bbH", "ggH-qqH", "cV-cF", "ggH", "bbH", ""],
                  help="Define the model for which you want to submit the process with option --multidim-fit ('ggH-bbH' (MSSM), 'ggH-qqH' (SM) and 'cV-cF' (SM)) or option --asymptotic ('ggH' (MSSM), 'bbH' (MSSM) and '' (SM)). [Default: \"\"]")
dgroup.add_option("--acceptance-corrected", dest="acceptCorrect", default=False, action="store_true",
                  help="For the models 'ggH' (MSSM) and 'bbH' (MSSM) when running with main option --asymptotic choose here whether to apply the acceotance corrections due to the chosen mass window on generator level or not. In any other configuration this option wil have no effect. [Default: False]")
parser.add_option_group(dgroup)
##
## SIGNIFICANCE
##
egroup = OptionGroup(parser, "SIGNIFICANCE OPTIONS", "These are the command line options that can be used to configure the submission of toys for significance calculations. The toys can be submitted to the grid or to lxb (lxq) using crab. The number of toys per mass that will be the same for all masses can be configured via the option --toys as described in section BATCH OPTIONS. The number of crab jobs that will be the same for all masses can be configured via the option --jobs as described in this section.")
egroup.add_option("--jobs", dest="jobs", default="100", type="string",
                  help="Set the number crab jobs that you want to submit to calculate the toy based expected significance. [Default: 100]")
egroup.add_option("--grid", dest="grid", default=False, action="store_true",
                  help="Use this option if you want to submit your jobs to the grid. Otherwise they will be submitted to lxb (lxq). [Default: False]")
parser.add_option_group(egroup)
##
## INJECTED OPTIONS
##
fgroup = OptionGroup(parser, "INJECTED OPTIONS", "These are the command line options that can be used to configure lxb (lxq) batch job submission for 95% CL upper asymptotic CLs limits with a SM signal injected via the script lxb-injected.py, which used the script limit.py. The expected limit with a SM signal injected is obtained from a large sample of toys. For each toy a pseudo data set is prepared and the observed limit is calculated. The toys are collected using the script limit.py with option --injected. The expected limit and the uncertainties are obtained from the median and the quantiles of the collected toys. The number of toys (--toys) and the batch queue options (--queue) can be configured using the options described in section BATCH OPTIONS of this parameter description. The option --bunch-masses as described below can be used to define a maximal number of masses that will be bunched into a single job before a new job is created. The option --nuisances can be used to pass a pre-defined set of nuisance parameters to limit.py that will be used instead of determining the central values of the nuisances by the prefit for each toy on its own.")
fgroup.add_option("--bunch-masses", dest="nmasses", default="10", type="string",
                  help="This is the maximal number of masses that will be bunched into a single job, before a new job will be created. If you want to do the calculation for nine masses, 1000 tos and maximal 4 masses per bunch you will create 3000 jobs, 2000 jobs for 4 masses each and 1000 jobs for a single mass. [Default: 10]")
fgroup.add_option("--external-pulls", dest="nuisances", default="", type="string",
                  help="Specify the full path to a root output file of limit.py with option --max-likelihood (e.g. mlfit.root) to enforce the use of pre-calculated central values of the nuisance parmeters involved in this fit. It is in the responsibility of the user that the nuisance parameter names in the output file and the nuisance parameter names in the current workspace fit together. The limit will be run w/ option --no-prefit. For more details have a look to the description of option --external-pulls of the script limit.py. [Default: \"\"]")
fgroup.add_option("--SplusB", dest="signal_plus_BG", default=False, action="store_true",
                  help="When using options --external-pulls, use the fit results with signal plus background. If 'False' the fit result of the background only hypothesis is used. [Default: False]")
parser.add_option_group(fgroup)
##
## TANB+
##
ggroup = OptionGroup(parser, "TANB+ OPTIONS", "These are the command line options that can be used to configure the submission of tanb+. This option is special in the way that it needs modifications of the directory structure before the limits can be run. Via the script submit.py this setup can only be run interactively using the commend option --setup. Once the directory structure has been set up the limit calculation can be run interactively or in batrch mode.")
ggroup.add_option("--setup", dest="setup", default=False, action="store_true",
                  help="Use the script to setup the directory structure for direct mA-tanb limits interactively. If false the the script will assume that this has already been done and execute the limit calculation either in batch mode or interactive. [Default: False]")
parser.add_option_group(ggroup)

## check number of arguments; in case print usage
(options, args) = parser.parse_args()
if len(args) < 1 :
    parser.print_usage()
    exit(1)

import re
import os

from HiggsAnalysis.HiggsToTauTau.utils import contained
from HiggsAnalysis.HiggsToTauTau.utils import is_number
from HiggsAnalysis.HiggsToTauTau.utils import get_mass

def subvec(vec, min, max):
    '''
    Create a subvector of a vector of strings that contains
    an integer and a / as substring
    '''
    subvec = []
    for directory in vec :
        if re.search(r"^.*/\d+?$", directory) :
            mass = int(directory[directory.rfind("/")+1:])
            if min <= mass and mass <= max:
                subvec.append(directory)
    return subvec

def vec2str(vec, delim=" "):
    '''
    Return a string concatenated from elements of a vector
    seperate these elements by a deliminator as specified
    by delim. The default deliminator is " "
    '''
    str = ""
    for i in vec:
        str+=i
        str+=delim
    return str

def directories(args) :
    ## prepare structure of parent directories
    dirs = []
    for dir in args :
        if is_number(get_mass(dir)) or get_mass(dir) == "common" :
            dir = dir[:dir.rstrip('/').rfind('/')]
        if not dir in dirs :
            dirs.append(dir)
    ## prepare mapping of masses per parent directory
    masses = {}
    for dir in dirs :
        buffer = []
        for path in args :
            if dir+'/' in path :
                if is_number(get_mass(path)) :
                    mass = get_mass(path)
                    if not contained(mass, buffer) :
                        buffer.append(mass)
        masses[dir] = list(buffer)
    return (dirs, masses)

def lxb_submit(dirs, masses, cmd='--asymptotic', opts='') :
    '''
    do a lxb submission for jobs that can be executed via limit.py.
    dirs corresponds to a list of input directories, masses to a 
    dictionary of masses per dir, cmd to the complete main option
    of limit.py and opts to additional options that should be passed
    on to limit.py.
    '''
    for dir in dirs:
        ana = dir[:dir.rfind('/')]
        limit = dir[len(ana)+1:]
        jobname = ana[ana.rfind('/')+1:]+'-'+limit
        ## add compliance with lxq or condor
        sys = ''
        if options.lxq :
            sys = ' --lxq'
        elif options.condor :
            sys = ' --condor'
        ## create inputs corresponding to the masses per parent directory in dirs
        inputs = ""
        for mass in masses[dir] :
            inputs+= dir+'/'+mass+' '
        ## create submission scripts
        if options.printOnly :
            print "lxb-limit.py --name {JOBNAME} --batch-options \"{QUEUE}\" --limit-options \"{METHOD} {OPTS}\" {SYS} {DIR}".format(
                JOBNAME=jobname, DIR=inputs.rstrip(), QUEUE=options.queue, METHOD=cmd, OPTS=opts.rstrip(), SYS=sys)
        else:
            os.system("lxb-limit.py --name {JOBNAME} --batch-options \"{QUEUE}\" --limit-options \"{METHOD} {OPTS}\" {SYS} {DIR}".format(
                JOBNAME=jobname, DIR=inputs.rstrip(), QUEUE=options.queue, METHOD=cmd, OPTS=opts.rstrip(), SYS=sys))
            ## execute
            os.system("./{JOBNAME}_submit.sh".format(JOBNAME=jobname))
            ## store
            os.system("mv {JOBNAME}_submit.sh {JOBNAME}".format(JOBNAME=jobname))

##
## MAX-LIKELIHOOD
##
if options.optMLFit :
    if options.interactive :
        for dir in args :
            mass = get_mass(dir)
            if mass == 'common' :
                continue
            if options.printOnly :
                print"limit.py --max-likelihood --stable --rMin -5 --rMax 5 {DIR}".format(DIR=dir)
            else :
                os.system("limit.py --max-likelihood --stable --rMin -5 --rMax 5 {USER} {DIR}".format(USER=options.opt, DIR=dir))
    else :
        ## directories and mases per directory
        struct = directories(args)
        lxb_submit(struct[0], struct[1], "--max-likelihood", "--stable --rMin -5 --rMax 5 {USER}".format(USER=options.opt))
##
## LIKELIHOOD-SCAN
##
if options.optNLLScan :
    if options.interactive :
        for dir in args :
            mass = get_mass(dir)
            if mass == 'common' :
                continue
            if options.printOnly :
                print"limit.py --likelihood-scan --points 100 --rMin -2 --rMax 2 {DIR}".format(DIR=dir)
            else :
                os.system("limit.py --likelihood-scan --points 100 --rMin -2 --rMax 2 {USER} {DIR}".format(USER=options.opt, DIR=dir))
    else :
        ## directories and mases per directory
        struct = directories(args)
        lxb_submit(struct[0], struct[1], "--likelihood-scan", "--points 100 --rMin -2 --rMax 2 {USER}".format(USER=options.opt))
##
## MULTIDIM-FIT
##
if options.optMDFit :
    for dir in args :
        mass = get_mass(dir)
        if mass == 'common' :
            continue
        ## add acceptance corrections if configured such
        if options.acceptCorrect :
            path = dir[:dir.rstrip('/').rfind('/')]
            if options.printOnly :
                print "scale2accept.py -i {PATH} {MASS}".format(PATH=path, MASS=mass)
            else :
                os.system("scale2accept.py -i {PATH} {MASS}".format(PATH=path, MASS=mass))
        ## define command line, model and model options
        cmd   = ""
        model = ""
        opts  = ""
        ## MSSM ggH versus bbH
        if "ggH-bbH" in options.fitModel :
            from HiggsAnalysis.HiggsToTauTau.mssm_multidim_fit_boundaries import mssm_multidim_fit_boundaries as bounds
            cmd   = "lxb-multidim-fit.py --name GGH-BBH-{MASS} --njob 100 --npoints 400".format(MASS=mass)
            model = "--physics-model 'ggH-bbH=HiggsAnalysis.HiggsToTauTau.PhysicsBSMModel:floatingMSSMXSHiggs'"
            opts  = "--physics-model-options 'modes=ggH,bbH;ggHRange=0:{GGH};bbHRange=0:{BBH}'".format(GGH=bounds[mass][0], BBH=bounds[mass][1])
        ## SM ggH versus qqH (this configuration is optimized fro mH=125)
        elif "ggH-qqH" in options.fitModel :
            cmd   = "lxb-multidim-fit.py --name GGH-QQH-{MASS} --njob 100 --npoints 625".format(MASS=mass)
            model = "--physics-model 'ggH-qqH=HiggsAnalysis.CombinedLimit.PhysicsModel:floatingXSHiggs'"
            opts  = "--physics-model-options 'modes=ggH,qqH ggHRange=0:5 qqHRange=0:5'"
        ## SM cV versus cF (this configuration is optimized fro mH=125)
        elif "cV-cF" in options.fitModel :
            cmd   = "lxb-multidim-fit.py --name CV-CF-{MASS} --njob 100 --npoints 225".format(MASS=mass)
            model = "--physics-model 'cV-cF=HiggsAnalysis.CombinedLimit.HiggsCouplings:cVcF'"
            opts  = "--physics-model-options 'modes=ggH,qqH cVRange=0:3 cFRange=0:3'"
        ## add lxq compliance
        sys = ""
        if options.lxq :
            sys = " --lxq"
        ## add batch options
        queue = " --batch-options '%s'" % options.queue
        if options.printOnly :
            print "{CMD} {MODEL} {OPTS} {QUEUE} {SYS} {USER} {DIR}".format(
                CMD=cmd, MODEL=model, OPTS=opts, QUEUE=queue, SYS=sys, USER=options.opt, DIR=dir)
        else :
            os.system("{CMD} {MODEL} {OPTS} {QUEUE} {SYS} {USER} {DIR}".format(
                CMD=cmd, MODEL=model, OPTS=opts, QUEUE=queue, SYS=sys, USER=options.opt, DIR=dir))
##
## SIGNIFICANCE
##
if options.optSig :
    optgrid = '--lsf' if not options.grid else ''
    if options.printOnly :
        print "submit-slave.py --method significance -t {TOYS} -j {JOBS} {USER} {GRID}".format(TOYS=options.toys, JOBS=options.jobs, USER=options.opt, GRID=optgrid)
    else :
        os.system("submit-slave.py --method significance -t {TOYS} -j {JOBS} {USER} {GRID}".format(TOYS=options.toys, JOBS=options.jobs, USER=options.opt, GRID=optgrid))
##
## ASYMPTOTIC (with dedicated models)
##
if options.optAsym :
    ## apply acceptance corrections for physics-models 'ggH or 'bbH' if configured such
    if options.fitModel == 'ggH' or options.fitModel == 'bbH' :
        if options.acceptCorrect :
            for dir in args :
                mass = get_mass(dir)
                path = dir[:dir.rstrip('/').rfind('/')]
                if options.printOnly :
                    print "scale2accept.py -i {PATH} {MASS}".format(PATH=path, MASS=mass)
                else :
                    os.system("scale2accept.py -i {PATH} {MASS}".format(PATH=path, MASS=mass))
    ## define command line, model and model options
    cmd   = "--asymptotic"
    model = ""
    opts  = ""
    ## prepare calculation
    if options.interactive :
        for dir in args :
            mass = get_mass(dir)
            if mass == 'common' :
                continue
            ## MSSM ggH while bbH is profiled
            if "ggH" in options.fitModel :
                from HiggsAnalysis.HiggsToTauTau.mssm_multidim_fit_boundaries import mssm_multidim_fit_boundaries as bounds
                model = "--physics-model 'tmp=HiggsAnalysis.HiggsToTauTau.PhysicsBSMModel:floatingMSSMXSHiggs'"
                opts  = "--physics-model-options 'modes=ggH;ggHRange=0:{GGH}'".format(GGH=bounds[mass][0])
            ## MSSM bbH while ggH is profiled
            elif "bbH" in options.fitModel :
                from HiggsAnalysis.HiggsToTauTau.mssm_multidim_fit_boundaries import mssm_multidim_fit_boundaries as bounds
                model = "--physics-model 'tmp=HiggsAnalysis.HiggsToTauTau.PhysicsBSMModel:floatingMSSMXSHiggs'"
                opts  = "--physics-model-options 'modes=bbH;bbHRange=0:{BBH}'".format(BBH=bounds[mass][1])
            if options.printOnly :
                print "limit.py {CMD} {MODEL} {OPTS} {USER} {DIR}".format(CMD=cmd, MASS=mass, MODEL=model, OPTS=opts, USER=options.opt, DIR=dir)
            else :
                os.system("limit.py {CMD} {MODEL} {OPTS} {USER} {DIR}".format(CMD=cmd, MASS=mass, MODEL=model, OPTS=opts, USER=options.opt, DIR=dir))
    else :
        ## MSSM ggH while bbH is profiled (needs to be run with a fixed range for all masses here)
        if "ggH" in options.fitModel :
            from HiggsAnalysis.HiggsToTauTau.mssm_multidim_fit_boundaries import mssm_multidim_fit_boundaries as bounds
            model = "--physics-model 'tmp=HiggsAnalysis.HiggsToTauTau.PhysicsBSMModel:floatingMSSMXSHiggs'"
            opts  = "--physics-model-options 'modes=ggH;ggHRange=0:{GGH}'".format(GGH='5')
        ## MSSM bbH while ggH is profiled (needs to be run with a fixed range for all masses here)
        elif "bbH" in options.fitModel :
            from HiggsAnalysis.HiggsToTauTau.mssm_multidim_fit_boundaries import mssm_multidim_fit_boundaries as bounds
            model = "--physics-model 'tmp=HiggsAnalysis.HiggsToTauTau.PhysicsBSMModel:floatingMSSMXSHiggs'"
            opts  = "--physics-model-options 'modes=bbH;bbHRange=0:{BBH}'".format(BBH='5')
        ## directories and mases per directory
        struct = directories(args)
        lxb_submit(struct[0], struct[1], cmd, "{MODEL} {OPTS} {USER}".format(MODEL=model, OPTS=opts, USER=options.opt))
##
## INJECTED (asymptotic limits with signal injected, implementation for SM only)
##
if options.optInject :
    ## the input for lxb-injected.py should be a path, that is passed on as an 
    ## option and the masses in question. Prepare here the corresponding paths
    ## and directories. 
    paths = []
    dirs = {}
    for dir in args :
        head = dir[:dir.rstrip('/').rfind('/')]
        if not contained(head, paths) :
            paths.append(head)
            dirs[head] = []
    for dir in args :
        for path in paths :
            if path in dir :
                tail = dir[dir.rstrip('/').rfind('/')+1:]
                if is_number(tail) :
                    dirs[path].append(tail)
    ## prepare options
    opts = options.opt
    opts+=" --observedOnly"
    if not options.nuisances == "" :
        opts+=" --no-prefit --external-pulls \"{PATH}\" --signal-plus-background {SPLUSB}".format(PATH=options.nuisances, SPLUSB=options.signal_plus_BG)
    ## do the submit 
    for path in paths :
        jobname = "injected-"+path[path.rstrip('/').rfind('/')+1:]
        if options.printOnly :
            print "lxb-injected.py --name {NAME} --input {PATH} --batch-options \"{SUB}\" --toys {NJOB} --mass-points-per-job {NMASSES} --limit-options \"{OPTS}\" {MASSES}".format(
                NAME=jobname, PATH=path, SUB=options.queue, NJOB=options.toys, NMASSES=options.nmasses, OPTS=opts, MASSES=' '.join(dirs[path]))
        else :
            os.system("lxb-injected.py --name {NAME} --input {PATH} --batch-options \"{SUB}\" --toys {NJOB} --mass-points-per-job {NMASSES} --limit-options \"{OPTS}\" {MASSES}".format(
                NAME=jobname, PATH=path, SUB=options.queue, NJOB=options.toys, NMASSES=options.nmasses, OPTS=opts, MASSES=' '.join(dirs[path])))
##
## CLs
##
if options.optCLs :
    cycle = options.cycles
    while cycle>0 :
        print "***********************************************************"
        print " %s cycle(s) to finish" % cycle
        print "***********************************************************"
        cmd = "submit-slave.py --bin combine --method CLs"
        sub = "--toysH 50 -t 1000 -j 500 --random --server --priority"
        if len(subvec(args, 90, 150))>0 :
            dirs = vec2str(subvec(args, 90, 150))
            if options.printOnly :
                print "{CMD} -n 5 --min   0.3  --max   1.5 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                print "{CMD} -n 5 --min   2.0  --max   4.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
            else :
                os.system("{CMD} -n 5 --min   0.3  --max   1.5 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                os.system("{CMD} -n 5 --min   2.0  --max   4.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
        cycle = cycle-1
##
## BAYES
##
if options.optBayes :
    cycle = options.cycles
    while cycle>0 :
        print "***********************************************************"
        print " %s cycle(s) to finish" % cycle
        print "***********************************************************"
        if len(subvec(args, 90, 150))>0 :
            cmd = "submit-slave.py --bin combine --method Bayesian"
            sub = "-t 1000 -j 100 --server --random --priority"
            if options.printOnly :
                print "{CMD} {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=vec2str(subvec(args, 90, 150)))
            else :
                os.system("{CMD} {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=vec2str(subvec(args, 90, 150))))
        cycle = cycle-1
##
## TANB
##
if options.optTanb or options.optTanbPlus :
    cycle = options.cycles
    while cycle>0 :
        if options.optTanb :
            print "***********************************************************"
            print " %s cycle(s) to finish" % cycle
            print "***********************************************************"
        cmd = ""
        if options.optTanb :
            cycle = 1
            cmd = "submit-slave.py --bin combine --method tanb"
        elif options.optTanbPlus :
            if options.setup :
                cmd = "submit-slave.py --bin combine --method tanb"
        if not cmd == "" :
            sub = "--interactive" if options.optTanbPlus else "--toysH 100 -t 200 -j 100 --random --server --priority"
            if len(subvec(args,  90, 249))>0 :
                dirs = vec2str(subvec(args,  90,  249))
                if options.printOnly :
                    print "{CMD} -n  3 --min  0.5  --max  0.9 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  3 --min  1.0  --max  3.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  4 --min  6.0  --max 15.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  2 --min 20.0  --max 30.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  2 --min 50.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                else :
                    os.system("{CMD} -n  3 --min  0.5  --max  0.9 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  3 --min  1.0  --max  3.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  4 --min  6.0  --max 15.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  2 --min 20.0  --max 30.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  2 --min 50.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
            if len(subvec(args, 250, 299))>0 :
                dirs = vec2str(subvec(args, 250,  299))
                if options.printOnly :
                    print "{CMD} -n  3 --min  0.5  --max  0.9 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  3 --min  1.0  --max  3.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  4 --min  6.0  --max 15.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  2 --min 20.0  --max 30.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  2 --min 50.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                else :
                    os.system("{CMD} -n  3 --min  0.5  --max  0.9 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  3 --min  1.0  --max  3.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  4 --min  6.0  --max 15.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  2 --min 20.0  --max 30.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  2 --min 50.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
            if len(subvec(args, 300, 399))>0 :                                                                                       
                dirs = vec2str(subvec(args, 300,  399))                                                                              
                if options.printOnly :
                    print "{CMD} -n  3 --min  1.0  --max  3.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  4 --min  6.0  --max 15.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  6 --min 20.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                else :
                    os.system("{CMD} -n  3 --min  1.0  --max  3.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  4 --min  6.0  --max 15.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  6 --min 20.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
            if len(subvec(args, 400, 499))>0 :                                                                                       
                dirs = vec2str(subvec(args, 400,  499))
                if options.printOnly :
                    print "{CMD} -n  3 --min  1.0  --max  5.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  4 --min 10.0  --max 25.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  5 --min 30.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                else :
                    os.system("{CMD} -n  3 --min  1.0  --max  5.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  4 --min 10.0  --max 25.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  5 --min 30.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
            if len(subvec(args, 500, 1000))>0 :                                                                                      
                dirs = vec2str(subvec(args, 500, 1000))
                if options.printOnly :
                    print "{CMD} -n  3 --min  1.0  --max  5.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  4 --min 10.0  --max 25.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                    print "{CMD} -n  5 --min 30.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs)
                else :
                    os.system("{CMD} -n  3 --min  1.0  --max  5.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  4 --min 10.0  --max 25.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
                    os.system("{CMD} -n  5 --min 30.0  --max 70.0 {SUB} {OPTS} {USER} {DIRS}".format(CMD=cmd, SUB=sub, OPTS=options.opt, USER=options.opt, DIRS=dirs))
        else :
            if options.interactive :
                for dir in args :
                    mass = get_mass(dir)
                    if mass == 'common' :
                        continue
                    if options.printOnly :
                        print "limit.py --tanb+ {OPTS} {DIR}".format(OPTS=options.opt, DIR=dir)
                    else :
                        os.system("limit.py --tanb+ {OPTS} {DIR}".format(OPTS=options.opt, DIR=dir))
            else :
                dirs = []
                for dir in args :
                    ## chop off masses directory if present as this will be added automatically by the submission script
                    if is_number(get_mass(dir)) or get_mass(dir) == "common" :
                        dir = dir[:dir.rstrip('/').rfind('/')]
                    if not dir in dirs :
                        dirs.append(dir)
                ## directories and mases per directory
                struct = directories(args)
                lxb_submit(struct[0], struct[1], "--tanb+", options.opt)
        cycle = cycle-1
