#ifndef MagCylinder_H
#define MagCylinder_H

#include "MagneticField/VolumeGeometry/interface/MagVolume.h"
#include "MagneticField/VolumeGeometry/interface/VolumeSide.h"

#include <vector>

//-- FIXME
#include <string>
//--

template <class T>
class MagneticFieldProvider;

class MagCylinder : public MagVolume {
public:

  MagCylinder( const PositionType& pos, const RotationType& rot, 
	       DDSolidShape shape, const std::vector<VolumeSide>& faces,
	       const MagneticFieldProvider<float> * mfp);

  virtual bool inside( const GlobalPoint& gp, double tolerance=0.) const;
  virtual bool inside( const LocalPoint& lp, double tolerance=0.) const;

  /// Access to volume faces
  const std::vector<VolumeSide>& faces() const {return theFaces;}

  //-- FIXME
  std::string name;
  //--

private:

  std::vector<VolumeSide> theFaces;
  Scalar theZmin;
  Scalar theZmax;
  Scalar theInnerR;
  Scalar theOuterR;

};

#endif
