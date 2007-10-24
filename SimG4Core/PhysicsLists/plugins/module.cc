#include "SimG4Core/PhysicsLists/interface/G4Version.h"
#include "SimG4Core/Physics/interface/PhysicsListFactory.h"

#include "DummyPhysics.hh"
#include "FTFPCMS.hh"
#include "LHEPCMS.hh"
#include "QGSCCMS.hh"
#include "QGSPCMS.hh"
#include "QGSPCMS_EMV.hh"
#ifndef G4V9
#include "QGSPCMS_G4v52.hh"
#endif
#include "QGSPCMS_BERT.hh"
#include "QGSPCMS_BERT_HP.hh"

DEFINE_PHYSICSLIST(DummyPhysics);
typedef FTFPCMS FTFP;
DEFINE_PHYSICSLIST(FTFP);
typedef LHEPCMS LHEP;
DEFINE_PHYSICSLIST(LHEP);
typedef QGSCCMS QGSC;
DEFINE_PHYSICSLIST(QGSC);
typedef QGSPCMS QGSP;
DEFINE_PHYSICSLIST(QGSP);
typedef QGSPCMS_EMV QGSP_EMV;
DEFINE_PHYSICSLIST(QGSP_EMV);
#ifndef G4V9
typedef QGSPCMS_G4v52 QGSP_G4v52;
DEFINE_PHYSICSLIST(QGSP_G4v52);
#endif
typedef QGSPCMS_BERT QGSP_BERT;
DEFINE_PHYSICSLIST(QGSP_BERT);
typedef QGSPCMS_BERT_HP QGSP_BERT_HP;
DEFINE_PHYSICSLIST(QGSP_BERT_HP);
