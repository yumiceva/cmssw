#ifndef TauAnalysis_CandidateTools_NSVfitTauToMuInputAnalyzer_h
#define TauAnalysis_CandidateTools_NSVfitTauToMuInputAnalyzer_h

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Utilities/interface/InputTag.h"

#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/CandidateTools/interface/NSVfitEventVertexRefitter.h"
#include "TauAnalysis/CandidateTools/interface/NSVfitDecayVertexFitter.h"

#include <string>

class NSVfitTauToMuInputAnalyzer : public edm::EDAnalyzer 
{
 public:
  // constructor 
  explicit NSVfitTauToMuInputAnalyzer(const edm::ParameterSet&);
    
  // destructor
  ~NSVfitTauToMuInputAnalyzer();
    
 private:
  void beginJob();
  void analyze(const edm::Event&, const edm::EventSetup&);

  std::string moduleLabel_;

  edm::InputTag srcGenParticles_;
  edm::InputTag srcElectrons_;
  edm::InputTag srcMuons_;
  edm::InputTag srcTaus_;
  edm::InputTag srcRecVertex_;

  typedef std::vector<edm::InputTag> vInputTag;
  vInputTag srcWeights_;

  NSVfitEventVertexRefitter* eventVertexFitAlgorithm_;
  NSVfitDecayVertexFitter* decayVertexFitAlgorithm_;

  std::string dqmDirectory_;

  MonitorElement* genTauPt_;
  MonitorElement* genTauEta_;
  MonitorElement* genTauPhi_;
  MonitorElement* genTauVisEnFrac_;
  MonitorElement* genTauDecayDistance_;
  MonitorElement* genTauDecayDistanceNormalized_;
  MonitorElement* genTau_phi_lab_;
  MonitorElement* genTau_gjAngle_;

  MonitorElement* genMuonPt_;
  MonitorElement* genMuonEta_;
  MonitorElement* genMuonPhi_;

  MonitorElement* recMuonDeltaPt_absolute_;
  MonitorElement* recMuonDeltaPt_relative_;
  MonitorElement* recMuonDeltaEta_;
  MonitorElement* recMuonDeltaPhi_;
  MonitorElement* recMuonDeltaVisMass_absolute_;
  MonitorElement* recMuonDeltaVisMass_relative_;

  MonitorElement* recLeadTrackDeltaPt_absolute_;
  MonitorElement* recLeadTrackDeltaPt_relative_;
  MonitorElement* recLeadTrackDeltaEta_;
  MonitorElement* recLeadTrackDeltaPhi_;
  MonitorElement* recLeadTrackNumHits_;
  MonitorElement* recLeadTrackNumPixelHits_;
  MonitorElement* recLeadTrackNormalizedChi2_;
  MonitorElement* recLeadTrackDCA_;
  MonitorElement* recLeadTrackPCApull3d_;
};

#endif   
