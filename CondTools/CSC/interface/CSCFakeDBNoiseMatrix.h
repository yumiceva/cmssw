#ifndef _CSCFAKEDBNOISEMATRIX_H
#define _CSCFAKEDBNOISEMATRIX_H

#include <memory>
#include "FWCore/Framework/interface/SourceFactory.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetupRecordIntervalFinder.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/CSCObjects/interface/CSCDBNoiseMatrix.h"
#include "CondFormats/DataRecord/interface/CSCDBNoiseMatrixRcd.h"
#include <DataFormats/MuonDetId/interface/CSCDetId.h>
#include <boost/shared_ptr.hpp>

class CSCFakeDBNoiseMatrix: public edm::ESProducer, public edm::EventSetupRecordIntervalFinder  {
   public:
      CSCFakeDBNoiseMatrix(const edm::ParameterSet&);
      ~CSCFakeDBNoiseMatrix();

      inline static CSCDBNoiseMatrix * prefillDBNoiseMatrix(); 

      typedef boost::shared_ptr<CSCDBNoiseMatrix> Pointer;

      Pointer produceDBNoiseMatrix(const CSCDBNoiseMatrixRcd&);

   private:
      // ----------member data ---------------------------
    void setIntervalFor(const edm::eventsetup::EventSetupRecordKey &, const edm::IOVSyncValue&, edm::ValidityInterval & );
    Pointer cndbNoiseMatrix ;

};

#include<fstream>
#include<vector>
#include<iostream>

// to workaround plugin library
inline CSCDBNoiseMatrix *  CSCFakeDBNoiseMatrix::prefillDBNoiseMatrix()
{
  int seed;
  long int M;
  float mean,min, minchi;
  const int MAX_SIZE = 217728; //or 252288 for ME4/2 chambers
  const int FACTOR=1000;
  
  CSCDBNoiseMatrix * cndbmatrix = new CSCDBNoiseMatrix();
  cndbmatrix->matrix.resize(MAX_SIZE);

  seed = 10000;	
  srand(seed);
  mean=6.8, min=-10.0, minchi=1.0, M=1000;
  cndbmatrix->factor_noise = int (FACTOR);
  
  for(int i=0; i<MAX_SIZE;i++){
    cndbmatrix->matrix[i].elem33 = (short int) (10.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem34 = (short int) (4.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem44 = (short int) (10.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem35 = (short int) (3.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem45 = (short int) (8.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem55 = (short int) (10.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem46 = (short int) (2.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem56 = (short int) (5.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem66 = (short int) (10.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem57 = (short int) (3.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem67 = (short int) (4.0*FACTOR+0.5);
    cndbmatrix->matrix[i].elem77 = (short int) (10.0*FACTOR+0.5);
  }
  return cndbmatrix;
}  

#endif

