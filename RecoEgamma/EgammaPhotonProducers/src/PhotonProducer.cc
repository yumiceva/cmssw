#include <iostream>
#include <vector>
#include <memory>

// Framework
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Utilities/interface/Exception.h"

#include "Geometry/Records/interface/CaloGeometryRecord.h"
#include "Geometry/CaloGeometry/interface/CaloSubdetectorGeometry.h"
#include "Geometry/CaloTopology/interface/CaloTopology.h"
#include "Geometry/CaloEventSetup/interface/CaloTopologyRecord.h"


#include "DataFormats/VertexReco/interface/Vertex.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/EgammaReco/interface/ClusterShape.h"
#include "DataFormats/EgammaCandidates/interface/PhotonCore.h"
#include "DataFormats/EgammaCandidates/interface/Photon.h"
#include "DataFormats/EgammaCandidates/interface/PhotonFwd.h"
#include "DataFormats/EgammaCandidates/interface/Conversion.h"

#include "DataFormats/EgammaReco/interface/ElectronSeed.h"
#include "RecoCaloTools/Selectors/interface/CaloConeSelector.h"
#include "RecoEgamma/EgammaPhotonProducers/interface/PhotonProducer.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaTowerIsolation.h"
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionBaseClass.h" 
#include "RecoEcal/EgammaCoreTools/interface/EcalClusterFunctionFactory.h" 
#include "RecoEcal/EgammaCoreTools/plugins/EcalClusterCrackCorrection.h"
#include "RecoEgamma/EgammaIsolationAlgos/interface/EgammaHadTower.h"

PhotonProducer::PhotonProducer(const edm::ParameterSet& config) : 

  conf_(config)
{

  // use onfiguration file to setup input/output collection names

  photonCoreProducer_   = conf_.getParameter<edm::InputTag>("photonCoreProducer");
  barrelEcalHits_   = conf_.getParameter<edm::InputTag>("barrelEcalHits");
  endcapEcalHits_   = conf_.getParameter<edm::InputTag>("endcapEcalHits");
  vertexProducer_   = conf_.getParameter<std::string>("primaryVertexProducer");
  hcalTowers_ = conf_.getParameter<edm::InputTag>("hcalTowers");
  hOverEConeSize_   = conf_.getParameter<double>("hOverEConeSize");
  highEt_        = conf_.getParameter<double>("highEt");
  // R9 value to decide converted/unconverted
  minR9Barrel_        = conf_.getParameter<double>("minR9Barrel");
  minR9Endcap_        = conf_.getParameter<double>("minR9Endcap");
  usePrimaryVertex_   = conf_.getParameter<bool>("usePrimaryVertex");
  runMIPTagger_       = conf_.getParameter<bool>("runMIPTagger");

  candidateP4type_ = config.getParameter<std::string>("candidateP4type") ;
 
  edm::ParameterSet posCalcParameters = 
    config.getParameter<edm::ParameterSet>("posCalcParameters");
  posCalculator_ = PositionCalc(posCalcParameters);

  thePhotonEnergyCorrector_ = new PhotonEnergyCorrector(conf_);

  // Parameters for the position calculation:
  //  std::map<std::string,double> providedParameters;
  // providedParameters.insert(std::make_pair("LogWeighted",conf_.getParameter<bool>("posCalc_logweight")));
  //providedParameters.insert(std::make_pair("T0_barl",conf_.getParameter<double>("posCalc_t0_barl")));
  //providedParameters.insert(std::make_pair("T0_endc",conf_.getParameter<double>("posCalc_t0_endc")));
  //providedParameters.insert(std::make_pair("T0_endcPresh",conf_.getParameter<double>("posCalc_t0_endcPresh")));
  //providedParameters.insert(std::make_pair("W0",conf_.getParameter<double>("posCalc_w0")));
  //providedParameters.insert(std::make_pair("X0",conf_.getParameter<double>("posCalc_x0")));
  //posCalculator_ = PositionCalc(providedParameters);
  // cut values for pre-selection
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("minSCEtBarrel")); 
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("maxHoverEBarrel")); 
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("ecalRecHitSumEtOffsetBarrel")); 
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("ecalRecHitSumEtSlopeBarrel")); 
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("hcalTowerSumEtOffsetBarrel"));
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("hcalTowerSumEtSlopeBarrel"));
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("nTrackSolidConeBarrel"));
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("nTrackHollowConeBarrel"));     
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("trackPtSumSolidConeBarrel"));     
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("trackPtSumHollowConeBarrel"));     
  preselCutValuesBarrel_.push_back(conf_.getParameter<double>("sigmaIetaIetaCutBarrel"));     
  //  
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("minSCEtEndcap")); 
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("maxHoverEEndcap")); 
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("ecalRecHitSumEtOffsetEndcap")); 
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("ecalRecHitSumEtSlopeEndcap")); 
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("hcalTowerSumEtOffsetEndcap"));
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("hcalTowerSumEtSlopeEndcap"));
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("nTrackSolidConeEndcap"));
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("nTrackHollowConeEndcap"));     
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("trackPtSumSolidConeEndcap"));     
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("trackPtSumHollowConeEndcap"));     
  preselCutValuesEndcap_.push_back(conf_.getParameter<double>("sigmaIetaIetaCutEndcap"));     
  //

  // Register the product
  produces< reco::PhotonCollection >(PhotonCollection_);

}

PhotonProducer::~PhotonProducer() {

  //delete energyCorrectionF;
}



void  PhotonProducer::beginRun (edm::Run& r, edm::EventSetup const & theEventSetup) {

    thePhotonIsolationCalculator_ = new PhotonIsolationCalculator();
    edm::ParameterSet isolationSumsCalculatorSet = conf_.getParameter<edm::ParameterSet>("isolationSumsCalculatorSet"); 
    thePhotonIsolationCalculator_->setup(isolationSumsCalculatorSet);
   
    thePhotonMIPHaloTagger_ = new PhotonMIPHaloTagger();
    edm::ParameterSet mipVariableSet = conf_.getParameter<edm::ParameterSet>("mipVariableSet"); 
    thePhotonMIPHaloTagger_->setup(mipVariableSet);
    thePhotonEnergyCorrector_ -> init(theEventSetup); 



}

void  PhotonProducer::endRun (edm::Run& r, edm::EventSetup const & theEventSetup) {

  delete thePhotonIsolationCalculator_;
  delete thePhotonMIPHaloTagger_;
  delete thePhotonEnergyCorrector_;
}




void PhotonProducer::produce(edm::Event& theEvent, const edm::EventSetup& theEventSetup) {

  using namespace edm;
  //  nEvt_++;

  reco::PhotonCollection outputPhotonCollection;
  std::auto_ptr< reco::PhotonCollection > outputPhotonCollection_p(new reco::PhotonCollection);


  // Get the PhotonCore collection
  bool validPhotonCoreHandle=true;
  Handle<reco::PhotonCoreCollection> photonCoreHandle;
  theEvent.getByLabel(photonCoreProducer_,photonCoreHandle);
  if (!photonCoreHandle.isValid()) {
    edm::LogError("PhotonProducer") << "Error! Can't get the product "<<photonCoreProducer_.label();
    validPhotonCoreHandle=false;
  }

 // Get EcalRecHits
  bool validEcalRecHits=true;
  Handle<EcalRecHitCollection> barrelHitHandle;
  EcalRecHitCollection barrelRecHits;
  theEvent.getByLabel(barrelEcalHits_, barrelHitHandle);
  if (!barrelHitHandle.isValid()) {
    edm::LogError("PhotonProducer") << "Error! Can't get the product "<<barrelEcalHits_.label();
    validEcalRecHits=false; 
  }
  if (  validEcalRecHits)  barrelRecHits = *(barrelHitHandle.product());

  
  Handle<EcalRecHitCollection> endcapHitHandle;
  theEvent.getByLabel(endcapEcalHits_, endcapHitHandle);
  EcalRecHitCollection endcapRecHits;
  if (!endcapHitHandle.isValid()) {
    edm::LogError("PhotonProducer") << "Error! Can't get the product "<<endcapEcalHits_.label();
    validEcalRecHits=false; 
  }
  if( validEcalRecHits) endcapRecHits = *(endcapHitHandle.product());


// get Hcal towers collection 
  Handle<CaloTowerCollection> hcalTowersHandle;
  theEvent.getByLabel(hcalTowers_, hcalTowersHandle);


  // get the geometry from the event setup:
  theEventSetup.get<CaloGeometryRecord>().get(theCaloGeom_);

  //
  // update energy correction function
  //  energyCorrectionF->init(theEventSetup);  

  edm::ESHandle<CaloTopology> pTopology;
  theEventSetup.get<CaloTopologyRecord>().get(theCaloTopo_);
  const CaloTopology *topology = theCaloTopo_.product();

  // Get the primary event vertex
  Handle<reco::VertexCollection> vertexHandle;
  reco::VertexCollection vertexCollection;
  bool validVertex=true;
  if ( usePrimaryVertex_ ) {
    theEvent.getByLabel(vertexProducer_, vertexHandle);
    if (!vertexHandle.isValid()) {
      edm::LogWarning("PhotonProducer") << "Error! Can't get the product primary Vertex Collection "<< "\n";
      validVertex=false;
    }
    if (validVertex) vertexCollection = *(vertexHandle.product());
  }
  math::XYZPoint vtx(0.,0.,0.);
  if (vertexCollection.size()>0) vtx = vertexCollection.begin()->position();


  int iSC=0; // index in photon collection
  // Loop over barrel and endcap SC collections and fill the  photon collection
  if ( validPhotonCoreHandle) 
    fillPhotonCollection(theEvent,
			 theEventSetup,
			 photonCoreHandle,
			 topology,
			 &barrelRecHits,
			 &endcapRecHits,
			 hcalTowersHandle,
			 vtx,
			 outputPhotonCollection,
			 iSC);
 

  // put the product in the event
  edm::LogInfo("PhotonProducer") << " Put in the event " << iSC << " Photon Candidates \n";
  outputPhotonCollection_p->assign(outputPhotonCollection.begin(),outputPhotonCollection.end());
  theEvent.put( outputPhotonCollection_p, PhotonCollection_);

}

void PhotonProducer::fillPhotonCollection(edm::Event& evt,
					  edm::EventSetup const & es,
					  const edm::Handle<reco::PhotonCoreCollection> & photonCoreHandle,
					  const CaloTopology* topology,
					  const EcalRecHitCollection* ecalBarrelHits,
					  const EcalRecHitCollection* ecalEndcapHits,
					  const edm::Handle<CaloTowerCollection> & hcalTowersHandle, 
					  math::XYZPoint & vtx,
					  reco::PhotonCollection & outputPhotonCollection, int& iSC) {
  
  const CaloGeometry* geometry = theCaloGeom_.product();
  const CaloSubdetectorGeometry* subDetGeometry =0 ;
  const CaloSubdetectorGeometry* geometryES = theCaloGeom_->getSubdetectorGeometry(DetId::Ecal, EcalPreshower);
  const EcalRecHitCollection* hits = 0 ;
  std::vector<double> preselCutValues;
  float minR9=0;



  for(unsigned int lSC=0; lSC < photonCoreHandle->size(); lSC++) {

    reco::PhotonCoreRef coreRef(reco::PhotonCoreRef(photonCoreHandle, lSC));
    reco::SuperClusterRef scRef=coreRef->superCluster();
    //    const reco::SuperCluster* pClus=&(*scRef);
    iSC++;

    int subdet = scRef->seed()->hitsAndFractions()[0].first.subdetId();
    subDetGeometry =  theCaloGeom_->getSubdetectorGeometry(DetId::Ecal, subdet);

    if (subdet==EcalBarrel) 
      { 
	preselCutValues = preselCutValuesBarrel_;
        minR9=minR9Barrel_;
        hits=  ecalBarrelHits;
      }
    else if  (subdet==EcalEndcap) 
      { 
	preselCutValues = preselCutValuesEndcap_;
        minR9=minR9Endcap_;
	hits=  ecalEndcapHits;
      }
    else
      { edm::LogWarning("")<<"PhotonProducer: do not know if it is a barrel or endcap SuperCluster" ; }

            
    // SC energy preselection
    if (scRef->energy()/cosh(scRef->eta()) <= preselCutValues[0] ) continue;
    // calculate HoE

    const CaloTowerCollection* hcalTowersColl = hcalTowersHandle.product();
    EgammaTowerIsolation towerIso1(hOverEConeSize_,0.,0.,1,hcalTowersColl) ;  
    EgammaTowerIsolation towerIso2(hOverEConeSize_,0.,0.,2,hcalTowersColl) ;  
    double HoE1=towerIso1.getTowerESum(&(*scRef))/scRef->energy();
    double HoE2=towerIso2.getTowerESum(&(*scRef))/scRef->energy(); 
    
    EgammaHadTower towerIsoBehindClus(es); 
    towerIsoBehindClus.setTowerCollection(hcalTowersHandle.product());
    std::vector<CaloTowerDetId> TowersBehindClus =  towerIsoBehindClus.towersOf(*scRef);
    float hcalDepth1OverEcalBc = towerIsoBehindClus.getDepth1HcalESum(TowersBehindClus)/scRef->energy();
    float hcalDepth2OverEcalBc = towerIsoBehindClus.getDepth2HcalESum(TowersBehindClus)/scRef->energy();
    //    std::cout << " PhotonProducer calculation of HoE with towers in a cone " << HoE1  << "  " << HoE2 << std::endl;
    //std::cout << " PhotonProducer calcualtion of HoE with towers behind the BCs " << hcalDepth1OverEcalBc  << "  " << hcalDepth2OverEcalBc << std::endl;


    // recalculate position of seed BasicCluster taking shower depth for unconverted photon
    math::XYZPoint unconvPos = posCalculator_.Calculate_Location(scRef->seed()->hitsAndFractions(),hits,subDetGeometry,geometryES);


    float maxXtal =   EcalClusterTools::eMax( *(scRef->seed()), &(*hits) );
    float e1x5    =   EcalClusterTools::e1x5(  *(scRef->seed()), &(*hits), &(*topology)); 
    float e2x5    =   EcalClusterTools::e2x5Max(  *(scRef->seed()), &(*hits), &(*topology)); 
    float e3x3    =   EcalClusterTools::e3x3(  *(scRef->seed()), &(*hits), &(*topology)); 
    float e5x5    =   EcalClusterTools::e5x5( *(scRef->seed()), &(*hits), &(*topology)); 
    std::vector<float> cov =  EcalClusterTools::covariances( *(scRef->seed()), &(*hits), &(*topology), geometry); 
    float sigmaEtaEta = sqrt(cov[0]);
    std::vector<float> locCov =  EcalClusterTools::localCovariances( *(scRef->seed()), &(*hits), &(*topology)); 
    float sigmaIetaIeta = sqrt(locCov[0]);


    float r9 =e3x3/(scRef->rawEnergy());
    // compute position of ECAL shower
    math::XYZPoint caloPosition;
    if (r9>minR9) {
      caloPosition = unconvPos;
    } else {
      caloPosition = scRef->position();
    }


    //// energy determination -- Default to create the candidate. Afterwards corrections are applied
    double photonEnergy=1.;
    // compute momentum vector of photon from primary vertex and cluster position
    math::XYZVector direction = caloPosition - vtx;
    //math::XYZVector momentum = direction.unit() * photonEnergy ;
    math::XYZVector momentum = direction.unit() ;

    // Create dummy candidate with unit momentum and zero energy
    math::XYZTLorentzVectorD p4(momentum.x(), momentum.y(), momentum.z(), photonEnergy );
    reco::Photon newCandidate(p4, caloPosition, coreRef, vtx);
    //std::cout << " standard p4 before " << newCandidate.p4() << " energy " << newCandidate.energy() <<  std::endl;
    //std::cout << " type " <<newCandidate.getCandidateP4type() <<  " standard p4 after " << newCandidate.p4() << " energy " << newCandidate.energy() << std::endl;

    /// get ecal photon specific corrected energy 
    /// plus values from regressions     and store them in the Photon
    // Photon candidate takes by default (set in photons_cfi.py)  a 4-momentum derived from the ecal photon-specific corrections. 
    thePhotonEnergyCorrector_->calculate(newCandidate, subdet);
   
    if ( candidateP4type_ == "fromEcalEnergy") {
      newCandidate.setP4( newCandidate.p4(reco::Photon::ecal_photons) );
    } else if ( candidateP4type_ == "fromRegression") {
      newCandidate.setP4( newCandidate.p4(reco::Photon::regression1) );
    }

    //       std::cout << " final p4 " << newCandidate.p4() << " energy " << newCandidate.energy() <<  std::endl;



    // Calculate fiducial flags and isolation variable. Blocked are filled from the isolationCalculator
    reco::Photon::FiducialFlags fiducialFlags;
    reco::Photon::IsolationVariables isolVarR03, isolVarR04;
    thePhotonIsolationCalculator_-> calculate ( &newCandidate,evt,es,fiducialFlags,isolVarR04, isolVarR03);
    newCandidate.setFiducialVolumeFlags( fiducialFlags );
    newCandidate.setIsolationVariables(isolVarR04, isolVarR03 );

    
    /// fill shower shape block
    reco::Photon::ShowerShape  showerShape;
    showerShape.e1x5= e1x5;
    showerShape.e2x5= e2x5;
    showerShape.e3x3= e3x3;
    showerShape.e5x5= e5x5;
    showerShape.maxEnergyXtal =  maxXtal;
    showerShape.sigmaEtaEta =    sigmaEtaEta;
    showerShape.sigmaIetaIeta =  sigmaIetaIeta;
    showerShape.hcalDepth1OverEcal = HoE1;
    showerShape.hcalDepth2OverEcal = HoE2;
    showerShape.hcalDepth1OverEcalBc = hcalDepth1OverEcalBc;
    showerShape.hcalDepth2OverEcalBc = hcalDepth2OverEcalBc;
    newCandidate.setShowerShapeVariables ( showerShape ); 

    // std::cout << " PhotonProducer from candidate HoE with towers in a cone " << newCandidate.hadronicOverEm()  << "  " <<  newCandidate.hadronicDepth1OverEm()  << " " <<  newCandidate.hadronicDepth2OverEm()  << std::endl;
    //    std::cout << " PhotonProducer from candidate  of HoE with towers behind the BCs " <<  newCandidate.hadTowOverEm()  << "  " << newCandidate.hadTowDepth1OverEm() << " " << newCandidate.hadTowDepth2OverEm() << std::endl;


  // fill MIP Vairables for Halo: Block for MIP are filled from PhotonMIPHaloTagger
   reco::Photon::MIPVariables mipVar ;
   if(subdet==EcalBarrel && runMIPTagger_ )
    {
  
     thePhotonMIPHaloTagger_-> MIPcalculate( &newCandidate,evt,es,mipVar);
    newCandidate.setMIPVariables(mipVar);
    }



    /// Pre-selection loose  isolation cuts
    bool isLooseEM=true;
    if ( newCandidate.pt() < highEt_) { 
      if ( newCandidate.hadronicOverEm()                   >= preselCutValues[1] )                                            isLooseEM=false;
      if ( newCandidate.ecalRecHitSumEtConeDR04()          > preselCutValues[2]+ preselCutValues[3]*newCandidate.pt() )       isLooseEM=false;
      if ( newCandidate.hcalTowerSumEtConeDR04()           > preselCutValues[4]+ preselCutValues[5]*newCandidate.pt() )       isLooseEM=false;
      if ( newCandidate.nTrkSolidConeDR04()                > int(preselCutValues[6]) )                                        isLooseEM=false;
      if ( newCandidate.nTrkHollowConeDR04()               > int(preselCutValues[7]) )                                        isLooseEM=false;
      if ( newCandidate.trkSumPtSolidConeDR04()            > preselCutValues[8] )                                             isLooseEM=false;
      if ( newCandidate.trkSumPtHollowConeDR04()           > preselCutValues[9] )                                             isLooseEM=false;
      if ( newCandidate.sigmaIetaIeta()                    > preselCutValues[10] )                                            isLooseEM=false;
    } 
    

        
    if ( isLooseEM)  
      outputPhotonCollection.push_back(newCandidate);
      
        
  }
}

