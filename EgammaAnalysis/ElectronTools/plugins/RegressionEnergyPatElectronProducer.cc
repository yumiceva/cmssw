#include "EgammaAnalysis/ElectronTools/plugins/RegressionEnergyPatElectronProducer.h"

#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/ConfigurationDescriptions.h"
#include "FWCore/ParameterSet/interface/ParameterSetDescription.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "DataFormats/PatCandidates/interface/Electron.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"
#include "EgammaAnalysis/ElectronTools/interface/SuperClusterHelper.h"

#include <iostream>

using namespace edm ;
using namespace std ;
using namespace reco ;
using namespace pat ;

RegressionEnergyPatElectronProducer::RegressionEnergyPatElectronProducer( const edm::ParameterSet & cfg )
{

  produces<ElectronCollection>();

  inputPatElectrons = cfg.getParameter<edm::InputTag>("inputPatElectronsTag");
  energyRegressionType = cfg.getParameter<uint32_t>("energyRegressionType");
  regressionInputFile = cfg.getParameter<std::string>("regressionInputFile");
  debug = cfg.getParameter<bool>("debug");
  geomInitialized_ = false;

  //****************************************************************************************
  //set up regression calculator
  //****************************************************************************************
  regressionEvaluator = new ElectronEnergyRegressionEvaluate();

  //set regression type
  ElectronEnergyRegressionEvaluate::ElectronEnergyRegressionType type = ElectronEnergyRegressionEvaluate::kNoTrkVar;
  if (energyRegressionType == 1) type = ElectronEnergyRegressionEvaluate::kNoTrkVar;
  else if (energyRegressionType == 2) type = ElectronEnergyRegressionEvaluate::kWithTrkVar;

  //load weights and initialize
  regressionEvaluator->initialize(regressionInputFile.c_str(),type);

}
 
RegressionEnergyPatElectronProducer::~RegressionEnergyPatElectronProducer()
{
  delete regressionEvaluator;
}

void RegressionEnergyPatElectronProducer::produce( edm::Event & event, const edm::EventSetup & setup )
{

  assert(regressionEvaluator->isInitialized());
  if (!geomInitialized_) {
    edm::ESHandle<CaloTopology> theCaloTopology;
    setup.get<CaloTopologyRecord>().get(theCaloTopology);
    ecalTopology_ = & (*theCaloTopology);
    
    edm::ESHandle<CaloGeometry> theCaloGeometry;
    setup.get<CaloGeometryRecord>().get(theCaloGeometry); 
    caloGeometry_ = & (*theCaloGeometry);
    geomInitialized_ = true;
  }


  //**************************************************************************
  //Get Number of Vertices
  //**************************************************************************
  Handle<reco::VertexCollection> hVertexProduct;
  event.getByLabel(edm::InputTag("goodPrimaryVertices"),hVertexProduct);
  const reco::VertexCollection inVertices = *(hVertexProduct.product());  

  // loop through all vertices
  Int_t nvertices = 0;
  for (reco::VertexCollection::const_iterator inV = inVertices.begin(); 
       inV != inVertices.end(); ++inV) {

    //pass these vertex cuts
    if (inV->ndof() >= 4
        && inV->position().Rho() <= 2.0
        && fabs(inV->z()) <= 24.0
	) {
      nvertices++;
    }
  }

  //**************************************************************************
  //Get Rho
  //**************************************************************************
  double rho = 0;
  Handle<double> hRhoKt6PFJets;
  event.getByLabel(edm::InputTag("kt6PFJets","rho"), hRhoKt6PFJets);
  rho = (*hRhoKt6PFJets);

  edm::Handle<edm::View<reco::Candidate> > oldElectrons ;
  event.getByLabel(inputPatElectrons,oldElectrons) ;

  std::auto_ptr<ElectronCollection> electrons( new ElectronCollection ) ;

  ElectronCollection::const_iterator electron ;
  ElectronCollection::iterator ele ;
  // first clone the initial collection
  for(edm::View<reco::Candidate>::const_iterator ele=oldElectrons->begin(); ele!=oldElectrons->end(); ++ele) {    
    const pat::ElectronRef elecsRef = edm::RefToBase<reco::Candidate>(oldElectrons,ele-oldElectrons->begin()).castTo<pat::ElectronRef>();
    pat::Electron clone = *edm::RefToBase<reco::Candidate>(oldElectrons,ele-oldElectrons->begin()).castTo<pat::ElectronRef>();
    electrons->push_back(clone);
  }

  for
    ( ele = electrons->begin() ;
      ele != electrons->end() ;
      ++ele ) 
    {     

      SuperClusterHelper mySCHelper(&(*ele),ele->recHits(),ecalTopology_,caloGeometry_);
      
      bool printDebug = debug;
      if (printDebug) {
	std::cout << "***********************************************************************\n";
	std::cout << "Run Lumi Event: " << event.id().run() << " " << event.luminosityBlock() << " " << event.id().event() << "\n";
	std::cout << "Pat Electron : " << ele->pt() << " " << ele->eta() << " " << ele->phi() << "\n";
      }

      // apply regression energy
      Double_t FinalMomentum = 0;
      Double_t FinalMomentumError = 0;
      Double_t RegressionMomentum = 0;
      Double_t RegressionMomentumError = 0;

      if (energyRegressionType == 1) {
	RegressionMomentum = regressionEvaluator->regressionValueNoTrkVar( mySCHelper.rawEnergy(),
									   mySCHelper.eta(),
									   mySCHelper.phi(),
									   mySCHelper.etaWidth(),
									   mySCHelper.phiWidth(),
									   mySCHelper.clustersSize(),
									   mySCHelper.hadronicOverEm(),
									   mySCHelper.r9(),
									   rho, 
									   nvertices, 
									   mySCHelper.seedEta(),
									   mySCHelper.seedPhi(),
									   mySCHelper.seedEnergy(),
									   mySCHelper.e3x3(),
									   mySCHelper.e5x5(),
									   mySCHelper.sigmaIetaIeta(),
									   mySCHelper.spp(),
									   mySCHelper.sep(),
									   mySCHelper.eMax(),
									   mySCHelper.e2nd(),
									   mySCHelper.eTop(),
									   mySCHelper.eBottom(),
									   mySCHelper.eLeft(),
									   mySCHelper.eRight(),
									   mySCHelper.e2x5Max(),
									   mySCHelper.e2x5Top(),
									   mySCHelper.e2x5Bottom(),
									   mySCHelper.e2x5Left(),
									   mySCHelper.e2x5Right(),
									   mySCHelper.ietaSeed(),
									   mySCHelper.iphiSeed(),
									   mySCHelper.etaCrySeed(),
									   mySCHelper.phiCrySeed(),
									   mySCHelper.preshowerEnergy(),
									   printDebug);
	RegressionMomentumError = regressionEvaluator->regressionUncertaintyNoTrkVar( 
										     mySCHelper.rawEnergy(),
										     mySCHelper.eta(),
										     mySCHelper.phi(),
										     mySCHelper.etaWidth(),
										     mySCHelper.phiWidth(),
										     mySCHelper.clustersSize(),
										     mySCHelper.hadronicOverEm(),
										     mySCHelper.r9(),
										     rho, 
										     nvertices, 
										     mySCHelper.seedEta(),
										     mySCHelper.seedPhi(),
										     mySCHelper.seedEnergy(),
										     mySCHelper.e3x3(),
										     mySCHelper.e5x5(),
										     mySCHelper.sigmaIetaIeta(),
										     mySCHelper.spp(),
										     mySCHelper.sep(),
										     mySCHelper.eMax(),
										     mySCHelper.e2nd(),
										     mySCHelper.eTop(),
										     mySCHelper.eBottom(),
										     mySCHelper.eLeft(),
										     mySCHelper.eRight(),
										     mySCHelper.e2x5Max(),
										     mySCHelper.e2x5Top(),
										     mySCHelper.e2x5Bottom(),
										     mySCHelper.e2x5Left(),
										     mySCHelper.e2x5Right(),
										     mySCHelper.ietaSeed(),
										     mySCHelper.iphiSeed(),
										     mySCHelper.etaCrySeed(),
										     mySCHelper.phiCrySeed(),
										     mySCHelper.preshowerEnergy(),
										     printDebug);
      
	// PAT method
	ele->setEcalRegressionEnergy(RegressionMomentum, RegressionMomentumError); 
	// GsfElectron method -> should it be called ? 
	//	ele->correctEcalEnergy(RegressionMomentum, RegressionMomentumError);

      } else if (energyRegressionType == 2) {
	RegressionMomentum = regressionEvaluator->regressionValueWithTrkVar(ele->p(),
									    mySCHelper.rawEnergy(),
									    mySCHelper.eta(),
									    mySCHelper.phi(),
									    mySCHelper.etaWidth(),
									    mySCHelper.phiWidth(),
									    mySCHelper.clustersSize(),
									    mySCHelper.hadronicOverEm(),
									    mySCHelper.r9(),
									    rho, 
									    nvertices, 
									    mySCHelper.seedEta(),
									    mySCHelper.seedPhi(),
									    mySCHelper.seedEnergy(),
									    mySCHelper.e3x3(),
									    mySCHelper.e5x5(),
									    mySCHelper.sigmaIetaIeta(),
									    mySCHelper.spp(),
									    mySCHelper.sep(),
									    mySCHelper.eMax(),
									    mySCHelper.e2nd(),
									    mySCHelper.eTop(),
									    mySCHelper.eBottom(),
									    mySCHelper.eLeft(),
									    mySCHelper.eRight(),
									    mySCHelper.e2x5Max(),
									    mySCHelper.e2x5Top(),
									    mySCHelper.e2x5Bottom(),
									    mySCHelper.e2x5Left(),
									    mySCHelper.e2x5Right(),
									    ele->pt(),
									    ele->trackMomentumAtVtx().R(),
									    ele->fbrem(),
									    ele->charge(),
									    ele->eSuperClusterOverP(),
									    mySCHelper.ietaSeed(),
									    mySCHelper.iphiSeed(),
									    mySCHelper.etaCrySeed(),
									    mySCHelper.phiCrySeed(),
									    mySCHelper.preshowerEnergy(),
									    printDebug);
	RegressionMomentumError = regressionEvaluator->regressionUncertaintyWithTrkVar(
										       ele->p(),
										       mySCHelper.rawEnergy(),
										       mySCHelper.eta(),
										       mySCHelper.phi(),
										       mySCHelper.etaWidth(),
										       mySCHelper.phiWidth(),
										       mySCHelper.clustersSize(),
										       mySCHelper.hadronicOverEm(),
										       mySCHelper.r9(),
										       rho, 
										       nvertices, 
										       mySCHelper.seedEta(),
										       mySCHelper.seedPhi(),
										       mySCHelper.seedEnergy(),
										       mySCHelper.e3x3(),
										       mySCHelper.e5x5(),
										       mySCHelper.sigmaIetaIeta(),
										       mySCHelper.spp(),
										       mySCHelper.sep(),
										       mySCHelper.eMax(),
										       mySCHelper.e2nd(),
										       mySCHelper.eTop(),
										       mySCHelper.eBottom(),
										       mySCHelper.eLeft(),
										       mySCHelper.eRight(),
										       mySCHelper.e2x5Max(),
										       mySCHelper.e2x5Top(),
										       mySCHelper.e2x5Bottom(),
										       mySCHelper.e2x5Left(),
										       mySCHelper.e2x5Right(),
										       ele->pt(),
										       ele->trackMomentumAtVtx().R(),
										       ele->fbrem(),
										       ele->charge(),
										       ele->eSuperClusterOverP(),
										       mySCHelper.ietaSeed(),
										       mySCHelper.iphiSeed(),
										       mySCHelper.etaCrySeed(),
										       mySCHelper.phiCrySeed(),
										       mySCHelper.preshowerEnergy(),
										       printDebug);
	FinalMomentum = RegressionMomentum;
	FinalMomentumError = RegressionMomentumError;
	math::XYZTLorentzVector oldMomentum = ele->p4();
	math::XYZTLorentzVector newMomentum = math::XYZTLorentzVector
	  ( oldMomentum.x()*FinalMomentum/oldMomentum.t(),
	    oldMomentum.y()*FinalMomentum/oldMomentum.t(),
	    oldMomentum.z()*FinalMomentum/oldMomentum.t(),
	    FinalMomentum ) ;
	
	ele->correctMomentum(newMomentum,ele->trackMomentumError(),FinalMomentumError);
	ele->correctEcalEnergy(RegressionMomentum, RegressionMomentumError);
	
      } else {
	cout << "Error: RegressionType = " << energyRegressionType << " is not supported.\n";
      }
    
				    
    }
  event.put(electrons) ;
     
}
  
  
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/ESProducer.h"
#include "FWCore/Framework/interface/ModuleFactory.h"
DEFINE_FWK_MODULE(RegressionEnergyPatElectronProducer);
