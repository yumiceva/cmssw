#include "RecoVertex/VertexTools/interface/VertexCompatibleWithBeam.h"
#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/GlobalError.h"
#include "RecoVertex/VertexTools/interface/VertexDistance.h"
#include "RecoVertex/VertexPrimitives/interface/ConvertError.h"


VertexCompatibleWithBeam::VertexCompatibleWithBeam(const VertexDistance & d, 
						   float cut) 
  : theDistance(d.clone()), theCut(cut) 
{
  BeamSpot beamSpot;
  theBeam = VertexState(beamSpot.position(), beamSpot.error()); 
}

VertexCompatibleWithBeam::VertexCompatibleWithBeam(const VertexDistance & d, 
						   float cut, const BeamSpot & beamSpot) 
    : theDistance(d.clone()), theCut(cut), theBeam(beamSpot.position(),beamSpot.error()){}


VertexCompatibleWithBeam::VertexCompatibleWithBeam(
  const VertexCompatibleWithBeam & other) : 
  theDistance((*other.theDistance).clone()), 
  theCut(other.theCut), theBeam(other.theBeam) {}


VertexCompatibleWithBeam::~VertexCompatibleWithBeam() {
  delete theDistance;
}


VertexCompatibleWithBeam & 
VertexCompatibleWithBeam::operator=(const VertexCompatibleWithBeam & other) 
{
  if (this == &other) return *this;

  theDistance = (*other.theDistance).clone();
  theCut = other.theCut;
  theBeam = other.theBeam;
  return *this;
}

void VertexCompatibleWithBeam::setBeamSpot(const BeamSpot & beamSpot){
  theBeam = VertexState(beamSpot.position(), beamSpot.error());
}

bool VertexCompatibleWithBeam::operator()(const reco::Vertex & v) const 
{
  GlobalPoint p(Basic3DVector<float> (v.position()));
  VertexState vs(p, RecoVertex::convertError(v.covariance()));
  return (theDistance->distance(vs, theBeam).value() < theCut);
}


float VertexCompatibleWithBeam::distanceToBeam(const reco::Vertex & v) const
{
  GlobalPoint p(Basic3DVector<float> (v.position()));
  VertexState vs(p, RecoVertex::convertError(v.covariance()));
  return theDistance->distance(vs, theBeam).value();
}


float VertexCompatibleWithBeam::distanceToBeam(const reco::Vertex & v, const VertexState & bs) const
{
  GlobalPoint p(Basic3DVector<float> (v.position()));
  VertexState vs(p, RecoVertex::convertError(v.covariance()));
  return theDistance->distance(vs, bs).value();
}


bool VertexCompatibleWithBeam::operator()(const reco::Vertex & v, const VertexState & bs) const 
{
  GlobalPoint p(Basic3DVector<float> (v.position()));
  VertexState vs(p, RecoVertex::convertError(v.covariance()));
  return (theDistance->distance(vs, bs).value() < theCut);
}


