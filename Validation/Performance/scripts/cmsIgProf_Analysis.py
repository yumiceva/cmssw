#! /usr/bin/env python

# Import statements. An '''Analogue''' of the include c++ statement,
# but really really not the same!
# In python everything is an object, even if we don't know. 
# os and time will become 2 objects for us.

import os,sys
import time

def execute(command):
    print '[IgAnalysis] %s ...' %command
    sys.stdout.flush()
    exitstate=os.system(command)
    return exitstate
    
def simple_igprof_analysis(profile_name,outdir,output_type):
    
    """This function takes as arguments:
    -a compressed (.gz) igprof output profile
    -the output directory where to store the output
    -the type of output wanted
    Based on this information it automatically selects the igprof counter
    (PERF_TICKS, MEM_TOTAL or MEM_LIVE) to use with igprof-analyse and it
    performs the analysis.
    If ASCII output_type is given, it keeps only the first seven lines of
    the output file (the report) allowing quick access to the totals for
    the counters in order to put together summary tables.
    If SQLite3 output_type is given, it simply dumps the full reports in
    sqlite3 format, ready to be browsed by igprof-navigator GUI (standalone
    or cgi-bin).
    """
    #Assume the profile_name (i.e. the input profile file) is handled properly by the user or the scripts
    #Use the outdir to determine what kind of analysis to make:

    AnalysisType=''
    #Will use IgProf Analyse with all three IgProf counters
    if "IgProfperf" in outdir:
        AnalysisType='PERF_TICKS'
    elif "IgProfMemTotal" in outdir:
        AnalysisType='MEM_TOTAL'
    elif "IgProfMemLive" in outdir:
        AnalysisType='MEM_LIVE'

    #Use profile input file to determine the name of the output file (based still on the outdir though):
    if len(profile_name.split(".")) == 3: #Dumped profiles case        
        outfile=outdir+"/"+outdir+"___"+profile_name.split(".")[1]+".res"
    else:
        outfile=outdir+"/"+outdir+"___EndOfJob.res"

    #Launch the 1 command:
    
    #command='igprof-analyse -d -v -g -r %s %s|tee -a \%s'%(AnalysisType,profile_name,outfile)
    #Replacing tee: it is polluting the log files...

    #igprof-analyse command depends on the output type desired:
    if output_type =="SQLite3":
        #Case of dumping the igprof-analyse report in sqlite format to be igprof-navigator browseable
        outfile= outfile[:-3]+"sql3" #NEED TO USE sql3 to be compatible with CGI-BIN igprof-navigator
        command='igprof-analyse --sqlite -d -v -g -r %s %s |sqlite3 %s'%(AnalysisType,profile_name,outfile)
        #Execute the igprof-analyse command
        exit=execute(command)
    elif output_type =="ASCII":
        #We can keep this part of the igprof-analysis, to create the html table with the totals at each dump
        command='igprof-analyse -d -v -g -r %s %s > %s'%(AnalysisType,profile_name,outfile)
        #Execute the igprof-analyse command
        exit=execute(command)
        #Let's manipulate the ASCII output to only keep the top 7 lines:
        # we open and read the txt ascii file
        print "Reading the res file"
        txt_file=open(outfile,'r')
        txt_file_content=txt_file.readlines()#again:everything is an object
        print "res file has %s lines!"%len(txt_file_content)
        txt_file.close()
        #overwrite the file to only save the first 7 lines:
        print "Overwriting the res file, to reduce it to 7 lines"
        out_file=open(outfile,'w')
        line_num=0
        for line in txt_file_content:
            out_file.write(line)
            line_num+=1
            if line_num == 7:
                break
        out_file.close()
    
    return exit
    
def diff_igprof_analysis(profile_name,regression_profile_name,outdir):
    
    """
    This function takes as arguments:
    -a compressed (.gz) igprof output profile
    -a second compressed (.gz) igprof output profile against which a regression analysis is intended to be done
    -the output directory where to store the output of the regression analysis
    Based on this information it automatically selects the igprof counter (PERF_TICKS, MEM_TOTAL or MEM_LIVE) to use
    with igprof-analyse and it performs the regression analysis with SQLite3 output.
    """
    AnalysisType=''
    #Will use IgProf Analyse with all three IgProf counters
    if "IgProfperf" in outdir:
        AnalysisType='PERF_TICKS'
    elif "IgProfMemTotal" in outdir:
        AnalysisType='MEM_TOTAL'
    elif "IgProfMemLive" in outdir:
        AnalysisType='MEM_LIVE'

    #Following Giulio's filename convention:
    outfile=outdir+"/"+outdir+"___"+profile_name.split(".")[1]+"_diff"+regression_profile_name.split(".")[1]+".sql3"
    command='igprof-analyse --sqlite -d -v -g --diff-mode -b %s -r %s %s |sqlite3 %s'%(regression_profile_name, AnalysisType,profile_name,outfile)
    exit=execute(command)

    return exit
def library_igprof_analysis(profile_name,outdir):

    """
    This function takes as arguments:
    -a compressed (.gz) igprof output profile
    -the output directory where to store the output of the igprof-analysis merging the results by library
    Based on this information it automatically selects the igprof counter (PERF_TICKS, MEM_TOTAL or MEM_LIVE) to use
    with igprof-analyse and it performs the igprof-analyse analysis merging the results by library and saving the results
    in the usual igprof-navigator browseable SQLite3 format.
    """
    #FIXME the analysisType should be determined in the main part of the script... no need to repeat it in each function!
    AnalysisType=''
    #Will use IgProf Analyse with all three IgProf counters
    if "IgProfperf" in outdir:
        AnalysisType='PERF_TICKS'
    elif "IgProfMemTotal" in outdir:
        AnalysisType='MEM_TOTAL'
    elif "IgProfMemLive" in outdir:
        AnalysisType='MEM_LIVE'

    #Following Giulio's filename convention:
    outfile=outdir+"/"+outdir+"___"+profile_name.split(".")[1]+"_merged"+".sql3"
    #Regular Expression supplied by Giulio:
    command="igprof-analyse --sqlite -d -v -g -r %s -ml -mr 's|.*/(.*)$|\\1|' %s |sqlite3 %s"%(AnalysisType,profile_name,outfile)
    exit=execute(command)
    
    return exit
#-------------------------------------------------------------------------------

# A procedure used for importing the function above with the import statement
# or to run it if the script is called: power python..
if __name__ == '__main__':
    
    import optparse
    
    # Here we define an option parser to handle commandline options..
    usage='IgProf_Analysis.py <options>'
    parser = optparse.OptionParser(usage)
    parser.add_option('-i', '--in_  profile',
                      help='The profile to manipulate' ,
                      default='',
                      dest='profile')
                      
    parser.add_option('-o', '--outdir',
                      help='The directory of the output' ,
                      default='',
                      dest='outdir')
    parser.add_option('-t', '--output_type',
                      help='The type of the output file (ASCII or sqlite3)' ,
                      default='',
                      dest='output_type')
    parser.add_option('-r', '--regression',
                      help='The IgProf output file to make a regression analysis against' ,
                      default='',
                      dest='regressionprofile')
    parser.add_option('-l','--library',
                      help='Perform igprof-analyse merging the results by library',
                      action='store_true',
                      default=False,
                      dest='library') 
    (options,args) = parser.parse_args()
    
    # Now some fault control..If an error is found we raise an exception
    if options.profile=='' or\
       options.outdir=='':
        raise('Please select a profile, an output dir AND a type of output (ASCII or SQLITE3)!')
    
    if not os.path.exists(options.profile) or\
       not os.path.exists(options.outdir):
        raise ('Outdir or input profile not present!')
    
    #launch the function!
    if options.regressionprofile:
        diff_igprof_analysis(options.profile,options.regressionprofile,options.outdir)
    elif options.library:
        library_igprof_analysis(options.profile,options.outdir)
    else:
        simple_igprof_analysis(options.profile,options.outdir,options.output_type)        
 
