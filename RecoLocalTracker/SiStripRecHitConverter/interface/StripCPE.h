#ifndef RecoLocalTracker_ESProducers_StripCPE_H
#define RecoLocalTracker_ESProducers_StripCPE_H
#include "MagneticField/Engine/interface/MagneticField.h"
#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"
#include "RecoLocalTracker/ClusterParameterEstimator/interface/StripClusterParameterEstimator.h"
#include "RecoLocalTracker/ClusterParameterEstimator/interface/ClusterParameterEstimator.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "Geometry/TrackerSimAlgo/interface/StripGeomDetUnit.h"

class StripCPE : public StripClusterParameterEstimator 
{
 public:
  
  StripCPE(edm::ParameterSet & conf, const MagneticField * mag, const TrackingGeometry* geom);
    
  // LocalValues is typedef for pair<LocalPoint,LocalError> 
  StripClusterParameterEstimator::LocalValues localParameters( const SiStripCluster & cl,const GeomDetUnit& det){
    return localParameters(cl);
  }; 
  StripClusterParameterEstimator::LocalValues localParameters( const SiStripCluster & cl); 
  
 private:
  
  LocalVector driftDirection(const StripGeomDetUnit* det);
  
  const TrackingGeometry * geom_;
  const MagneticField * magfield_ ;
  float theTanLorentzAnglePerTesla;
};

#endif




