/*
 * \file EcalDQMStatusReader.cc
 *
 * $Date: 2010/08/07 20:47:42 $
 * $Revision: 1.2 $
 * \author G. Della Ricca
 *
*/

#include <fstream>
#include <iostream>
#include <string>
#include <cstring>
#include <time.h>
#include <unistd.h>

#include "FWCore/ServiceRegistry/interface/Service.h"
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "FWCore/Framework/interface/ESHandle.h"
#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"
#include "Geometry/CaloTopology/interface/EcalTrigTowerConstituentsMap.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "CondFormats/DataRecord/interface/EcalDQMChannelStatusRcd.h"
#include "CondFormats/DataRecord/interface/EcalDQMTowerStatusRcd.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "DQM/EcalCommon/interface/Numbers.h"

#include "DQM/EcalCommon/interface/EcalDQMStatusDictionary.h"

#include "DQM/EcalCommon/interface/EcalDQMStatusReader.h"

EcalDQMStatusReader::EcalDQMStatusReader(const edm::ParameterSet& ps) {

  verbose_ = ps.getUntrackedParameter<bool>("verbose", false);

}

void EcalDQMStatusReader::beginRun(const edm::Run& r, const edm::EventSetup& c) {

  std::vector<EcalDQMStatusDictionary::codeDef> dictionary;
  EcalDQMStatusDictionary::getDictionary( dictionary );

  const EcalElectronicsMapping* map_;
  edm::ESHandle<EcalElectronicsMapping> handle;
  c.get<EcalMappingRcd>().get(handle);
  map_ = handle.product();

  const EcalTrigTowerConstituentsMap* mapTT_;
  edm::ESHandle<EcalTrigTowerConstituentsMap> handleTT;
  c.get<IdealGeometryRecord>().get(handleTT);
  mapTT_ = handleTT.product();

  const EcalDQMChannelStatus* channelStatus = 0;
  if ( c.find( edm::eventsetup::EventSetupRecordKey::makeKey< EcalDQMChannelStatusRcd >() ) ) {
    edm::ESHandle< EcalDQMChannelStatus > handle;
    c.get< EcalDQMChannelStatusRcd >().get(handle);
    if ( handle.isValid() ) channelStatus = handle.product();
  }

  const EcalDQMTowerStatus* towerStatus = 0;
  if ( c.find( edm::eventsetup::EventSetupRecordKey::makeKey< EcalDQMTowerStatusRcd >() ) ) {
    edm::ESHandle< EcalDQMTowerStatus > handle;
    c.get< EcalDQMTowerStatusRcd >().get(handle);
    if ( handle.isValid() ) towerStatus = handle.product();
  }

  // barrel
  for ( int ie=-EBDetId::MAX_IETA; ie<=EBDetId::MAX_IETA; ie++ ) {
  if ( ie==0 ) continue;
    for ( int ip=EBDetId::MIN_IPHI; ip<=EBDetId::MAX_IPHI; ip++ ) {
      if ( EBDetId::validDetId(ie, ip) ) {
        EBDetId id(ie, ip);
        if ( channelStatus ) {
          EcalDQMChannelStatus::const_iterator it = channelStatus->find( id.rawId() );
          if ( it != channelStatus->end() ) {
            if ( it->getStatusCode() != 0 ) {
              std::cout << "# EB:channel, ism=" << id.ism() << " hi=" << id.hashedIndex() << " status=" << it->getStatusCode() << std::endl;
              std::vector<EcalDQMStatusDictionary::codeDef> codes;
              EcalDQMStatusDictionary::getCodes( codes, it->getStatusCode() );
              for ( unsigned int i=0; i<codes.size(); i++ ) {
                std::cout << "Crystal " << Numbers::sEB(Numbers::iSM(id.ism(), EcalBarrel)) << " " << id.ic() << " " << codes[i].desc << std::endl;
              }
            }
          }
        }
        if ( towerStatus ) {
          EcalDQMTowerStatus::const_iterator it = towerStatus->find( id.tower().rawId() );
          if ( it != towerStatus->end() ) {
            if ( it->getStatusCode() != 0 ) {
              std::cout << "# EB:tower, ism=" << id.ism() << " hi=" << id.hashedIndex() << " status=" << it->getStatusCode() << std::endl;
              std::vector<EcalDQMStatusDictionary::codeDef> codes;
              EcalDQMStatusDictionary::getCodes( codes, it->getStatusCode() );
              for ( unsigned int i=0; i<codes.size(); i++ ) {
                std::cout << "TT " << Numbers::sEB(Numbers::iSM(id.ism(), EcalBarrel)) << " " << 4*int((id.ietaSM()-1)/5)+int((id.iphiSM()-1)/5)+1 << " " << codes[i].desc << std::endl;
              }
            }
          }
        }
      }
    }
  }

  // endcap
  for ( int ix=EEDetId::IX_MIN; ix<=EEDetId::IX_MAX; ix++ ) {
    for ( int iy=EEDetId::IY_MIN; iy<=EEDetId::IY_MAX; iy++ ) {
      if ( EEDetId::validDetId(ix, iy, +1) ) {
        EEDetId id(ix, iy, +1);
        if ( channelStatus ) {
          EcalDQMChannelStatus::const_iterator it = channelStatus->find( id.rawId() );
          if ( it != channelStatus->end() ) {
            if ( it->getStatusCode() != 0 ) {
              std::cout << "# EE:channel " << ix << " " << iy << " hi=" << id.hashedIndex() << " " << it->getStatusCode() << std::endl;
              std::vector<EcalDQMStatusDictionary::codeDef> codes;
              EcalDQMStatusDictionary::getCodes( codes, it->getStatusCode() );
              for ( unsigned int i=0; i<codes.size(); i++ ) {
                std::cout << "Crystal EE-99 " << 1000*id.ix()+id.iy() << " " << codes[i].desc << std::endl;
              }
            }
          }
        }
        if ( towerStatus ) {
          EcalDQMTowerStatus::const_iterator it = towerStatus->find( id.sc().rawId() );
          if ( it != towerStatus->end() ) {
            if ( it->getStatusCode() != 0 ) {
              std::cout << "# EE:tower " << ix << " " << iy << " hi=" << id.hashedIndex() << " " << it->getStatusCode() << std::endl;
              std::vector<EcalDQMStatusDictionary::codeDef> codes;
              EcalDQMStatusDictionary::getCodes( codes, it->getStatusCode() );
              for ( unsigned int i=0; i<codes.size(); i++ ) {
                std::cout << "TT EE-99 " << map_->iTT(mapTT_->towerOf(id)) << " " << codes[i].desc << std::endl;
              }
            }
          }
        }
      }
      if ( EEDetId::validDetId(ix, iy, -1) ) {
        EEDetId id(ix, iy, -1);
        if ( channelStatus ) {
          EcalDQMChannelStatus::const_iterator it = channelStatus->find( id.rawId() );
          if ( it != channelStatus->end() ) {
            if ( it->getStatusCode() != 0 ) {
              std::cout << "# EE:channel " << ix << " " << iy << " hi=" << id.hashedIndex() << " " << it->getStatusCode() << std::endl;
              std::vector<EcalDQMStatusDictionary::codeDef> codes;
              EcalDQMStatusDictionary::getCodes( codes, it->getStatusCode() );
              for ( unsigned int i=0; i<codes.size(); i++ ) {
                std::cout << "Crystal EE+99 " << 1000*id.ix()+id.iy() << " " << codes[i].desc << std::endl;
              }
            }
          }
        }
        if ( towerStatus ) {
          EcalDQMTowerStatus::const_iterator it = towerStatus->find( id.sc().rawId() );
          if ( it != towerStatus->end() ) {
            if ( it->getStatusCode() != 0 ) {
              std::cout << "# EE:tower " << ix << " " << iy << " hi=" << id.hashedIndex() << " " << it->getStatusCode() << std::endl;
              std::vector<EcalDQMStatusDictionary::codeDef> codes;
              EcalDQMStatusDictionary::getCodes( codes, it->getStatusCode() );
              for ( unsigned int i=0; i<codes.size(); i++ ) {
                std::cout << "TT EE+99 " << map_->iTT(mapTT_->towerOf(id)) << " " << codes[i].desc << std::endl;
              }
            }
          }
        }
      }
    }
  }

}

void EcalDQMStatusReader::endRun(const edm::Run& r, const edm::EventSetup& c) {

}

void EcalDQMStatusReader::beginJob() {

}

void EcalDQMStatusReader::endJob() {

}

void EcalDQMStatusReader::analyze(const edm::Event& e, const edm::EventSetup& c) {

}

EcalDQMStatusReader::~EcalDQMStatusReader() {

}

