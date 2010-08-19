#include <iostream>
#include <cmath>

#include "boost/lexical_cast.hpp"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Utilities/interface/RandomNumberGenerator.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "GeneratorInterface/Core/interface/RNDMEngineAccess.h"

#include "GeneratorInterface/AMPTInterface/interface/AMPTHadronizer.h"
#include "GeneratorInterface/AMPTInterface/interface/AMPTWrapper.h"

#include "SimDataFormats/GeneratorProducts/interface/HepMCProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenEventInfoProduct.h"
#include "SimDataFormats/GeneratorProducts/interface/GenRunInfoProduct.h"

#include "HepMC/GenEvent.h"
#include "HepMC/HeavyIon.h"
#include "HepMC/SimpleVector.h"
#include "CLHEP/Random/RandomEngine.h"

static const double pi = 3.14159265358979;

using namespace edm;
using namespace std;
using namespace gen;

CLHEP::HepRandomEngine* _amptRandomEngine;

extern "C"
{
  double gen::ranart_(int *idummy)
  {
    if(0) idummy = idummy; 
    return _amptRandomEngine->flat();
  }
}

AMPTHadronizer::AMPTHadronizer(const ParameterSet &pset) :
    BaseHadronizer(pset),
    evt(0), 
    pset_(pset),
    bmax_(pset.getParameter<double>("bMax")),
    bmin_(pset.getParameter<double>("bMin")),
    efrm_(pset.getParameter<double>("comEnergy")),
    frame_(pset.getParameter<string>("frame")),
    proj_(pset.getParameter<string>("proj")),
    targ_(pset.getParameter<string>("targ")),
    iap_(pset.getParameter<int>("iap")),
    izp_(pset.getParameter<int>("izp")),
    iat_(pset.getParameter<int>("iat")),
    izt_(pset.getParameter<int>("izt")),
    phi0_(0.),
    sinphi0_(0.),
    cosphi0_(1.),
    rotate_(pset.getParameter<bool>("rotateEventPlane"))
{
  // Default constructor
  edm::Service<RandomNumberGenerator> rng;
  _amptRandomEngine = &(rng->getEngine());
}


//_____________________________________________________________________
AMPTHadronizer::~AMPTHadronizer()
{
}

//_____________________________________________________________________
void AMPTHadronizer::add_heavy_ion_rec(HepMC::GenEvent *evt)
{
  // heavy ion record in the final CMSSW Event
  HepMC::HeavyIon* hi = new HepMC::HeavyIon(
    hmain1.jatt,                               // Ncoll_hard/N of SubEvents
    hmain1.np,                               // Npart_proj
    hmain1.nt,                               // Npart_targ
    hmain1.n0,                          // Ncoll
    0,                                   // spectator_neutrons
    0,                                   // spectator_protons
    hmain1.n0,                          // N_Nwounded_collisions
    hmain1.n0,                          // Nwounded_N_collisions
    hmain1.n0,                          // Nwounded_Nwounded_collisions
    hparnt.hint1[18],                   // impact_parameter in [fm]
    phi0_,                               // event_plane_angle
    0,                                   // eccentricity
    hparnt.hint1[11]                    // sigma_inel_NN
  );
  evt->set_heavy_ion(*hi);
  delete hi;
}

//___________________________________________________________________     
HepMC::GenParticle* AMPTHadronizer::build_ampt(int index, int barcode)
{
   // Build particle object corresponding to index in ampt
                                                                                                                                                         
   double x0 = hmain2.patt[0][index];
   double y0 = hmain2.patt[1][index];

   //cout<<"px="<<x0<<" py="<<y0<<" pz="<<hmain2.patt[2][index]<<" mass="<<sqrt(hmain2.patt[3][index]*hmain2.patt[3][index]-hmain2.patt[2][index]*hmain2.patt[2][index]-x0*x0-y0*y0);

   double x = x0*cosphi0_-y0*sinphi0_;
   double y = y0*cosphi0_+x0*sinphi0_;

   HepMC::GenParticle* p = new HepMC::GenParticle(
                                                  HepMC::FourVector(x,  // px                                                                            
                                                                    y,  // py                                                                            
                                                                    hmain2.patt[2][index],  // pz                                                         
                                                                    hmain2.patt[3][index]), // E                                                          
                                                  hmain2.katt[0][index],// id                                                                             
                                                  hmain2.katt[3][index] // status                                                          
                                                  );
   p->suggest_barcode(barcode);

   return p;
}

//___________________________________________________________________     
HepMC::GenVertex* AMPTHadronizer::build_ampt_vertex(int i,int id)
{
   // build verteces for the ampt stored events                        
   HepMC::GenVertex* vertex = new HepMC::GenVertex(HepMC::FourVector(0,0,0,0),id);
   return vertex;
}

bool AMPTHadronizer::generatePartonsAndHadronize()
{
   // generate single event
   if(rotate_) rotateEvtPlane();

   // generate a AMPT event
   AMPT(frame_.data(), bmin_, bmax_, strlen(frame_.data()));

   // event information
   HepMC::GenEvent *evt = new HepMC::GenEvent();
   get_particles(evt); 

   //   evt->set_signal_process_id(pypars.msti[0]);      // type of the process
   //   evt->set_event_scale(pypars.pari[16]);           // Q^2
   add_heavy_ion_rec(evt);

   event().reset(evt);


   return true;
}

//_____________________________________________________________________  
bool AMPTHadronizer::get_particles(HepMC::GenEvent *evt )
{
      HepMC::GenVertex*  vertice;

      vector<HepMC::GenParticle*> particles;
      vector<int>                 mother_ids;
      vector<HepMC::GenVertex*>   prods;

      vertice = new HepMC::GenVertex(HepMC::FourVector(0,0,0,0),0);
      evt->add_vertex(vertice);
      if(!evt->signal_process_vertex()) evt->set_signal_process_vertex(vertice);

      const unsigned int knumpart = hmain1.natt;
      // cout<<"# of particles "<<knumpart<<endl;
      for (unsigned int ipart = 0; ipart<knumpart; ipart++) {

         int mid = hmain2.katt[2][ipart];
         particles.push_back(build_ampt(ipart,ipart+1));
         prods.push_back(build_ampt_vertex(ipart,0));
         mother_ids.push_back(mid);
         LogDebug("DecayChain")<<"Mother index : "<<mid;
      }
      
      LogDebug("AMPT")<<"Number of particles in vector "<<particles.size();

      for (unsigned int ipart = 0; ipart<particles.size(); ipart++) {
	 HepMC::GenParticle* part = particles[ipart];

         int mid = mother_ids[ipart];
	 LogDebug("DecayChain")<<"Particle "<<ipart;
	 LogDebug("DecayChain")<<"Mother's ID "<<mid;
	 LogDebug("DecayChain")<<"Particle's PDG ID "<<part->pdg_id();

         if(mid <= 0){
            vertice->add_particle_out(part);
            continue;
         }

         if(mid > 0){
	    HepMC::GenParticle* mother = particles[mid];
	    LogDebug("DecayChain")<<"Mother's PDG ID "<<mother->pdg_id();

	    HepMC::GenVertex* prod_vertex = mother->end_vertex();
            if(!prod_vertex){
               prod_vertex = prods[ipart];
               prod_vertex->add_particle_in(mother);
               evt->add_vertex(prod_vertex);
               prods[ipart]=0; // mark to protect deletion                                                                                                   

            }
            prod_vertex->add_particle_out(part);
         }
      }

      // cleanup vertices not assigned to evt                                                                                                            
      for (unsigned int i = 0; i<prods.size(); i++) {
         if(prods[i]) delete prods[i];
      }

   return true;
}

//_____________________________________________________________________
bool AMPTHadronizer::call_amptset(double efrm, std::string frame, std::string proj, std::string targ, int iap, int izp, int iat, int izt)
{
  // initialize hydjet  
   AMPTSET(efrm,frame.data(),proj.data(),targ.data(),iap,izp,iat,izt,strlen(frame.data()),strlen(proj.data()),strlen(targ.data()));
   return true;
}

//_____________________________________________________________________
bool AMPTHadronizer::initializeForInternalPartons(){

  //initialize pythia5
  /*
  if(0){
    std::string dumstr = "";
    call_pygive(dumstr);
  }
  */

   // initialize ampt
   LogInfo("AMPTinAction") << "##### Calling HIJSET(" << efrm_ << "," <<frame_<<","<<proj_<<","<<targ_<<","<<iap_<<","<<izp_<<","<<iat_<<","<<izt_<<") ####";

   call_amptset(efrm_,frame_,proj_,targ_,iap_,izp_,iat_,izt_);

   return true;

}

bool AMPTHadronizer::declareStableParticles( std::vector<int> pdg )
{
   return true;
}

//________________________________________________________________                                                                    
void AMPTHadronizer::rotateEvtPlane(){
   phi0_ = 2.*pi*gen::ranart_(0) - pi;
   sinphi0_ = sin(phi0_);
   cosphi0_ = cos(phi0_);
}

//________________________________________________________________ 
bool AMPTHadronizer::hadronize()
{
   return false;
}

bool AMPTHadronizer::decay()
{
   return true;
}
   
bool AMPTHadronizer::residualDecay()
{  
   return true;
}

void AMPTHadronizer::finalizeEvent(){
    return;
}

void AMPTHadronizer::statistics(){
    return;
}

const char* AMPTHadronizer::classname() const
{  
   return "gen::AMPTHadronizer";
}

