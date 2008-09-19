// -*- C++ -*-
//
// Package:    L1TriggerKeyDummyProd
// Class:      L1TriggerKeyDummyProd
// 
/**\class L1TriggerKeyDummyProd L1TriggerKeyDummyProd.h CondTools/L1TriggerKeyDummyProd/src/L1TriggerKeyDummyProd.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Werner Man-Li Sun
//         Created:  Sat Mar  1 01:08:46 CET 2008
// $Id: L1TriggerKeyDummyProd.cc,v 1.1 2008/03/03 21:52:18 wsun Exp $
//
//


// system include files

// user include files
#include "CondTools/L1Trigger/plugins/L1TriggerKeyDummyProd.h"

//
// class declaration
//

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
L1TriggerKeyDummyProd::L1TriggerKeyDummyProd(const edm::ParameterSet& iConfig)
{
   //the following line is needed to tell the framework what
   // data is being produced
  bool subsystemKeysOnly =
    iConfig.getParameter< bool >( "subsystemKeysOnly" ) ;

  std::string label = subsystemKeysOnly ? "SubsystemKeysOnly" : "" ;
  setWhatProduced(this, label);

   //now do what ever other initialization is needed
   m_key.setTSCKey( iConfig.getParameter< std::string >( "tscKey" ) ) ;
   m_key.setSubsystemKey( L1TriggerKey::kCSCTF,
			  iConfig.getParameter< std::string >( "csctfKey" ) ) ;
   m_key.setSubsystemKey( L1TriggerKey::kDTTF,
			  iConfig.getParameter< std::string >( "dttfKey" ) ) ;
   m_key.setSubsystemKey( L1TriggerKey::kRPC,
			  iConfig.getParameter< std::string >( "rpcKey" ) ) ;
   m_key.setSubsystemKey( L1TriggerKey::kGMT,
			  iConfig.getParameter< std::string >( "gmtKey" ) ) ;
   m_key.setSubsystemKey( L1TriggerKey::kRCT,
			  iConfig.getParameter< std::string >( "rctKey" ) ) ;
   m_key.setSubsystemKey( L1TriggerKey::kGCT,
			  iConfig.getParameter< std::string >( "gctKey" ) ) ;
   m_key.setSubsystemKey( L1TriggerKey::kGT,
			  iConfig.getParameter< std::string >( "gtKey" ) ) ;
   m_key.setSubsystemKey( L1TriggerKey::kTSP0,
			  iConfig.getParameter< std::string >( "tsp0Key" ) ) ;

   if( !subsystemKeysOnly )
     {
       typedef std::vector< edm::ParameterSet > ObjectKeys;
       ObjectKeys keys = iConfig.getParameter< ObjectKeys >( "objectKeys" ) ;

       for( ObjectKeys::const_iterator it = keys.begin ();
	    it != keys.end() ;
	    ++it )
	 {
	   m_key.add( it->getParameter< std::string >( "record" ),
		      it->getParameter< std::string >( "type" ),
		      it->getParameter< std::string >( "key" ) ) ;
	 }
     }
}


L1TriggerKeyDummyProd::~L1TriggerKeyDummyProd()
{
 
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}


//
// member functions
//

// ------------ method called to produce the data  ------------
L1TriggerKeyDummyProd::ReturnType
L1TriggerKeyDummyProd::produce(const L1TriggerKeyRcd& iRecord)
{
   using namespace edm::es;
   boost::shared_ptr<L1TriggerKey> pL1TriggerKey ;

   pL1TriggerKey = boost::shared_ptr< L1TriggerKey >(
      new L1TriggerKey( m_key ) ) ;

   return pL1TriggerKey ;
}

//define this as a plug-in
//DEFINE_FWK_EVENTSETUP_MODULE(L1TriggerKeyDummyProd);
