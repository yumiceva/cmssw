/*  \author Anna Cimmino*/
#include <algorithm>
#include <DQM/RPCMonitorClient/interface/RPCFEDIntegrity.h>
#include <DQM/RPCMonitorClient/interface/RPCRawDataCountsHistoMaker.h>
// Framework
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <FWCore/Framework/interface/LuminosityBlock.h>
#include <FWCore/Framework/interface/Event.h>
#include "FWCore/ParameterSet/interface/ParameterSet.h"
//DQM Services
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

//EventFilter
#include "EventFilter/RPCRawToDigi/interface/RPCRawDataCounts.h"
#include "EventFilter/RPCRawToDigi/interface/DataRecord.h"
#include "EventFilter/RPCRawToDigi/interface/ReadoutError.h"


using namespace edm;
using namespace std;

typedef std::map<std::pair<int, int>, int >::const_iterator IT;

RPCFEDIntegrity::RPCFEDIntegrity(const ParameterSet& ps ) {
  LogVerbatim ("rpcfedintegrity") << "[RPCFEDIntegrity]: Constructor";

  prefixDir_ = ps.getUntrackedParameter<string>("RPCPrefixDir", "RPC");
  merge_ = ps.getUntrackedParameter<bool>("MergeRuns", false);
  minFEDNum_ =  ps.getUntrackedParameter<int>("MinimumFEDID", 790);
  maxFEDNum_ =  ps.getUntrackedParameter<int>("MaximumFEDID", 792);

  init_ = false;
  numOfFED_=   maxFEDNum_ -  minFEDNum_ + 1;
  FATAL_LIMIT = 5;
}

RPCFEDIntegrity::~RPCFEDIntegrity(){
  LogVerbatim ("rpcfedintegrity") << "[RPCFEDIntegrity]: Destructor ";
  //  dbe_=0;
}

void RPCFEDIntegrity::beginJob(){
 LogVerbatim ("rpcfedintegrity") << "[RPCFEDIntegrity]: Begin job ";
 dbe_ = Service<DQMStore>().operator->();
}

void RPCFEDIntegrity::beginRun(const Run& r, const EventSetup& c){
 LogVerbatim ("rpcfedintegrity") << "[RPCFEDIntegrity]: Begin run ";

 if (!init_) this->bookFEDMe();
 else if (!merge_) this->reset();

}

void RPCFEDIntegrity::beginLuminosityBlock(LuminosityBlock const& lumiSeg, EventSetup const& context){} 

void RPCFEDIntegrity::analyze(const Event& iEvent, const EventSetup& c) 
{
  Handle<RPCRawDataCounts> rawCounts;
  iEvent.getByType (rawCounts);

  const RPCRawDataCounts  & counts = *rawCounts.product();
 
  for (int fed=minFEDNum_; fed <=maxFEDNum_; ++fed) {
    if (counts.fedBxRecords(fed) ) fedMe_[Entries]->Fill(fed);
    if (counts.fedFormatErrors(fed)) fedMe_[Fatal]->Fill(fed);
    if (counts.fedErrorRecords(fed)) fedMe_[NonFatal]->Fill(fed);
  }
}

void RPCFEDIntegrity::endLuminosityBlock(LuminosityBlock const& lumiSeg, EventSetup const& iSetup) {}

void RPCFEDIntegrity::endRun(const Run& r, const EventSetup& c){}

void RPCFEDIntegrity::endJob(){
  dbe_=0;
}

//Fill report summary
void  RPCFEDIntegrity::bookFEDMe(void){

  if(dbe_){
    dbe_->setCurrentFolder(prefixDir_+"/FEDIntegrity/");

    fedMe_[Entries] =  dbe_->book1D("FEDEntries","FED Events",numOfFED_, minFEDNum_, maxFEDNum_ +1);
    this->labelBins(fedMe_[Entries]);
    fedMe_[Fatal] =  dbe_->book1D("FEDFatal","Data Format Errors",numOfFED_, minFEDNum_, maxFEDNum_ +1);
    this->labelBins(fedMe_[Fatal]);
    fedMe_[NonFatal] =  dbe_->book1D("FEDNonFatal","FED Error Record Present",numOfFED_, minFEDNum_, maxFEDNum_ +1);
   this->labelBins(fedMe_[NonFatal]);
  }

  init_ = true;
}

void RPCFEDIntegrity::labelBins( MonitorElement * myMe){

  int xbins = myMe->getNbinsX();
  
  if (xbins!= numOfFED_ ) return;
  stringstream xLabel;

  for (int i = 0; i<xbins; i++){
    xLabel.str("");
    int fedNum =  minFEDNum_ + i;
    xLabel<<fedNum;
    myMe->setBinLabel(i+1, xLabel.str(),1);    
  }
}


void  RPCFEDIntegrity::reset(void){

  MonitorElement * me;  

  if(dbe_){
    for(unsigned int i = 0; i<histoName_.size(); i++){
      me = 0;
      me = dbe_->get(prefixDir_ +"FEDIntegrity/"+ histoName_[i]);
      if(0!=me ) me->Reset();
    }
  }
}
