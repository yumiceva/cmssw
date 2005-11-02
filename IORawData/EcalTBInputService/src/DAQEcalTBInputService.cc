/*
 *  $Date: 2005/10/06 17:47:01 $
 *  $Revision: 1.6 $
 *  \author N. Amapane - S. Argiro'
 *  \author G. Franzoni
 */

#include "DAQEcalTBInputService.h"

#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h> 



#include "IORawData/EcalTBInputService/src/EcalTBDaqFileReader.h"

#include <FWCore/EDProduct/interface/Timestamp.h>
#include <FWCore/EDProduct/interface/EventID.h>
#include <FWCore/EDProduct/interface/EDProduct.h>
#include <FWCore/EDProduct/interface/Wrapper.h>
#include <FWCore/Framework/src/TypeID.h> 
#include <FWCore/Framework/interface/InputSourceDescription.h>
#include <FWCore/Framework/interface/EventPrincipal.h>
#include <FWCore/Framework/interface/ProductRegistry.h>

#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <iostream>

using namespace edm;
using namespace std;
using namespace raw;


DAQEcalTBInputService::DAQEcalTBInputService(const ParameterSet& pset, 
			 const InputSourceDescription& desc) : 
  InputSource(desc),
  description_(desc),
  reader_(EcalTBDaqFileReader::instance()), 
  remainingEvents_(pset.getUntrackedParameter<int>("maxEvents", -1))

{    

    ModuleDescription      mdesc;
    mdesc.pid = PS_ID("DAQEcalTBInputService");
    mdesc.moduleName_ = "DAQEcalTBInputService";

    mdesc.moduleLabel_ = "EcalDaqRawData";

#warning version number is hardcoded

    mdesc.versionNumber_ = 1UL;
    mdesc.processName_ = description_.processName_;
    mdesc.pass = description_.pass;

    fedrawdataDescription_.module= mdesc;

    FEDRawDataCollection tmp;

    edm::TypeID fedcoll_type(tmp);
    fedrawdataDescription_.fullClassName_=fedcoll_type.userClassName();
    fedrawdataDescription_.friendlyClassName_=fedcoll_type.friendlyClassName();

    preg_->addProduct(fedrawdataDescription_); 


    std::string filename= pset.getParameter<string>("fileName");
    cout << "[DAQEcalTBInputService] input data file: " << filename << endl;
    bool isBinary= pset.getUntrackedParameter<bool>("isBinary",true);
    if ( isBinary ) {
      cout << "[DAQEcalTBInputService] BINARY input data file" << endl;
    } else {
      cout << "[DAQEcalTBInputService] ASCII input data file" << endl;
    }
    reader_->initialize(filename,isBinary);

}



DAQEcalTBInputService::~DAQEcalTBInputService(){
  
  //  clear();
}


// void DAQEcalTBInputService::clear() {
//   for(map<int, DaqFEDRawData *>::iterator it = daqevdata_.begin();
//       it != daqevdata_.end(); ++it) {
//     delete (*it).second;
//   }
//   daqevdata_.clear();
// }


auto_ptr<EventPrincipal> DAQEcalTBInputService::read() {

  auto_ptr<EventPrincipal> result(0);

 
  EventID id;
  Timestamp tstamp;

  FEDRawDataCollection* bare_product = new FEDRawDataCollection;


  
  if (remainingEvents_-- == 0 || !reader_->fillDaqEventData(id, *bare_product)   )
    return result;

  cout << " DAQEcalTBInputService::read run " << id.run() << " ev " << id.event() << endl;

  result = auto_ptr<EventPrincipal>(new EventPrincipal(id, tstamp, *preg_));
  
  edm::Wrapper<FEDRawDataCollection> *wrapped_product = 
    new edm::Wrapper<FEDRawDataCollection> (*bare_product);

  auto_ptr<EDProduct>  prod(wrapped_product);
  
  auto_ptr<Provenance> prov(new Provenance(fedrawdataDescription_));
  
   
  
  result->put(prod, prov);


  return result;
}


#include "PluginManager/ModuleDef.h"
#include "FWCore/Framework/interface/InputSourceMacros.h"

DEFINE_FWK_INPUT_SOURCE(DAQEcalTBInputService)
