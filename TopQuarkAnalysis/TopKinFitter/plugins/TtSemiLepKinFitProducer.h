#ifndef TtSemiLepKinFitProducer_h
#define TtSemiLepKinFitProducer_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "PhysicsTools/JetMCUtils/interface/combination.h"
#include "AnalysisDataFormats/TopObjects/interface/TtSemiLepEvtPartons.h"
#include "TopQuarkAnalysis/TopKinFitter/interface/TtSemiLepKinFitter.h"

template <typename LeptonCollection>
class TtSemiLepKinFitProducer : public edm::EDProducer {
  
 public:
  
  explicit TtSemiLepKinFitProducer(const edm::ParameterSet&);
  ~TtSemiLepKinFitProducer();
  
 private:
  // produce
  virtual void produce(edm::Event&, const edm::EventSetup&);

  // convert unsigned to Param
  TtSemiLepKinFitter::Param param(unsigned);
  // convert unsigned to Param
  TtSemiLepKinFitter::Constraint constraint(unsigned);
  // convert unsigned to Param
  std::vector<TtSemiLepKinFitter::Constraint> constraints(std::vector<unsigned>&);
  
  edm::InputTag jets_;
  edm::InputTag leps_;
  edm::InputTag mets_;
  
  edm::InputTag match_;
  bool useOnlyMatch_;
  
  int maxNJets_;
  int maxNComb_;
  
  unsigned int maxNrIter_;
  double maxDeltaS_;
  double maxF_;
  unsigned int jetParam_;
  unsigned int lepParam_;
  unsigned int metParam_;
  std::vector<unsigned> constraints_;

  TtSemiLepKinFitter* fitter;

  struct KinFitResult {
    int Status;
    double Chi2;
    double Prob;
    pat::Particle HadB;
    pat::Particle HadP;
    pat::Particle HadQ;
    pat::Particle LepB;
    pat::Particle LepL;
    pat::Particle LepN;
    std::vector<int> JetCombi;
    bool operator< (const KinFitResult& rhs) { return Chi2 < rhs.Chi2; };
  };
};

template<typename LeptonCollection>
TtSemiLepKinFitProducer<LeptonCollection>::TtSemiLepKinFitProducer(const edm::ParameterSet& cfg):
  jets_        (cfg.getParameter<edm::InputTag>("jets")),
  leps_        (cfg.getParameter<edm::InputTag>("leps")),
  mets_        (cfg.getParameter<edm::InputTag>("mets")),
  match_       (cfg.getParameter<edm::InputTag>("match")),
  useOnlyMatch_(cfg.getParameter<bool>             ("useOnlyMatch"      )),
  maxNJets_    (cfg.getParameter<int>              ("maxNJets"          )),
  maxNComb_    (cfg.getParameter<int>              ("maxNComb"          )),
  maxNrIter_   (cfg.getParameter<unsigned>         ("maxNrIter"         )),
  maxDeltaS_   (cfg.getParameter<double>           ("maxDeltaS"         )),
  maxF_        (cfg.getParameter<double>           ("maxF"              )),
  jetParam_    (cfg.getParameter<unsigned>         ("jetParametrisation")),
  lepParam_    (cfg.getParameter<unsigned>         ("lepParametrisation")),
  metParam_    (cfg.getParameter<unsigned>         ("metParametrisation")),
  constraints_ (cfg.getParameter<std::vector<unsigned> >("constraints"  ))
{
  fitter = new TtSemiLepKinFitter(param(jetParam_), param(lepParam_), param(metParam_), maxNrIter_, maxDeltaS_, maxF_, constraints(constraints_));

  produces< std::vector<pat::Particle> >("PartonsHadP");
  produces< std::vector<pat::Particle> >("PartonsHadQ");
  produces< std::vector<pat::Particle> >("PartonsHadB");
  produces< std::vector<pat::Particle> >("PartonsLepB");
  produces< std::vector<pat::Particle> >("Leptons");
  produces< std::vector<pat::Particle> >("Neutrinos");

  produces< std::vector<std::vector<int> > >();
  produces< std::vector<double> >("Chi2");
  produces< std::vector<double> >("Prob");
  produces< std::vector<int> >("Status");
}

template<typename LeptonCollection>
TtSemiLepKinFitProducer<LeptonCollection>::~TtSemiLepKinFitProducer()
{
  delete fitter;
}

template<typename LeptonCollection>
void TtSemiLepKinFitProducer<LeptonCollection>::produce(edm::Event& evt, const edm::EventSetup& setup)
{
  std::auto_ptr< std::vector<pat::Particle> > pPartonsHadP( new std::vector<pat::Particle> );
  std::auto_ptr< std::vector<pat::Particle> > pPartonsHadQ( new std::vector<pat::Particle> );
  std::auto_ptr< std::vector<pat::Particle> > pPartonsHadB( new std::vector<pat::Particle> );
  std::auto_ptr< std::vector<pat::Particle> > pPartonsLepB( new std::vector<pat::Particle> );
  std::auto_ptr< std::vector<pat::Particle> > pLeptons    ( new std::vector<pat::Particle> );
  std::auto_ptr< std::vector<pat::Particle> > pNeutrinos  ( new std::vector<pat::Particle> );

  std::auto_ptr< std::vector<std::vector<int> > > pCombi ( new std::vector<std::vector<int> > );
  std::auto_ptr< std::vector<double>            > pChi2  ( new std::vector<double> );
  std::auto_ptr< std::vector<double>            > pProb  ( new std::vector<double> );
  std::auto_ptr< std::vector<int>               > pStatus( new std::vector<int> );

  edm::Handle<std::vector<pat::Jet> > jets;
  evt.getByLabel(jets_, jets);

  edm::Handle<std::vector<pat::MET> > mets;
  evt.getByLabel(mets_, mets);

  edm::Handle<LeptonCollection> leps;
  evt.getByLabel(leps_, leps);

  unsigned int nPartons = 4;

  edm::Handle<std::vector<int> > match;
  bool unvalidMatch = false;
  if(useOnlyMatch_) {
    evt.getByLabel(match_, match);
    // check if match is valid
    if(match->size()!=nPartons) unvalidMatch=true;
    else {
      for(unsigned int idx=0; idx<jets->size(); ++idx) {
	if(idx<0 || idx>=jets->size()) {
	  unvalidMatch=true;
	  break;
	}
      }
    }
  }

  // -----------------------------------------------------
  // skip events with no appropriate lepton candidate in
  // or empty MET or less jets than partons or unvalid match
  // -----------------------------------------------------

  if( leps->empty() || mets->empty() || jets->size()<nPartons || unvalidMatch ) {
    // the kinFit getters return empty objects here
    pPartonsHadP->push_back( fitter->fittedHadP()     );
    pPartonsHadQ->push_back( fitter->fittedHadQ()     );
    pPartonsHadB->push_back( fitter->fittedHadB()     );
    pPartonsLepB->push_back( fitter->fittedLepB()     );
    pLeptons    ->push_back( fitter->fittedLepton()   );
    pNeutrinos  ->push_back( fitter->fittedNeutrino() );
    // indices referring to the jet combination
    std::vector<int> invalidCombi;
    for(unsigned int i = 0; i < nPartons; ++i) 
      invalidCombi.push_back( -1 );
    pCombi->push_back( invalidCombi );
    // chi2
    pChi2->push_back( -1. );
    // chi2 probability
    pProb->push_back( -1. );
    // status of the fitter
    pStatus->push_back( -1 );
    // feed out all products
    evt.put(pCombi);
    evt.put(pPartonsHadP, "PartonsHadP");
    evt.put(pPartonsHadQ, "PartonsHadQ");
    evt.put(pPartonsHadB, "PartonsHadB");
    evt.put(pPartonsLepB, "PartonsLepB");
    evt.put(pLeptons    , "Leptons"    );
    evt.put(pNeutrinos  , "Neutrinos"  );
    evt.put(pChi2       , "Chi2"       );
    evt.put(pProb       , "Prob"       );
    evt.put(pStatus     , "Status"     );
    return;
  }

  // -----------------------------------------------------
  // analyze different jet combinations using the KinFitter
  // (or only a given jet combination if useOnlyMatch=true)
  // -----------------------------------------------------
  
  std::vector<int> jetIndices;
  if(!useOnlyMatch_) {
    for(unsigned int i=0; i<jets->size(); ++i){
      if(maxNJets_ >= (int) nPartons && maxNJets_ == (int) i) break;
      jetIndices.push_back(i);
    }
  }
  
  std::vector<int> combi;
  for(unsigned int i=0; i<nPartons; ++i) {
    if(useOnlyMatch_) combi.push_back( (*match)[i] );
    else combi.push_back(i);
  }

  std::list<KinFitResult> FitResultList;

  do{
    for(int cnt = 0; cnt < TMath::Factorial( combi.size() ); ++cnt){
      // take into account indistinguishability of the two jets from the hadr. W decay,
      // reduces combinatorics by a factor of 2
      if( combi[TtSemiLepEvtPartons::LightQ] < combi[TtSemiLepEvtPartons::LightQBar]
	 || useOnlyMatch_ ) {
	
	std::vector<pat::Jet> jetCombi;
	jetCombi.resize(nPartons);
	jetCombi[TtSemiLepEvtPartons::LightQ   ] = (*jets)[combi[TtSemiLepEvtPartons::LightQ   ]];
	jetCombi[TtSemiLepEvtPartons::LightQBar] = (*jets)[combi[TtSemiLepEvtPartons::LightQBar]];
	jetCombi[TtSemiLepEvtPartons::HadB     ] = (*jets)[combi[TtSemiLepEvtPartons::HadB     ]];
	jetCombi[TtSemiLepEvtPartons::LepB     ] = (*jets)[combi[TtSemiLepEvtPartons::LepB     ]];

	// do the kinematic fit
	int status = fitter->fit(jetCombi, (*leps)[0], (*mets)[0]);

	if( status != -10 ) { // skip this jet combination if kinematic fit was aborted
	                      // (due to errors during fitting)
	  KinFitResult result;
	  result.Status = status;
	  result.Chi2 = fitter->fitS();
	  result.Prob = fitter->fitProb();
	  result.HadB = fitter->fittedHadB();
	  result.HadP = fitter->fittedHadP();
	  result.HadQ = fitter->fittedHadQ();
	  result.LepB = fitter->fittedLepB();
	  result.LepL = fitter->fittedLepton();
	  result.LepN = fitter->fittedNeutrino();
	  result.JetCombi = combi;

	  FitResultList.push_back(result);
	}

      }
      if(useOnlyMatch_) break; // don't go through combinatorics if useOnlyMatch was chosen
      next_permutation( combi.begin(), combi.end() );
    }
    if(useOnlyMatch_) break; // don't go through combinatorics if useOnlyMatch was chosen
  }
  while(stdcomb::next_combination( jetIndices.begin(), jetIndices.end(), combi.begin(), combi.end() ));

  // sort results w.r.t. chi2 values
  FitResultList.sort();
  
  // -----------------------------------------------------
  // feed out result
  // starting with the JetComb having the smallest chi2
  // -----------------------------------------------------

  if( FitResultList.size() < 1 ) { // in case no fit results were stored in the list (all fits aborted)
    pPartonsHadP->push_back( fitter->fittedHadP()     );
    pPartonsHadQ->push_back( fitter->fittedHadQ()     );
    pPartonsHadB->push_back( fitter->fittedHadB()     );
    pPartonsLepB->push_back( fitter->fittedLepB()     );
    pLeptons    ->push_back( fitter->fittedLepton()   );
    pNeutrinos  ->push_back( fitter->fittedNeutrino() );
    // indices referring to the jet combination
    std::vector<int> invalidCombi;
    for(unsigned int i = 0; i < nPartons; ++i) 
      invalidCombi.push_back( -1 );
    pCombi->push_back( invalidCombi );
    // chi2
    pChi2->push_back( -1. );
    // chi2 probability
    pProb->push_back( -1. );
    // status of the fitter
    pStatus->push_back( -1 );
  }
  else {
    unsigned int iComb = 0;
    for(typename std::list<KinFitResult>::const_iterator result = FitResultList.begin(); result != FitResultList.end(); ++result) {
      if(maxNComb_ >= 1 && iComb == (unsigned int) maxNComb_) break;
      iComb++;
      // partons
      pPartonsHadP->push_back( result->HadP );
      pPartonsHadQ->push_back( result->HadQ );
      pPartonsHadB->push_back( result->HadB );
      pPartonsLepB->push_back( result->LepB );
      // lepton
      pLeptons->push_back( result->LepL );
      // neutrino
      pNeutrinos->push_back( result->LepN );
      // indices referring to the jet combination
      pCombi->push_back( result->JetCombi );
      // chi2
      pChi2->push_back( result->Chi2 );
      // chi2 probability
      pProb->push_back( result->Prob );
      // status of the fitter
      pStatus->push_back( result->Status );
    }
  }
  evt.put(pCombi);
  evt.put(pPartonsHadP, "PartonsHadP");
  evt.put(pPartonsHadQ, "PartonsHadQ");
  evt.put(pPartonsHadB, "PartonsHadB");
  evt.put(pPartonsLepB, "PartonsLepB");
  evt.put(pLeptons    , "Leptons"    );
  evt.put(pNeutrinos  , "Neutrinos"  );
  evt.put(pChi2       , "Chi2"       );
  evt.put(pProb       , "Prob"       );
  evt.put(pStatus     , "Status"     );
}
 
template<typename LeptonCollection>
TtSemiLepKinFitter::Param TtSemiLepKinFitProducer<LeptonCollection>::param(unsigned val) 
{
  TtSemiLepKinFitter::Param result;
  switch(val){
  case TtSemiLepKinFitter::kEMom       : result=TtSemiLepKinFitter::kEMom;       break;
  case TtSemiLepKinFitter::kEtEtaPhi   : result=TtSemiLepKinFitter::kEtEtaPhi;   break;
  case TtSemiLepKinFitter::kEtThetaPhi : result=TtSemiLepKinFitter::kEtThetaPhi; break;
  default: 
    throw cms::Exception("WrongConfig") 
      << "Chosen jet parametrization is not supported: " << val << "\n";
    break;
  }
  return result;
} 

template<typename LeptonCollection>
TtSemiLepKinFitter::Constraint TtSemiLepKinFitProducer<LeptonCollection>::constraint(unsigned val) 
{
  TtSemiLepKinFitter::Constraint result;
  switch(val){
  case TtSemiLepKinFitter::kWHadMass     : result=TtSemiLepKinFitter::kWHadMass;     break;
  case TtSemiLepKinFitter::kWLepMass     : result=TtSemiLepKinFitter::kWLepMass;     break;
  case TtSemiLepKinFitter::kTopHadMass   : result=TtSemiLepKinFitter::kTopHadMass;   break;
  case TtSemiLepKinFitter::kTopLepMass   : result=TtSemiLepKinFitter::kTopLepMass;   break;
  case TtSemiLepKinFitter::kNeutrinoMass : result=TtSemiLepKinFitter::kNeutrinoMass; break;
  default: 
    throw cms::Exception("WrongConfig") 
      << "Chosen fit constraint is not supported: " << val << "\n";
    break;
  }
  return result;
} 

template<typename LeptonCollection>
std::vector<TtSemiLepKinFitter::Constraint> TtSemiLepKinFitProducer<LeptonCollection>::constraints(std::vector<unsigned>& val)
{
  std::vector<TtSemiLepKinFitter::Constraint> result;
  for(unsigned i=0; i<val.size(); ++i){
    result.push_back(constraint(val[i]));
  }
  return result; 
}

#endif
