#ifndef TrackTSelector_h
#define TrackTSelector_h
/** \class TrackTSelector
 *
 * Simple interactive analysis example based on FWLite TSelector
 * accessing EDM data
 *
 * \author Luca Lista, INFN
 *
 * $Id: TrackTSelector.h,v 1.2 2006/10/17 08:21:58 llista Exp $
 */
#include "DataFormats/TrackReco/interface/Track.h"
// needed because missing in TFWLiteSelector.h
#include <TFile.h>
#include "FWCore/TFWLiteSelector/interface/TFWLiteSelector.h"
#include "PhysicsTools/ParallelAnalysis/interface/TrackAnalysisAlgorithm.h"

namespace examples {

  class TrackTSelector : 
    public TFWLiteSelector<TrackAnalysisAlgorithm> {
  public :
    /// default constructor
    TrackTSelector();
    /// begin processing
    void begin( TList * & );
    /// terminate processing
    void terminate( TList & );
    
  private:
    /// avoid copy constructor
    TrackTSelector( const TrackTSelector & );
    /// avoid assignment operator
    TrackTSelector operator=( TrackTSelector const & );
  };
  
}

#endif
