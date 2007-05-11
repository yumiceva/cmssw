// -*- C++ -*-
//
// Package:    HcalTopologyIdealEP
// Class:      HcalTopologyIdealEP
// 
/**\class HcalTopologyIdealEP HcalTopologyIdealEP.h tmp/HcalTopologyIdealEP/interface/HcalTopologyIdealEP.h

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Jeremiah Mans
//         Created:  Mon Oct  3 11:35:27 CDT 2005
// $Id: HcalTopologyIdealEP.cc,v 1.1.2.1 2007/05/08 20:39:50 mansj Exp $
//
//

#include "Geometry/HcalEventSetup/src/HcalTopologyIdealEP.h"
#include "Geometry/CaloTopology/interface/HcalTopologyRestrictionParser.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
HcalTopologyIdealEP::HcalTopologyIdealEP(const edm::ParameterSet& conf) :
  m_restrictions(conf.getUntrackedParameter<std::string>("Exclude","")),
  m_h2mode(conf.getUntrackedParameter<bool>("H2Mode",false))
{
   //the following line is needed to tell the framework what
   // data is being produced
   setWhatProduced(this);
}


HcalTopologyIdealEP::~HcalTopologyIdealEP()
{ 
}


//
// member functions
//

// ------------ method called to produce the data  ------------
HcalTopologyIdealEP::ReturnType
HcalTopologyIdealEP::produce(const IdealGeometryRecord& iRecord)
{
   using namespace edm::es;
   if (m_h2mode) edm::LogInfo("HCAL") << "Using H2 Topology";

   std::auto_ptr<HcalTopology> myTopo(new HcalTopology(m_h2mode));

   HcalTopologyRestrictionParser parser(*myTopo);
   if (!m_restrictions.empty()) {
     std::string error=parser.parse(m_restrictions);
     if (!error.empty()) {
       throw cms::Exception("Parse Error","Parse error on Exclude "+error);
     }
   }

   return myTopo ;
}


