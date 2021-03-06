#include "TauAnalysis/CandidateTools/interface/ParticleAntiOverlapSelector.h"

#include "CommonTools/UtilAlgos/interface/ObjectSelector.h"

#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/PatCandidates/interface/Muon.h"
#include "DataFormats/PatCandidates/interface/Tau.h"
#include "DataFormats/PatCandidates/interface/Jet.h"

typedef ObjectSelector<ParticleAntiOverlapSelector<pat::Electron> > PATElectronAntiOverlapSelector;
typedef ObjectSelector<ParticleAntiOverlapSelector<pat::Muon> > PATMuonAntiOverlapSelector;
typedef ObjectSelector<ParticleAntiOverlapSelector<pat::Tau> > PATTauAntiOverlapSelector;
typedef ObjectSelector<ParticleAntiOverlapSelector<pat::Jet> > PATJetAntiOverlapSelector;

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PATElectronAntiOverlapSelector);
DEFINE_FWK_MODULE(PATMuonAntiOverlapSelector);
DEFINE_FWK_MODULE(PATTauAntiOverlapSelector);
DEFINE_FWK_MODULE(PATJetAntiOverlapSelector);
