#include "SimG4CMS/Muon/interface/MuonSensitiveDetector.h"
#include "SimG4CMS/Muon/interface/MuonSlaveSD.h"
#include "Geometry/MuonBaseAlgo/interface/MuonSubDetector.h"

#include "Geometry/Vector/interface/LocalPoint.h"
#include "Geometry/Vector/interface/LocalVector.h"

#include "SimG4CMS/Muon/interface/SimHitPrinter.h"
#include "SimDataFormats/TrackingHit/interface/UpdatablePSimHit.h"

#include "SimG4CMS/Muon/interface/MuonG4Numbering.h"
#include "Geometry/MuonBaseAlgo/interface/MuonBaseNumber.h"
#include "Geometry/MuonBaseAlgo/interface/MuonSimHitNumberingScheme.h"

#include "SimG4Core/Geometry/interface/SDCatalog.h"
#include "SimG4Core/Notification/interface/TrackInformation.h"
#include "SimG4Core/Notification/interface/G4TrackToParticleID.h"
#include "SimG4Core/Physics/interface/G4ProcessTypeEnumerator.h"

#include "G4SDManager.hh"
#include "G4VProcess.hh"
#include "G4EventManager.hh"

#include <iostream>

//#define DEBUG
//#define DEBUGST

MuonSensitiveDetector::MuonSensitiveDetector(std::string name, 
					     const DDCompactView & cpv,
					     edm::ParameterSet const & p,
					     const SimTrackManager* manager) 
  : SensitiveTkDetector(name, cpv, p),
    thePV(0), theHit(0), theDetUnitId(0), theTrackID(0), theManager(manager)
{
  edm::ParameterSet m_MuonSD = p.getParameter<edm::ParameterSet>("MuonSD");
  STenergyPersistentCut = m_MuonSD.getParameter<double>("EnergyThresholdForPersistency");//Default 1. GeV
  STallMuonsPersistent = m_MuonSD.getParameter<bool>("AllMuonsPersistent");
  printHits  = m_MuonSD.getParameter<bool>("PrintHits");
  
  //
  // Here simply create 1 MuonSlaveSD for the moment
  //  
  
#ifdef DEBUG 
  std::cout << "create MuonSubDetector "<<name<<std::endl;
#endif
  detector = new MuonSubDetector(name);
#ifdef DEBUG 
  std::cout << "create MuonSlaveSD"<<std::endl;
#endif
  slaveMuon  = new MuonSlaveSD(detector,theManager);
#ifdef DEBUG 
  std::cout << "create MuonSimHitNumberingScheme"<<std::endl;
#endif
  numbering  = new MuonSimHitNumberingScheme(detector);
  g4numbering = new MuonG4Numbering;
  

  //
  // Now attach the right detectors (LogicalVolumes) to me
  //
  std::vector<std::string>  lvNames= SensitiveDetectorCatalog::instance()->logicalNames(name);
  this->Register();
  for (std::vector<std::string>::iterator it = lvNames.begin();  it != lvNames.end(); it++){
#ifdef DEBUG
    std::cout << name << " MuonSensitiveDetector:: attaching SD to LV " << *it << std::endl;
#endif
    this->AssignSD(*it);
  }

  if (printHits) {
    thePrinter = new SimHitPrinter("HitPositionOSCAR.dat");
  }


#ifdef DEBUGST
    std::cout << "  EnergyThresholdForPersistency " << STenergyPersistentCut << " AllMuonsPersistent " <<  STallMuonsPersistent << std::endl;
#endif
    
    theG4ProcessTypeEnumerator = new G4ProcessTypeEnumerator;
    myG4TrackToParticleID = new G4TrackToParticleID;

}


MuonSensitiveDetector::~MuonSensitiveDetector() { 
  delete g4numbering;
  delete numbering;
  delete slaveMuon;
  delete detector;

  delete theG4ProcessTypeEnumerator;
  
  delete myG4TrackToParticleID;
}

void MuonSensitiveDetector::update(const BeginOfEvent * i){
  clearHits();

  //----- Initialize variables to check if two steps belong to same hit
  thePV = 0;
  theDetUnitId = 0;
  theTrackID = 0;

}

void MuonSensitiveDetector::update(const  ::EndOfEvent * ev)
{
  slaveMuon->renumbering(theManager);
}


void MuonSensitiveDetector::clearHits()
{
#ifdef DEBUG 
  std::cout << "MuonSensitiveDetector::clearHits"<<std::endl;
#endif
  slaveMuon->Initialize();
}

bool MuonSensitiveDetector::ProcessHits(G4Step * aStep, G4TouchableHistory * ROhist)
{
#ifdef DEBUG
  std::cout <<" MuonSensitiveDetector::ProcessHits "<<InitialStepPosition(aStep,WorldCoordinates)<<std::endl;
#endif

 // TimeMe t1( theHitTimer, false);

  if (aStep->GetTotalEnergyDeposit()>0.){
    // do not count neutrals that are killed by User Limits MinEKine
    if( aStep->GetTrack()->GetDynamicParticle()->GetCharge() != 0 ){
  
      if (newHit(aStep)) {
	saveHit();
	createHit(aStep);
      } else {
	updateHit(aStep);
      }    
      return true;
    } else {
      storeVolumeAndTrack(aStep);
      return false;
    }
  }
  return false;
}

uint32_t MuonSensitiveDetector::setDetUnitId(G4Step * aStep)
{ 
  //  G4VPhysicalVolume * pv = aStep->GetPreStepPoint()->GetPhysicalVolume();
  MuonBaseNumber num = g4numbering->PhysicalVolumeToBaseNumber(aStep);
  return numbering->baseNumberToUnitNumber(num);
}

Local3DPoint MuonSensitiveDetector::toOrcaUnits(Local3DPoint in){
  return Local3DPoint(in.x()/cm,in.y()/cm,in.z()/cm);
}

Global3DPoint MuonSensitiveDetector::toOrcaUnits(Global3DPoint in){
  return Global3DPoint(in.x()/cm,in.y()/cm,in.z()/cm);
}

void MuonSensitiveDetector::storeVolumeAndTrack(G4Step * aStep)
{
  G4VPhysicalVolume* pv = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4Track * t  = aStep->GetTrack();
  thePV=pv;
  theTrackID=t->GetTrackID();
}

bool MuonSensitiveDetector::newHit(G4Step * aStep){
  
  G4VPhysicalVolume* pv = aStep->GetPreStepPoint()->GetPhysicalVolume();
  G4Track * t  = aStep->GetTrack();
  uint32_t currentUnitId=setDetUnitId(aStep);
  unsigned int currentTrackID=t->GetTrackID();
  //unsigned int currentEventID=G4EventManager::GetEventManager()->GetConstCurrentEvent()->GetEventID();
  bool changed=((pv!=thePV) || 
		(currentUnitId!=theDetUnitId) || 
		(currentTrackID!=theTrackID));
  return changed;
}

void MuonSensitiveDetector::createHit(G4Step * aStep){

  G4Track * theTrack  = aStep->GetTrack(); 

  Local3DPoint theEntryPoint= toOrcaUnits(InitialStepPosition(aStep,LocalCoordinates));  
  Local3DPoint theExitPoint = toOrcaUnits(FinalStepPosition(aStep,LocalCoordinates)); 
  float thePabs             = aStep->GetPreStepPoint()->GetMomentum().mag()/GeV;
  float theTof              = aStep->GetPreStepPoint()->GetGlobalTime()/nanosecond;
  float theEnergyLoss       = aStep->GetTotalEnergyDeposit()/GeV;
  //  short theParticleType     = theTrack->GetDefinition()->GetPDGEncoding();
  short theParticleType     = myG4TrackToParticleID->particleID(theTrack);
  G4ThreeVector gmd  = aStep->GetPreStepPoint()->GetMomentumDirection();
  G4ThreeVector lmd = ((G4TouchableHistory *)(aStep->GetPreStepPoint()->GetTouchable()))->GetHistory()
    ->GetTopTransform().TransformAxis(gmd);
  Local3DPoint lnmd = ConvertToLocal3DPoint(lmd);
  float theThetaAtEntry = lnmd.theta();
  float thePhiAtEntry = lnmd.phi();

  storeVolumeAndTrack( aStep );
  theDetUnitId              = setDetUnitId(aStep);

#ifdef DEBUG
  Global3DPoint theGlobalPos;
  const G4RotationMatrix * theGlobalRot;
#endif
  if (printHits) {   
    Local3DPoint theGlobalHelp = InitialStepPosition(aStep,WorldCoordinates);
    theGlobalEntry = toOrcaUnits(Global3DPoint (theGlobalHelp.x(),theGlobalHelp.y(),theGlobalHelp.z()));
#ifdef DEBUG
    G4StepPoint * preStepPoint = aStep->GetPreStepPoint();
    G4TouchableHistory * theTouchable=(G4TouchableHistory *)
                                      (preStepPoint->GetTouchable());
    theGlobalHelp=ConvertToLocal3DPoint(theTouchable->GetTranslation());
    theGlobalPos = toOrcaUnits(Global3DPoint (theGlobalHelp.x(),theGlobalHelp.y(),theGlobalHelp.z()));
    theGlobalRot = theTouchable->GetRotation();
#endif
  }
  
  
#ifdef DEBUG 
  std::cout << "MuonSensitiveDetector::createHit UpdatablePSimHit"<<std::endl;
#endif
  theHit = new UpdatablePSimHit(theEntryPoint,theExitPoint,thePabs,theTof,
                  theEnergyLoss,theParticleType,theDetUnitId,
                  theTrackID,theThetaAtEntry,thePhiAtEntry,
                  theG4ProcessTypeEnumerator->processId(theTrack->GetCreatorProcess()));

#ifdef DEBUG      
  std::cout <<"=== NEW ==================> ELOSS   = "<<theEnergyLoss<<" "
       <<thePV->GetLogicalVolume()->GetName()<<std::endl;
  const G4VProcess* p = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  const G4VProcess* p2 = aStep->GetPreStepPoint()->GetProcessDefinedStep();
  if (p)
    std::cout <<" POST PROCESS = "<<p->GetProcessName()<<std::endl;
  if (p2)
    std::cout <<" PRE  PROCESS = "<<p2->GetProcessName()<<std::endl;
  std::cout << "newhit theta " << theThetaAtEntry<<std::endl;
  std::cout << "newhit phi   " << thePhiAtEntry<<std::endl;
  std::cout << "newhit pabs  " << thePabs<<std::endl;
  std::cout << "newhit tof   " << theTof<<std::endl;
  std::cout << "newhit track " << theTrackID<<std::endl;
  std::cout << "newhit entry " << theEntryPoint<<std::endl;
  std::cout << "newhit exit  " << theExitPoint<<std::endl;
  std::cout << "newhit eloss " << theEnergyLoss << std::endl;
  std::cout << "newhit detid " << theDetUnitId<<std::endl;
  std::cout << "newhit delta " << (theExitPoint-theEntryPoint)<<std::endl;
  std::cout << "newhit deltu " << (theExitPoint-theEntryPoint).unit();
  std::cout << " " << (theExitPoint-theEntryPoint).mag()<<std::endl;
  std::cout << "newhit glob  " << theGlobalEntry<<std::endl;
  std::cout << "newhit dpos  " << theGlobalPos<<std::endl;
  std::cout << "newhit drot  " << std::endl;
  //  theGlobalRot->print(std::cout);
#endif

  //
  //----- SimTracks: Make it persistent?
  //
  int thePID = theTrack->GetDefinition()->GetPDGEncoding();
#ifdef DEBUGST
  std::cout << " checking simtrack " << thePID << " " << thePabs << " STenergyPersistentCut " << STenergyPersistentCut << std::endl;
#endif

  if( thePabs*GeV > STenergyPersistentCut 
      || ( abs(thePID) == 13 && STallMuonsPersistent ) ){
    TrackInformation* info = getOrCreateTrackInformation(theTrack);
#ifdef DEBUGST
    std::cout <<" track leaving hit in muons made selected for persistency"<<std::endl;
#endif
    info->storeTrack(true);
  }
     
}

void MuonSensitiveDetector::updateHit(G4Step * aStep){
  //  float thePabs             = aStep->GetPreStepPoint()->GetMomentum().mag()/GeV;
  //  Local3DPoint theEntryPoint= InitialStepPosition(aStep,LocalCoordinates);  
  Local3DPoint theExitPoint = toOrcaUnits(FinalStepPosition(aStep,LocalCoordinates)); 
  float theEnergyLoss = aStep->GetTotalEnergyDeposit()/GeV;  

  if( theHit == 0 ){ 
    std::cerr << "!!ERRROR in MuonSensitiveDetector::updateHit. It is called when there is no hit " << std::endl;
  }

  theHit->updateExitPoint(theExitPoint);
  theHit->addEnergyLoss(theEnergyLoss);

#ifdef DEBUG      
  std::cout <<"=== UPDATE ===============> ELOSS   = "<<theEnergyLoss<<" "
       <<thePV->GetLogicalVolume()->GetName()<<std::endl;
  const G4VProcess* p = aStep->GetPostStepPoint()->GetProcessDefinedStep();
  const G4VProcess* p2 = aStep->GetPreStepPoint()->GetProcessDefinedStep();
  if (p)
    std::cout <<" POST PROCESS = "<<p->GetProcessName()<<std::endl;
  if (p2)
    std::cout <<" PRE  PROCESS = "<<p2->GetProcessName()<<std::endl;
  std::cout << "updhit exit  " << theExitPoint<<std::endl;
  std::cout << "updhit eloss " << theHit->energyLoss() <<std::endl;
  std::cout << "updhit detid " << theDetUnitId<<std::endl;
  std::cout << "updhit delta " << (theExitPoint-theHit->entryPoint())<<std::endl;
  std::cout << "updhit deltu " << (theExitPoint-theHit->entryPoint()).unit();
  std::cout << " " << (theExitPoint-theHit->entryPoint()).mag()<<std::endl; 
#endif

}

void MuonSensitiveDetector::saveHit(){

  if (theHit) {
    if (printHits) {
      thePrinter->startNewSimHit(detector->name());
      thePrinter->printId(theHit->detUnitId());
      //      thePrinter->printTrack(theHit->trackId());
      //thePrinter->printPabs(theHit->pabs());
      //thePrinter->printEloss(theHit->energyLoss());
      thePrinter->printLocal(theHit->entryPoint(),theHit->exitPoint());
      thePrinter->printGlobal(theGlobalEntry);
    }
    slaveMuon->processHits(*theHit);
    // seems the hit does not want to be deleted
    // done by the hit collection?
    delete theHit;
    theHit = 0; //set it to 0, because you are checking that is 0
  }

}

TrackInformation* MuonSensitiveDetector::getOrCreateTrackInformation( const G4Track* gTrack)
{
  G4VUserTrackInformation* temp = gTrack->GetUserInformation();
  if (temp == 0){
    std::cout <<" ERROR: no G4VUserTrackInformation available"<<std::endl;
    abort();
  }else{
    TrackInformation* info = dynamic_cast<TrackInformation*>(temp);
    if (info ==0){
      std::cout <<" ERROR: TkSimTrackSelection: the UserInformation does not appear to be a TrackInformation"<<std::endl;
      abort();
    }
    return info;
  }
}

void MuonSensitiveDetector::EndOfEvent(G4HCofThisEvent*)
{
//  TimeMe t("MuonSensitiveDetector::EndOfEvent", false);
 // std::cout << "MuonSensitiveDetector::EndOfEvent saving last hit en event " << std::endl;
  saveHit();
}


void MuonSensitiveDetector::fillHits(edm::PSimHitContainer& c, std::string n){
  //
  // do it once for low, once for High
  //

  if (slaveMuon->name() == n) c=slaveMuon->hits();

}

std::vector<std::string> MuonSensitiveDetector::getNames(){
  std::vector<std::string> temp;
  temp.push_back(slaveMuon->name());
  return temp;
}

