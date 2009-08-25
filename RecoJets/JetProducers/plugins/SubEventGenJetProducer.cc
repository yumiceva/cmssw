
//#include <iostream>
#include "FWCore/Framework/interface/MakerMacros.h"
#include "RecoJets/JetProducers/plugins/SubEventGenJetProducer.h"
#include "FWCore/Utilities/interface/Exception.h"
#include "RecoJets/JetProducers/interface/JetSpecific.h"
#include "DataFormats/JetReco/interface/GenJetCollection.h"


using namespace std;
using namespace reco;
using namespace edm;
using namespace cms;

namespace {
  const bool debug = false;

}

namespace {
   bool checkHydro(GenParticleRef& p){
      const Candidate* m1 = p->mother();
      while(m1){
	 int pdg = abs(m1->pdgId());
	 int st = m1->status();
	 LogDebug("SubEventMothers")<<"Pdg ID : "<<pdg<<endl;
	 if(st == 3 || pdg < 9 || pdg == 21){
	    LogDebug("SubEventMothers")<<"Sub-Collision  Found! Pdg ID : "<<pdg<<endl;
	    return false;
	 }
         const Candidate* m = m1->mother();
	 m1 = m;
	 if(!m1) LogDebug("SubEventMothers")<<"No Mother, particle is : "<<pdg<<" with status "<<st<<endl;
      }
      return true;
   }
}

SubEventGenJetProducer::SubEventGenJetProducer(edm::ParameterSet const& conf):
  VirtualJetProducer( conf )
{
   mapSrc_ = conf.getParameter<edm::InputTag>( "srcMap");
   ignoreHydro_ = conf.getUntrackedParameter<bool>("ignoreHydro", true);
   produces<reco::BasicJetCollection>();
  // the subjet collections are set through the config file in the "jetCollInstanceName" field.
}


void SubEventGenJetProducer::inputTowers( ) 
{
   reco::CandidateView::const_iterator inBegin = inputs_.begin(),
      inEnd = inputs_.end(), i = inBegin;
   for (; i != inEnd; ++i ) {
      reco::CandidatePtr input = inputs_.ptrAt( i - inBegin );
      if (isnan(input->pt()))           continue;
      if (input->et()    <inputEtMin_)  continue;
      if (input->energy()<inputEMin_)   continue;
      if (isAnomalousTower(input))      continue;

      GenParticleRef pref = inputs_.refAt(i - inBegin).castTo<GenParticleRef>();
      int subevent = (*subEvMap_)[pref];
      LogDebug("SubEventContainers")<<"SubEvent is : "<<subevent<<endl;
      LogDebug("SubEventContainers")<<"SubSize is : "<<subInputs_.size()<<endl;

      if(subevent >= subInputs_.size()){ 
	 hydroTag_.resize(subevent+1, -1);
	 //	 hydroTag_[subevent] = -1;
         subInputs_.resize(subevent+1);
      LogDebug("SubEventContainers")<<"SubSize is : "<<subInputs_.size()<<endl;
      LogDebug("SubEventContainers")<<"HydroTagSize is : "<<hydroTag_.size()<<endl;
      }

      LogDebug("SubEventContainers")<<"HydroTag is : "<<hydroTag_[subevent]<<endl;
      if(hydroTag_[subevent] != 0) hydroTag_[subevent] = (int)checkHydro(pref);

      subInputs_[subevent].push_back(fastjet::PseudoJet(input->px(),input->py(),input->pz(),
						input->energy()));
      subInputs_[subevent].back().set_user_index(subInputs_[subevent].size() - 1);


   }
}

void SubEventGenJetProducer::produce(edm::Event& iEvent,const edm::EventSetup& iSetup){
   LogDebug("VirtualJetProducer") << "Entered produce\n";

   subEvMap_ = 0;
   fjJets_.clear();
   subInputs_.clear();
   nSubParticles_.clear();
   hydroTag_.clear();

   // get inputs and convert them to the fastjet format (fastjet::PeudoJet)
   edm::Handle<reco::CandidateView> inputsHandle;
   iEvent.getByLabel(src_,inputsHandle);
   inputs_ = *inputsHandle;
   LogDebug("VirtualJetProducer") << "Got inputs\n";

   edm::Handle<edm::SubEventMap> subs;
   iEvent.getByLabel(mapSrc_,subs);
   subEvMap_ = subs.product();
 
   inputTowers();
   // Convert candidates to fastjet::PseudoJets.
   // Also correct to Primary Vertex. Will modify fjInputs_
   // and use inputs_

   ////////////////

   LogDebug("VirtualJetProducer") << "Inputted towers\n";

   int nsub = subInputs_.size();

   for(size_t isub = 0; isub < nsub; ++isub){
      if(hydroTag_[isub]) continue;
      fjInputs_.clear();
      fjInputs_ = subInputs_[isub];
      runAlgorithm( iEvent, iSetup );
   }

   output( iEvent, iSetup );
   LogDebug("VirtualJetProducer") << "Wrote jets\n";
  
   return;
}

void SubEventGenJetProducer::runAlgorithm( edm::Event & iEvent, edm::EventSetup const& iSetup)
{
   // run algorithm
   fjClusterSeq_ = ClusterSequencePtr( new fastjet::ClusterSequence( fjInputs_, *fjJetDefinition_ ) );
   std::vector<fastjet::PseudoJet> newJets_ = fastjet::sorted_by_pt(fjClusterSeq_->inclusive_jets(jetPtMin_));
   
   fjJets_.insert(fjJets_.end(),newJets_.begin(),newJets_.end());
   
}


DEFINE_FWK_MODULE(SubEventGenJetProducer);


