/*
 * RecoTauCleaner
 *
 * Author: Evan K. Friis, UC Davis
 *
 * Given a input collection of PFTaus, produces an output collection of PFTaus
 * such that no two PFTaus come from the same PFJet.  If multiple taus in the
 * collection come from the same PFJet, (dirty) they are ordered according to a
 * list of cleaners.  Each cleaner is a RecoTauCleanerPlugin, and returns a
 * double corresponding to the 'quality' for a given tau - an example would be
 * the level of isolation.  The set of dirty taus is then ranked
 * lexicographically by these cleaners, and the best one is placed in the
 * output collection.
 *
 * $Id $
 */

#include <boost/ptr_container/ptr_vector.hpp>
#include <algorithm>

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "RecoTauTag/RecoTau/interface/RecoTauBuilderPlugins.h"
#include "RecoTauTag/RecoTau/interface/RecoTauCleaningTools.h"
#include "RecoTauTag/RecoTau/interface/RecoTauCommonUtilities.h"

#include "DataFormats/TauReco/interface/PFTau.h"
#include "DataFormats/TauReco/interface/PFTauFwd.h"

template<typename Prod>
class RecoTauCleanerImpl : public edm::EDProducer {
  typedef reco::tau::RecoTauCleanerPlugin Cleaner;
  typedef boost::ptr_vector<Cleaner> CleanerList;
  // Define our output type - i.e. reco::PFTau OR reco::PFTauRef
  typedef typename Prod::value_type output_type;

  // Predicate that determines if two taus 'overlap' i.e. share a base PFJet
  class RemoveDuplicateJets {
    public:
      bool operator()(const reco::PFTauRef& a, const reco::PFTauRef& b)
      { return a->jetRef() == b->jetRef(); }
  };

  public:
    explicit RecoTauCleanerImpl(const edm::ParameterSet& pset);
    ~RecoTauCleanerImpl() {}
    void produce(edm::Event& evt, const edm::EventSetup& es);

  private:
    // Define scoring predicate for taus
    typedef reco::tau::RecoTauLexicographicalRanking<
      CleanerList, reco::PFTauRef> Predicate;
    std::auto_ptr<Predicate> predicate_;
    edm::InputTag tauSrc_;
    CleanerList cleaners_;
};

template<typename Prod>
RecoTauCleanerImpl<Prod>::RecoTauCleanerImpl(const edm::ParameterSet& pset) {
  tauSrc_ = pset.getParameter<edm::InputTag>("src");
  // Build our list of quality plugins
  typedef std::vector<edm::ParameterSet> VPSet;
  // Get each of our tau builders
  const VPSet& cleaners = pset.getParameter<VPSet>("cleaners");
  for (VPSet::const_iterator cleanerPSet = cleaners.begin();
      cleanerPSet != cleaners.end(); ++cleanerPSet) {
    // Get plugin name
    const std::string& pluginType =
      cleanerPSet->getParameter<std::string>("plugin");
    // Build the plugin
    cleaners_.push_back(
        RecoTauCleanerPluginFactory::get()->create(pluginType, *cleanerPSet));
  }

  // Build the predicate that ranks our taus.  The predicate takes a list of
  // cleaners, and uses them to create a lexicographic ranking.
  predicate_ = std::auto_ptr<Predicate>(new Predicate(cleaners_));
  produces<Prod>();
}

namespace {
// Template to convert a ref to desired output type
template<typename T> const T& convert(const reco::PFTauRef &tau);

template<> const reco::PFTauRef&
convert<reco::PFTauRef>(const reco::PFTauRef &tau) { return tau; }

template<> const reco::PFTau&
convert<reco::PFTau>(const reco::PFTauRef &tau) { return *tau; }
}

template<typename Prod>
void RecoTauCleanerImpl<Prod>::produce(edm::Event& evt,
                                   const edm::EventSetup& es) {
  // Update all our cleaners with the event info if they need it
  for (CleanerList::iterator cleaner = cleaners_.begin();
      cleaner != cleaners_.end(); ++cleaner) {
    cleaner->setup(evt, es);
  }

  // Get the input collection to clean
  edm::Handle<reco::CandidateView> input;
  evt.getByLabel(tauSrc_, input);

  // Cast the input candidates to Refs to real taus
  reco::PFTauRefVector inputRefs =
      reco::tau::castView<reco::PFTauRefVector>(input);

  // Make an STL algorithm friendly vector of refs
  typedef std::vector<reco::PFTauRef> PFTauRefs;
  // Collection of all taus. Some are from the same PFJet. We must clean them.
  PFTauRefs dirty;
  dirty.reserve(inputRefs.size());
  std::copy(inputRefs.begin(), inputRefs.end(), std::back_inserter(dirty));

  // Sort the input tau refs according to our predicate
  std::sort(dirty.begin(), dirty.end(), *predicate_);

  // Clean the taus, ensuring that only one tau per jet is produced
  PFTauRefs cleanTaus = reco::tau::cleanOverlaps<PFTauRefs,
            RemoveDuplicateJets>(dirty);

  // create output collection
  std::auto_ptr<Prod> output(new Prod());
  output->reserve(cleanTaus.size());

  // Copy clean refs into output
  for (PFTauRefs::const_iterator tau = cleanTaus.begin();
       tau != cleanTaus.end(); ++tau) {
    output->push_back(convert<output_type>(*tau));
  }
  evt.put(output);
}

typedef RecoTauCleanerImpl<reco::PFTauCollection> RecoTauCleaner;
typedef RecoTauCleanerImpl<reco::PFTauRefVector> RecoTauRefCleaner;

#include "FWCore/Framework/interface/MakerMacros.h"
DEFINE_FWK_MODULE(RecoTauCleaner);
DEFINE_FWK_MODULE(RecoTauRefCleaner);

