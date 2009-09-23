#ifndef TrackingTools_TrackRefitter_TracksToTrajectories_H
#define TrackingTools_TrackRefitter_TracksToTrajectories_H

/** \class TracksToTrajectories
 *  This class, which is a EDProducer, takes a reco::TrackCollection from the Event and refits the rechits 
 *  strored in the reco::Tracks. The final result is a std::vector of Trajectories (objs of the type "Trajectory"), 
 *  which is loaded into the Event in a transient way
 *
 *  $Date: 2007/04/20 09:40:59 $
 *  $Revision: 1.1 $
 *  \author R. Bellan - INFN Torino <riccardo.bellan@cern.ch>
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

namespace edm {class ParameterSet; class Event; class EventSetup;}
class TrackTransformerBase;

class TracksToTrajectories: public edm::EDProducer{
public:

  /// Constructor
  TracksToTrajectories(const edm::ParameterSet&);

  /// Destructor
  virtual ~TracksToTrajectories();
  
  // Operations

  /// Convert a reco::TrackCollection into std::vector<Trajectory>
  virtual void produce(edm::Event&, const edm::EventSetup&);
  
 protected:
  
 private:
  
  edm::InputTag theTracksLabel;
  TrackTransformerBase *theTrackTransformer;
};
#endif

