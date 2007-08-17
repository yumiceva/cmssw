#ifndef Geometry_TrackerGeometryBuilder_TrackerGeometry_H
#define Geometry_TrackerGeometryBuilder_TrackerGeometry_H

#include "Geometry/CommonDetUnit/interface/TrackingGeometry.h"

class GeometricDet;

#include "Geometry/TrackerGeometryBuilder/interface/trackerHierarchy.h"

#include "DataFormats/Common/interface/Trie.h"

// FIXME here just to allow prototyping...
namespace trackerTrie {
  typedef TrackingGeometry::DetUnitContainer::const_iterator Iter;
  typedef GeomDet const* PDet;
  typedef edm::Trie<PDet> DetTrie;
  typedef edm::TrieNode<PDet> Node;
  typedef Node const * node_pointer; // sigh....
  typedef edm::TrieNodeIter<PDet> node_iterator;
}


/**
 * A specific Tracker Builder which builds a Tracker from a list of DetUnits. 
 * Pattern recognition is used to discover layers, rings etc.
 */
class TrackerGeometry : public TrackingGeometry {
public:

  explicit TrackerGeometry(GeometricDet const* gd=0);  

  virtual const DetTypeContainer&  detTypes()         const;
  virtual const DetUnitContainer&  detUnits()         const;
  virtual const DetContainer&      dets()             const;
  virtual const DetIdContainer&    detUnitIds()       const;
  virtual const DetIdContainer&    detIds()           const;
  virtual const GeomDetUnit*       idToDetUnit(DetId) const;
  virtual const GeomDet*           idToDet(DetId)     const;


  void addType(GeomDetType* p);
  void addDetUnit(GeomDetUnit* p);
  void addDetUnitId(DetId p);
  void addDet(GeomDet* p);
  void addDetId(DetId p);




  GeometricDet const * trackerDet() const; 

  const DetContainer& detsPXB() const;
  const DetContainer& detsPXF() const;
  const DetContainer& detsTIB() const;
  const DetContainer& detsTID() const;
  const DetContainer& detsTOB() const;
  const DetContainer& detsTEC() const;

private:

  GeometricDet const * theTrackerDet; 

  /// Aligner has access to map
  friend class GeometryAligner;

  DetTypeContainer  theDetTypes;
  DetUnitContainer  theDetUnits;
  DetContainer      theDets;
  DetIdContainer    theDetUnitIds;
  DetIdContainer    theDetIds;
  mapIdToDetUnit    theMapUnit;
  mapIdToDet        theMap;

  DetContainer      thePXBDets;
  DetContainer      thePXFDets;
  DetContainer      theTIBDets;
  DetContainer      theTIDDets;
  DetContainer      theTOBDets;
  DetContainer      theTECDets;


};

#endif
