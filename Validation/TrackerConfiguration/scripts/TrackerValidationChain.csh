#! /bin/csh

setenv SCRAM_ARCH slc5_ia32_gcc434
eval `scramv1 runtime -csh`

set RefRelease="CMSSW_3_6_0_pre6"
set WebDir=$CMSSW_VERSION # this is where you want the plots to end up; normally, you should put here the current release

# Possible values are:
# Reconstruction  : preforms reconstruction+validation + histograms comparison
# Validation : validation + histograms comparison
# Harvesting : harvesting + histogram comparison
# Comparison :histogram comparison

set Mode="Comparison"
#set Mode="Harvesting"
#set Mode="Reconstruction"
#set Mode="Validation"


# do you want to copy histograms on the validation page?

set copyWWW="false"
#set copyWWW="true"

# set the histogram file name in Comparison mode
set histogramfile="DQM_V0001_R000000001__RelValSingleMuPt10__CMSSW_3_6_0-MC_36Y_V4-v1__GEN-SIM-RECO.root"

#reference histogram file name
#set refhistogramfile="DQM_V0001_R000000001__${RefRelease}__RelVal__Validation.root"
set refhistogramfile="DQM_V0001_R000000001__RelValSingleMuPt10__CMSSW_3_6_0_pre6-MC_36Y_V4-v1__GEN-SIM-RECO.root"

setenv DATADIR $CMSSW_BASE/src
setenv REFDIRS /afs/cern.ch/cms/performance/tracker/activities/validation/ReferenceFiles
setenv REFDIR $REFDIRS/$RefRelease
setenv NEWREFDIR $REFDIRS/$CMSSW_VERSION
if ( ! -e $NEWREFDIR ) mkdir $NEWREFDIR

#if ( ! -e $NEWREFDIR/SimHits ) mkdir $NEWREFDIR/SimHits
#if ( ! -e $NEWREFDIR/Digis ) mkdir $NEWREFDIR/Digis
#if ( ! -e $NEWREFDIR/RecHits ) mkdir $NEWREFDIR/RecHits
#if ( ! -e $NEWREFDIR/LiteGeometry ) mkdir $NEWREFDIR/LiteGeometry
#if ( ! -e $NEWREFDIR/TrackingRecHits ) mkdir $NEWREFDIR/TrackingRecHits
#if ( ! -e $NEWREFDIR/Tracks ) mkdir $NEWREFDIR/Tracks
#if ( ! -e $NEWREFDIR/TrackingParticles) mkdir $NEWREFDIR/TrackingParticles

cd ${DATADIR}



# Get the relevant packages
#
if (! -e Validation/Geometry) cvs co Validation/Geometry
if (! -e Validation/TrackerHits) cvs co Validation/TrackerHits
if (! -e Validation/TrackerDigis) cvs co Validation/TrackerDigis
if (! -e Validation/TrackerRecHits) cvs co Validation/TrackerRecHits
if (! -e Validation/TrackingMCTruth) cvs co Validation/TrackingMCTruth
if (! -e Validation/RecoTrack) cvs co  Validation/RecoTrack ### TEMPORARY: I MODIFIED THE HEAD OF /test
#Add also co of CalibTracker for test o Lite version of Geometry
#if (! -e CalibTracker/SiStripCommon) cvs co -r $CMSSW_VERSION CalibTracker/SiStripCommon
#
# Geometry Validation
#
#if ('${1}' == 'GEOMETRY' ) then
#cd ${DATADIR}/Validation/Geometry/test
#./TrackerGeometryValidation.sh ${RefRelease}
#    if ($copyWWW == "true") then 
#    chmod a+x copyWWWTrackerGeometry.sh    
#    ./copyWWWTrackerGeometry.sh
#    endif
#endif
#
# Run validation chain
#
cd ${DATADIR}/Validation/TrackerConfiguration/test

#cp /afs/cern.ch/cms/data/CMSSW/Validation/TrackerHits/data/Muon.root .

if ($Mode == "Reconstruction") then
cmsRun Muon_FullChain_cfg.py

else if ($Mode == "Validation") then
cmsRun ValidationChainOnly_cfg.py

else if ($Mode == "Harvesting")then

cmsRun HarvestingGlobalValidation_Tracking.py
set histogramfile="DQM_V0001_R000000001__"$CMSSW_VERSION"__RelVal__Validation.root"
endif 

root -q -b -l CopySubdir.C\(\"$histogramfile\",\"newfile.root\"\)
mv newfile.root $histogramfile
if ( ($Mode == "Comparison") || ($Mode == "Harvesting") ) then
ln -fs $histogramfile TrackerHitHisto.root
ln -fs $histogramfile stripdigihisto.root
ln -fs $histogramfile pixeldigihisto.root
ln -fs $histogramfile trackingtruthhisto.root
ln -fs $histogramfile validationPlots.root
ln -fs $histogramfile sistriprechitshisto.root
ln -fs $histogramfile pixelrechitshisto.root
ln -fs $histogramfile pixeltrackingrechitshist.root
ln -fs $histogramfile striptrackingrechitshisto.root
cp $histogramfile $NEWREFDIR
endif

#if ( -e Muon.root ) /bin/rm Muon.root


cd ${DATADIR}/Validation/TrackerHits/test 

ln -fs ${DATADIR}/Validation/TrackerConfiguration/test/TrackerHitHisto.root .


ln -fs ${REFDIR}/${refhistogramfile} ../TrackerHitHisto.root

#cp TrackerHitHisto.root $NEWREFDIR/SimHit
if ( ! -e plots ) mkdir plots
root -b -p -q SiTrackerHitsCompareEnergy.C
if ( ! -e plots/muon ) mkdir plots/muon
gzip -f *.eps
mv eloss*.eps.gz plots/muon
mv eloss*.gif plots/muon

root -b -p -q SiTrackerHitsComparePosition.C
if ( ! -e plots/muon ) mkdir plots/muon
gzip -f *.eps
mv pos*.eps.gz plots/muon
mv pos*.gif plots/muon

root -b -p -q SiTrackerHitsCompareToF.C
if ( ! -e plots/muon ) mkdir plots/muon
gzip -f *.eps
mv Tof.eps.gz plots/muon
mv Tof.gif plots/muon

if ($copyWWW == "true") source copyWWWall.csh $WebDir

cd ${DATADIR}/Validation/TrackerDigis/test 

ln -fs ${DATADIR}/Validation/TrackerConfiguration/test/stripdigihisto.root .
ln -fs ${DATADIR}/Validation/TrackerConfiguration/test/pixeldigihisto.root .

ln -fs ${REFDIR}/${refhistogramfile} ../stripdigihisto.root
ln -fs ${REFDIR}/${refhistogramfile} ../pixeldigihisto.root

#cp pixeldigihisto.root $NEWREFDIR/Digis
#cp stripdigihisto.root $NEWREFDIR/Digis

root -b -p -q  SiPixelDigiCompare.C
gzip -f *.eps
if ($copyWWW == "true") source copyWWWPixel.csh $WebDir
root -b -p -q  SiStripDigiCompare.C
gzip -f *.eps
if ($copyWWW == "true") source copyWWWStrip.csh $WebDir

cd ${DATADIR}/Validation/TrackerRecHits/test

ln -fs  ${DATADIR}/Validation/TrackerConfiguration/test/sistriprechitshisto.root .
ln -fs  ${DATADIR}/Validation/TrackerConfiguration/test/pixelrechitshisto.root .

ln -fs  ${REFDIR}/${refhistogramfile} ../sistriprechitshisto.root
ln -fs  ${REFDIR}/${refhistogramfile} ../pixelrechitshisto.root
#cp sistriprechitshisto.root $NEWREFDIR/RecHits/
#cp pixelrechitshisto.root $NEWREFDIR/RecHits/

root -b -p -q SiPixelRecHitsCompare.C
gzip -f *.eps
if ($copyWWW == "true") source copyWWWPixel.csh $WebDir
root -b -p -q SiStripRecHitsCompare.C
gzip -f *.eps
if ($copyWWW == "true") source copyWWWStrip.csh $WebDir

cd ${DATADIR}/Validation/TrackingMCTruth/test
ln -fs ${DATADIR}/Validation/TrackerConfiguration/test/trackingtruthhisto.root .
ln -fs ${REFDIR}/${refhistogramfile} ../trackingtruthhisto.root
#cp trackingtruthhisto.root $NEWREFDIR/TrackingParticles/


root -b -p -q TrackingTruthCompare.C
gzip -f *.eps
if ($copyWWW == "true") source copyWWWTP.csh $WebDir

cd ${DATADIR}/Validation/RecoTrack/test

ln -fs ${DATADIR}/Validation/TrackerConfiguration/test/validationPlots.root .
ln -fs ${DATADIR}/Validation/TrackerConfiguration/test/pixeltrackingrechitshist.root .
ln -fs ${DATADIR}/Validation/TrackerConfiguration/test/striptrackingrechitshisto.root .

ln -fs ${REFDIR}/${refhistogramfile} ../validationPlots.root
ln -fs ${REFDIR}/${refhistogramfile} ../pixeltrackingrechitshist.root
ln -fs ${REFDIR}/${refhistogramfile} ../striptrackingrechitshisto.root

#cp validationPlots.root $NEWREFDIR/Tracks/
#cp pixeltrackingrechitshist.root $NEWREFDIR/TrackingRecHits/
#cp striptrackingrechitshisto.root $NEWREFDIR/TrackingRecHits/

#root -b -p -q TracksCompareChain.C
#gzip -f *.eps
#if ($copyWWW == "true") source copyWWWTracks.csh

root -b -p -q SiPixelRecoCompare.C 
gzip -f *.eps
if ($copyWWW == "true") source copyWWWPixel.csh $WebDir

root -b -p -q SiStripTrackingRecHitsCompare.C 
gzip -f *.eps
if ($copyWWW == "true") source copyWWWStrip.csh $WebDir

#Check on the fly in order to check the correctness of LiteGeometry
#cd ${DATADIR}/CalibTracker/SiStripCommon/test
#cp ${REFDIR}/LiteGeometry/* oldgeometrylite.txt
#cmsRun writeFile.cfg
#cp myfile.txt $NEWREFDIR/LiteGeometry/

#diff myfile.txt oldgeometrylite.txt > ! litegeometryDIFF.txt
#mkdir /afs/cern.ch/cms/performance/tracker/activities/validation/${RefRelease}/LiteGeometry
#cp litegeometryDIFF.txt  /afs/cern.ch/cms/performance/tracker/activities/validation/${RefRelease}/LiteGeometry

