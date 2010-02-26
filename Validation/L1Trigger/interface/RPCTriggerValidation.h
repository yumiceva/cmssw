#ifndef Validation_L1Trigger_RPCTriggerValidation_H
#define Validation_L1Trigger_RPCTriggerValidation_H
// -*- C++ -*-
//
// Package:    RPCTriggerValidation
// Class:      RPCTriggerValidation
// 
/**\class RPCTriggerValidation RPCTriggerValidation.h Validation/L1Trigger/interface/RPCTriggerValidation.h

 Description: Validation module for RPCTrigger

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Tomasz Maciej Frueboes
//         Created:  Wed Aug  5 16:03:51 CEST 2009
// $Id: RPCTriggerValidation.h,v 1.3 2009/11/06 14:30:33 fruboes Exp $
//
//


// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"

#include <L1Trigger/RPCTrigger/interface/RPCConst.h>
#include <L1Trigger/RPCTrigger/interface/RPCLogCone.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//#include "DataFormats/L1GlobalMuonTrigger/interface/L1MuRegionalCand.h"
#include "DataFormats/Candidate/interface/Candidate.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "RPCTriggerValidationStruct.h"



#include <vector>
#include <string>
//
// class decleration
//

#include <iosfwd>
#include <iostream>

using namespace RPCTriggerValidationStruct;

class L1MuRegionalCand;

class RPCTriggerValidation : public edm::EDAnalyzer {
   public:
      explicit RPCTriggerValidation(const edm::ParameterSet&);
      ~RPCTriggerValidation();

//       struct L1MuonCandLocalInfo {
//          L1MuonCandLocalInfo(L1MuRegionalCand const & cand);
//          private:
//            int _charge;
//            int _bx;
//            int _ptCode;
//            int _tower;
// 	   float _phi;
//            float _eta;
// 	   int _quality;
//          public:
//            float eta() const {return _eta;};
//            float phi() const {return _phi;};
//            int ptCode() const {return  _ptCode;};
//            int bx()  const {return _bx;};
//            int charge()  const {return _charge;};    
//            int tower() const {return  (_tower> 16) ? -( (~_tower & 63) + 1) : _tower ;};
// 	   int quality() const {return _quality;};
// 	
// 
//       };
// 
//       struct GenMuonLocalInfo {
//         GenMuonLocalInfo(reco::CandidateBaseRef ref) : _charge( ref->charge()), 
//                                                        _pt( ref->pt()), 
//                                                        _phi( ref->phi() ), 
//                                                        _eta( ref->eta()) {} ;
//         private:
//           int _charge;
//           float _pt;
//           float _phi; 
//           float _eta;
//         public:
//           float eta() const {return _eta;};
//           float phi() const {return _phi;};
//           float pt() const {return _pt; };
//           int charge() const {return _charge;};
//      	  std::vector<L1MuonCandLocalInfo> _l1cands;
// 
// 	
// 
// 
// 
//       };

/*
      struct MEResolution {
        public:
          MEResolution(edm::ParameterSet ps, DQMStore * dqm): _etaL(ps.getParameter<double>("etaL")), 
                                              _etaH(ps.getParameter<double>("etaH")),
                                              _ptL(ps.getParameter<double>("ptL")),
                                              _ptH(ps.getParameter<double>("ptH"))
	      
          {

      
	    
            // 1) unique name containing information about eta{L,H} and pt{L,H} 
            // 2) nice title with same information 
	                for (  int it=0; it<(2*(_NumberOfQuality+1)) ; ++it  ) {
			std::stringstream name;
	    std::stringstream title;
	name<<"Resolution_eta"<<changedot(_etaL)<<"_"<<changedot(_etaH)<<"_pt"<<changedot(_ptL)<<"_"<<changedot(_ptH)<<"_quality_"<< ((it>4) ? it-5 : it);
	    title<<"RPCTrigger: Resolution  #eta ["<<_etaL<<":"<<_etaH<<"] pt ["<<_ptL<<":"<<_ptH<<"] quality "<<((it>4) ? (it-5) : it);
	    if(it>4){
	    name<<"_ghost";
	    title<<" ghost";
	  		}
            _meVec[it] = dqm->book1D(name.str(),title.str(), 32, -0.5, _const.m_PT_CODE_MAX+0.5); 
            _meVec[it]->setAxisTitle("Pt Code",1);
	    _meVec[it]->setAxisTitle("Muons",2);
	    }                               
          };
          
          void fill( GenMuonLocalInfo gl ) 
          {
            // TODO check if ptGen (not ptCode) and etaGen is in range. If so - fill _me with reconstructed ptCode
            if((gl.pt()>=_ptL)&&(gl.pt()<=_ptH)&&(gl.eta()>=_etaL)&&(gl.eta()<=_etaH)){
	    
	    if((gl._l1cands.size()>0)){
	    _meVec[(gl._l1cands.begin()->quality())]->Fill((gl._l1cands.begin())->ptCode());
	    _meVec[_NumberOfQuality]->Fill((gl._l1cands.begin())->ptCode());
	    }
	    if(gl._l1cands.size()>1) {
	    _meVec[(++gl._l1cands.begin())->quality()+1+_NumberOfQuality]->Fill((++gl._l1cands.begin())->ptCode());
	    _meVec[2*_NumberOfQuality+1]->Fill((++gl._l1cands.begin())->ptCode());
	    }
	    
	    if((gl._l1cands.size()==0)){
	    _meVec[0]->Fill(0);
	    _meVec[_NumberOfQuality]->Fill(0);
	    }
	    
	    }
          };
	  
	  
	  
        private:
          float _etaL, _etaH, _ptL, _ptH;
          
	  MonitorElement * _meVec[18];

	  std::string changedot(double x)
	  {
		std::stringstream ss;
		ss<<x;
		std::string s=ss.str();
		s.replace(s.find('.'),1,1,'x');
		return s;
	  };  

      
      };

      struct MEDistribution {
        public:
          MEDistribution(edm::ParameterSet ps, DQMStore * dqm): 
                                              _ptL(ps.getParameter<double>("ptL")),
                                              _ptH(ps.getParameter<double>("ptH")) 
          {
	  
 	
	   float pi = 3.14159265;	
	   
	    for (  int it=0; it<4; ++it  ) {
			std::stringstream name;
	    std::stringstream title;
		name<<"Distribution_"<<_tag[it]<<"_TowerVsPhi_pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	    title<<"RPCTrigger: Distribution "<<_tag[it]<<" Tower Vs #phi pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meVecTowerVsPhiGen[it] = dqm->book2D(name.str(),title.str(), 2*_const.m_TOWER_COUNT,-1* _const.m_TOWER_COUNT+1,_const.m_TOWER_COUNT,RPCConst::NSEG,-pi,pi); 
            _meVecTowerVsPhiGen[it]->setAxisTitle("Tower",1);
	    _meVecTowerVsPhiGen[it]->setAxisTitle("#phi",2);
	    
	    }    
	    for (  int it=0; it<4; ++it  ) {
			std::stringstream name;
	    std::stringstream title;
		name<<"Distribution_"<<_tag[it]<<"_EtaVsPhi_pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	    title<<"RPCTrigger: Distribution "<<_tag[it]<<" #eta Vs #phi pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meVecEtaVsPhiGen[it] = dqm->book2D(name.str(),title.str(), 100, -2.15, 2.15,RPCConst::NSEG,-pi,pi); 
            _meVecEtaVsPhiGen[it]->setAxisTitle("#eta",1);
	    _meVecEtaVsPhiGen[it]->setAxisTitle("#phi",2);
	    
	    } 
	           std::stringstream name;
	    std::stringstream title;
		name<<"Distribution_"<<_tag[1]<<"_TowerL1VsPhiL1_pt_"<<changedot(_ptL)<<"_"<<changedot(_ptH);
	    title<<"RPCTrigger: Distribution "<<_tag[1]<<" TowerL1 Vs PhiL1 pt ["<<_ptL<<","<<_ptH<<"]";
	    
            _meTowerVsPhiL1 = dqm->book2D(name.str(),title.str(), 2*_const.m_TOWER_COUNT,-1* _const.m_TOWER_COUNT+1,_const.m_TOWER_COUNT,RPCConst::NSEG,-pi,pi); 
            _meTowerVsPhiL1->setAxisTitle("Tower",1);
	    _meTowerVsPhiL1->setAxisTitle("#phi",2);                       
          };
          
          void fill( GenMuonLocalInfo gl ) {
            
            if((gl.pt()>=_ptL)&&(gl.pt()<=_ptH)){ 
	    
	    _meVecEtaVsPhiGen[3]->Fill(gl.eta(),gl.phi());
	    _meVecTowerVsPhiGen[3]->Fill(_const.towerNumFromEta(gl.eta()),gl.phi());
	    
	    if(gl._l1cands.size()>0) {
	     _meVecEtaVsPhiGen[1]->Fill(gl.eta(),gl.phi());
	     _meVecTowerVsPhiGen[1]->Fill(_const.towerNumFromEta(gl.eta()),gl.phi());
	     _meTowerVsPhiL1->Fill(gl._l1cands.begin()->tower(),gl._l1cands.begin()->phi());
	     }
	    
	    if(gl._l1cands.size()>1)  {
	    _meVecEtaVsPhiGen[2]->Fill(gl.eta(),gl.phi());
	    _meVecTowerVsPhiGen[2]->Fill(_const.towerNumFromEta(gl.eta()),gl.phi());
	    }
	    if(gl._l1cands.size()==0)  {
	    _meVecEtaVsPhiGen[0]->Fill(gl.eta(),gl.phi());
	    _meVecTowerVsPhiGen[0]->Fill(_const.towerNumFromEta(gl.eta()),gl.phi());
	   }
	   } 
	    
          }
        private:
          float  _ptL, _ptH;
          
          MonitorElement* _meVecEtaVsPhiGen[4];
	  MonitorElement* _meVecTowerVsPhiGen[4];
	  MonitorElement * _meTowerVsPhiL1;
      	 static const std::string _tag[] ;
	 
	  std::string changedot(double x)
	  {
		std::stringstream ss;
		ss<<x;
		std::string s=ss.str();
		s.replace(s.find('.'),1,1,'x');
		return s;
	  }

	 
      };const
     
      
       struct MEEfficieny {
        public:
          MEEfficieny(edm::ParameterSet ps, DQMStore * dqm): 
                                              _etaL(ps.getParameter<double>("etaL")),
                                              _etaH(ps.getParameter<double>("etaH")) 
          {
     
	std::stringstream name;
	std::stringstreamusing namespace std; title;
	
	name<<"EfficienyVsPt_eta_"<<changedot(_etaL)<<"_"<<changedot(_etaH);
	title<<"RPCTrigger: Efficieny Vs Pt #eta ["<<_etaL<<","<<_etaH<<"]";
	    
        _meNomPt = dqm->book1D(name.str(),title.str(),32, -0.5, _const.m_PT_CODE_MAX+0.5);
	    // 1) unique name containing information about eta{L,H} and pt{L,H} 
            // 2) nice title with same information 
	 name<<"_Denom";
	 title<<" Denom";
	 
   	_meDenomPt = dqm->book1D(name.str(),title.str(),32, -0.5, _const.m_PT_CODE_MAX+0.5);
	
            _meNomPt->setAxisTitle("Pt Code",1);
	    _meNomPt->setAxisTitle("Efficieny",2);                       
          };
          
          void fill( GenMuonLocalInfo gl ) {
            
            if((gl.eta()>=_etaL)&&(gl.eta()<=_etaH)){
	    
	    if(gl._l1cands.size()>0) _meNomPt->Fill(_const.iptFromPt(gl.pt()));
	    _meDenomPt->Fill(_const.iptFromPt(gl.pt()));
	    
	    
	   } 
	    
          }
	  
	  void dev(  ) {
            
            _meNomPt->getTH1F()->Divide((_meDenomPt->getTH1F()));
	    
	    
	   } 
	    
          
	  
	  
        private:
          float  _etaL, _etaH;
          

	  MonitorElement * _meDenomPt;
	  MonitorElement * _meNomPt;
	  
	  std::string changedot(double x)
	  {
		std::stringstream ss;
		ss<<x;
		std::string s=ss.str();
		s.replace(s.find('.'),1,1,'x');
		return s;
	  }

      	 
      }; 
   
   
   */
   private:
      virtual void beginJob() ;
      virtual void analyze(const edm::Event&, const edm::EventSetup&);
      virtual void endJob() ;

      void assignCandidatesToGens( std::vector<GenMuonLocalInfo> & gens, 
                                   std::vector<L1MuonCandLocalInfo> & l1cands);
				   
	//static int  _NumberOfQuality; 			   
        //static RPCConst _const;
      MonitorElement * nomEta, * denomEta, * ghost, * unassigned,* nomPt, * denomPt;
      std::vector<MEResolution> _meResolutionVec; 
      std::vector<MEDistribution> _meDistributionVec;
      std::vector<MEEfficieny> _meEfficienyVec;
      

      
      std::vector<edm::InputTag > m_l1CollectionsVec; 
      edm::InputTag m_inColMC;       /// input tag for reco::muon (DQMOffline) or genParticles (Validation)
      std::string m_outputDirectory; /// where to put DQM histos
      std::string m_outputFile;
      DQMStore * dqm;
      double deltaRThreshold;
      
      bool m_L1MuonFromReco;
      
};


std::ostream& operator<<( std::ostream& os, const RPCTriggerValidationStruct::L1MuonCandLocalInfo& l1 );
std::ostream& operator<<( std::ostream& os, const RPCTriggerValidationStruct::GenMuonLocalInfo& l1 );

#endif

