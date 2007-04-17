/** \class DTSurvey
 *
 *  Implements a set of measurements given by survey, tipically a wheel.  
 *  Contains DTSurveyChambers and the displacements and rotations for each are 
 *  calculated.
 *
 *  $Date: 2007/04/13 18:05:35 $
 *  $Revision: 1.1 $
 *  \author Pablo Martinez Ruiz del Arbol
 */



#ifndef Alignment_SurveyAnalysis_DTSurvey_H
#define Alignment_SurveyAnalysis_DTSurvey_H


#include <TMatrixD.h>
#include <string>

#include "Alignment/SurveyAnalysis/interface/DTSurveyChamber.h"

//#include "Alignment/MuonAlignment/interface/MuonAlignment.h"


#include <FWCore/Framework/interface/ESHandle.h>

#include <Geometry/CommonDetUnit/interface/GeomDetUnit.h>

#include <Geometry/DTGeometry/interface/DTGeometry.h>
#include <Geometry/Records/interface/MuonGeometryRecord.h>


using namespace std;


class DTSurvey {

  
 public:
  DTSurvey(string, string, int);
  ~DTSurvey();
 
  void ReadChambers(edm::ESHandle<DTGeometry>);
  void CalculateChambers();

  DTSurveyChamber * getChamber(int, int);

  int getId();

  //void ToDB(MuonAlignment *);
 
  private:
  void FillWheelInfo();

  string nameOfWheelInfoFile, nameOfChamberInfoFile;
  int id; 
  
  //This is the displacement (vector) and rotation (matrix) for the wheel
  float OffsetZ;
  TMatrixD delta;
  TMatrixD Rot; 

  DTSurveyChamber ***chambers;
  
};


std::ostream &operator<<(std::ostream &, DTSurvey);

#endif
