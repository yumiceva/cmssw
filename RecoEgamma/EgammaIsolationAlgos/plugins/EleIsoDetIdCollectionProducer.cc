#include "RecoEgamma/EgammaIsolationAlgos/plugins/EleIsoDetIdCollectionProducer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"

#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"

#include "DataFormats/GeometryVector/interface/GlobalPoint.h"
#include "RecoCaloTools/Selectors/interface/CaloDualConeSelector.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectron.h"
#include "DataFormats/EgammaCandidates/interface/GsfElectronFwd.h"
#include "DataFormats/EgammaReco/interface/SuperCluster.h"
#include "DataFormats/EgammaReco/interface/SuperClusterFwd.h"
#include "DataFormats/RecoCandidate/interface/RecoCandidate.h"
#include "DataFormats/EcalDetId/interface/EcalSubdetector.h"
#include "RecoCaloTools/MetaCollections/interface/CaloRecHitMetaCollections.h"
#include "Geometry/Records/interface/CaloGeometryRecord.h"

#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "DataFormats/DetId/interface/DetIdCollection.h"


EleIsoDetIdCollectionProducer::EleIsoDetIdCollectionProducer(const edm::ParameterSet& iConfig) {
    recHitsLabel_ = iConfig.getParameter< edm::InputTag > ("recHitsLabel");
    emObjectLabel_ = iConfig.getParameter< edm::InputTag > ("emObjectLabel");
    etCandCut_ = iConfig.getParameter<double> ("etCandCut");
    energyCut_ = iConfig.getParameter<double>("energyCut");
    etCut_ = iConfig.getParameter<double>("etCut");
    outerRadius_ = iConfig.getParameter<double>("outerRadius");
    innerRadius_ = iConfig.getParameter<double>("innerRadius");
    interestingDetIdCollection_ = iConfig.getParameter<std::string>("interestingDetIdCollection");
    
    //register your products
    produces< DetIdCollection > (interestingDetIdCollection_) ;
}

EleIsoDetIdCollectionProducer::~EleIsoDetIdCollectionProducer() {}

void EleIsoDetIdCollectionProducer::beginJob () 
{}

// ------------ method called to produce the data  ------------
    void
EleIsoDetIdCollectionProducer::produce (edm::Event& iEvent, 
        const edm::EventSetup& iSetup)
{
    using namespace edm;
    using namespace std;

    //Get EM Object
    Handle<reco::GsfElectronCollection> emObjectH;
    iEvent.getByLabel(emObjectLabel_,emObjectH);

    // take EcalRecHits
    Handle<EcalRecHitCollection> recHitsH;
    iEvent.getByLabel(recHitsLabel_,recHitsH);
    std::auto_ptr<CaloRecHitMetaCollectionV> recHits_(0); 
    recHits_ = std::auto_ptr<CaloRecHitMetaCollectionV>(new EcalRecHitMetaCollection(*recHitsH));

    edm::ESHandle<CaloGeometry> pG;
    iSetup.get<CaloGeometryRecord>().get(pG);    
    const CaloGeometry* caloGeom = pG.product();

    CaloDualConeSelector *doubleConeSel_ = 0;
    if(recHitsLabel_.instance() == "EcalRecHitsEB")
        doubleConeSel_= new CaloDualConeSelector(innerRadius_,outerRadius_, &*pG, DetId::Ecal, EcalBarrel);
    else if(recHitsLabel_.instance() == "EcalRecHitsEE")
        doubleConeSel_= new CaloDualConeSelector(innerRadius_,outerRadius_, &*pG, DetId::Ecal, EcalEndcap);

    //Create empty output collections
    std::auto_ptr< DetIdCollection > detIdCollection (new DetIdCollection() ) ;

    reco::GsfElectronCollection::const_iterator emObj;
    if(doubleConeSel_) { //if cone selector was created
        for (emObj = emObjectH->begin(); emObj != emObjectH->end();  emObj++) { //Loop over candidates

            if(emObj->et() < etCandCut_) continue; //don't calculate if object hasn't enough energy
            
            GlobalPoint pclu (emObj->caloPosition().x(),emObj->caloPosition().y(),emObj->caloPosition().z());
            std::auto_ptr<CaloRecHitMetaCollectionV> chosen = doubleConeSel_->select(pclu,*recHits_);

            CaloRecHitMetaCollectionV::const_iterator recIt;
            for (recIt = chosen->begin(); recIt!= chosen->end () ; ++recIt) { // Select RecHits 

                if ( fabs(recIt->energy()) < energyCut_) continue;  //dont fill if below E noise value


                double et = recIt->energy() * 
                            caloGeom->getPosition(recIt->detid()).perp() / 
                            caloGeom->getPosition(recIt->detid()).mag();

                if ( fabs(et) < etCut_) continue;  //dont fill if below ET noise value

                if(std::find(detIdCollection->begin(),detIdCollection->end(),recIt->detid()) == detIdCollection->end()) 
		            detIdCollection->push_back(recIt->detid()); 
            } //end rechits

        } //end candidates

        delete doubleConeSel_;
    } //end if cone selector was created
    
    iEvent.put( detIdCollection, interestingDetIdCollection_ );
}
