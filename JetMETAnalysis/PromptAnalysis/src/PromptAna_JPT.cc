#include "JetMETAnalysis/PromptAnalysis/interface/PromptAna_JPT.h"
//#include "JetMETCorrections/Algorithms/interface/JetPlusTrackCorrector.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "DataFormats/Math/interface/deltaR.h"
#include "RecoJets/JetAssociationAlgorithms/interface/JetTracksAssociationDRCalo.h"
#include "DataFormats/Math/interface/Point3D.h"
#include "MagneticField/Records/interface/IdealMagneticFieldRecord.h"
#include "TrackingTools/Records/interface/TrackingComponentsRecord.h"
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
//jets
#include "DataFormats/JetReco/interface/CaloJet.h"
#include "DataFormats/JetReco/interface/CaloJetCollection.h"
#include "DataFormats/JetReco/interface/GenJet.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"
#include "JetMETCorrections/Objects/interface/JetCorrector.h"
#include "DataFormats/JetReco/interface/JetExtendedAssociation.h"
#include "DataFormats/JetReco/interface/JetID.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ParameterSet/interface/InputTag.h"
// candidates
#include "DataFormats/Candidate/interface/LeafCandidate.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "DataFormats/Candidate/interface/CandidateFwd.h"
#include   "DataFormats/JetReco/interface/JPTJetCollection.h"
#include   "DataFormats/JetReco/interface/JPTJet.h"


#include <string>
#include <memory>
#include <vector>
using namespace reco;

namespace jptJetAnalysis {

  // Helpper class to propagate tracks to the calo surface using the same implementation as the JetTrackAssociator
  class TrackPropagatorToCalo
  {
   public:
    TrackPropagatorToCalo();
    void update(const edm::EventSetup& eventSetup);
    math::XYZPoint impactPoint(const reco::Track& track) const;
   private:
    const MagneticField* magneticField_;
    const Propagator* propagator_;
    uint32_t magneticFieldCacheId_;
    uint32_t propagatorCacheId_;
  };
  
  TrackPropagatorToCalo::TrackPropagatorToCalo()
    : magneticField_(NULL),
      propagator_(NULL),
      magneticFieldCacheId_(0),
      propagatorCacheId_(0)
    {}

  void TrackPropagatorToCalo::update(const edm::EventSetup& eventSetup)
  {
    //update magnetic filed if necessary
    const IdealMagneticFieldRecord& magneticFieldRecord = eventSetup.get<IdealMagneticFieldRecord>();
    const uint32_t newMagneticFieldCacheId = magneticFieldRecord.cacheIdentifier();
    if ((newMagneticFieldCacheId != magneticFieldCacheId_) || !magneticField_) {
      edm::ESHandle<MagneticField> magneticFieldHandle;
      magneticFieldRecord.get(magneticFieldHandle);
      magneticField_ = magneticFieldHandle.product();
      magneticFieldCacheId_ = newMagneticFieldCacheId;
    }
    //update propagator if necessary
    const TrackingComponentsRecord& trackingComponentsRecord = eventSetup.get<TrackingComponentsRecord>();
    const uint32_t newPropagatorCacheId = trackingComponentsRecord.cacheIdentifier();
    if ((propagatorCacheId_ != newPropagatorCacheId) || !propagator_) {
      edm::ESHandle<Propagator> propagatorHandle;
      trackingComponentsRecord.get("SteppingHelixPropagatorAlong",propagatorHandle);
      propagator_ = propagatorHandle.product();
      propagatorCacheId_ = newPropagatorCacheId;
    }
  }

  inline math::XYZPoint TrackPropagatorToCalo::impactPoint(const reco::Track& track) const
  {
    return JetTracksAssociationDRCalo::propagateTrackToCalorimeter(track,*magneticField_,*propagator_);
  }

}

class JPTPromptAnaTrackVariables
{
 public:
  JPTPromptAnaTrackVariables(const std::string& prefix, const std::string& suffix, const std::string& type);
  static void produce(edm::EDProducer* producer, const std::string& prefix, const std::string& suffix, const std::string& type);
  void set(const reco::TrackRefVector& tracks, const reco::CaloJet& jet, const jptJetAnalysis::TrackPropagatorToCalo* trackPropagator);
  void put(edm::Event& event);
 private:
  const std::string prefix_, suffix_, type_;
  std::auto_ptr< std::vector<int> > n_;
  std::auto_ptr< std::vector<double> > avgPt_, maxPt_, avgNHits_, avgNLayers_, pt_, eta_, phi_;
  std::auto_ptr< std::vector<int> > nHits_, nLayers_;
  std::auto_ptr< std::vector<double> > dz_, dxy_, drCalo_, drVertex_;
};

PromptAna_JPT::PromptAna_JPT(const edm::ParameterSet& iConfig) 
  : inputTag(iConfig.getParameter<edm::InputTag>("InputTag"))      		
  , jetCorrectionService(iConfig.getParameter<std::string>  ("JetCorrectionService"  ))
  , prefix  (iConfig.getParameter<std::string>  ("Prefix"  ))
  , suffix  (iConfig.getParameter<std::string>  ("Suffix"  ))
  , allVariables  (iConfig.getParameter<bool>  ("AllVariables"  ))
{
  // Jet ID helper
  jetIDHelper = reco::helper::JetIDHelper(iConfig.getParameter<edm::ParameterSet>("jetID")  );

  produces <std::vector<int> > ( prefix + "NJets"  + suffix );
  produces <std::vector<int> > ( prefix + "NcleanedJets"  + suffix );
  produces <std::vector<double> > ( prefix + "pT"  + suffix );
  produces <std::vector<double> > ( prefix + "Energy"  + suffix );
  produces <std::vector<int> > ( prefix + "NConstituents"  + suffix );
  produces <std::vector<double> > ( prefix + "Eta"  + suffix );
  produces <std::vector<double> > ( prefix + "Phi"  + suffix );
  produces <std::vector<double> > ( prefix + "JIDfHPD"  + suffix );
  produces <std::vector<int> >    ( prefix + "JIDn90Hits"  + suffix );
  produces <std::vector<double> > ( prefix + "JIDfRBX"  + suffix );
  
  produces <std::vector<double> > ( prefix + "JIDresEMF"  + suffix );
  produces <std::vector<double> > ( prefix + "EmEnergyFraction"  + suffix );
  produces <std::vector<double> > ( prefix + "EnergyFractionHadronic"  + suffix );
  produces <std::vector<double> > ( prefix + "TowersArea"  + suffix );
  produces <std::vector<double> > ( prefix + "MaxEInEmTowers"  + suffix );
  produces <std::vector<double> > ( prefix + "MaxEInHadTowers"  + suffix );
 
  produces <std::vector<int> > ( prefix + "N60"  + suffix ); 
  produces <std::vector<int> > ( prefix + "N90"  + suffix ); 

  //JPT specific
  produces <std::vector<double> > ( prefix + "RawPt" + suffix );    
  produces <std::vector<double> > ( prefix + "scaleL2L3"  + suffix ); 
  produces <std::vector<double> > ( prefix + "RawE" + suffix );
  produces <std::vector<double> > ( prefix + "RawEta" + suffix );
  produces <std::vector<double> > ( prefix + "RawPhi" + suffix );
  produces <std::vector<double> > ( prefix + "DeltaEta" + suffix );
  produces <std::vector<double> > ( prefix + "DeltaPhi" + suffix );
  produces <std::vector<double> > ( prefix + "JPTZSPCorrFactor" + suffix );
  produces <std::vector<double> > ( prefix + "ZSPCorrFactor" + suffix );
  produces <std::vector<double> > ( prefix + "JPTCorrFactor" + suffix );
  produces <std::vector<double> > ( prefix + "PtFractionInCone" + suffix );
  std::vector<std::string> particleTypes;
  std::vector<std::string> trackTypes;
  if (allVariables){
  particleTypes.push_back("Pions"); particleTypes.push_back("Muons"); particleTypes.push_back("Electrons");
  }
  if (allVariables){
  trackTypes.push_back("All"); trackTypes.push_back("InCaloInVertex"); trackTypes.push_back("OutCaloInVertex"); trackTypes.push_back("InCaloOutVertex");
  for (std::vector<std::string>::const_iterator iParticleType = particleTypes.begin(); iParticleType != particleTypes.end(); ++iParticleType) {
    for (std::vector<std::string>::const_iterator iTrackType = trackTypes.begin(); iTrackType != trackTypes.end(); ++iTrackType) {
      JPTPromptAnaTrackVariables::produce(this,prefix,suffix,(*iTrackType)+(*iParticleType));
    }
  }
  } // AllVariables
}

void JPTPromptAnaTrackVariables::produce(edm::EDProducer* producer, const std::string& prefix, const std::string& suffix, const std::string& type)
{
  producer->produces <std::vector<int> > ( prefix + "n"+type + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"AveragePt" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"MaxPt" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"AverageNHits" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"AverageNLayers" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"Pt" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"Eta" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"Phi" + suffix );
  producer->produces <std::vector<int> > ( prefix + type+"nHits" + suffix );
  producer->produces <std::vector<int> > ( prefix + type+"nLayers" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"dz" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"dxy" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"drCalo" + suffix );
  producer->produces <std::vector<double> > ( prefix + type+"drVertex" + suffix );
}

JPTPromptAnaTrackVariables::JPTPromptAnaTrackVariables(const std::string& prefix, const std::string& suffix, const std::string& type)
  : prefix_(prefix),
    suffix_(suffix),
    type_(type),
    n_(new std::vector<int>()),
    avgPt_(new std::vector<double>()),
    maxPt_(new std::vector<double>()),
    avgNHits_(new std::vector<double>()),
    avgNLayers_(new std::vector<double>()),
    pt_(new std::vector<double>()),
    eta_(new std::vector<double>()),
    phi_(new std::vector<double>()),
    nHits_(new std::vector<int>()),
    nLayers_(new std::vector<int>()),
    dz_(new std::vector<double>()),
    dxy_(new std::vector<double>()),
    drCalo_(new std::vector<double>()),
    drVertex_(new std::vector<double>())
{
}

void JPTPromptAnaTrackVariables::set(const reco::TrackRefVector& tracks, const reco::CaloJet& jet, const jptJetAnalysis::TrackPropagatorToCalo* trackPropagator)
{
  unsigned int n = 0;
  double totalPt = 0;
  unsigned int totalHits = 0;
  unsigned int totalLayers = 0;
  double maxPt = 0;
  for (reco::TrackRefVector::const_iterator iTrack = tracks.begin(); iTrack != tracks.end(); ++iTrack) {
    n++;
    totalPt += (*iTrack)->pt();
    if ((*iTrack)->pt() > maxPt) maxPt = (*iTrack)->pt();
    totalHits += (*iTrack)->found();
    totalLayers += (*iTrack)->hitPattern().trackerLayersWithMeasurement();
    pt_->push_back((*iTrack)->pt());
    eta_->push_back((*iTrack)->eta());
    phi_->push_back((*iTrack)->phi());
    nHits_->push_back((*iTrack)->found());
    nLayers_->push_back((*iTrack)->hitPattern().trackerLayersWithMeasurement());
    dz_->push_back((*iTrack)->dz());
    dxy_->push_back((*iTrack)->dxy());
    drCalo_->push_back(deltaR(trackPropagator->impactPoint(**iTrack),jet));
    drVertex_->push_back(deltaR(**iTrack,jet));
  }
  n_->push_back(n);
  avgPt_->push_back(totalPt/double(n));
  maxPt_->push_back(maxPt);
  avgNHits_->push_back(double(totalHits)/double(n));
  avgNLayers_->push_back(double(totalLayers)/double(n));
}

void JPTPromptAnaTrackVariables::put(edm::Event& event)
{
  event.put(n_,prefix_+"n"+type_+suffix_);
  event.put(avgPt_,prefix_+type_+"AveragePt"+suffix_);
  event.put(maxPt_,prefix_+type_+"MaxPt"+suffix_);
  event.put(avgNHits_,prefix_+type_+"AverageNHits"+suffix_);
  event.put(avgNLayers_,prefix_+type_+"AverageNLayers"+suffix_);
  event.put(pt_,prefix_+type_+"Pt"+suffix_);
  event.put(eta_,prefix_+type_+"Eta"+suffix_);
  event.put(phi_,prefix_+type_+"Phi"+suffix_);
  event.put(nHits_,prefix_+type_+"nHits"+suffix_);
  event.put(nLayers_,prefix_+type_+"nLayers"+suffix_);
  event.put(dz_,prefix_+type_+"dz"+suffix_);
  event.put(dxy_,prefix_+type_+"dxy"+suffix_);
  event.put(drCalo_,prefix_+type_+"drCalo"+suffix_);
  event.put(drVertex_,prefix_+type_+"drVertex"+suffix_);
}

void PromptAna_JPT::produce(edm::Event& iEvent, const edm::EventSetup& iSetup) 
{
  std::auto_ptr<std::vector<int> >  njets  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<int> >  ncleanedjets  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> >  pt  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  energy  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  nconstituents  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> >  eta  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  phi  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  fHPD  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<int> >  fn90Hits  ( new std::vector<int>()  ) ;
  std::auto_ptr<std::vector<double> >  fRBX  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  resEMF  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  emEnergyFraction  ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  energyFractionHadronic ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  towersArea   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  maxEInEmTowers   ( new std::vector<double>()  ) ;
  std::auto_ptr<std::vector<double> >  maxEInHadTowers   ( new std::vector<double>()  ) ;  
  std::auto_ptr<std::vector<double> > scaleL2L3   ( new std::vector<double>()  ); 
  std::auto_ptr<std::vector<int> >  n60   ( new std::vector<int>()  ); 
  std::auto_ptr<std::vector<int> >  n90   ( new std::vector<int>()  ); 

  //JPT specific
  std::auto_ptr< std::vector<double> > rawPt (new std::vector<double>());
  std::auto_ptr< std::vector<double> > rawE (new std::vector<double>());
  std::auto_ptr< std::vector<double> > rawEta (new std::vector<double>());
  std::auto_ptr< std::vector<double> > rawPhi (new std::vector<double>());
  std::auto_ptr< std::vector<double> > deltaEta (new std::vector<double>());
  std::auto_ptr< std::vector<double> > deltaPhi (new std::vector<double>());
  std::auto_ptr< std::vector<double> > jptZspCorrFactor (new std::vector<double>());
  std::auto_ptr< std::vector<double> > zspCorrFactor (new std::vector<double>());
  std::auto_ptr< std::vector<double> > jptCorrFactor (new std::vector<double>());
  std::auto_ptr< std::vector<double> > ptFractionInCone (new std::vector<double>());
  //if (allVariables) {
  JPTPromptAnaTrackVariables AllPions(prefix,suffix,"AllPions");
  JPTPromptAnaTrackVariables InCaloInVertexPions(prefix,suffix,"InCaloInVertexPions");
  JPTPromptAnaTrackVariables OutCaloInVertexPions(prefix,suffix,"OutCaloInVertexPions");
  JPTPromptAnaTrackVariables InCaloOutVertexPions(prefix,suffix,"InCaloOutVertexPions");
  JPTPromptAnaTrackVariables AllMuons(prefix,suffix,"AllMuons");
  JPTPromptAnaTrackVariables InCaloInVertexMuons(prefix,suffix,"InCaloInVertexMuons");
  JPTPromptAnaTrackVariables OutCaloInVertexMuons(prefix,suffix,"OutCaloInVertexMuons");
  JPTPromptAnaTrackVariables InCaloOutVertexMuons(prefix,suffix,"InCaloOutVertexMuons");
  JPTPromptAnaTrackVariables AllElectrons(prefix,suffix,"AllElectrons");
  JPTPromptAnaTrackVariables InCaloInVertexElectrons(prefix,suffix,"InCaloInVertexElectrons");
  JPTPromptAnaTrackVariables OutCaloInVertexElectrons(prefix,suffix,"OutCaloInVertexElectrons");
  JPTPromptAnaTrackVariables InCaloOutVertexElectrons(prefix,suffix,"InCaloOutVertexElectrons");
  //}

  //Get the JPT Jets Collection

  edm::Handle<reco::JPTJetCollection> jetcollection;
  iEvent.getByLabel(inputTag, jetcollection);

  jptJetAnalysis::TrackPropagatorToCalo trackPropagator;
  trackPropagator.update(iSetup);
  
  int clj=0;       int jc = 0;

  //Fill the variables
  for(reco::JPTJetCollection::const_iterator it = jetcollection->begin(); it != jetcollection->end() ; ++it ){
   
    edm::RefToBase<Jet> jptjetRef = it->getCaloJetRef();
    reco::CaloJet const * rawcalojet = dynamic_cast<reco::CaloJet const *>( &* jptjetRef);

     const JetCorrector* corrector = JetCorrector::getJetCorrector (jetCorrectionService,iSetup); 
     if (!corrector) edm::LogError("JPTPromptAnalysis") << "Failed to get corrector with name " << jetCorrectionService<< "from the EventSetup";
     scaleL2L3->push_back( corrector->correction((*it).p4()) );
    
     reco::TrackRefVector allPions;
     allPions.reserve((*it).getPionsInVertexInCalo().size() +  (*it).getPionsInVertexOutCalo().size() +  (*it).getPionsOutVertexInCalo().size()) ;
     reco::TrackRefVector allMuons;
     allMuons.reserve((*it).getMuonsInVertexInCalo().size() +  (*it).getMuonsInVertexOutCalo().size() +  (*it).getMuonsOutVertexInCalo().size()) ;
     reco::TrackRefVector allElectrons;
     allElectrons.reserve((*it).getElecsInVertexInCalo().size() +  (*it).getElecsInVertexOutCalo().size() +  (*it).getElecsOutVertexInCalo().size()) ;


    if(it==jetcollection->begin()){
      njets->push_back(jetcollection->size());     
    }
    jetIDHelper.calculate(iEvent, *rawcalojet);
    pt->push_back((*it).pt());
    energy->push_back((*it).energy());
    nconstituents->push_back((*rawcalojet).getCaloConstituents().size());
    eta->push_back((*it).eta());
    phi->push_back((*it).phi());
    fHPD->push_back(jetIDHelper.fHPD() );
    fRBX->push_back(jetIDHelper.fRBX() );
    fn90Hits->push_back(jetIDHelper.n90Hits() );
  
    resEMF->push_back(jetIDHelper.restrictedEMF() );
    //ok for the moment i add this by hand, just to be sure
    if(jetIDHelper.fHPD()<0.98 && jetIDHelper.n90Hits()>=2 && jetIDHelper.restrictedEMF()>0.01) clj++;
    emEnergyFraction->push_back((*rawcalojet).emEnergyFraction());
    energyFractionHadronic->push_back((*rawcalojet).energyFractionHadronic());
    towersArea->push_back((*rawcalojet).towersArea());
    maxEInEmTowers->push_back((*rawcalojet).maxEInEmTowers());
    maxEInHadTowers->push_back((*rawcalojet).maxEInHadTowers()); 
    n60->push_back((*rawcalojet).n60()); 
    n90->push_back((*rawcalojet).n90());
    rawPt->push_back((*rawcalojet).pt());
    rawE->push_back((*rawcalojet).energy());
    rawEta->push_back((*rawcalojet).eta());
    rawPhi->push_back((*rawcalojet).phi());
    deltaEta->push_back((*it).eta()-(*rawcalojet).eta());
    deltaPhi->push_back((*it).phi()-(*rawcalojet).phi());
 
    if (allVariables) {
      ptFractionInCone->push_back(findPtFractionInCone((*it).getPionsInVertexInCalo(),(*it).getPionsInVertexOutCalo()));
    //fill track hiostos
    AllPions.set(allPions,*rawcalojet,&trackPropagator);
    InCaloInVertexPions.set((*it).getPionsInVertexInCalo(),*rawcalojet,&trackPropagator);
    OutCaloInVertexPions.set((*it).getPionsInVertexOutCalo(),*rawcalojet,&trackPropagator);
    InCaloOutVertexPions.set((*it).getPionsOutVertexInCalo(),*rawcalojet,&trackPropagator);
    AllMuons.set(allMuons,*rawcalojet,&trackPropagator);
    InCaloInVertexMuons.set( (*it).getMuonsInVertexInCalo()  ,*rawcalojet,&trackPropagator);
    OutCaloInVertexMuons.set( (*it).getMuonsInVertexOutCalo()   ,*rawcalojet,&trackPropagator);
    InCaloOutVertexMuons.set( (*it).getMuonsOutVertexInCalo()  ,*rawcalojet,&trackPropagator);
    AllElectrons.set(allElectrons,*rawcalojet,&trackPropagator);
    InCaloInVertexElectrons.set(  (*it).getElecsInVertexInCalo()   ,*rawcalojet,&trackPropagator);
    OutCaloInVertexElectrons.set(  (*it).getElecsInVertexOutCalo() ,*rawcalojet,&trackPropagator);
    InCaloOutVertexElectrons.set(  (*it).getElecsOutVertexInCalo()   ,*rawcalojet,&trackPropagator);
    }
 	 jc++;

  }
  ncleanedjets->push_back(clj);

  iEvent.put( njets,  prefix + "NJets"  + suffix );
  iEvent.put( ncleanedjets,  prefix + "NcleanedJets"  + suffix );
  iEvent.put( pt,  prefix + "pT"  + suffix );
  iEvent.put( energy,  prefix + "Energy"  + suffix );
  iEvent.put( nconstituents,  prefix + "NConstituents"  + suffix );
  iEvent.put( eta,  prefix + "Eta"  + suffix );
  iEvent.put( phi,  prefix + "Phi"  + suffix );
  iEvent.put( fHPD,  prefix + "JIDfHPD"    + suffix );
  iEvent.put( fn90Hits,  prefix + "JIDn90Hits"   + suffix );
  iEvent.put( fRBX,  prefix + "JIDfRBX"    + suffix );

  iEvent.put( resEMF,  prefix + "JIDresEMF"    + suffix );
  iEvent.put( emEnergyFraction,  prefix + "EmEnergyFraction"  + suffix );
  iEvent.put( energyFractionHadronic,  prefix + "EnergyFractionHadronic"  + suffix );
  iEvent.put( towersArea,  prefix + "TowersArea"  + suffix );
  iEvent.put( maxEInEmTowers,  prefix + "MaxEInEmTowers"  + suffix );
  iEvent.put( maxEInHadTowers,  prefix + "MaxEInHadTowers"  + suffix );
 
  iEvent.put( n60,  prefix + "N60"  + suffix ); 
  iEvent.put( n90,  prefix + "N90"  + suffix );
  
  iEvent.put( rawPt, prefix + "RawPt" + suffix );
  iEvent.put( rawE, prefix + "RawE" + suffix );
  iEvent.put( rawEta, prefix + "RawEta" + suffix );
  iEvent.put( rawPhi, prefix + "RawPhi" + suffix );
  iEvent.put( deltaEta, prefix + "DeltaEta" + suffix );
  iEvent.put( deltaPhi, prefix + "DeltaPhi" + suffix );
  iEvent.put( jptZspCorrFactor, prefix + "JPTZSPCorrFactor" + suffix );
  iEvent.put( zspCorrFactor, prefix + "ZSPCorrFactor" + suffix );
  iEvent.put( jptCorrFactor, prefix + "JPTCorrFactor" + suffix );
  iEvent.put( ptFractionInCone, prefix + "PtFractionInCone" + suffix );  
  iEvent.put( scaleL2L3,  prefix + "scaleL2L3"  + suffix );
  if (allVariables) {
  AllPions.put(iEvent);
  InCaloInVertexPions.put(iEvent);
  OutCaloInVertexPions.put(iEvent);
  InCaloOutVertexPions.put(iEvent);
  AllMuons.put(iEvent);
  InCaloInVertexMuons.put(iEvent);
  OutCaloInVertexMuons.put(iEvent);
  InCaloOutVertexMuons.put(iEvent);
  AllElectrons.put(iEvent);
  InCaloInVertexElectrons.put(iEvent);
  OutCaloInVertexElectrons.put(iEvent);
  InCaloOutVertexElectrons.put(iEvent);
  }
}

double PromptAna_JPT::findPtFractionInCone(const reco::TrackRefVector& inConeTracks, const reco::TrackRefVector& outOfConeTracks)
{                                      
  double totalPt = 0;
  double inConePt = 0;
  const reco::TrackRefVector::const_iterator inConeTracksEnd = inConeTracks.end();
  for (reco::TrackRefVector::const_iterator iInConeTrack = inConeTracks.begin(); iInConeTrack != inConeTracksEnd; ++iInConeTrack) {
    const double pt = (*iInConeTrack)->pt();
    totalPt += pt;
    inConePt += pt;
  }
  const reco::TrackRefVector::const_iterator outOfConeTracksEnd = outOfConeTracks.end();
  for (reco::TrackRefVector::const_iterator iOutOfConeTrack = outOfConeTracks.begin(); iOutOfConeTrack != outOfConeTracksEnd; ++iOutOfConeTrack) {
    const double pt = (*iOutOfConeTrack)->pt();
    totalPt += pt;
  } 
  if (totalPt) return inConePt/totalPt;
  //return 0 if there are no tracks at all
  else return 0;
}
