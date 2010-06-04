#include "DTRecoIdealDBLoader.h"

#include <Geometry/DTGeometryBuilder/src/DTGeometryParsFromDD.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "CondFormats/RecoGeometryObjects/interface/RecoIdealGeometry.h"
//#include "CondFormats/RecoGeometryObjects/interface/CSCRecoDigiParameters.h"
// #include "CondFormats/DataRecord/interface/RecoIdealGeometryRcd.h"
// #include "CondFormats/DataRecord/interface/CSCRecoDigiParametersRcd.h"
#include "Geometry/Records/interface/RecoIdealGeometryRcd.h"
//#include "Geometry/Records/interface/CSCRecoDigiParametersRcd.h"
#include <Geometry/Records/interface/MuonGeometryRecord.h>

#include <DetectorDescription/Core/interface/DDCompactView.h>

#include "Geometry/Records/interface/MuonNumberingRecord.h"
#include "Geometry/MuonNumbering/interface/MuonDDDConstants.h"

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>
#include <algorithm>
using namespace std;

DTRecoIdealDBLoader::DTRecoIdealDBLoader(const edm::ParameterSet& iConfig) : label_()
{
  std::cout<<"DTRecoIdealDBLoader::DTRecoIdealDBLoader"<<std::endl;
}

DTRecoIdealDBLoader::~DTRecoIdealDBLoader()
{
  std::cout<<"DTRecoIdealDBLoader::~DTRecoIdealDBLoader"<<std::endl;
}

void
DTRecoIdealDBLoader::analyze( const edm::Event & evt ,const edm::EventSetup & es) 
{
  std::cout<<"DTRecoIdealDBLoader::beginJob"<<std::endl;
  RecoIdealGeometry* rig = new RecoIdealGeometry;
  //CSCRecoDigiParameters* rdp = new CSCRecoDigiParameters;
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if( !mydbservice.isAvailable() ){
    std::cout<<"PoolDBOutputService unavailable"<<std::endl;
    return;
  }

  edm::ESHandle<DDCompactView> pDD;
  edm::ESHandle<MuonDDDConstants> pMNDC;
  es.get<IdealGeometryRecord>().get(label_, pDD );
  es.get<MuonNumberingRecord>().get( pMNDC );

  const DDCompactView& cpv = *pDD;
  DTGeometryParsFromDD dtgp;

  dtgp.build( &cpv, *pMNDC, *rig );
  std::cout << "RecoIdealGeometry " << rig->size() << std::endl;

  if ( mydbservice->isNewTagRequest("RecoIdealGeometryRcd") ) {
    cout << "mydbservice " << mydbservice->beginOfTime() << " to " << mydbservice->endOfTime() << endl;
    mydbservice->createNewIOV<RecoIdealGeometry>(rig
                                                 , mydbservice->beginOfTime()
                                                 , mydbservice->endOfTime()
                                                 , "RecoIdealGeometryRcd");
  } else {
    std::cout << "RecoIdealGeometryRcd Tag is already present." << std::endl;
  }
  // if ( mydbservice->isNewTagRequest("CSCRecoDigiParametersRcd") ) {
  //   mydbservice->createNewIOV<DTRecoDigiParameters>(rdp
  //                            , mydbservice->beginOfTime()
  //                            , mydbservice->endOfTime()
  //                            , "CSCRecoDigiParametersRcd");
  // } else {
  //   std::cout << "CSCRecoDigiParametersRcd Tag is already present." << std::endl;
  // }

}

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(DTRecoIdealDBLoader);
