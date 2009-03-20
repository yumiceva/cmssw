// system include files
#include <memory>
#include <iostream>

// user include files
#include "ExhumeAnalyzer.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenParticle.h"

#include "CLHEP/Vector/LorentzVector.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"


ExhumeAnalyzer::ExhumeAnalyzer(const edm::ParameterSet& iConfig){
        Ebeam_ = iConfig.getUntrackedParameter<double>("EBeam",7000.);
	outputFilename_=iConfig.getUntrackedParameter<std::string>("OutputFilename","dummy.root");
        std::cout << "Output file: " << outputFilename_ << std::endl;
        hist_eta_ = new TH1D("hist_eta","#eta system",100,-4.5,4.5);
        hist_t1_ = new TH1D("hist_t1","t proton 1",100,-1.4,0);
        hist_xigen1_ = new TH1D("hist_xigen1","#xi proton 1",100,0.,0.1);
        hist_t2_ = new TH1D("hist_t1","t proton 1",100,-1.4,0);
        hist_xigen2_ = new TH1D("hist_xigen2","#xi proton 2",100,0.,0.1);
        hist_sHat_ = new TH1D("hist_sHat", "Central inv. mass",100,118.,122.);
        hist_MX_ = new TH1D("hist_MX", "Missing mass",100,118.,122.);
}

void ExhumeAnalyzer::beginJob(edm::EventSetup const&iSetup) {}

ExhumeAnalyzer::~ExhumeAnalyzer() {}

// ------------ method called to for each event  ------------
void
ExhumeAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
{
   using namespace edm;
  
   // get HepMC::GenEvent ...
   Handle<HepMCProduct> evt_h;
   iEvent.getByType(evt_h);
   HepMC::GenEvent * evt = new HepMC::GenEvent(*(evt_h->GetEvent()));
   //const HepMC::GenEvent *evt = evt_h->GetEvent();	


   // look for protons
   //std::vector<HepMC::GenParticle*> protons;
   HepMC::GenParticle* proton1 = 0;
   HepMC::GenParticle* proton2 = 0;	
   double pz1max = 0.;
   double pz2min = 0.;
   for(HepMC::GenEvent::particle_iterator it = evt->particles_begin(); it != evt->particles_end(); ++it) {
     double pz = (*it)->momentum().pz();
     if(((*it)->pdg_id() == 2212)&&((*it)->status() == 1)&&(pz > 0.75*Ebeam_)){
        if(pz > pz1max){proton1 = *it;pz1max=pz;}
     } else if(((*it)->pdg_id() == 2212)&&((*it)->status() == 1)&&(pz < -0.75*Ebeam_)){
        if(pz < pz2min){proton2 = *it;pz2min=pz;}
     }
   }
   if(proton1&&proton2){
   std::cout << "Proton 1: " << proton1->momentum().perp() << "  " << proton1->momentum().pseudoRapidity() << "  " << proton1->momentum().phi() << std::endl;
   std::cout << "Proton 2: " << proton2->momentum().perp() << "  " << proton2->momentum().pseudoRapidity() << "  " << proton2->momentum().phi() << std::endl;	

   HepLorentzVector proton1in(0.,0.,Ebeam_,Ebeam_);
   HepLorentzVector proton1diff(proton1->momentum().px(),proton1->momentum().py(),proton1->momentum().pz(),proton1->momentum().e());    
   double t1 = (proton1diff - proton1in).m2();
   double xigen1 = 1 - proton1diff.pz()/Ebeam_;
   HepLorentzVector proton2in(0.,0.,-Ebeam_,Ebeam_);
   HepLorentzVector proton2diff(proton2->momentum().px(),proton2->momentum().py(),proton2->momentum().pz(),proton2->momentum().e());
   double t2 = (proton2diff - proton2in).m2();
   double xigen2 = 1 + proton2diff.pz()/Ebeam_;

   double eta = 0.5*log(xigen1/xigen2);
   double pt1 = sqrt(-t1);
   double pt2 = sqrt(-t2);
   double phi1 = proton1diff.phi();
   double phi2 = proton2diff.phi();
   double s = 4*Ebeam_*Ebeam_;
   double sHat = t1 + t2 - 2*pt1*pt2*cos(phi1-phi2) + s*xigen1*xigen2;
   double MX = sqrt(s*xigen1*xigen2);
 
   //Fill histograms
   hist_eta_->Fill(eta);
   hist_t1_->Fill(t1);
   hist_xigen1_->Fill(xigen1);
   hist_t2_->Fill(t2);
   hist_xigen2_->Fill(xigen2);	
   hist_sHat_->Fill(sqrt(sHat));
   hist_MX_->Fill(MX);
   }

   delete evt;
}

// ------------ method called once each job just after ending the event loop  ------------
void ExhumeAnalyzer::endJob() {
	// save histograms into file
	TFile file(outputFilename_.c_str(),"RECREATE");
	hist_eta_->Write();
	hist_t1_->Write();
	hist_xigen1_->Write();
	hist_t2_->Write();
	hist_xigen2_->Write();
	hist_sHat_->Write();
	hist_MX_->Write();	
	file.Close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(ExhumeAnalyzer);
