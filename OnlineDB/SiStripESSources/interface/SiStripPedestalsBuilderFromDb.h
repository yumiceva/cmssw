// Last commit: $Id: SiStripPedestalsBuilderFromDb.h,v 1.5 2008/05/26 13:37:25 giordano Exp $
// Latest tag:  $Name: V02-00-06 $
// Location:    $Source: /cvs_server/repositories/CMSSW/CMSSW/OnlineDB/SiStripESSources/interface/SiStripPedestalsBuilderFromDb.h,v $

#ifndef OnlineDB_SiStripESSources_SiStripPedestalsBuilderFromDb_H
#define OnlineDB_SiStripESSources_SiStripPedestalsBuilderFromDb_H

#include "CalibTracker/SiStripESProducers/interface/SiStripPedestalsESSource.h"
#include "DataFormats/SiStripCommon/interface/SiStripConstants.h"
#include "OnlineDB/SiStripESSources/interface/SiStripCondObjBuilderFromDb.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "OnlineDB/SiStripConfigDb/interface/SiStripDbParams.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <vector>
#include <string>



class SiStripPedestalsBuilderFromDb : public SiStripPedestalsESSource {
  
 public:

  SiStripPedestalsBuilderFromDb( const edm::ParameterSet& );
  virtual ~SiStripPedestalsBuilderFromDb();
  
  /** Builds pedestals using info from configuration database. */
  virtual SiStripPedestals* makePedestals();
  
 protected:
  
  /** Virtual method that is called by makePedestals() to allow
      pedestals to be written to the conditions database. */
  virtual void writePedestalsToCondDb( const SiStripPedestals& ) {;}
  
  /** Container for DB connection parameters. */
  SiStripDbParams dbParams_;

  /** Service to access onlineDB and extract pedestal/noise */
  edm::Service<SiStripCondObjBuilderFromDb> condObjBuilder;
  
};

#endif // OnlineDB_SiStripESSources_SiStripPedestalsBuilderFromDb_H

