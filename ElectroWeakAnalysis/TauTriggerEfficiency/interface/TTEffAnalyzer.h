// -*- C++ -*-
//
// Package:    TTEffAnalyzer
// Class:      TTEffAnalyzer
// 
/**\class TTEffAnalyzer TTEffAnalyzer.cc ElectroWeakAnalysis/TTEffAnalyzer/src/TTEffAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Chi Nhan Nguyen
//         Created:  Wed Oct  1 13:04:54 CEST 2008
// $Id: TTEffAnalyzer.h,v 1.1 2008/10/01 13:09:30 chinhan Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "ElectroWeakAnalysis/TauTriggerEfficiency/interface/L1TauEfficiencyAnalyzer.h"


//
class TTEffAnalyzer : public edm::EDAnalyzer {
   public:
      explicit TTEffAnalyzer(const edm::ParameterSet&);
      ~TTEffAnalyzer();

      TTree *_TTEffTree;
      TFile *_TTEffFile;


   private:
      virtual void beginJob(const edm::EventSetup&) ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      virtual void fillPFTau(const reco::PFTau&); 

      // ----------member data ---------------------------
      edm::InputTag  PFTaus_; //Path to analyze
      std::string rootFile_;

      // PF Variables
      int NEGCandsInAnnulus,NHadCandsInAnnulus;
      float PFPt, PFEt,PFEta,PFPhi,PFEGIsolEt,PFHighestClusterEt;

      L1TauEfficiencyAnalyzer _L1analyzer;
};

