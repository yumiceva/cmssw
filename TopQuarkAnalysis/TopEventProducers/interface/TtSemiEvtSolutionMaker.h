//
// $Id: TtSemiEvtSolutionMaker.h,v 1.13 2007/07/06 02:49:41 lowette Exp $
//

#ifndef TopEventProducers_TtSemiEvtSolutionMaker_h
#define TopEventProducers_TtSemiEvtSolutionMaker_h

#include "FWCore/Framework/interface/EDProducer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

#include <vector>
#include <string>


class TtSemiKinFitterEtThetaPhi;
class TtSemiKinFitterEtEtaPhi;
class TtSemiKinFitterEMom;
class TtSemiSimpleBestJetComb;
class TtSemiLRJetCombObservables;
class TtSemiLRJetCombCalc;
class TtSemiLRSignalSelObservables;
class TtSemiLRSignalSelCalc;


class TtSemiEvtSolutionMaker : public edm::EDProducer {

  public:

    explicit TtSemiEvtSolutionMaker(const edm::ParameterSet & iConfig);
    ~TtSemiEvtSolutionMaker();

    virtual void produce(edm::Event & iEvent, const edm::EventSetup & iSetup);

  private:

    // configurables
    edm::InputTag electronSrc_;
    edm::InputTag muonSrc_;
    edm::InputTag metSrc_;
    edm::InputTag lJetSrc_;
    edm::InputTag bJetSrc_;
    std::string leptonFlavour_;
    std::string lrSignalSelFile_, lrJetCombFile_;
    bool addLRSignalSel_, addLRJetComb_, doKinFit_, matchToGenEvt_;
    int maxNrIter_;
    double maxDeltaS_, maxF_;
    int param_;
    std::vector<int> lrSignalSelObs_, lrJetCombObs_, constraints_;
    // tools
    TtSemiKinFitterEtThetaPhi    * myKinFitterEtThetaPhi;
    TtSemiKinFitterEtEtaPhi      * myKinFitterEtEtaPhi;
    TtSemiKinFitterEMom          * myKinFitterEMom;
    TtSemiSimpleBestJetComb      * mySimpleBestJetComb;
    TtSemiLRJetCombObservables   * myLRJetCombObservables;
    TtSemiLRJetCombCalc          * myLRJetCombCalc;
    TtSemiLRSignalSelObservables * myLRSignalSelObservables;
    TtSemiLRSignalSelCalc        * myLRSignalSelCalc;

};


#endif
