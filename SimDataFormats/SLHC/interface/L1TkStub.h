/// ////////////////////////////////////////
/// Stacked Tracker Simulations          ///
///                                      ///
/// Andrew W. Rose, IC                   ///
/// Nicola Pozzobon, UNIPD               ///
/// UNIPD                                ///
///                                      ///
/// 2008                                 ///
/// 2010, June                           ///
/// 2011, June                           ///
/// 2012, October                        ///
/// 2013, January                        ///
/// ////////////////////////////////////////

#ifndef STACKED_TRACKER_L1TK_STUB_FORMAT_H
#define STACKED_TRACKER_L1TK_STUB_FORMAT_H

#include "Geometry/TrackerGeometryBuilder/interface/StackedTrackerGeometry.h"
#include "DataFormats/SiPixelDetId/interface/StackedTrackerDetId.h"
#include "CLHEP/Units/PhysicalConstants.h"

#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"
#include "Geometry/CommonTopologies/interface/Topology.h" 

#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/Common/interface/Ref.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/MeasurementPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryVector/interface/GlobalVector.h"
#include "DataFormats/Common/interface/DetSetVector.h"
#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"
#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"
#include "SimDataFormats/SLHC/interface/L1TkCluster.h"

  /** ************************ **/
  /**                          **/
  /**   DECLARATION OF CLASS   **/
  /**                          **/
  /** ************************ **/

  template< typename T >
  class L1TkStub
  {
    public:
      /// Constructors
      L1TkStub();
      L1TkStub( DetId aDetId );

      /// Destructor
      ~L1TkStub();

      /// Data members:   getABC( ... )
      /// Helper methods: findABC( ... )

      /// Clusters composing the Stub
      std::vector< edm::Ptr< L1TkCluster< T > > > getClusterPtrs() const;
      const edm::Ptr< L1TkCluster< T > >&         getClusterPtr( unsigned int hitIdentifier ) const;
      void                                        addClusterPtr( edm::Ptr< L1TkCluster< T > > aL1TkCluster );

      /// NOTE they are added and stored as edm::Ptr< Cluster > but
      /// returned as just Cluster for backward compatibility with
      /// HitMatching Algorithms

      /// Detector element
      DetId getDetId() const;
      void  setDetId( DetId aDetId );

      /// Trigger information
      int  getTriggerDisplacement() const;
      void setTriggerDisplacement( int aDisplacement );
      int  getTriggerOffset() const;
      void setTriggerOffset( int anOffset );

      /// MC truth
      edm::Ptr< SimTrack > getSimTrackPtr() const;
      bool                 isGenuine() const;
      bool                 isCombinatoric() const;
      bool                 isUnknown() const;
      int                  findType() const;
      unsigned int         findSimTrackId() const;

      /// Collect MC truth
      void checkSimTrack();

      /// Calculate position and momentum
      double       findRoughPt( double aMagneticFieldStrength, const StackedTrackerGeometry *aStackedTracker ) const;
      GlobalPoint  findGlobalPosition( const StackedTrackerGeometry *aStackedTracker ) const;
      GlobalVector findGlobalDirection( const StackedTrackerGeometry *aStackedTracker ) const;

      /// Information
      std::string print( unsigned int i=0 ) const;

    private:
      /// Data members
      DetId                                       theDetId;
      std::vector< edm::Ptr< L1TkCluster< T > > > theClusters;
      edm::Ptr< SimTrack >                        theSimTrack;
      int                                         theDisplacement;
      int                                         theOffset;

  }; /// Close class

  /** ***************************** **/
  /**                               **/
  /**   IMPLEMENTATION OF METHODS   **/
  /**                               **/
  /** ***************************** **/

  /// Default Constructor
  template< typename T >
  L1TkStub< T >::L1TkStub()
  {
    /// Set default data members
    theDetId = 0;
    theClusters.clear();
    edm::Ptr< SimTrack >* testSimTrack = new edm::Ptr< SimTrack >();
    theSimTrack = *testSimTrack;
    theDisplacement = 999999;
    theOffset = 0;
  }

  /// Another Constructor
  template< typename T >
  L1TkStub< T >::L1TkStub( DetId aDetId )
  {
    /// Set default data members
    theDetId = aDetId;
    theClusters.clear();
    edm::Ptr< SimTrack >* testSimTrack = new edm::Ptr< SimTrack >();
    theSimTrack = *testSimTrack;
    theDisplacement = 999999;
    theOffset = 0;
  }

  /// Destructor
  template< typename T >
  L1TkStub< T >::~L1TkStub(){}

  /// Get the Pointers to the Clusters composing the Stub
  template< typename T >
  std::vector< edm::Ptr< L1TkCluster< T > > > L1TkStub< T >::getClusterPtrs() const { return theClusters; }

  /// Get the Pointer to a Cluster
  template< typename T >
  const edm::Ptr< L1TkCluster< T > >& L1TkStub< T >::getClusterPtr( unsigned int hitIdentifier ) const
  {
    typename std::vector< edm::Ptr< L1TkCluster< T > > >::const_iterator clusIter;
    for ( clusIter = theClusters.begin();
          clusIter != theClusters.end();
          ++clusIter )
    {
      if ( (*clusIter)->getStackMember() == hitIdentifier )
        return *clusIter;
    }
    edm::Ptr< L1TkCluster< T > >* tmpCluPtr = new edm::Ptr< L1TkCluster< T > >();
    return *tmpCluPtr;
  }

  /// Add the Clusters to the candidate Stub
  template< typename T >
  void L1TkStub< T >::addClusterPtr( edm::Ptr< L1TkCluster< T > > aL1TkCluster )
  {
    /// NOTE: this must be used ONLY as it is used
    /// within the L1TkStubBuilder!
    /// So, pushing back in the right order!!
    theClusters.push_back( aL1TkCluster );
  }

  /// Detector element
  template< typename T >
  DetId L1TkStub< T >::getDetId() const { return theDetId; }

  template< typename T >
  void L1TkStub< T >::setDetId( DetId aDetId ) { theDetId = aDetId; }

  /// Trigger info
  template< typename T >
  int L1TkStub< T >::getTriggerDisplacement() const { return theDisplacement; }

  template< typename T >
  void L1TkStub< T >::setTriggerDisplacement( int aDisplacement ) { theDisplacement = aDisplacement; }

  template< typename T >
  int L1TkStub< T >::getTriggerOffset() const { return theOffset; }

  template< typename T >
  void L1TkStub< T >::setTriggerOffset( int anOffset ) { theOffset = anOffset; }

  /// MC truth
  template< typename T >
  edm::Ptr< SimTrack > L1TkStub< T >::getSimTrackPtr() const { return theSimTrack; }

  template< typename T >
  bool L1TkStub< T >::isGenuine() const
  {
    /*
    /// GENUINE for clusters means not combinatoric and
    /// not unknown: same MC truth content MUST be found
    /// in both clusters composing the stub
    if ( theClusters.at(0)->isUnknown() || theClusters.at(1)->isUnknown() )
      /// If at least one cluster is unknown, it means
      /// either unknown, either combinatoric
      return false;

    else
    {
      /// Here both are clusters are genuine/combinatoric
      /// If both clusters have some known SimTrack content
      /// they must be compared to each other
      if ( theClusters.at(0)->isGenuine() && theClusters.at(1)->isGenuine() )
      {
        if ( theClusters.at(0)->findSimTrackId() == theClusters.at(1)->findSimTrackId() )
          /// Two genuine clusters with same SimTrack content mean genuine
          return true;
        else
          return false;
      }
      else
      {
        /// Here, at least one cluster is combinatoric
        int prevTrack = -99999; // SimTrackId storage
        std::vector< edm::Ptr< SimTrack > > innerSimTracks = theClusters.at(0)->getSimTrackPtrs();
        std::vector< edm::Ptr< SimTrack > > outerSimTracks = theClusters.at(1)->getSimTrackPtrs();
        for ( unsigned int i = 0; i < innerSimTracks.size(); i++ )
        {
          /// Skip NULL pointers
          if ( innerSimTracks.at(i).isNull() );
            continue;
          for ( unsigned int j = 0; j < outerSimTracks.size(); j++ )
          {
            /// Skip NULL pointers
            if ( outerSimTracks.at(j).isNull() );
              continue;

            if ( innerSimTracks.at(i)->trackId() == outerSimTracks.at(j)->trackId() )
            {
              /// Same SimTrack is present in both clusters
              if ( prevTrack < 0 )
                prevTrack = outerSimTracks.at(j)->trackId();

              if ( prevTrack != (int)outerSimTracks.at(j)->trackId() )
                /// If two different SimTracks are found in both clusters,
                /// then the stub is for sure combinatoric
                return false;
            }
          }
        }
        if ( prevTrack < 0 )
          /// No SimTracks were found to be in both clusters
          return false;
        else
          /// Only one SimTrack was found to be present in both clusters
          /// even if one of the clusters (or both) are combinatoric:
          /// this means there is only one track that participates in
          /// both clusters, hence the stub is genuine
          return true;
      }
    }
    /// Default
    /// Should never get here
    std::cerr << "W A R N I N G! L1TkStub::isGenuine() \t we should never get here" << std::endl;
    return true;
    */

    if ( theSimTrack.isNull() )
      return false;

    return true;
  }

  template< typename T >
  bool L1TkStub< T >::isCombinatoric() const
  {
    if ( this->isGenuine() )
      return false;

    /// COMBINATORIC means that the same MC truth content
    /// cannot be found in the pair of clusters that compose
    /// the stub, and at leask one of them is not unknown
    if ( theClusters.at(0)->isUnknown() && theClusters.at(1)->isUnknown() )
      /// Two unknown clusters mean that the stub is unknown
      return false;

    else if ( theClusters.at(0)->isUnknown() || theClusters.at(1)->isUnknown() )
      /// One unknown and one combinatoric mean the stub is combinatoric
      /// One unknown and one genuine mean the stub is combinatoric
      return true;

    else
    {
      /// Here both are clusters are genuine/combinatoric
      /// If both clusters have some known SimTrack content
      /// they must be compared to each other
      if ( theClusters.at(0)->isGenuine() && theClusters.at(1)->isGenuine() )
      {
        if ( theClusters.at(0)->findSimTrackId() == theClusters.at(1)->findSimTrackId() )
          /// Two genuine clusters with same SimTrack content mean genuine
          return false;
        else
          return true;
      }
      else
      {
        /// Here, at least one cluster is combinatoric
        int prevTrack = -99999; // SimTrackId storage
        std::vector< edm::Ptr< SimTrack > > innerSimTracks = theClusters.at(0)->getSimTrackPtrs();
        std::vector< edm::Ptr< SimTrack > > outerSimTracks = theClusters.at(1)->getSimTrackPtrs();
        for ( unsigned int i = 0; i < innerSimTracks.size(); i++ )
        {
          /// Skip NULL pointers
          if ( innerSimTracks.at(i).isNull() );
            continue;
          for ( unsigned int j = 0; j < outerSimTracks.size(); j++ )
          {
            /// Skip NULL pointers
            if ( outerSimTracks.at(j).isNull() );
              continue;

            if ( innerSimTracks.at(i)->trackId() == outerSimTracks.at(j)->trackId() )
            {
              /// Same SimTrack is present in both clusters
              if ( prevTrack < 0 )
                prevTrack = outerSimTracks.at(j)->trackId();

              if ( prevTrack != (int)outerSimTracks.at(j)->trackId() )
                /// If two different SimTracks are found in both clusters,
                /// then the stub is for sure combinatoric
                return true;
            }
          }
        }
        if ( prevTrack < 0 )
          /// No common SimTracks were found to be in both clusters
          return true;
        else
          /// Only one SimTrack was found to be present in both clusters
          /// even if one of the clusters (or both) are combinatoric:
          /// this means there is only one track that participates in
          /// both clusters, hence the stub is genuine
          return false;
      }
    }
    /// Default
    /// Should never get here
    std::cerr << "W A R N I N G! L1TkStub::isCombinatoric() \t we should never get here" << std::endl;
    return false; 
  }

  template< typename T >
  bool L1TkStub< T >::isUnknown() const
  {
    /// UNKNOWN means that both clusters are unknown
    return ( theClusters.at(0)->isUnknown() && theClusters.at(1)->isUnknown() );
  }

  template< typename T >
  int L1TkStub< T >::findType() const
  {
    if ( theSimTrack.isNull() )
      return 999999999;
    return theSimTrack->type();
  }

  template< typename T >
  unsigned int L1TkStub< T >::findSimTrackId() const
  {
    if ( theSimTrack.isNull() )
      return 0;
    return theSimTrack->trackId();
  }

  /// Collect MC truth
  template< typename T >
  void L1TkStub< T >::checkSimTrack()
  {
    /// This method is based on the early version of
    /// isGenuine >>> same approach to store the SimTrack

    /// GENUINE for clusters means not combinatoric and
    /// not unknown: same MC truth content MUST be found
    /// in both clusters composing the stub
    if ( theClusters.at(0)->isUnknown() || theClusters.at(1)->isUnknown() )
      /// If at least one cluster is unknown, it means
      /// either unknown, either combinatoric
      /// Do nothing, leave the default NULL
      return;

    else
    {
      /// Here both are clusters are genuine/combinatoric
      /// If both clusters have some known SimTrack content
      /// they must be compared to each other
      if ( theClusters.at(0)->isGenuine() && theClusters.at(1)->isGenuine() )
      {
        if ( theClusters.at(0)->findSimTrackId() == theClusters.at(1)->findSimTrackId() )
        {
          /// Two genuine clusters with same SimTrack content mean genuine
          std::vector< edm::Ptr< SimTrack > > curSimTracks = theClusters.at(0)->getSimTrackPtrs();
          for ( unsigned int k = 0; k < curSimTracks.size(); k++ )
          {
            if ( curSimTracks.at(k).isNull() == false )
            {
              theSimTrack = curSimTracks.at(k);
              return;
            }
          }
        }
        else
          return;
      }
      else
      {
        /// Here, at least one cluster is combinatoric
        int prevTrack = -99999; // SimTrackId storage
        unsigned int whichSimTrack = 0;
        std::vector< edm::Ptr< SimTrack > > innerSimTracks = theClusters.at(0)->getSimTrackPtrs();
        std::vector< edm::Ptr< SimTrack > > outerSimTracks = theClusters.at(1)->getSimTrackPtrs();
        for ( unsigned int i = 0; i < innerSimTracks.size(); i++ )
        {
          /// Skip NULL pointers
          if ( innerSimTracks.at(i).isNull() );
            continue;
          for ( unsigned int j = 0; j < outerSimTracks.size(); j++ )
          {
            /// Skip NULL pointers
            if ( outerSimTracks.at(j).isNull() );
              continue;

            if ( innerSimTracks.at(i)->trackId() == outerSimTracks.at(j)->trackId() )
            {
              /// Same SimTrack is present in both clusters
              if ( prevTrack < 0 )
              {
                prevTrack = outerSimTracks.at(j)->trackId();
                whichSimTrack = j;
              }

              if ( prevTrack != (int)outerSimTracks.at(j)->trackId() )
                /// If two different SimTracks are found in both clusters,
                /// then the stub is for sure combinatoric
                return;
            }
          }
        }
        if ( prevTrack < 0 )
          /// No SimTracks were found to be in both clusters
          return;
        else
          /// Only one SimTrack was found to be present in both clusters
          /// even if one of the clusters (or both) are combinatoric:
          /// this means there is only one track that participates in
          /// both clusters, hence the stub is genuine
          theSimTrack = outerSimTracks.at(whichSimTrack);
      }
    }
  }

  /// Fit Stub as in Builder
  /// To be used for out-of-Builder Stubs
  template< typename T >
  double L1TkStub< T >::findRoughPt( double aMagneticFieldStrength, const StackedTrackerGeometry *aStackedTracker ) const
  {
    /// Get the magnetic field
    /// Usually it is done like the following three lines
    //iSetup.get<IdealMagneticFieldRecord>().get(magnet);
    //magnet_ = magnet.product();
    //mMagneticFieldStrength = magnet_->inTesla(GlobalPoint(0,0,0)).z();
    /// Calculate factor for rough Pt estimate
    /// B rounded to 4.0 or 3.8
    /// This is B * C / 2 * appropriate power of 10
    /// So it's B * 0.0015
    double mPtFactor = (floor(aMagneticFieldStrength*10.0 + 0.5))/10.0*0.0015;

    /// Get average position of Clusters composing the Stub
    GlobalPoint innerHitPosition = theClusters.at(0)->findAverageGlobalPosition( aStackedTracker );
    GlobalPoint outerHitPosition = theClusters.at(1)->findAverageGlobalPosition( aStackedTracker );

    /// Get useful quantities
    double outerPointRadius = outerHitPosition.perp();
    double innerPointRadius = innerHitPosition.perp();
    double outerPointPhi = outerHitPosition.phi();
    double innerPointPhi = innerHitPosition.phi();
    double deltaRadius = outerPointRadius - innerPointRadius;

    /// Here a switch on Barrel/Endcap is introduced
    StackedTrackerDetId tempDetId(theDetId);
    if (tempDetId.isBarrel())
    {
      /// Calculate angular displacement from hit phi locations
      /// and renormalize it, if needed
      double deltaPhi = outerPointPhi - innerPointPhi;
      if (deltaPhi < 0)
        deltaPhi = -deltaPhi;
      if (deltaPhi > M_PI)
        deltaPhi = 2*M_PI - deltaPhi;

      /// Return the rough Pt
      return ( deltaRadius * mPtFactor / deltaPhi );
    }
    else if (tempDetId.isEndcap())
    {
      /// Test approximated formula for Endcap stubs
      /// Check always to be consistent with HitMatchingAlgorithm_window2012.h
      double roughPt = innerPointRadius * innerPointRadius * mPtFactor / fabs(theClusters.at(0)->findAverageLocalPosition( aStackedTracker ).x()) ;
      roughPt +=       outerPointRadius * outerPointRadius * mPtFactor / fabs(theClusters.at(1)->findAverageLocalPosition( aStackedTracker ).x()) ;
      roughPt = roughPt / 2.;

      /// Return the rough Pt
      return roughPt;
    }

    /// Default
    /// Should never get here
    std::cerr << "W A R N I N G! L1TkStub::findRoughPt() \t we should never get here" << std::endl;
    return 99999.9;
  }

  template< typename T >
  GlobalPoint L1TkStub< T >::findGlobalPosition( const StackedTrackerGeometry *aStackedTracker ) const
  {
    /*
    /// Get average position of Clusters composing the Stub
    GlobalPoint innerHitPosition = theClusters.at(0)->findAverageGlobalPosition( aStackedTracker );
    GlobalPoint outerHitPosition = theClusters.at(1)->findAverageGlobalPosition( aStackedTracker );

    /// Calculate the position
    GlobalPoint globalPosition( (innerHitPosition.x()+outerHitPosition.x())/2.,
                                (innerHitPosition.y()+outerHitPosition.y())/2.,
                                (innerHitPosition.z()+outerHitPosition.z())/2. );

    return globalPosition;
    */

    /// Fast version: only inner cluster matters
    return theClusters.at(0)->findAverageGlobalPosition( aStackedTracker );
  }

  template< typename T >
  GlobalVector L1TkStub< T >::findGlobalDirection( const StackedTrackerGeometry *aStackedTracker ) const
  {
    /// Get average position of Clusters composing the Stub
    GlobalPoint innerHitPosition = theClusters.at(0)->findAverageGlobalPosition( aStackedTracker );
    GlobalPoint outerHitPosition = theClusters.at(1)->findAverageGlobalPosition( aStackedTracker );

    /// Calculate the direction
    GlobalVector directionVector( outerHitPosition.x()-innerHitPosition.x(),
                                  outerHitPosition.y()-innerHitPosition.y(),
                                  outerHitPosition.z()-innerHitPosition.z() );

    return directionVector;
  }

  /// Information
  template< typename T >
  std::string L1TkStub< T >::print( unsigned int i ) const
  {
    std::string padding("");
    for ( unsigned int j=0; j!=i; ++j )
      padding+="\t";
    std::stringstream output;
    output<<padding<<"L1TkStub:\n";
    padding+='\t';
    output << padding << "DetId: " << theDetId.rawId() << '\n';
    unsigned int iClu = 0;
    typename std::vector< edm::Ptr< L1TkCluster< T > > >::const_iterator clusIter;
    for ( clusIter = theClusters.begin(); clusIter!= theClusters.end(); ++clusIter )
      output << padding << "cluster: " << iClu++ << ", member: " << (*clusIter)->getStackMember() << ", cluster size: " << (*clusIter)->getHits().size() << '\n';
    return output.str();
  }

  template< typename T >
  std::ostream& operator << (std::ostream& os, const L1TkStub< T >& aL1TkStub) { return ( os<<aL1TkStub.print() ); }



#endif

