#ifndef RecoLuminosity_LumiProducer_LumiCalculator_h
#define RecoLuminosity_LumiProducer_LumiCalculator_h
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/Run.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"
#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/Luminosity/interface/LumiSummary.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "HLTrigger/HLTcore/interface/HLTConfigProvider.h"
#include <boost/regex.hpp>
#include <iostream>
#include <map>
struct MyPerTriggerInfo{
  std::string hltname;
  std::string l1name;
  unsigned int l1prescale;
  unsigned int l1out;
  unsigned int hltin;
  unsigned int hltout;
  unsigned int hltprescale;
};
struct MyPerLumiInfo{
  unsigned int lsnum;
  float livefraction;
  float intglumi;
  std::vector<MyPerTriggerInfo> triggers;
};
class LumiCalculator : public edm::EDAnalyzer{
public:
  
  explicit LumiCalculator(edm::ParameterSet const&);
  virtual ~LumiCalculator();

private:  
  virtual void beginJob(const edm::EventSetup& );
  virtual void beginRun(const edm::Run& run, const edm::EventSetup& c);
  virtual void analyze(edm::Event const& e, edm::EventSetup const& c);
  virtual void endLuminosityBlock(edm::LuminosityBlock const& lumiBlock, 
				  edm::EventSetup const& c);
  virtual void endRun(edm::Run const&, edm::EventSetup const&);
  virtual void endJob();
  std::vector<std::string> splitpathstr(const std::string& strValue,const std::string separator);
  HLTConfigProvider hltConfig_;
  std::multimap<std::string,std::string> trgpathMmap_;
  std::vector<MyPerLumiInfo> perrunlumiinfo_;
};//end class

// -----------------------------------------------------------------

LumiCalculator::LumiCalculator(edm::ParameterSet const& iConfig){
}

// -----------------------------------------------------------------

LumiCalculator::~LumiCalculator(){
}

// -----------------------------------------------------------------

void LumiCalculator::analyze(edm::Event const& e,edm::EventSetup const&){
  
}

// -----------------------------------------------------------------

void LumiCalculator::beginJob(const edm::EventSetup& c){
  
}

// -----------------------------------------------------------------

void LumiCalculator::beginRun(const edm::Run& run, const edm::EventSetup& c){
  std::cout<<"I'm in run number "<<run.run()<<std::endl;
  if(!hltConfig_.init("HLT")){
    throw cms::Exception("HLT process cannot be initialized");
  }
  perrunlumiinfo_.clear();
  trgpathMmap_.clear();
  
  hltConfig_.dump("processName");
  hltConfig_.dump("TableName");
  //hltConfig_.dump("Triggers");
  //hltConfig_.dump("Modules");  
  unsigned int totaltrg=hltConfig_.size();
  for (unsigned int t=0;t<totaltrg;++t){
    std::string hltname(hltConfig_.triggerName(t));
    std::vector<std::string> numpathmodules=hltConfig_.moduleLabels(hltname);
    std::cout<<t<<" HLT path\t"<<hltname<<std::endl;
    std::vector<std::string>::iterator hltpathBeg=numpathmodules.begin();
    std::vector<std::string>::iterator hltpathEnd=numpathmodules.end();
    for(std::vector<std::string>::iterator numpathmodule = hltpathBeg;
	numpathmodule!=hltpathEnd; ++numpathmodule ) {
      if (hltConfig_.moduleType(*numpathmodule) == "HLTLevel1GTSeed"){
	edm::ParameterSet l1GTPSet=hltConfig_.modulePSet(*numpathmodule);
	std::string l1pathname=l1GTPSet.getParameter<std::string>("L1SeedsLogicalExpression");
	if(l1pathname.find("OR")!=std::string::npos){
	  std::cout<<"    L1SeedsLogicalExpression(ORed)\t"<< l1pathname<< std::endl;
	  std::vector<std::string> seeds=splitpathstr(l1pathname," OR ");
	  for(std::vector<std::string>::iterator i=seeds.begin();i!=seeds.end();++i){
	    if(i->size()!=0)std::cout<<"\t\tseed: "<<*i<<std::endl;
	    if(i==seeds.begin()){//for now we take the first one
	      trgpathMmap_.insert(std::make_pair(hltname,*i));
	    }
	  }
	}else if (l1pathname.find("AND")!=std::string::npos){
	  std::cout<<"    L1SeedsLogicalExpression(ANDed)\t"<< l1pathname<< std::endl;
	  std::vector<std::string> seeds=splitpathstr(l1pathname," AND ");
	  for(std::vector<std::string>::iterator i=seeds.begin();
	      i!=seeds.end();++i){
	    if(i->size()!=0)std::cout<<"\t\tseed: "<<*i<<std::endl;
	    if(i==seeds.begin()){//for now we take the first one 
	      trgpathMmap_.insert(std::make_pair(hltname,*i));
	    }
	  }
	}else{
	  std::cout<<"    L1Seeds(ONE)\t"<< l1pathname<< std::endl;
	  trgpathMmap_.insert(std::make_pair(hltname,l1pathname));
	}
      }
    }
    
  }  
}

// -----------------------------------------------------------------
void LumiCalculator::endLuminosityBlock(edm::LuminosityBlock const& lumiBlock, 
					edm::EventSetup const& c){
      /**Integrated Luminosity per Lumi Section
     instantaneousLumi*93.244(sec)
   **/
  //std::cout<<"I'm in lumi block "<<lumiBlock.id()<<std::endl;
  MyPerLumiInfo l;
  l.lsnum=lumiBlock.id().luminosityBlock();
  edm::Handle<LumiSummary> lumiSummary;
  lumiBlock.getByLabel("lumiProducer", lumiSummary);
  l.intglumi=lumiSummary->avgInsDelLumi()*93.244;
  l.livefraction=lumiSummary->liveFrac();
  std::cout<<"Integrated Luminosity for Lumi Section "<<lumiBlock.id().luminosityBlock()<<" "<<l.intglumi<<std::endl;
  std::multimap<std::string,std::string>::iterator trgIt;
  std::multimap<std::string,std::string>::iterator trgBeg=trgpathMmap_.begin();
  std::multimap<std::string,std::string>::iterator trgEnd=trgpathMmap_.end();
  for(trgIt=trgBeg; trgIt!=trgEnd;++trgIt){
    MyPerTriggerInfo trg;
    trg.hltname=trgIt->first;
    trg.l1name=trgIt->second;
    trg.hltin=lumiSummary->hltinfo(trg.hltname).inputcount;
    trg.hltout=lumiSummary->hltinfo(trg.hltname).ratecount;
    trg.hltprescale=lumiSummary->hltinfo(trg.hltname).scalingfactor;
    //std::cout<<"trigger name "<<trg.l1name<<std::endl;
    trg.l1out=lumiSummary->l1info(trg.l1name).ratecount;
    trg.l1prescale=lumiSummary->l1info(trg.l1name).scalingfactor;
    l.triggers.push_back(trg);
  }
  perrunlumiinfo_.push_back(l);
}
 
// -----------------------------------------------------------------
void LumiCalculator::endRun(edm::Run const& run, edm::EventSetup const& c){
  /**Integrated Luminosity per run (delivered,recorded)
     Delivered: sum over all LS lumiSummary->avgInsDelLumi()*93.244 
     Recorded: sum over HLX&&HF certified LS
     avgInsDelLumi()*93.244*livefraction()
     
     For the moment, we take only the first L1 seed in case of 'OR' or 'AND'
     relationship between HLT and L1 seeds
     Effective Luminosity per run per trigger line
      avgInsDelLumi()*93.244*livefraction()/(HLTprescale*L1prescale)
  **/
  std::cout<<"valid trigger lines "<<trgpathMmap_.size()<<std::endl;
  std::cout<<"total lumi lines "<<perrunlumiinfo_.size()<<std::endl;
  std::vector<MyPerLumiInfo>::const_iterator lumiIt;
  std::vector<MyPerLumiInfo>::const_iterator lumiItBeg=perrunlumiinfo_.begin();
  std::vector<MyPerLumiInfo>::const_iterator lumiItEnd=perrunlumiinfo_.end();  
  for(lumiIt=lumiItBeg;lumiIt!=lumiItEnd;++lumiIt){
    std::cout<<"\tLumiSection "<<lumiIt->lsnum<<std::endl;
    std::vector<MyPerTriggerInfo>::const_iterator trIt;
    std::vector<MyPerTriggerInfo>::const_iterator trItBeg=lumiIt->triggers.begin();
    std::vector<MyPerTriggerInfo>::const_iterator trItEnd=lumiIt->triggers.end();
    for(trIt=trItBeg;trIt!=trItEnd;++trIt){
      std::cout<<"\t   HLT name "<<trIt->hltname<<" : HLT in "<<trIt->hltin<<" : HLT out "<<trIt->hltout<<" : HLT prescale "<<trIt->hltprescale<<std::endl;
      std::cout<<"\t     L1 name "<<trIt->l1name<<" : L1 out "<<trIt->l1out<<" : L1 prescale "<<trIt->l1prescale<<std::endl;
    }
  }
}

// -----------------------------------------------------------------
void LumiCalculator::endJob(){
}

std::vector<std::string>
LumiCalculator::splitpathstr(const std::string& strValue,const std::string separator){
  std::vector<std::string> vecstrResult;
  boost::regex re(separator);
  boost::sregex_token_iterator p(strValue.begin(),strValue.end(),re,-1);
  boost::sregex_token_iterator end;
  while(p!=end){
    vecstrResult.push_back(*p++);
  }
  return vecstrResult;
}

DEFINE_FWK_MODULE(LumiCalculator);
#endif
