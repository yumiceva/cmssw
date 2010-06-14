// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

// Conditions database
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "DBWriter.h"
#include "CondFormats/RecoMuonObjects/interface/MuScleFitDBobject.h"

using namespace std;
using namespace edm;

DBWriter::DBWriter(const edm::ParameterSet& ps)
{
  // This string is one of: scale, resolution, background.
  string type( ps.getUntrackedParameter<string>("Type") );
  // Create the corrector and set the parameters
  if( type == "scale" ) corrector_.reset(new MomentumScaleCorrector( ps.getUntrackedParameter<string>("CorrectionsIdentifier") ) );
  else if( type == "resolution" ) corrector_.reset(new ResolutionFunction( ps.getUntrackedParameter<string>("CorrectionsIdentifier") ) );
  else if( type == "background" ) corrector_.reset(new BackgroundFunction( ps.getUntrackedParameter<string>("CorrectionsIdentifier") ) );
  else {
    cout << "Error: unrecognized type. Use one of those: 'scale', 'resolution', 'background'" << endl;
    exit(1);
  }
}

DBWriter::~DBWriter()
{
   // do anything here that needs to be done at desctruction time
   // (e.g. close files, deallocate resources etc.)

}

// ------------ method called to for each event  ------------
void
DBWriter::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
  MuScleFitDBobject * dbObject = new MuScleFitDBobject;

  dbObject->identifiers = corrector_->identifiers();
  dbObject->parameters = corrector_->parameters();

//   if( dbObject->identifiers.size() != dbObject->parameters.size() ) {
//     cout << "Error: size of parameters("<<dbObject->parameters.size()<<") and identifiers("<<dbObject->identifiers.size()<<") don't match" << endl;
//     exit(1);
//   }

//   vector<vector<double> >::const_iterator parVec = dbObject->parameters.begin();
//   vector<int>::const_iterator id = dbObject->identifiers.begin();
//   for( ; id != dbObject->identifiers.end(); ++id, ++parVec ) {
//     cout << "id = " << *id << endl;
//     vector<double>::const_iterator par = parVec->begin();
//     int i=0;
//     for( ; par != parVec->end(); ++par, ++i ) {
//       cout << "par["<<i<<"] = " << *par << endl;
//     }
//   }

  // Save the parameters to the db.
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if( mydbservice.isAvailable() ){
    if( mydbservice->isNewTagRequest("MuScleFitDBobjectRcd") ){
      mydbservice->createNewIOV<MuScleFitDBobject>(dbObject,mydbservice->beginOfTime(),mydbservice->endOfTime(),"MuScleFitDBobjectRcd");
    } else {
      mydbservice->appendSinceTime<MuScleFitDBobject>(dbObject,mydbservice->currentTime(),"MuScleFitDBobjectRcd");      
    }
  } else {
    edm::LogError("DBWriter")<<"Service is unavailable"<<std::endl;
  }

}

//define this as a plug-in
DEFINE_FWK_MODULE(DBWriter);
