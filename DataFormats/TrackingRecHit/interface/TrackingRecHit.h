#ifndef TrackingRecHit_h
#define TrackingRecHit_h

#include "DataFormats/CLHEP/interface/AlgebraicObjects.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/GeometrySurface/interface/LocalError.h"
#include "DataFormats/GeometryVector/interface/LocalPoint.h"
#include "DataFormats/TrackingRecHit/interface/TrackingRecHitFwd.h"

class TrackingRecHit {
public:
/** Type of hits:
 *   valid    = valid hit
 *   missing  = detector is good, but no rec hit found
 *   inactive = detector is off, so there was no hope
 *   bad      = there were many bad strips within the ellipse */
  enum Type { valid = 0, missing = 1, inactive = 2, bad = 3 };

  virtual ~TrackingRecHit() {}

  virtual TrackingRecHit * clone() const = 0;

  virtual AlgebraicVector parameters() const = 0;

  virtual AlgebraicSymMatrix parametersError() const = 0;
  
  virtual AlgebraicMatrix projectionMatrix() const = 0;

  virtual int dimension() const = 0;

  /// Access to component RecHits (if any)
  virtual std::vector<const TrackingRecHit*> recHits() const = 0;

  /// Non-const access to component RecHits (if any)
  virtual std::vector<TrackingRecHit*> recHits() = 0;

  virtual DetId geographicalId() const = 0;

  virtual LocalPoint localPosition() const = 0;

  virtual LocalError localPositionError() const = 0;

  virtual float weight() const {return 1.;}

  virtual Type getType() const { return valid; }
  virtual bool isValid() const {return true;}

};

#endif
