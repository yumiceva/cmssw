// -*- C++ -*-
//
// Package:     Core
// Class  :     FWGenParticleProxyBuilder
//
/**\class FWGenParticleProxyBuilder 

   Description: <one line class summary>

   Usage:
    <usage>

 */
//
// Original Author:
//         Created:  Thu Dec  6 18:01:21 PST 2007
// $Id: FWGenParticleProxyBuilder.cc,v 1.8 2010/04/07 14:24:38 yana Exp $
// 

#include "TDatabasePDG.h"
#include "TEveTrack.h"

#include "Fireworks/Core/interface/FWEventItem.h"
#include "Fireworks/Core/interface/FWSimpleProxyBuilderTemplate.h"
#include "Fireworks/Candidates/interface/CandidateUtils.h"

#include "DataFormats/HepMCCandidate/interface/GenParticle.h"
#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

class FWGenParticleProxyBuilder : public FWSimpleProxyBuilderTemplate<reco::GenParticle> {

public:
   FWGenParticleProxyBuilder() {}
   virtual ~FWGenParticleProxyBuilder() {}

   // ---------- member functions ---------------------------
   REGISTER_PROXYBUILDER_METHODS();

private:
   FWGenParticleProxyBuilder(const FWGenParticleProxyBuilder&); // stop default

   const FWGenParticleProxyBuilder& operator=(const FWGenParticleProxyBuilder&); // stop default
   
   void build(const reco::GenParticle& iData, unsigned int iIndex,TEveElement& oItemHolder) const;

   // ---------- member data --------------------------------
   static TDatabasePDG* s_pdg;
};

//______________________________________________________________________________

TDatabasePDG* FWGenParticleProxyBuilder::s_pdg = 0;

void
FWGenParticleProxyBuilder::build(const reco::GenParticle& iData, unsigned int iIndex, TEveElement& oItemHolder) const
{
   if (!s_pdg)
      s_pdg = new TDatabasePDG();
 
   char s[1024];
   TParticlePDG* pID = s_pdg->GetParticle(iData.pdgId());
   if ( pID )
      sprintf(s,"gen %s, Pt: %0.1f GeV", pID->GetName(), iData.pt());
   else
      sprintf(s,"gen pdg %d, Pt: %0.1f GeV", iData.pdgId(), iData.pt());

   TEveTrack* trk = fireworks::prepareCandidate( iData, context().getTrackPropagator() );    
   trk->MakeTrack();
   trk->SetTitle(s);
   oItemHolder.AddElement( trk );
}

REGISTER_FWPROXYBUILDER(FWGenParticleProxyBuilder, reco::GenParticle, "GenParticles", FWViewType::k3DBit | FWViewType::kRhoPhiBit  | FWViewType::kRhoZBit);

