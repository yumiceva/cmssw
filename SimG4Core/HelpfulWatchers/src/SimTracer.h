#ifndef HelpfulWatchers_SimTracer_h
#define HelpfulWatchers_SimTracer_h
// -*- C++ -*-
//
// Package:     HelpfulWatchers
// Class  :     SimTracer
// 
/**\class SimTracer SimTracer.h SimG4Core/HelpfulWatchers/interface/SimTracer.h

 Description: Prints a message for each Oscar signal

 Usage:
    <usage>

*/
//
// Original Author:  
//         Created:  Tue Nov 22 16:41:33 EST 2005
// $Id$
//

// system include files
#include <iostream>

// user include files
#include "SimG4Core/Notification/interface/Observer.h"
#include "SimG4Core/Watcher/interface/SimWatcher.h"

// forward declarations
class DDDWorld;
class BeginOfJob;
class BeginOfRun;
class BeginOfEvent;
class BeginOfTrack;
class G4Step;

class EndOfRun;
class EndOfEvent;
class EndOfTrack;

#define OBSERVES(type) public Observer<const type*>
#define UPDATE(type) void update(const type*) { std::cout <<"++ signal " #type<<std::endl; }
class SimTracer : public SimWatcher, 
OBSERVES(DDDWorld),
OBSERVES(BeginOfJob),
OBSERVES(BeginOfRun),
OBSERVES(BeginOfEvent),
OBSERVES(BeginOfTrack),
OBSERVES(G4Step),
OBSERVES(EndOfRun),
OBSERVES(EndOfEvent),
OBSERVES(EndOfTrack)
{

   public:
     SimTracer(const edm::ParameterSet&) {}
     //virtual ~SimTracer();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------
UPDATE(DDDWorld)
UPDATE(BeginOfJob)
UPDATE(BeginOfRun)
UPDATE(BeginOfEvent)
UPDATE(BeginOfTrack)
UPDATE(G4Step)
UPDATE(EndOfRun)
UPDATE(EndOfEvent)
UPDATE(EndOfTrack)

   private:
     //SimTracer(const SimTracer&); // stop default

     //const SimTracer& operator=(const SimTracer&); // stop default

     // ---------- member data --------------------------------

};


#endif
