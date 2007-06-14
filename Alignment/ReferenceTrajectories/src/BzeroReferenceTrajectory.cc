
#include "Alignment/ReferenceTrajectories/interface/BzeroReferenceTrajectory.h"

#include "DataFormats/CLHEP/interface/AlgebraicObjects.h" 
#include "DataFormats/TrajectoryState/interface/LocalTrajectoryParameters.h"

#include "TrackingTools/TrajectoryState/interface/TrajectoryStateOnSurface.h"
#include "TrackingTools/TransientTrackingRecHit/interface/TransientTrackingRecHit.h"


BzeroReferenceTrajectory::BzeroReferenceTrajectory(const TrajectoryStateOnSurface &refTsos,
						   const TransientTrackingRecHit::ConstRecHitContainer
						   &recHits, bool hitsAreReverse,
						   const MagneticField *magField, 
						   MaterialEffects materialEffects,
						   double mass, double momentumEstimate) :
  ReferenceTrajectory( refTsos.localParameters().mixedFormatVector().kSize, recHits.size() ),
  theMomentumEstimate( momentumEstimate )
{
  // no check against magField == 0

  // No estimate for momentum of cosmics available -> set to default value.
  theParameters = asHepVector<5>( refTsos.localParameters().mixedFormatVector() );
  theParameters[0] = 1./theMomentumEstimate;

  LocalTrajectoryParameters locParamWithFixedMomentum( theParameters, refTsos.localParameters().pzSign(), false );

  const TrajectoryStateOnSurface refTsosWithFixedMomentum(locParamWithFixedMomentum, refTsos.localError(),
							  refTsos.surface(), magField, beforeSurface);

  if (hitsAreReverse)
  {
    TransientTrackingRecHit::ConstRecHitContainer fwdRecHits;
    fwdRecHits.reserve(recHits.size());

    for (TransientTrackingRecHit::ConstRecHitContainer::const_reverse_iterator it=recHits.rbegin(); it != recHits.rend(); ++it)
      fwdRecHits.push_back(*it);

    theValidityFlag = this->construct(refTsosWithFixedMomentum, fwdRecHits, mass, materialEffects, magField);
  } else {
    theValidityFlag = this->construct(refTsosWithFixedMomentum, recHits, mass, materialEffects, magField);
  }

  // Exclude momentum from the parameters and also the derivatives of the measurements w.r.t. the momentum.
  theParameters = theParameters.sub( 2, 5 );
  theDerivatives = theDerivatives.sub( 1, theDerivatives.num_row(), 2, 5 );
}
