#include "SimG4Core/Application/interface/RunManager.h"
#include "SimG4Core/Application/interface/PrimaryTransformer.h"
#include "SimG4Core/Application/interface/RunAction.h"
#include "SimG4Core/Application/interface/EventAction.h"
#include "SimG4Core/Application/interface/StackingAction.h"
#include "SimG4Core/Application/interface/TrackingAction.h"
#include "SimG4Core/Application/interface/SteppingAction.h"
#include "SimG4Core/Application/interface/G4SimEvent.h"

#include "SimG4Core/Geometry/interface/DDDWorld.h"
#include "SimG4Core/SensitiveDetector/interface/AttachSD.h"
#include "SimG4Core/Generators/interface/Generator.h"
#include "SimG4Core/Physics/interface/PhysicsListFactory.h"

#include "SimG4Core/Notification/interface/SimG4Exception.h"

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/Records/interface/IdealGeometryRecord.h"

#include "G4StateManager.hh"
#include "G4ApplicationState.hh"
#include "G4RunManagerKernel.hh"
#include "G4UImanager.hh"

#include "G4EventManager.hh"
#include "G4Run.hh"
#include "G4Event.hh"

#include "CLHEP/Random/JamesRandom.h"
 
#include "Randomize.hh"

#include <iostream>
#include <memory>
#include <strstream>
#include <fstream>

RunManager * RunManager::me = 0;
RunManager * RunManager::init(edm::ParameterSet const & p)
{
    if (me != 0) abort();
    me = new RunManager(p);
    return me;
}

RunManager * RunManager::instance() 
{
    if (me==0) abort();
    return me;
}

RunManager::RunManager(edm::ParameterSet const & p) 
    : m_context (new seal::Context),
      m_generator(0), m_primaryTransformer(0), m_engine(0), m_managerInitialized(false), 
      m_geometryInitialized(true), m_physicsInitialized(true),
      m_runInitialized(false), m_runTerminated(false), m_runAborted(false),
      m_currentRun(0), m_currentEvent(0), m_simEvent(0), 
      m_rndmStore(p.getParameter<bool>("StoreRndmSeeds")),
      m_rndmRestore(p.getParameter<bool>("RestoreRndmSeeds")),
      m_PhysicsTablesDir(p.getParameter<std::string>("PhysicsTablesDirectory")),
      m_StorePhysicsTables(p.getParameter<bool>("StorePhysicsTables")),
      m_RestorePhysicsTables(p.getParameter<bool>("RestorePhysicsTables")),
      m_EvtMgrVerbosity(p.getParameter<int>("G4EventManagerVerbosity")),
      m_Override(p.getParameter<bool>("OverrideUserStackingAction")),
      m_RunNumber(p.getParameter<int>("RunNumber")),
      m_pGenerator(p.getParameter<edm::ParameterSet>("Generator")),
      m_pPhysics(p.getParameter<edm::ParameterSet>("Physics")),
      m_pRunAction(p.getParameter<edm::ParameterSet>("RunAction")),      
      m_pEventAction(p.getParameter<edm::ParameterSet>("EventAction")),
      m_pTrackingAction(p.getParameter<edm::ParameterSet>("TrackingAction")),
      m_pSteppingAction(p.getParameter<edm::ParameterSet>("SteppingAction")),
      m_p(p)
{    
    m_kernel = G4RunManagerKernel::GetRunManagerKernel();
    if (m_kernel==0) m_kernel = new G4RunManagerKernel();
    m_engine= dynamic_cast<HepJamesRandom*>(HepRandom::getTheEngine());
    std::cout << " Run Manager constructed " << std::endl;
}

RunManager::~RunManager() 
{ 
    if (worldObserver!=0) delete worldObserver;
    if (worldDispatcher!=0) delete worldDispatcher;
    if (m_kernel!=0) delete m_kernel; 
}

void RunManager::initG4(const edm::EventSetup & es)
{
    if (m_managerInitialized) return;

    // DDDWorld: get the DDCV from the ES and use it to build the World
    edm::ESHandle<DDCompactView> pDD;
    es.get<IdealGeometryRecord>().get(pDD);
   
    const DDDWorld * world = new DDDWorld(&(*pDD));
    
    worldObserver = new DDDWorldObserver<DDDWorld >(); 
    worldDispatcher = new Dispatcher<DDDWorld >(world);
    m_attach = new AttachSD;
    std::vector<SensitiveDetector*> sensDets = m_attach->create(*world,(*pDD),m_p);
    m_sensTkDets.clear();
    m_sensCaloDets.clear();

    for (std::vector<SensitiveDetector*>::iterator it = sensDets.begin();
	 it != sensDets.end(); it++)
    {
	if (dynamic_cast<SensitiveTkDetector*>(*it))
	    m_sensTkDets.push_back(dynamic_cast<SensitiveTkDetector*>(*it));
	if (dynamic_cast<SensitiveCaloDetector*>(*it))
	    m_sensCaloDets.push_back(dynamic_cast<SensitiveCaloDetector*>(*it));
    }
    std::cout << " Sensitive Detector building finished; found " << m_sensTkDets.size()
	      << " Tk type Producers, and " << m_sensCaloDets.size() 
	      << " Calo type producers " << std::endl;

    m_generator = new Generator(m_pGenerator);
    m_primaryTransformer = new PrimaryTransformer();
    
    seal::Handle<PhysicsList> physics = PhysicsListFactory::get()->create
	(m_pPhysics.getParameter<std::string> ("type"),m_context.get(),m_pPhysics);
    if (physics.get()==0) throw SimG4Exception("Physics list construction failed!");
    m_kernel->SetPhysics(physics.get());
    m_kernel->InitializePhysics();

    physics->ResetStoredInAscii();
    std::string tableDir = m_PhysicsTablesDir;
    if (m_RestorePhysicsTables) physics->SetPhysicsTableRetrieved(tableDir);
 
    if (m_kernel->RunInitialization()) m_managerInitialized = true;
    else throw SimG4Exception("G4RunManagerKernel initialization failed!");
     
    if (m_StorePhysicsTables)
    {
	std::ostrstream dir;
	dir << tableDir << '\0';
	std::string cmd = std::string("/control/shell mkdir -p ")+tableDir;
	if (!std::ifstream(dir.str(), std::ios::in))
	    G4UImanager::GetUIpointer()->ApplyCommand(cmd);
	physics->StorePhysicsTable(tableDir);
    }
 
    initializeUserActions();
    initializeRun();
}

void RunManager::produce(const edm::EventSetup & es)
{
    static int i = 0;
    m_currentEvent = generateEvent(i);
    i++;
    
    m_simEvent = new G4SimEvent;
    m_simEvent->hepEvent(m_generator->genEvent());
    m_simEvent->weight(m_generator->eventWeight());
    m_simEvent->collisionPoint(HepLorentzVector(m_generator->genVertex().vect()/centimeter,
                                                m_generator->genVertex().t()/second));
 
    if (m_currentEvent->GetNumberOfPrimaryVertex()==0)
    {
       std::cout << " RunManager::produce event " << i
            << " with no G4PrimaryVertices " << std::endl;
       std::cout << " Aborting run " << std::endl;
       abortRun(false);
    }
    else
        m_kernel->GetEventManager()->ProcessOneEvent(m_currentEvent);

    std::cout << " saved : Event  " << i << " of weight " << m_simEvent->weight()
         << " with " << m_simEvent->nTracks() << " tracks and " << m_simEvent->nVertices()
         << " vertices, generated by " << m_simEvent->nGenParts() << " particles " << std::endl;

}
 
G4Event * RunManager::generateEvent(int i)
{                       
    if (m_currentEvent!=0) delete m_currentEvent;
    m_currentEvent = 0;
    if (m_simEvent!=0) delete m_simEvent;
    m_simEvent = 0;
    G4Event * e = new G4Event(i);
    const HepMC::GenEvent * g = m_generator->generateEvent();
    m_generator->HepMC2G4(g,e);
    return e;
}

void RunManager::abortEvent()
{
    m_currentEvent->SetEventAborted();
    m_kernel->GetEventManager()->AbortCurrentEvent();
}

void RunManager::initializeUserActions()
{
    m_userRunAction = new RunAction(m_pRunAction);
    G4EventManager * eventManager = m_kernel->GetEventManager();
    eventManager->SetVerboseLevel(m_EvtMgrVerbosity);
    if (m_generator!=0)
    {
        EventAction * userEventAction = new EventAction(m_pEventAction);
        eventManager->SetUserAction(userEventAction);
        eventManager->SetUserAction(new TrackingAction(userEventAction,m_pTrackingAction));
        eventManager->SetUserAction(new SteppingAction(m_pSteppingAction));
        if (m_Override)
        {
            std::cout << " RunManager: user StackingAction overridden " 
		      << std::endl;
            eventManager->SetUserAction(new StackingAction);
        }
    }
    else std::cout << " WARNING: No generator; initialized only RunAction!" 
		   << std::endl;
}

void RunManager::initializeRun()
{
    m_runInitialized = false;
    if (m_currentRun==0) m_currentRun = new G4Run();
    m_currentRun->SetRunID(m_RunNumber);
    G4StateManager::GetStateManager()->SetNewState(G4State_GeomClosed);
    if (m_userRunAction!=0) m_userRunAction->BeginOfRunAction(m_currentRun);
    m_runAborted = false;
    if (m_rndmStore) runRNDMstore(m_RunNumber);
    if (m_rndmRestore) runRNDMrestore(m_RunNumber);
    m_runInitialized = true;
}
 
void RunManager::terminateRun()
{
    m_runTerminated = false;
    if (m_userRunAction!=0)
    {
        m_userRunAction->EndOfRunAction(m_currentRun);
        delete m_userRunAction; m_userRunAction = 0;
    }
    if (m_currentRun!=0) { delete m_currentRun; m_currentRun = 0; }
    if (m_kernel!=0) m_kernel->RunTermination();
    m_runInitialized = false;
    m_runTerminated = true;
}

void RunManager::abortRun(bool softAbort)
{
    m_runAborted = false;
    if (!softAbort) abortEvent();
    if (m_currentRun!=0) { delete m_currentRun; m_currentRun = 0; }
    m_runInitialized = false;
    m_runAborted = true;
}

void RunManager::runRNDMstore(int run)
{
    std::ostrstream dir;
    dir << "Run" << run << '\0';
    std::string cmd = std::string("/control/shell mkdir -p ")+dir.str();
    G4UImanager::GetUIpointer()->ApplyCommand(cmd);
    std::ostrstream os;
    os << "Run" << run << "/run" << run << ".rndm" << '\0';
    m_engine->saveStatus(os.str());
    std::cout << "Random number status saved in: " << os.str() << std::endl;
    m_engine->showStatus();
}
 
void RunManager::runRNDMrestore(int run)
{
    std::ostrstream os;
    os << "Run" << run << "/run" << run << ".rndm" << '\0';
    if (!std::ifstream(os.str(), std::ios::in))
    {
        std::cout << " rndm directory does not exist for run " << run << std::endl;
        return;
    }
    m_engine->restoreStatus(os.str());
    std::cout << "Random number status restored from: " << os.str() << std::endl;
    m_engine->showStatus();
}
 
void RunManager::eventRNDMstore(int run, int event)
{
    std::ostrstream os;
    os << "Run" << run << "/evt" << event << ".rndm" << '\0';
    m_engine->saveStatus(os.str());
    if (m_EvtMgrVerbosity>2)
    {
        std::cout << " random numbers saved in: " << os.str() << std::endl;
        m_engine->showStatus();
    }
}

void RunManager::eventRNDMrestore(int run, int event)
{
    std::ostrstream os;
    os << "Run" << run << "/evt" << event << ".rndm" << '\0';
    if (!std::ifstream(os.str(), std::ios::in))
    {
        std::cout << " rndm file does not exist for event " << event << std::endl;
        return;
    }
    m_engine->restoreStatus(os.str());
    if (m_EvtMgrVerbosity>2)
    {
        std::cout << "Random number status restored from: " << os.str() <<std:: endl;
        m_engine->showStatus();
    }
}
 
