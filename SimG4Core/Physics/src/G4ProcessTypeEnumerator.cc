#include "SimG4Core/Physics/interface/G4ProcessTypeEnumerator.h"

#include "SimG4Core/Physics/interface/ProcessTypeEnumerator.h"

#include "G4VProcess.hh"

#include <iostream>

//#define DEBUG
//#define G4v7

G4ProcessTypeEnumerator::G4ProcessTypeEnumerator(){
  mapProcesses["Undefined"] = "Undefined";
  mapProcesses["Unknown"] = "Unknown";
  //
  mapProcesses["Primary"] = "Primary";
  // nuclear stuff
  mapProcesses["AntiNeutronInelastic"] = "Hadronic";
  mapProcesses["PositronNuclear"] = "Hadronic";
  mapProcesses["ElectroNuclear"] = "Hadronic";
  mapProcesses["AntiProtonAnnihilationAtRest"] = "Hadronic";
  mapProcesses["AntiProtonInelastic"] = "Hadronic";
  mapProcesses["ProtonInelastic"] = "Hadronic";
  mapProcesses["PhotonInelastic"] = "Hadronic";
  mapProcesses["DeuteronInelastic"] = "Hadronic";
  mapProcesses["KaonMinusAbsorption"] = "Hadronic";
  mapProcesses["KaonMinusInelastic"] = "Hadronic";
  mapProcesses["KaonPlusInelastic"] = "Hadronic";
  mapProcesses["KaonZeroLInelastic"] = "Hadronic";
  mapProcesses["KaonZeroSInelastic"] = "Hadronic";
  mapProcesses["LCapture"] = "Hadronic";
  mapProcesses["LElastic"] = "Hadronic";
  mapProcesses["LambdaInelastic"] = "Hadronic";
  mapProcesses["NeutronInelastic"] = "Hadronic";
  mapProcesses["PionMinusAbsorptionAtRest"] = "Hadronic";
  mapProcesses["PionMinusInelastic"] = "Hadronic";
  mapProcesses["PionPlusInelastic"] = "Hadronic";
  mapProcesses["SigmaMinusInelastic"] = "Hadronic";
  mapProcesses["AntiSigmaMinusInelastic"] = "Hadronic";
  mapProcesses["AntiSigmaPlusInelastic"] = "Hadronic";
  mapProcesses["AntiLambdaInelastic"] = "Hadronic";
  mapProcesses["TritonInelastic"] = "Hadronic"; 
  mapProcesses["XiMinusInelastic"] = "Hadronic";
  mapProcesses["XiPlusInelastic"] = "Hadronic";
  mapProcesses["AntiXiZeroInelastic"] = "Hadronic";
  mapProcesses["SigmaPlusInelastic"] = "Hadronic";
  mapProcesses["XiZeroInelastic"] = "Hadronic";
  mapProcesses["AntiXiMinusInelastic"] = "Hadronic";

  // ionizations
  mapProcesses["eIoni"] = "EIoni";
  mapProcesses["hIoni"] = "HIoni";
  mapProcesses["ionIoni"] = "HIoni";
#ifdef G4v7
  mapProcesses["muIoni"] = "MuIoni";
#else
  mapProcesses["MuIoni"] = "MuIoni";
#endif
  // Annihilation
  mapProcesses["annihil"] = "Annihilation";
  // MuBrem
#ifdef G4v7
  mapProcesses["muBrems"] = "MuBrem";
#else
  mapProcesses["MuBrems"] = "MuBrem";
#endif
  // MuNucl
  mapProcesses["MuonMinusCaptureAtRest"] = "MuNucl";
  mapProcesses["MuonPlusCaptureAtRest"] = "MuNucl";
  // Conversions
  mapProcesses["conv"] = "Conversions";
  // Brems
  mapProcesses["eBrem"] = "EBrem";
  // Decay
  mapProcesses["Decay"] = "Decay";
  // PairProd
#ifdef G4v7
  mapProcesses["muPairProd"] = "MuPairProd";
#else
  mapProcesses["MuPairProd"] = "MuPairProd";
#endif
  // Photon
  mapProcesses["phot"] = "Photon";
  // Sync
  mapProcesses["SynchrotronRadiation"] = "SynchrotronRadiation";
  // Conversion
  mapProcesses["conv"] = "Conversions";
  // Compton
  mapProcesses["compt"] = "Compton";
  //
  buildReverseMap();
  //
  //
  theProcessTypeEnumerator = new ProcessTypeEnumerator;
}

G4ProcessTypeEnumerator::~G4ProcessTypeEnumerator(){
  delete theProcessTypeEnumerator;
  mapProcesses.clear();
  reverseMapProcesses.clear();
}


unsigned int G4ProcessTypeEnumerator::processId(const G4VProcess* process){
  if (process == 0) {
    //
    // it is primary!
    //
    std::string temp = "Primary";
#ifdef DEBUG
    std::cout <<" G4ProcessTypeEnumerator : Primary process, returning "<<
      theProcessTypeEnumerator->processId(temp)<<std::endl;
#endif
    return theProcessTypeEnumerator->processId(temp);
  }else{
    std::string temp = process->GetProcessName();
#ifdef DEBUG
    std::cout <<" G4ProcessTypeEnumerator : G4Process "<<temp<<" mapped to "<<
      processCMSName(temp)<<
      "; returning "<<
      theProcessTypeEnumerator->processId(processCMSName(temp))<<std::endl;
#endif
    return theProcessTypeEnumerator->processId(processCMSName(temp));
  }
}

std::string G4ProcessTypeEnumerator::processCMSName(std::string in){
  if (mapProcesses[in] == ""){
    //    throw MantisException("G4ProcessTypeEnumerator: unknown G4 process "+in);
    std::cout <<" NOT FOUND G4ProcessTypeEnumerator: "<<in<<std::endl;
    return "Unknown";
  }
  return mapProcesses[in];
}
std::vector<std::string> G4ProcessTypeEnumerator::processG4Name(std::string in){
  if (reverseMapProcesses[in].size() ==0)
    //    throw MantisException("G4ProcessTypeEnumerator: unknown CMS process "+in);
    ;
  return reverseMapProcesses[in];
}

unsigned int G4ProcessTypeEnumerator::numberOfKnownCMSProcesses(){
  return reverseMapProcesses.size();
}
unsigned int G4ProcessTypeEnumerator::numberOfKnownG4Processes(){
  return mapProcesses.size();
}

void G4ProcessTypeEnumerator::buildReverseMap(){
  for (MapType::const_iterator it = mapProcesses.begin();  it != mapProcesses.end(); it++)
    (reverseMapProcesses[(*it).second]).push_back((*it).first);
}
