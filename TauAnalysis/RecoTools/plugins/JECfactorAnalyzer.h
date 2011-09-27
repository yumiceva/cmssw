#ifndef TauAnalysis_RecoTools_JECfactorAnalyzer_h
#define TauAnalysis_RecoTools_JECfactorAnalyzer_h

/** \executable JECfactorAnalyzer
 *
 * Make plots of jet energy correction (JEC) factors
 * as function of jetPt, in different bins of jetEta
 *
 * \author Christian Veelken, UC Davis
 *
 * \version $Revision: 1.6 $
 *
 * $Id: JECfactorAnalyzer.cc,v 1.6 2011/09/09 10:25:36 veelken Exp $
 *
 */

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "CondFormats/JetMETObjects/interface/FactorizedJetCorrector.h"

class JECfactorAnalyzer : public edm::EDAnalyzer
{
 public:

  explicit JECfactorAnalyzer(const edm::ParameterSet&);
  virtual ~JECfactorAnalyzer();
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob();

 private:

  double etaMin_;
  double etaMax_;
  double etaStepSize_;
  double ptMin_;
  double ptMax_;
  double ptStepSize_;

  typedef std::vector<std::string> vstring;
  vstring payloads_;

  vstring levels_;

  double yMin_;
  double yMax_;

  std::string outputFileName_;

  bool hasRun_;
};

#endif

