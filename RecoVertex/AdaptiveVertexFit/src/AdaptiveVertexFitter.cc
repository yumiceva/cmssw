#include "RecoVertex/AdaptiveVertexFit/interface/AdaptiveVertexFitter.h"
#include "DataFormats/GeometryCommonDetAlgo/interface/GlobalError.h"
#include "RecoVertex/VertexTools/interface/AnnealingSchedule.h"
#include "RecoVertex/VertexTools/interface/GeometricAnnealing.h"
#include "RecoVertex/VertexTools/interface/VertexTrackFactory.h"
#include "RecoVertex/AdaptiveVertexFit/interface/KalmanChiSquare.h"
#include "RecoVertex/VertexPrimitives/interface/VertexException.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include <algorithm>

using namespace edm;

// #define STORE_WEIGHTS
#ifdef STORE_WEIGHTS
#include <dataharvester/Writer.h>
#endif

using namespace std;

namespace {
  static const float initialError = 10000;

  #ifdef STORE_WEIGHTS
  map < RefCountedLinearizedTrackState, int > ids;
  int iter=0;

  int getId ( const RefCountedLinearizedTrackState & r )
  {
    static int ctr=1;
    if ( ids.count(r) == 0 )
    {
      ids[r]=ctr++;
    }
    return ids[r];
  }
  #endif
}

AdaptiveVertexFitter::AdaptiveVertexFitter(
      const AnnealingSchedule & ann,
      const LinearizationPointFinder & linP,
      const VertexUpdator<5> & updator,
      const VertexTrackCompatibilityEstimator<5> & crit,
      const VertexSmoother<5> & smoother,
      const AbstractLTSFactory<5> & ltsf ) :
    theNr(0),
    theLinP(linP.clone()), theUpdator( updator.clone()),
    theSmoother ( smoother.clone() ), theAssProbComputer( ann.clone() ),
    theComp ( crit.clone() ), theLinTrkFactory ( ltsf.clone() )
{
  setParameters();
}

void AdaptiveVertexFitter::setWeightThreshold ( float w )
{
  theWeightThreshold=w;
}

AdaptiveVertexFitter::AdaptiveVertexFitter
                        (const AdaptiveVertexFitter & o ) :
    theMaxShift ( o.theMaxShift ), theMaxLPShift ( o.theMaxLPShift ),
    theMaxStep ( o.theMaxStep ), theWeightThreshold ( o.theWeightThreshold ),
    theNr ( o.theNr ),
    theLinP ( o.theLinP->clone() ), theUpdator ( o.theUpdator->clone() ),
    theSmoother ( o.theSmoother->clone() ),
    theAssProbComputer ( o.theAssProbComputer->clone() ),
    theComp ( o.theComp->clone() ),
    theLinTrkFactory ( o.theLinTrkFactory->clone() )
{}

AdaptiveVertexFitter::~AdaptiveVertexFitter()
{
  delete theLinP;
  delete theUpdator;
  delete theSmoother;
  delete theAssProbComputer;
  delete theComp;
  delete theLinTrkFactory;
}

void AdaptiveVertexFitter::setParameters( double maxshift, double maxlpshift, 
                                          unsigned maxstep, double weightthreshold )
{
  theMaxShift = maxshift;
  theMaxLPShift = maxlpshift;
  theMaxStep = maxstep;
  theWeightThreshold=weightthreshold;
}


void AdaptiveVertexFitter::setParameters ( const edm::ParameterSet & s )
{
    setParameters ( s.getParameter<double>("maxshift"),
                    s.getParameter<double>("maxlpshift"),
                    s.getParameter<int>("maxstep"),
                    s.getParameter<double>("weightthreshold") );
}

CachingVertex<5>
AdaptiveVertexFitter::vertex(const vector<reco::TransientTrack> & tracks) const
{
  if ( tracks.size() < 2 )
  {
    LogError("RecoVertex/AdaptiveVertexFitter")
      << "Supplied fewer than two tracks. Vertex is invalid.";
    return CachingVertex<5>(); // return invalid vertex
  };
  // Linearization Point
  GlobalPoint linP(0.,0.,0.);
    linP = theLinP->getLinearizationPoint(tracks);
  // Initial vertex seed, with a very large error matrix
  AlgebraicSymMatrix we(3,1);
  GlobalError error( we * initialError );
  VertexState seed (linP, error);
  vector<RefCountedVertexTrack> vtContainer = linearizeTracks(tracks, seed);
  return fit(vtContainer, seed, false);
}

CachingVertex<5>
AdaptiveVertexFitter::vertex(const vector<RefCountedVertexTrack> & tracks) const
{
  if ( tracks.size() < 2 )
  {
    LogError("RecoVertex/AdaptiveVertexFitter")
      << "Supplied fewer than two tracks. Vertex is invalid.";
    return CachingVertex<5>(); // return invalid vertex
  };
  // Initial vertex seed, with a very small weight matrix
  GlobalPoint linP = tracks[0]->linearizedTrack()->linearizationPoint();
  AlgebraicSymMatrix we(3,1);
  GlobalError error( we * initialError );
  VertexState seed (linP, error);
  return fit(tracks, seed, false);
}

CachingVertex<5>
AdaptiveVertexFitter::vertex(const vector<RefCountedVertexTrack> & tracks, const reco::BeamSpot & spot ) const
{
  if ( tracks.size() < 1 )
  {
    LogError("RecoVertex/AdaptiveVertexFitter")
      << "Supplied no tracks. Vertex is invalid.";
    return CachingVertex<5>(); // return invalid vertex
  };
  VertexState beamSpotState(spot);
  return fit(tracks, beamSpotState, true );
}



/** Fit vertex out of a set of reco::TransientTracks.
 *  Uses the specified linearization point.
 */
CachingVertex<5>
AdaptiveVertexFitter::vertex(const vector<reco::TransientTrack> & tracks,
                  const GlobalPoint& linPoint) const
{
  if ( tracks.size() < 2 )
  {
    LogError("RecoVertex/AdaptiveVertexFitter")
      << "Supplied fewer than two tracks. Vertex is invalid.";
    return CachingVertex<5>(); // return invalid vertex
  };
  // Initial vertex seed, with a very large error matrix
  AlgebraicSymMatrix we(3,1);
  GlobalError error( we * initialError );
  VertexState seed (linPoint, error);
  vector<RefCountedVertexTrack> vtContainer = linearizeTracks(tracks, seed);
  return fit(vtContainer, seed, false);
}


/** Fit vertex out of a set of TransientTracks. 
 *  The specified BeamSpot will be used as priot, but NOT for the linearization.
 *  The specified LinearizationPointFinder will be used to find the linearization point.
 */
CachingVertex<5> 
AdaptiveVertexFitter::vertex(const vector<reco::TransientTrack> & tracks,
			       const reco::BeamSpot& beamSpot) const
{
  if ( tracks.size() < 1 )
  {
    LogError("RecoVertex/AdaptiveVertexFitter")
      << "Supplied no tracks. Vertex is invalid.";
    return CachingVertex<5>(); // return invalid vertex
  };

  VertexState beamSpotState(beamSpot);
  vector<RefCountedVertexTrack> vtContainer;

  if (tracks.size() > 1) {
    // Linearization Point search if there are more than 1 track
    GlobalPoint linP(0.,0.,0.);
      linP = theLinP->getLinearizationPoint(tracks);
    AlgebraicSymMatrix we(3,1);
    // AlgebraicSymMatrix33 we;
    // we(0,0)=1; we(1,1)=1; we(2,2);
    GlobalError error(we*10000);
    VertexState lpState(linP, error);
    vtContainer = linearizeTracks(tracks, lpState);
  } else {
    // otherwise take the beamspot position.
    vtContainer = linearizeTracks(tracks, beamSpotState);
  }

  return fit(vtContainer, beamSpotState, true);
}


/** Fit vertex out of a set of reco::TransientTracks.
 *   Uses the position as both the linearization point AND as prior
 *   estimate of the vertex position. The error is used for the
 *   weight of the prior estimate.
 */
CachingVertex<5> AdaptiveVertexFitter::vertex(const vector<reco::TransientTrack> & tracks,
                  const GlobalPoint& priorPos,
                  const GlobalError& priorError) const

{
  if ( tracks.size() < 1 )
  {
    LogError("RecoVertex/AdaptiveVertexFitter")
      << "Supplied no tracks. Vertex is invalid.";
    return CachingVertex<5>(); // return invalid vertex
  };
  VertexState seed (priorPos, priorError);
  vector<RefCountedVertexTrack> vtContainer = linearizeTracks(tracks, seed);
  return fit( vtContainer, seed, true );
}


/** Fit vertex out of a set of VertexTracks
 *   Uses the position and error for the prior estimate of the vertex.
 *   This position is not used to relinearize the tracks.
 */
CachingVertex<5> AdaptiveVertexFitter::vertex(
                const vector<RefCountedVertexTrack> & tracks,
                  const GlobalPoint& priorPos,
                  const GlobalError& priorError) const
{
  if ( tracks.size() < 1 )
  {
    LogError("RecoVertex/AdaptiveVertexFitter")
      << "Supplied no tracks. Vertex is invalid.";
    return CachingVertex<5>(); // return invalid vertex
  };
  VertexState seed (priorPos, priorError);
  return fit(tracks, seed, true);
}


/**
 * Construct a container of VertexTrack from a set of reco::TransientTracks.
 * As this is the first iteration of the adaptive fit, the initial error
 * does not enter in the computation of the weights.
 * This is to avoid that all tracks get the same weight when
 * using a very large initial error matrix.
 */
vector<AdaptiveVertexFitter::RefCountedVertexTrack>
AdaptiveVertexFitter::linearizeTracks(const vector<reco::TransientTrack> & tracks,
                                      const VertexState & seed ) const
{
  GlobalPoint linP = seed.position();
  vector<RefCountedLinearizedTrackState> lTracks;
  for(vector<reco::TransientTrack>::const_iterator i = tracks.begin();
      i != tracks.end(); ++i )
  {
    try {
      RefCountedLinearizedTrackState lTrData
        = theLinTrkFactory->linearizedTrackState(linP, *i);
      lTracks.push_back(lTrData);
    } catch ( exception & e ) {
      LogWarning("RecoVertex/AdaptiveVertexFitter") 
        << "Exception " << e.what() << " in ::linearizeTracks."
        << "Your future vertex has just lost a track.";
    };
  }
  AlgebraicSymMatrix we(3,0);
  GlobalError nullError( we );
  VertexState initialSeed (linP, nullError);
  return weightTracks(lTracks, initialSeed);
}

/**
 * Construct new a container of VertexTrack with a new linearization point
 * and vertex seed, from an existing set of VertexTrack, from which only the
 * recTracks will be used.
 */
vector<AdaptiveVertexFitter::RefCountedVertexTrack>
AdaptiveVertexFitter::reLinearizeTracks(
                                const vector<RefCountedVertexTrack> & tracks,
                                const CachingVertex<5> & vertex ) const
{
  VertexState seed = vertex.vertexState();
  GlobalPoint linP = seed.position();
  vector<RefCountedLinearizedTrackState> lTracks;
  for(vector<RefCountedVertexTrack>::const_iterator i = tracks.begin();
    i != tracks.end(); i++)
  {
    try {
      RefCountedLinearizedTrackState lTrData
        = theLinTrkFactory->linearizedTrackState( linP, (**i).linearizedTrack()->track() );
      /*
      RefCountedLinearizedTrackState lTrData =
              (**i).linearizedTrack()->stateWithNewLinearizationPoint(linP);
              */
      lTracks.push_back(lTrData);
    } catch ( exception & e ) {
      LogWarning("RecoVertex/AdaptiveVertexFitter") 
        << "Exception " << e.what() << " in ::relinearizeTracks. "
        << "Will not relinearize this track.";
      lTracks.push_back ( (**i).linearizedTrack() );
    };
  };
  return reWeightTracks(lTracks, vertex );
}

AdaptiveVertexFitter * AdaptiveVertexFitter::clone() const
{
  return new AdaptiveVertexFitter( * this );
}

float AdaptiveVertexFitter::getWeight ( float chi2 ) const
{
  float weight = theAssProbComputer->weight(chi2);

  if ( weight > 1.0 )
  {
    LogWarning("RecoVertex/AdaptiveVertexFitter") << "Weight " << weight << " > 1.0!";
    weight=1.0;
  };

  if ( weight < 0.0 )
  {
    LogWarning("RecoVertex/AdaptiveVertexFitter") << "Weight " << weight << " < 0.0!";
    weight=0.0;
  };
  return weight;
}

vector<AdaptiveVertexFitter::RefCountedVertexTrack>
AdaptiveVertexFitter::reWeightTracks(
                    const vector<RefCountedLinearizedTrackState> & lTracks,
                    const CachingVertex<5> & vertex ) const
{
  VertexState seed = vertex.vertexState();
  theNr++;
  // GlobalPoint pos = seed.position();

  vector<RefCountedVertexTrack> finalTracks;
  VertexTrackFactory<5> vTrackFactory;
  #ifdef STORE_WEIGHTS
  iter++;
  #endif
  for(vector<RefCountedLinearizedTrackState>::const_iterator i
        = lTracks.begin(); i != lTracks.end(); i++)
  {
    float weight=0.;
    try {
      float chi2 = theComp->estimate ( vertex, *i );
      weight=getWeight ( chi2 );
    } catch ( cms::Exception & c ) {
      cout << "[AdaptiveVertexFitter] Aiee! " << c.what() << endl;
      LogWarning("AdaptiveVertexFitter" ) << "When reweighting, a track threw \"" 
                                          << c.what() << "\". Will add this track with w=0.";
    }
    
    RefCountedVertexTrack vTrData
       = vTrackFactory.vertexTrack(*i, seed, weight );

    #ifdef STORE_WEIGHTS
    map < string, dataharvester::MultiType > m;
    m["chi2"]=chi2;
    m["w"]=theAssProbComputer->weight(chi2);
    m["T"]=theAssProbComputer->currentTemp();
    m["n"]=iter;
    m["pos"]="reweight";
    m["id"]=getId ( *i );
    dataharvester::Writer::file("w.txt").save ( m );
    #endif

    finalTracks.push_back(vTrData);
  }
  return finalTracks;
}

vector<AdaptiveVertexFitter::RefCountedVertexTrack>
AdaptiveVertexFitter::weightTracks(
                    const vector<RefCountedLinearizedTrackState> & lTracks,
                    const VertexState & seed ) const
{
  theNr++;
  GlobalPoint pos = seed.position();
  /** track weighting, as opposed to re-weighting, must always 
   * be done with a reset annealer! */
  theAssProbComputer->resetAnnealing();

  vector<RefCountedVertexTrack> finalTracks;
  VertexTrackFactory<5> vTrackFactory;
  KalmanChiSquare computer;
  #ifdef STORE_WEIGHTS
  iter++;
  #endif
  for(vector<RefCountedLinearizedTrackState>::const_iterator i
        = lTracks.begin(); i != lTracks.end(); i++)
  {
    float weight = 0.;
    try {
      float chi2 = computer.estimate ( pos, *i );
      weight = getWeight ( chi2 );
    } catch ( cms::Exception & c ) {
      cout << "[AdaptiveVertexFitter] Aiee! " << c.what() << endl;
      LogWarning ("AdaptiveVertexFitter" ) << "When weighting a track, track threw \"" << c.what()
                                           << " will add with w=0.";
    }

    RefCountedVertexTrack vTrData
       = vTrackFactory.vertexTrack(*i, seed, weight );
    #ifdef STORE_WEIGHTS
    map < string, dataharvester::MultiType > m;
    m["chi2"]=chi2;
    m["w"]=theAssProbComputer->weight(chi2);
    m["T"]=theAssProbComputer->currentTemp();
    m["n"]=iter;
    m["id"]=getId ( *i );
    m["pos"]="weight";
    dataharvester::Writer::file("w.txt").save ( m );
    #endif
    finalTracks.push_back(vTrData);
  }
  return finalTracks;
}

/**
 * Construct new a container of VertexTrack with new weights
 * accounting for vertex error, from an existing set of VertexTracks.
 * From these the LinearizedTracks will be reused.
 */
vector<AdaptiveVertexFitter::RefCountedVertexTrack>
AdaptiveVertexFitter::reWeightTracks(
                            const vector<RefCountedVertexTrack> & tracks,
                            const CachingVertex<5> & seed) const
{
  vector<RefCountedLinearizedTrackState> lTracks;
  for(vector<RefCountedVertexTrack>::const_iterator i = tracks.begin();
    i != tracks.end(); i++)
  {
    lTracks.push_back((**i).linearizedTrack());
  }

  return reWeightTracks(lTracks, seed);
}


/*
 * The method where the vertex fit is actually done!
 */

CachingVertex<5>
AdaptiveVertexFitter::fit(const vector<RefCountedVertexTrack> & tracks,
                          const VertexState & priorSeed,
                          bool withPrior) const
{
  theAssProbComputer->resetAnnealing();
  vector<RefCountedVertexTrack> initialTracks;
  GlobalPoint priorVertexPosition = priorSeed.position();
  GlobalError priorVertexError = priorSeed.error();

  CachingVertex<5> returnVertex( priorVertexPosition,priorVertexError,
                              initialTracks,0);
  if (withPrior)
  {
    returnVertex = CachingVertex<5>(priorVertexPosition,priorVertexError,
                    priorVertexPosition,priorVertexError,initialTracks,0);
  }

  vector<RefCountedVertexTrack> globalVTracks = tracks;

  // main loop through all the VTracks
  int lpStep = 0; int step = 0;

  CachingVertex<5> initialVertex = returnVertex;

  GlobalPoint newPosition = priorVertexPosition;
  GlobalPoint previousPosition = newPosition;

  int ns_trks=0; // number of significant tracks.
  // If we have only two significant tracks, we return an invalid vertex

  do {
    ns_trks=0;
    CachingVertex<5> fVertex = initialVertex;
    if ((previousPosition - newPosition).transverse() > theMaxLPShift)
    {
      // relinearize and reweight.
      // (reLinearizeTracks also reweights tracks)
      globalVTracks = reLinearizeTracks( globalVTracks,
                             returnVertex );
      lpStep++;
    } else if (step) {
      // reweight, if it is not the first step
      globalVTracks = reWeightTracks( globalVTracks,
                                      returnVertex );
    }
    // update sequentially the vertex estimate
    CachingVertex<5> nVertex;
    for(vector<RefCountedVertexTrack>::const_iterator i
          = globalVTracks.begin(); i != globalVTracks.end(); i++)
    {
      try {
        nVertex = theUpdator->add( fVertex, *i );
      } catch ( cms::Exception & c ) {
        edm::LogWarning("AdaptiveVertexFitter" ) << "when updating, received " << c.what()
                                                 << " final result might miss the info of a track.";
      }
      if (nVertex.isValid()) {
        if ( (**i).weight() >= theWeightThreshold )
        {
          ns_trks++;
        };
	fVertex = nVertex;
      } else {
        LogWarning("RecoVertex/AdaptiveVertexFitter") 
          << "The updator returned an invalid vertex when adding track "
          << i-globalVTracks.begin() <<endl
	  << "Your vertex might just have lost one good track.";
      };
    }
    previousPosition = newPosition;
    newPosition = fVertex.position();
    returnVertex = fVertex;
    theAssProbComputer->anneal();
    step++;
    if ( step >= theMaxStep ) break;

  } while (
      // repeat as long as
      // - vertex moved too much or
      // - we're not yet annealed
         ( ((previousPosition - newPosition).mag() > theMaxShift) ||
           (!(theAssProbComputer->isAnnealed()) ) ) ) ;

  if ( theWeightThreshold > 0. &&  ns_trks < 2 && !withPrior ) 
  {
    LogDebug("RecoVertex/AdaptiveVertexFitter") 
      << "fewer than two significant tracks (w>" << theWeightThreshold << ")."
      << " Fitted vertex is invalid.";
    return CachingVertex<5>(); // return invalid vertex
  }

  #ifdef STORE_WEIGHTS
  map < string, dataharvester::MultiType > m;
  m["chi2"]=chi2;
  m["w"]=theAssProbComputer->weight(chi2);
  m["T"]=theAssProbComputer->currentTemp();
  m["n"]=iter;
  m["id"]=getId ( *i );
  m["pos"]="final";
  dataharvester::Writer::file("w.txt").save ( m );
  #endif
  return theSmoother->smooth( returnVertex );
}
