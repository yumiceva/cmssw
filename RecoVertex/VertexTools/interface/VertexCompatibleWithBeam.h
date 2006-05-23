#ifndef VertexCompatibleWithBeam_H
#define VertexCompatibleWithBeam_H

#include "DataFormats/VertexReco/interface/Vertex.h"
#include "RecoVertex/VertexPrimitives/interface/VertexState.h"

class VertexDistance;

/**
 * True if the distance between the beam spot and the vertex, 
 * computed by the `dist` algorithm, is smaller than `cut`. 
 */

class VertexCompatibleWithBeam {

public:

  VertexCompatibleWithBeam(const VertexDistance & dist, float cut);
  VertexCompatibleWithBeam(const VertexCompatibleWithBeam & other);
  VertexCompatibleWithBeam & operator=(const VertexCompatibleWithBeam & other);
  virtual ~VertexCompatibleWithBeam();

  virtual bool operator()(const reco::Vertex &) const;

  // return value of VertexDistance to beam
  float distanceToBeam(const reco::Vertex &) const;

private:

  VertexDistance * theDistance;
  float theCut;
  VertexState theBeam;

};

#endif
