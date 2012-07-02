#!/bin/tcsh

#Check to see if the CMS environment is set up
if ($?CMSSW_BASE != 1) then
    echo "CMS environment not set up"
#    exit
endif

#Check for correct number of arguments
if ($#argv<2) then
    echo "Script needs 2 input variable"
#    exit
endif

set NEW_VERS=$1
set OLD_VERS=$2

# Two bit value with the first corresponding to whether the validation version is centrally
# harvested (1) or not (0) and the second to whether the reference version is harvested. Thus:
# 00: both are privately produced
# 01: reference version is harvested, validation version is private
# 10: validation version is harvested, reference version is private
# 11: both versions are harvested
# Any other value is the same as 0
set harvest=11

#Check if base directory already exists
if (-d ${NEW_VERS}_vs_${OLD_VERS}_RelVal) then
    echo "Directory already exists"
    exit
endif

#Create base directory and top directories
mkdir ${NEW_VERS}_vs_${OLD_VERS}_RelVal
cd ${NEW_VERS}_vs_${OLD_VERS}_RelVal

cp ../html_indices/TopLevelRelVal_TTbar.html index.html


#TTbar
mkdir TTbar
mkdir TTbar/CaloTowers
mkdir TTbar/RecHits
mkdir TTbar/RBX
mkdir TTbar/HcalDigis

cp ../html_indices/RelVal_HcalDigis.html TTbar/HcalDigis/index.html
cat ../html_indices/RelVal_RecHits.html | sed -e s/DATA_SAMPLE/TTbar/ > TTbar/RecHits/index.html
cp ../html_indices/RelVal_CaloTowers.html TTbar/CaloTowers/index.html
cp ../html_indices/RBX.html               TTbar/RBX/index.html

cd ../

#Process Startup TTbar
root -b -q 'RelValMacro.C("'${OLD_VERS}_Startup'","'${NEW_VERS}_Startup'","'HcalRecHitValidationRelVal_TTbar_Startup_${OLD_VERS}.root'","'HcalRecHitValidationRelVal_TTbar_Startup_${NEW_VERS}.root'","InputRelVal_Medium.txt",'${harvest}')'

mv *HcalDigi*.gif   ${NEW_VERS}_vs_${OLD_VERS}_RelVal/TTbar/HcalDigis/
mv *CaloTowers*.gif ${NEW_VERS}_vs_${OLD_VERS}_RelVal/TTbar/CaloTowers/
mv RBX*gif          ${NEW_VERS}_vs_${OLD_VERS}_RelVal/TTbar/RBX/
mv *gif             ${NEW_VERS}_vs_${OLD_VERS}_RelVal/TTbar/RecHits/


exit
