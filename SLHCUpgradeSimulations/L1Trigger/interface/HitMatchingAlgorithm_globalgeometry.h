/// ////////////////////////////////////////
/// Stacked Tracker Simulations          ///
/// Written by:                          ///
/// Andrew W. Rose                       ///
/// 2008                                 ///
///                                      ///
/// Changed by:                          ///
/// Nicola Pozzobon                      ///
/// UNIPD                                ///
/// 2010, May                            ///
///                                      ///
/// Added features:                      ///
/// Possibility to have a flag telling   ///
/// if the Stub is compatible with a     ///
/// higher Pt threshold than the default ///
/// one. Just performs the cross check   ///
/// twice.                               ///
/// ////////////////////////////////////////

#ifndef HIT_MATCHING_ALGORITHM_globalgeometry_H
#define HIT_MATCHING_ALGORITHM_globalgeometry_H

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
#include "FWCore/Framework/interface/ESProducer.h"

//#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithm_globalgeometry.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithm.h"
#include "SLHCUpgradeSimulations/L1Trigger/interface/HitMatchingAlgorithmRecord.h"

#include "SLHCUpgradeSimulations/Utilities/interface/constants.h"
#include "SLHCUpgradeSimulations/Utilities/interface/classInfo.h"

#include <boost/shared_ptr.hpp>

#include <memory>
#include <string>

#include <map>

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// the algorithm is defined here...
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace cmsUpgrades{

template<  typename T  >
class HitMatchingAlgorithm_globalgeometry : public HitMatchingAlgorithm<T> {
  public:
    HitMatchingAlgorithm_globalgeometry( const cmsUpgrades::StackedTrackerGeometry *i , double  aCompatibilityScalingFactor , double  aCompatibilityScalingFactorTight , double aIPWidth  ) : 
                                                    cmsUpgrades::HitMatchingAlgorithm<T>( i ),
                                                    //mName(__PRETTY_FUNCTION__),
                                                    mClassInfo( new cmsUpgrades::classInfo(__PRETTY_FUNCTION__) ),
                                                    mCompatibilityScalingFactor(aCompatibilityScalingFactor),
                                                    mCompatibilityScalingFactorTight(aCompatibilityScalingFactorTight),
                                                    mIPWidth(aIPWidth) {}
    ~HitMatchingAlgorithm_globalgeometry() {}

    std::pair<bool,bool> CheckTwoMemberHitsForCompatibility( const cmsUpgrades::LocalStub<T> & aLocalStub ) const
    { 
      std::pair<bool,bool> a(false,false);
      cmsUpgrades::StackedTrackerDetId id = aLocalStub.Id();

      GlobalPoint innerHitPosition = aLocalStub.averagePosition( &(*HitMatchingAlgorithm<T>::theStackedTracker) , 0);
      GlobalPoint outerHitPosition = aLocalStub.averagePosition( &(*HitMatchingAlgorithm<T>::theStackedTracker) , 1);

      double outerPointRadius = outerHitPosition.perp();
      double innerPointRadius = innerHitPosition.perp();
      double outerPointPhi = outerHitPosition.phi();
      double innerPointPhi = innerHitPosition.phi();
      double innerPointZ, outerPointZ;

      // Rebase the angles in terms of 0-2PI, should
      // really have been written this way in CMSSW...
      //if ( innerPointPhi < 0.0 ) innerPointPhi += 2.0 * cmsUpgrades::KGMS_PI;
      //if ( outerPointPhi < 0.0 ) outerPointPhi += 2.0 * cmsUpgrades::KGMS_PI;

      // Check for seed compatibility given a pt cut
      // Threshold computed from radial location of hits
      double deltaPhiThreshold = (outerPointRadius - innerPointRadius) * mCompatibilityScalingFactor;  
      double deltaPhiThresholdTight = (outerPointRadius - innerPointRadius) * mCompatibilityScalingFactorTight;  

      // Delta phi computed from hit phi locations
      double deltaPhi = outerPointPhi - innerPointPhi;
      if (deltaPhi<0) deltaPhi = -deltaPhi;
      //while( deltaPhi>2.0 * cmsUpgrades::KGMS_PI ) deltaPhi-=(2.0 * cmsUpgrades::KGMS_PI);
      if (deltaPhi > cmsUpgrades::KGMS_PI) deltaPhi = 2 * cmsUpgrades::KGMS_PI - deltaPhi;

      if ( deltaPhi < deltaPhiThreshold ) {

        innerPointZ = innerHitPosition.z();
        outerPointZ = outerHitPosition.z();
        double positiveZBoundary = (mIPWidth - outerPointZ) * (outerPointRadius - innerPointRadius);
        double negativeZBoundary = -(mIPWidth + outerPointZ) * (outerPointRadius - innerPointRadius);
        double multipliedLocation = (innerPointZ - outerPointZ) * outerPointRadius;

        if ( ( multipliedLocation < positiveZBoundary ) &&   ( multipliedLocation > negativeZBoundary ) ){
          a.first = true;
          if ( deltaPhi < deltaPhiThresholdTight ) a.second = true;
          else a.second = false;
        }else{
          a.first = false;
          a.second = false;
        }
      }else{
        a.first = false;
        a.second = false;
      }
      //if (a.first == false) a.second = false; // redundancy

      return a;
    }

    std::string AlgorithmName() const { 
      return ( (mClassInfo->FunctionName())+"<"+(mClassInfo->TemplateTypes().begin()->second)+">" );
      //return mName;
    }

  private:
    //std::string mName;
    const cmsUpgrades::classInfo *mClassInfo;
    double mCompatibilityScalingFactorTight;
    double mCompatibilityScalingFactor;
    double mIPWidth;

};

}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ...and declared to the framework here
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template< typename T >
class  ES_HitMatchingAlgorithm_globalgeometry: public edm::ESProducer{
  public:
    ES_HitMatchingAlgorithm_globalgeometry(const edm::ParameterSet & p) :  mPtThreshold( p.getParameter<double>("minPtThreshold") ),
                                        mPtThresholdTight( p.getParameter<double>("minPtThresholdTight") ),
                                        mIPWidth( p.getParameter<double>("ipWidth") ){setWhatProduced( this );}

    virtual ~ES_HitMatchingAlgorithm_globalgeometry() {}

    boost::shared_ptr< cmsUpgrades::HitMatchingAlgorithm<T> > produce(const cmsUpgrades::HitMatchingAlgorithmRecord & record)
    { 

      edm::ESHandle<MagneticField> magnet;
      record.getRecord<IdealMagneticFieldRecord>().get(magnet);
      double mMagneticFieldStrength = magnet->inTesla(GlobalPoint(0,0,0)).z();

      double mCompatibilityScalingFactor = (100.0 * 2.0e+9 * mPtThreshold) / (cmsUpgrades::KGMS_C * mMagneticFieldStrength);
      // Invert so we use multiplication instead of division in the comparison
      mCompatibilityScalingFactor = 1.0 / mCompatibilityScalingFactor;

      double mCompatibilityScalingFactorTight = (100.0 * 2.0e+9 * mPtThresholdTight) / (cmsUpgrades::KGMS_C * mMagneticFieldStrength);
      // Invert so we use multiplication instead of division in the comparison
      mCompatibilityScalingFactorTight = 1.0 / mCompatibilityScalingFactorTight;


      edm::ESHandle<cmsUpgrades::StackedTrackerGeometry> StackedTrackerGeomHandle;
      record.getRecord<cmsUpgrades::StackedTrackerGeometryRecord>().get( StackedTrackerGeomHandle );
  
      cmsUpgrades::HitMatchingAlgorithm<T>* HitMatchingAlgo = new cmsUpgrades::HitMatchingAlgorithm_globalgeometry<T>( &(*StackedTrackerGeomHandle), mCompatibilityScalingFactor, mCompatibilityScalingFactorTight , mIPWidth  );

      _theAlgo  = boost::shared_ptr< cmsUpgrades::HitMatchingAlgorithm<T> >( HitMatchingAlgo );

      return _theAlgo;
    } 

  private:
    boost::shared_ptr< cmsUpgrades::HitMatchingAlgorithm<T> > _theAlgo;
    double mPtThreshold;
    double mPtThresholdTight;
    double mIPWidth;

};


#endif


