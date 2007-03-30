#ifndef L3MuonIsolationProducer_L3MuonIsolationProducer_H
#define L3MuonIsolationProducer_L3MuonIsolationProducer_H

/**  \class L3MuonIsolationProducer
 */

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoMuon/MuonIsolation/interface/Cuts.h"
#include "RecoMuon/MuonIsolation/interface/MuIsoExtractor.h"

#include <string>

namespace edm { class Event; }
namespace edm { class EventSetup; }

class L3MuonIsolationProducer : public edm::EDProducer {

public:

  /// constructor with config
  L3MuonIsolationProducer(const edm::ParameterSet&);
  
  /// destructor
  virtual ~L3MuonIsolationProducer(); 

  /// initialisation
  virtual void beginJob(const edm::EventSetup& iSetup);
  
  /// Produce isolation maps
  virtual void produce(edm::Event&, const edm::EventSetup&);

private:

  edm::ParameterSet theConfig;

  // Muon track Collection Label
  edm::InputTag theMuonCollectionLabel;

  // Isolation cuts
  muonisolation::Cuts theCuts;

  // Option to write MuIsoDeposits into the event
  double optOutputIsoDeposits;

  // MuIsoExtractor
  muonisolation::MuIsoExtractor * theExtractor;

};

#endif
