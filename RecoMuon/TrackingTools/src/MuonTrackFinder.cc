/** \class MuonTrackFinder
 *  Concrete Track finder for the Muon Reco
 *
 *  $Date: 2006/06/21 17:36:51 $
 *  $Revision: 1.8 $
 *  \author R. Bellan - INFN Torino
 */

#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Handle.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "DataFormats/TrajectorySeed/interface/TrajectorySeedCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"

#include "TrackingTools/PatternTools/interface/TSCPBuilderNoMaterial.h"

#include "RecoMuon/TrackingTools/interface/MuonTrackFinder.h"
#include "RecoMuon/TrackingTools/interface/MuonTrajectoryBuilder.h"
#include "RecoMuon/TrackingTools/interface/MuonTrajectoryCleaner.h"
#include "RecoMuon/TrackingTools/interface/MuonTrackLoader.h"

using namespace std;

// constructor
MuonTrackFinder::MuonTrackFinder(MuonTrajectoryBuilder *ConcreteMuonTrajectoryBuilder,
				 MuonTrackLoader *concreteMuonTrackLoader):
  theTrajBuilder(ConcreteMuonTrajectoryBuilder),theTrackLoader(concreteMuonTrackLoader)
{
  theTrajCleaner = new MuonTrajectoryCleaner();
}

// Destructor
MuonTrackFinder::~MuonTrackFinder(){
  LogDebug("Muon|RecoMuon|MuonTrackFinder")<<"MuonTrackFinder destructor called"<<endl;
  delete theTrajBuilder;
  delete theTrajCleaner;
  delete theTrackLoader;
};

// percolate the event setup
void MuonTrackFinder::setES(const edm::EventSetup &eSetup){
  theTrajBuilder->setES(eSetup);
}

// percolate the event setup
void MuonTrackFinder::setEvent(const edm::Event &event){
  theTrajBuilder->setEvent(event);
}

// convert the trajectories into tracks and load them in to the event
void MuonTrackFinder::load(const TrajectoryContainer &trajectories, 
			   edm::Event &event){
  theTrackLoader->loadTracks(trajectories, event);
}

// Reconstruct trajectories
void MuonTrackFinder::reconstruct(const edm::Handle<TrajectorySeedCollection>& seeds,
				  edm::Event &event,
				  const edm::EventSetup &eSetup){

  std::string metname = "Muon|RecoMuon|MuonTrackFinder";
  
  // Percolate the event 
  LogDebug(metname)<<"Event setup percolation"<<endl;
  setEvent(event);

  // Percolate the event setup
  LogDebug(metname)<<"Event percolation"<<endl;
  setES(eSetup);
  
  // Traj container
  TrajectoryContainer muonTrajectories;
  
  // reconstruct the traj
  for(TrajectorySeedCollection::const_iterator seed = seeds->begin();
      seed != seeds->end(); seed++){
    TrajectoryContainer muonTrajs_temp = theTrajBuilder->trajectories(*seed);
    for(TrajectoryContainer::const_iterator it = muonTrajs_temp.begin(); 
	it != muonTrajs_temp.end(); it++) 
      muonTrajectories.push_back(*it); 
  }

  // clean the clone traj
  LogDebug(metname)<<"Clean the trajectories container"<<endl;
  theTrajCleaner->clean(muonTrajectories); //used by reference...
  
  // convert the trajectories into tracks and load them in to the event
  LogDebug(metname)
    <<"Convert the trajectories into tracks and load them in to the event"<<endl;
  load(muonTrajectories,event);
}
