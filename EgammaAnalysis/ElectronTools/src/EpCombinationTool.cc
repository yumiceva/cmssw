#include "EgammaAnalysis/ElectronTools/interface/EpCombinationTool.h"
#include "CondFormats/EgammaObjects/interface/GBRForest.h"
#include <TFile.h>
#include <TSystem.h>
#include <math.h>
#include <vector>
#include <iostream>

using namespace std;


/*****************************************************************/
EpCombinationTool::EpCombinationTool():
    m_forest(NULL)
/*****************************************************************/
{
}



/*****************************************************************/
EpCombinationTool::~EpCombinationTool()
/*****************************************************************/
{
    if(m_forest) delete m_forest;
}


/*****************************************************************/
bool EpCombinationTool::init(const string& regressionFileName, const string& bdtName)
/*****************************************************************/
{
    TFile* regressionFile = TFile::Open(regressionFileName.c_str());
    if(!regressionFile)
    {
        cout<<"ERROR: Cannot open regression file "<<regressionFileName<<"\n";
        return false;
    }
    m_forest = (GBRForest*) regressionFile->Get(bdtName.c_str());
    //regressionFile->GetObject(bdtName.c_str(), m_forest); 
    if(!m_forest)
    {
        cout<<"ERROR: Cannot find forest "<<bdtName<<" in "<<regressionFileName<<"\n";
        regressionFile->Close();
        return false;
    }
    regressionFile->Close();
    return true;
}



/*****************************************************************/
void EpCombinationTool::combine(SimpleElectron & mySimpleElectron)
/*****************************************************************/
{
    if(!m_forest)
    {
        cout<<"ERROR: The combination tool is not initialized\n";
        return;
    }

    float energy = mySimpleElectron.getRegEnergy();
    float energyError = mySimpleElectron.getRegEnergyError();
    float momentum = mySimpleElectron.getTrackerMomentum();
    float momentumError = mySimpleElectron.getTrackerMomentumError();
    int electronClass = mySimpleElectron.getElClass();
    bool isEcalDriven = mySimpleElectron.isEcalDriven();
    bool isTrackerDriven =  mySimpleElectron.isTrackerDriven();
    bool isEB = mySimpleElectron.isEB();

    // compute relative errors and ratio of errors
    float energyRelError = energyError / energy;
    float momentumRelError = momentumError / momentum;
    float errorRatio = energyRelError / momentumRelError;

    // calculate E/p and corresponding error
    float eOverP = energy / momentum;
    float eOverPerror = sqrt(
            (energyError/momentum)*(energyError/momentum) +
            (energy*momentumError/momentum/momentum)*
            (energy*momentumError/momentum/momentum));

    // fill input variables
    float* regressionInputs = new float[11];
    regressionInputs[0]  = energy;
    regressionInputs[1]  = energyRelError;
    regressionInputs[2]  = momentum;
    regressionInputs[3]  = momentumRelError;
    regressionInputs[4]  = errorRatio;
    regressionInputs[5]  = eOverP;
    regressionInputs[6]  = eOverPerror;
    regressionInputs[7]  = static_cast<float>(isEcalDriven);
    regressionInputs[8]  = static_cast<float>(isTrackerDriven);
    regressionInputs[9]  = static_cast<float>(electronClass);
    regressionInputs[10] = static_cast<float>(isEB);
    
    // retrieve combination weight
    float weight = 0.;
    if(eOverP>0.025) // protection against crazy track measurement
    {
        weight = m_forest->GetResponse(regressionInputs);
        if(weight>1.) weight = 1.;
        else if(weight<0.) weight = 0.;
    }

    float combinedMomentum = weight*momentum + (1.-weight)*energy;
    float combinedMomentumError = weight*sqrt(momentumError*momentumError + (1.-weight)*(1.-weight)*energyError*energyError/(weight*weight));

    mySimpleElectron.setCombinedMomentum(combinedMomentum);
    mySimpleElectron.setCombinedMomentumError(combinedMomentumError);

    delete[] regressionInputs;
}
