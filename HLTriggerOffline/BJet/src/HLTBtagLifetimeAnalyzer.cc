#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <algorithm>
#include <cmath>

#include <TFile.h>
#include <TH1.h>
#include <Math/GenVector/VectorUtil.h>

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/Registry.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "SimDataFormats/JetMatching/interface/JetFlavour.h"
#include "SimDataFormats/JetMatching/interface/JetFlavourMatching.h"
#include "DataFormats/Common/interface/View.h"
#include "DataFormats/Common/interface/TriggerResults.h"
#include "DataFormats/JetReco/interface/Jet.h"
#include "DataFormats/JetReco/interface/JetTracksAssociation.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/BTauReco/interface/JetTag.h"
#include "HLTriggerOffline/BJet/interface/JetPlots.h"
#include "HLTriggerOffline/BJet/interface/OfflineJetPlots.h"
#include "HLTriggerOffline/BJet/interface/FlavouredJetPlots.h"
#include "HLTriggerOffline/BJet/interface/VertexPlots.h"
#include "HLTriggerOffline/BJet/interface/RatePlots.h"

// force LogDebug statements to be generated
#define ML_DEBUG

// search the Registry for the ParameterSetID of the ParameterSet describing the given process
// returns an invalid ID if the process name is not found
edm::ParameterSetID psetIdForProcess(const std::string & process) {
  const edm::pset::Registry * registry = edm::pset::Registry::instance();
  for (edm::pset::Registry::const_iterator i = registry->begin(); i != registry->end(); ++i) {
    if (i->second.exists("@process_name") and i->second.getParameter<std::string>("@process_name") == process)
      return i->first;
  }
  return edm::ParameterSetID();
}


// find the index of the object key of an association vector closest to a given jet, within a given distance
template <typename T, typename V>
int closestJet(const reco::Jet & jet, const edm::AssociationVector<T, V> & association, double distance) {
  int closest = -1;
  for (unsigned int i = 0; i < association.size(); ++i) {
    double d = ROOT::Math::VectorUtil::DeltaR(jet.momentum(), association[i].first->momentum());
    if (d < distance) {
      distance = d;
      closest  = i;
    }
  }
  return closest;
}


class HLTBtagLifetimeAnalyzer : public edm::EDAnalyzer {
public:
  explicit HLTBtagLifetimeAnalyzer(const edm::ParameterSet& config);
  virtual ~HLTBtagLifetimeAnalyzer();

  virtual void beginJob();
  virtual void analyze(const edm::Event & event, const edm::EventSetup & setup);
  virtual void endJob();

private:
  bool cachePathDescription(const edm::ParameterSetID & triggerPSetID, const edm::ParameterSetID & processPSetID);
  bool cachePathDescription(const edm::TriggerResults & triggerResult, const std::string & triggerProcessName);

private:
  struct InputData {
    std::string     m_name;                     // name used for the ROOT plots
    std::string     m_title;                    // title shown on the plots
    edm::InputTag   m_filter;                   // filter checked for pass/fail
    edm::InputTag   m_jets;                     // jet collection used for detailed efficiencies
    edm::InputTag   m_tracks;                   // track collection, associated to jets
    unsigned int    m_filterIndex;              // index of the filter relative to its own path
  };

  // input collections
  std::string               m_triggerPath;      // HLT path
  edm::InputTag             m_triggerResults;   // HLT trigger results
  edm::InputTag             m_vertex;           // primary vertex
  std::vector<InputData>    m_levels;

  // match to MC truth
  edm::InputTag             m_mcPartons;        // MC truth match - jet association to partons
  std::vector<std::string>  m_mcLabels;         // MC truth match - labels
  std::vector<flavours_t>   m_mcFlavours;       // MC truth match - flavours selection
  double                    m_mcRadius;         // MC truth match - deltaR association radius
  bool                      m_mcMatching;       // MC truth matching anabled/disabled

  // match to Offline reco
  edm::InputTag             m_offlineBJets;     // Offline match - jet association to discriminator
  std::vector<std::string>  m_offlineLabels;    // Offline match - labels
  std::vector<double>       m_offlineCuts;      // Offline match - discriminator cuts
  double                    m_offlineRadius;    // Offline match - deltaR association radius
  bool                      m_offlineMatching;  // Offline matching anabled/disabled

  // compute efficiencies
  bool m_doStepEfficiencies;
  bool m_doCumulativeEfficiencies;

  // plot configuration
  double m_jetMinEnergy;
  double m_jetMaxEnergy;
  double m_jetMaxEta;

  double m_vertexMaxR;
  double m_vertexMaxZ;

  // plot data
  RatePlots                         m_ratePlots;    // number of events paassing each level
  VertexPlots                       m_vertexPlots;
  std::vector<JetPlots>             m_jetPlots;
  std::vector<FlavouredJetPlots>    m_mcPlots;
  std::vector<OfflineJetPlots>      m_offlinePlots;

  // output configuration
  std::string m_outputFile;

  // keep a cache of the path description
  std::vector<std::string>          m_pathModules;
  unsigned int                      m_pathIndex;
  bool                              m_pathCached;
};


HLTBtagLifetimeAnalyzer::HLTBtagLifetimeAnalyzer(const edm::ParameterSet & config) :
  m_triggerPath( config.getParameter<std::string>("triggerPath") ),
  m_triggerResults( config.getParameter<edm::InputTag>("triggerResults") ),
  m_vertex( config.getParameter<edm::InputTag>("vertex") ),
  m_levels(),
  m_mcPartons( config.getParameter<edm::InputTag>("mcPartons") ),
  m_mcLabels(),
  m_mcFlavours(),
  m_mcRadius( config.getParameter<double>("mcRadius") ),
  m_mcMatching( m_mcPartons.label() != "none" ),
  m_offlineBJets( config.getParameter<edm::InputTag>("offlineBJets") ),
  m_offlineLabels(),
  m_offlineCuts(),
  m_offlineRadius( config.getParameter<double>("offlineRadius") ),
  m_offlineMatching( m_offlineBJets.label() != "none" ),
  m_doStepEfficiencies( config.getParameter<bool>("computeStepEfficiencies") ),
  m_doCumulativeEfficiencies( config.getParameter<bool>("computeCumulativeEfficiencies") ),
  m_jetMinEnergy(  0. ),    //   0 GeV
  m_jetMaxEnergy( 300. ),   // 300 GeV
  m_jetMaxEta( 5. ),        //  ±5 pseudorapidity units
  m_vertexMaxR( 0.1 ),      //   1 mm
  m_vertexMaxZ( 15. ),      //  15 cm
  m_ratePlots(),
  m_vertexPlots(),
  m_jetPlots(),
  m_mcPlots(),
  m_offlinePlots(),
  m_outputFile( config.getParameter<std::string>("outputFile") ),
  m_pathModules(),
  m_pathIndex((unsigned int) -1),
  m_pathCached(false)
{
  const std::vector<edm::ParameterSet> levels = config.getParameter<std::vector<edm::ParameterSet> >("levels");
  for (unsigned int i = 0; i < levels.size(); ++i) {
    InputData level;
    level.m_jets   = levels[i].getParameter<edm::InputTag>("jets");
    level.m_filter = levels[i].getParameter<edm::InputTag>("filter");
    level.m_tracks = levels[i].exists("tracks") ? levels[i].getParameter<edm::InputTag>("tracks") : edm::InputTag("none");
    level.m_name   = levels[i].exists("name")   ? levels[i].getParameter<std::string>("name")     : level.m_jets.encode();
    level.m_title  = levels[i].exists("title")  ? levels[i].getParameter<std::string>("title")    : level.m_name;
    m_levels.push_back(level);
  }

  const edm::ParameterSet & jetConfig = config.getParameter<edm::ParameterSet>("jetConfiguration");
  m_jetMaxEnergy = jetConfig.getParameter<double>("maxEnergy");
  m_jetMaxEta    = jetConfig.getParameter<double>("maxEta");
  const edm::ParameterSet & vertexConfig = config.getParameter<edm::ParameterSet>("vertexConfiguration");
  m_vertexMaxR = vertexConfig.getParameter<double>("maxR");
  m_vertexMaxZ = vertexConfig.getParameter<double>("maxZ");

  edm::ParameterSet mc = config.getParameter<edm::ParameterSet>("mcFlavours");
  m_mcLabels = mc.getParameterNamesForType<std::vector<unsigned int> >();
  for (unsigned int i = 0; i < m_mcLabels.size(); ++i)
    m_mcFlavours.push_back( mc.getParameter<std::vector<unsigned int> >(m_mcLabels[i]) );

  edm::ParameterSet offline = config.getParameter<edm::ParameterSet>("offlineCuts");
  m_offlineLabels = offline.getParameterNamesForType<double>();
  for (unsigned int i = 0; i < m_offlineLabels.size(); ++i)
    m_offlineCuts.push_back( offline.getParameter<double>(m_offlineLabels[i]) );
}

HLTBtagLifetimeAnalyzer::~HLTBtagLifetimeAnalyzer()
{
}

void HLTBtagLifetimeAnalyzer::beginJob()
{
  m_ratePlots.init( "Event", "Event", m_levels.size() );
  m_vertexPlots.init( "PrimaryVertex", "Primary vertex", vertex1DBins, m_vertexMaxZ, m_vertexMaxR );

  m_jetPlots.resize( m_levels.size() );
  if (m_mcMatching)      m_mcPlots.resize( m_levels.size() );
  if (m_offlineMatching) m_offlinePlots.resize( m_levels.size() );

  for (unsigned int i = 0; i < m_levels.size(); ++i) {
    m_jetPlots[i].init( m_levels[i].m_name, m_levels[i].m_title, jetEnergyBins, m_jetMinEnergy, m_jetMaxEnergy, jetGeometryBins, m_jetMaxEta, m_levels[i].m_tracks.label() != "none" );
    if (m_mcMatching)      m_mcPlots[i].init(      m_levels[i].m_name, m_levels[i].m_title, m_mcFlavours,  m_mcLabels,      jetEnergyBins, m_jetMinEnergy, m_jetMaxEnergy, jetGeometryBins, m_jetMaxEta, m_levels[i].m_tracks.label() != "none" );
    if (m_offlineMatching) m_offlinePlots[i].init( m_levels[i].m_name, m_levels[i].m_title, m_offlineCuts, m_offlineLabels, jetEnergyBins, m_jetMinEnergy, m_jetMaxEnergy, jetGeometryBins, m_jetMaxEta, m_levels[i].m_tracks.label() != "none" );
  }
}

// access and cache the description of the HLT path and filters
// find the PSet describing the TriggerResults and Process, and pass them to the real cachePathDescription(...)
bool HLTBtagLifetimeAnalyzer::cachePathDescription(const edm::TriggerResults & triggerResults, const std::string & triggerProcessName)
{
  return cachePathDescription(triggerResults.parameterSetID(), psetIdForProcess(triggerProcessName));
}

// access and cache the description of the HLT path and filters
// this will compute
// - m_pathIndex
// - m_levels[].m_filterIndex
bool HLTBtagLifetimeAnalyzer::cachePathDescription(const edm::ParameterSetID & triggerPSetID, const edm::ParameterSetID & processPSetID)
{
  if (m_pathCached)
    return true;

  const edm::pset::Registry * registry = edm::pset::Registry::instance();
  edm::ParameterSet           pset;
  std::vector<std::string>    paths;

  if (registry->getMapped(triggerPSetID, pset)) {
    paths = pset.getParameter<std::vector<std::string> >("@trigger_paths");
  } else {
    edm::LogWarning("HLTBtagAnalyzer") << "cannot map HLT trigger names to indices";
    return false;
  }
  if (registry->getMapped(processPSetID, pset)) {
    m_pathModules = pset.getParameter<std::vector<std::string> >(m_triggerPath);
  } else {
    edm::LogWarning("HLTBtagAnalyzer") << "cannot find HLT path " << m_triggerPath << " in the process description";
    return false;
  }

  for (m_pathIndex = 0; m_pathIndex < paths.size(); ++m_pathIndex)
    if (paths[m_pathIndex] == m_triggerPath)
      break;
  if (m_pathIndex == paths.size()) {
    edm::LogWarning("HLTBtagAnalyzer") << "cannot find HLT path " << m_triggerPath;
    return false;
  }

  // find the filter index for each "level"
  for (unsigned int l = 0; l < m_levels.size(); ++l) {
    InputData & level = m_levels[l];
    std::vector<std::string>::const_iterator i = std::find(m_pathModules.begin(), m_pathModules.end(), level.m_filter.label());
    if (i != m_pathModules.end()) {
      level.m_filterIndex = i - m_pathModules.begin();
      edm::LogVerbatim("HLTBtagAnalyzer") << "filter " << level.m_filter.label() << " has index " << level.m_filterIndex << " in path " << m_triggerPath;
    } else {
      level.m_filterIndex = 0;
      edm::LogWarning("HLTBtagAnalyzer") << "filter " << level.m_filter.label() << " not found in path " << m_triggerPath;
    }
  }

  m_pathCached = true;
  return true;
}

void HLTBtagLifetimeAnalyzer::analyze(const edm::Event & event, const edm::EventSetup & setup)
{
  edm::Handle<edm::TriggerResults> h_triggerResults;
  event.getByLabel(m_triggerResults, h_triggerResults);
  if (not h_triggerResults.isValid()) {
    edm::LogWarning("HLTBtagAnalyzer") << "invalid edm::TriggerResults handle";
    return;
  }

  if (not cachePathDescription(* h_triggerResults, h_triggerResults.provenance()->processName() )) {
    edm::LogWarning("HLTBtagAnalyzer") << "unable to access trigger informations and description for path " << m_triggerPath;
    return;
  }

  bool         wasrun   = h_triggerResults->wasrun( m_pathIndex );
  unsigned int latest   = h_triggerResults->index( m_pathIndex );
  bool         accepted = h_triggerResults->accept( m_pathIndex );
  if (latest >= m_pathModules.size()) {
    edm::LogWarning("HLTBtagAnalyzer") << "error determinig the path stopping condition: module position exceeds path length";
    return;
  }

  // debug information regarding the path status
  if (not wasrun)
    LogTrace("HLTBtagAnalyzer") << "  path " << m_triggerPath << " was not run";
  else if (accepted)
    LogTrace("HLTBtagAnalyzer") << "  path " << m_triggerPath << " accepted the event";
  else
    LogTrace("HLTBtagAnalyzer") << "  path " << m_triggerPath << " rejected the event at module " << m_pathModules[latest];

  edm::Handle<reco::VertexCollection> h_vertex;
  event.getByLabel(m_vertex, h_vertex);
  if (h_vertex.isValid() and not h_vertex->empty())
    m_vertexPlots.fill(h_vertex->front());

  // match to MC parton flavour - accessed on demand
  edm::Handle<reco::JetFlavourMatchingCollection> h_mcPartons;
  if (m_mcMatching)
    event.getByLabel(m_mcPartons, h_mcPartons);

  // match to Offline b-tagged jets - accessed on demand
  edm::Handle<reco::JetTagCollection> h_offlineBJets;
  if (m_offlineMatching)
    event.getByLabel(m_offlineBJets, h_offlineBJets);

  m_ratePlots.fill(0);
  for (unsigned int l = 0; l < m_levels.size(); ++l) {
    const InputData & level = m_levels[l];

    bool passed = accepted or (latest > level.m_filterIndex);               // accepted by this filter
    bool failed = (not accepted) and (latest == level.m_filterIndex);       // rejected by this filter
    bool notrun = (not accepted) and (latest  < level.m_filterIndex);       // did not reach this filter
    LogDebug("HLTBtagAnalyzer") << "  path " << std::setw(32) << m_triggerPath << ", filter " << std::setw(32) << std::left << level.m_filter.label() << std::right << (passed ? "passed" : failed ? "failed" : "not run");

    edm::Handle<edm::View<reco::Jet> >                  h_jets;
    edm::Handle<reco::JetTracksAssociation::Container>  h_tracks;

    if (level.m_jets.label() != "none")
      event.getByLabel(level.m_jets, h_jets);
    if (level.m_tracks.label() != "none")
      event.getByLabel(level.m_tracks, h_tracks);

    if (passed) {
      // event did pass this filter, analyze the content
      m_ratePlots.fill(l+1);    // 0 for no filters, 1 for 1st filter, ...

      if (not h_jets.isValid()) {
        edm::LogError("HLTBtagAnalyzer") << "Jet handle for " << m_triggerPath << " at " << level.m_title << " with label " << level.m_jets << " not found";
      } else {
        const edm::View<reco::Jet> & jets = * h_jets;
        for (unsigned int j = 0; j < jets.size(); ++j) {
          const reco::Jet & jet = jets[j];

          // match to MC parton
          unsigned int flavour = 0;
          if (m_mcMatching) {
            int m = closestJet(jet, *h_mcPartons, m_mcRadius);
            flavour = (m != -1) ? abs((*h_mcPartons)[m].second.getFlavour()) : 0;
          }

          // match to offline reconstruted b jets - only if they are available
          double discriminator = -INFINITY;
          if (m_offlineMatching) {
            int o = closestJet(jet, *h_offlineBJets, m_offlineRadius);
            discriminator = (o > 0 and (unsigned int) o < h_offlineBJets->size()) ? (*h_offlineBJets)[o].second : -INFINITY;
          }

          if (not h_tracks.isValid()) {
            // no tracks, fill only the jets
            m_jetPlots[l].fill( jet );
            if (m_mcMatching)      m_mcPlots[l].fill( jet, flavour);
            if (m_offlineMatching) m_offlinePlots[l].fill( jet, discriminator );
          } else {
            // fill jets and tracks
            const reco::TrackRefVector & tracks = (*h_tracks)[jets.refAt(j)];
            m_jetPlots[l].fill( jet, tracks );
            if (m_mcMatching)      m_mcPlots[l].fill( jet, tracks, flavour);
            if (m_offlineMatching) m_offlinePlots[l].fill( jet, tracks, discriminator );
          }
        }
      }
    } else {
      // event did not pass this filter, no need to check the following ones
      break;
    }
  }
}

void HLTBtagLifetimeAnalyzer::endJob()
{
  // print event rates
  edm::LogVerbatim("HLTBtagAnalyzer") << "HLT Trigger path: " << m_triggerPath;
  {
    std::stringstream out;
    out << std::left << std::setw(32) << m_triggerPath << ": "
        << std::setw(32) << "total number of events: " << std::right << std::setw(12) << m_ratePlots.rate(0);
    edm::LogVerbatim("HLTBtagAnalyzer") << out.str();
  }
  for (unsigned int i = 0; i < m_levels.size(); ++i) {
    std::stringstream out;
    out << std::left << std::setw(32) << m_triggerPath << ": "
        << std::setw(32) << ("events passing " + m_levels[i].m_title) << std::right << std::setw(12) << m_ratePlots.rate(i+1);
    edm::LogVerbatim("HLTBtagAnalyzer") << out.str();
  }
  if (m_doStepEfficiencies) for (unsigned int i = 0; i < m_levels.size(); ++i) {
    // compute and print step-by-step event efficiencies
    std::stringstream out;
    out << std::left << std::setw(32) << m_triggerPath << ": "
        << std::setw(32) << ("step efficiency at " + m_levels[i].m_title);
    double eff = m_ratePlots.stepEfficiency(i+1);
    if (std::isnormal(eff)) {
      std::pair<double, double> interval = m_ratePlots.stepConfidence(i+1);
      out << std::right << std::setw(11) << std::fixed << std::setprecision(2) << eff * 100. << "%";
      out << " - " << (eff-interval.first) * 100. << "% + " << (interval.second-eff) * 100. << "%";
    } else {
      out << std::right << std::setw(12) << "n/a";
    }
    edm::LogVerbatim("HLTBtagAnalyzer") << out.str();
  }
  if (m_doCumulativeEfficiencies) for (unsigned int i = 0; i < m_levels.size(); ++i) {
    // compute and print cumulative event efficiencies
    std::stringstream out;
    out << std::left << std::setw(32) << m_triggerPath << ": "
        << std::setw(32) << ("cumulative efficiency at " + m_levels[i].m_title);
    double eff = m_ratePlots.efficiency(i+1);
    if (std::isnormal(eff)) {
      std::pair<double, double> interval = m_ratePlots.confidence(i+1);
      out << std::right << std::setw(11) << std::fixed << std::setprecision(2) << eff * 100. << "%";
      out << " - " << (eff-interval.first) * 100. << "% + " << (interval.second-eff) * 100. << "%";
    } else {
      out << std::right << std::setw(12) << "n/a";
    }
    edm::LogVerbatim("HLTBtagAnalyzer") << out.str();
  }
  edm::LogVerbatim("HLTBtagAnalyzer");


  // make eficiency plots
  if ((m_doStepEfficiencies or m_doCumulativeEfficiencies) and m_levels.size() > 1) {
    // make second-vs-first level efficiency plots
    m_jetPlots[1].efficiency( m_jetPlots[0] );
    if (m_mcMatching)
      m_mcPlots[1].efficiency( m_mcPlots[0] );
    if (m_offlineMatching)
      m_offlinePlots[1].efficiency( m_offlinePlots[0] );
  }
  if (m_doStepEfficiencies) for (unsigned int i = 2; i < m_levels.size(); ++i) {
    // make step-by-step efficiency plots
    m_jetPlots[i].efficiency( m_jetPlots[i-1] );
    if (m_mcMatching)
      m_mcPlots[i].efficiency( m_mcPlots[i-1] );
    if (m_offlineMatching)
      m_offlinePlots[i].efficiency( m_offlinePlots[i-1] );
  }
  if (m_doCumulativeEfficiencies) for (unsigned int i = 2; i < m_levels.size(); ++i) {
    // make cumulative efficiency plots
    m_jetPlots[i].efficiency( m_jetPlots[0] );
    if (m_mcMatching)
      m_mcPlots[i].efficiency( m_mcPlots[0] );
    if (m_offlineMatching)
      m_offlinePlots[i].efficiency( m_offlinePlots[0] );
  }
}


#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(HLTBtagLifetimeAnalyzer);
