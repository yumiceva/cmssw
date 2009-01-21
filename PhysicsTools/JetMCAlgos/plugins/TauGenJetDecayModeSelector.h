#ifndef PhysicsTools_JetMCAlgos_TauGenJetDecayModeSelector_h
#define PhysicsTools_JetMCAlgos_TauGenJetDecayModeSelector_h

/** \class TauGenJetDecayModeSelector
 *
 * Selection of GenJet objects representing generated tau-decays 
 * by tau decay mode
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.10 $
 *
 * $Id: GenEventSelector.h,v 1.10 2007/06/18 18:33:52 llista Exp $
 *
 */

#include "PhysicsTools/UtilAlgos/interface/SingleObjectSelector.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"

class TauGenJetDecayModeSelectorImp
{
 public:
  // constructor 
  explicit TauGenJetDecayModeSelectorImp(const edm::ParameterSet&);
  
  bool operator()(const reco::GenJet&) const;

 private:
  typedef std::vector<std::string> vstring;
  vstring selectedTauDecayModes_;
};

typedef SingleObjectSelector<
            reco::GenJetCollection,
            TauGenJetDecayModeSelectorImp
        > TauGenJetDecayModeSelector;

#endif
