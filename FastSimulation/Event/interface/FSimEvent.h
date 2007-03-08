#ifndef FastSimulation_Event_FSimEvent_H
#define FastSimulation_Event_FSimEvent_H

// CMSSW Headers
#include "DataFormats/Provenance/interface/EventID.h"
#include "SimDataFormats/Track/interface/SimTrackContainer.h"
#include "SimDataFormats/Vertex/interface/SimVertexContainer.h"

// FAMOS Headers
#include "FastSimulation/Event/interface/FBaseSimEvent.h"
 
/** The FAMOS SimEvent: inherits from FBaseSimEvent,
 *  where the latter provides FAMOS-specific event features (splitting
 *  proposed by Maya STAVRIANAKOU)
 *
 * An FSimEvent contains, at filling time, only particles from the 
 * GenEvent it is being filled with. Material Effects then 
 * update its content, so that it resembles the output of Geant
 * at the end of the material effect processing.
 *
 * Important : All distances are in mm 
 *
 * \author Patrick Janot, CERN
 * \date: 9-Dec-2003
 *
 */

class RandomEngine;

class FSimEvent : public FBaseSimEvent {

public:

  /// Default constructor
  FSimEvent(const edm::ParameterSet& kine);

  FSimEvent(const edm::ParameterSet& vtx,
	    const edm::ParameterSet& kine,
	    const RandomEngine* engine);

  ///  usual virtual destructor
  virtual ~FSimEvent();

  /// fill the FBaseSimEvent from the current HepMC::GenEvent
  void fill(const HepMC::GenEvent & hev, edm::EventID & Id);

  /// fill the FBaseSimEvent from the SimTrack's and SimVert'ices
  void fill(const std::vector<SimTrack>& simTracks, 
	  const std::vector<SimVertex>& simVertices);

  ///Method to return the EventId
  virtual edm::EventID id() const;

  ///Method to return the event weight
  virtual float weight() const;
    
  /// Number of tracks
  virtual unsigned int nTracks() const;
  /// Number of vertices
  virtual unsigned int nVertices() const;
  /// Number of MC particles
  virtual unsigned int nGenParts() const;

  /// Load containers of tracks and vertices for the edm::Event
  void load(edm::SimTrackContainer & c) const;
  void load(edm::SimVertexContainer & c) const;

private:

  edm::EventID id_;
  double weight_;

};

#endif // FSIMEVENT_H
