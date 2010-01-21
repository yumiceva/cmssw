#include "DQM/SiStripMonitorSummary/plugins/SiStripPlotGain.h"

#include "DataFormats/SiStripDetId/interface/TIBDetId.h"
#include "DataFormats/SiStripDetId/interface/TOBDetId.h"
#include "DataFormats/SiStripDetId/interface/TECDetId.h"
#include "DataFormats/SiStripDetId/interface/TIDDetId.h"



SiStripPlotGain::SiStripPlotGain(const edm::ParameterSet& iConfig):
  cacheID(0xFFFFFFFF)
{
  //now do what ever initialization is needed
  if(!edm::Service<SiStripDetInfoFileReader>().isAvailable()){
    edm::LogError("TkLayerMap") << 
      "\n------------------------------------------"
      "\nUnAvailable Service SiStripDetInfoFileReader: please insert in the configuration file an instance like"
      "\n\tprocess.SiStripDetInfoFileReader = cms.Service(\"SiStripDetInfoFileReader\")"
      "\n------------------------------------------";
  }
 
  fr=edm::Service<SiStripDetInfoFileReader>().operator->();
  file = new TFile("correlTest.root","RECREATE");
  tkmap = new TrackerMap();
}


SiStripPlotGain::~SiStripPlotGain()
{}

//

void
SiStripPlotGain::beginRun(const edm::Run& run, const edm::EventSetup& es){

  if(getCache(es)==cacheID )
    return;
  cacheID=getCache(es);  
  
  edm::LogInfo("") << "[SiStripPlotGain::beginRun] cacheID " << cacheID << std::endl; 
  
  es.get<SiStripApvGainRcd>().get(Handle_);
  DoAnalysis(*Handle_.product());


}

void 
SiStripPlotGain::DoAnalysis(const SiStripApvGain& gain){

  edm::LogInfo("") << "[Doanalysis]";

  std::vector<TH1F *>histos;

  SiStripApvGain::RegistryPointers p=gain.getRegistryPointers();
  SiStripApvGain::RegistryConstIterator iter, iterE;
  iter=p.detid_begin;
  iterE=p.detid_end;

  float value;

  //Divide result by d
  for(;iter!=iterE;++iter){
    getHistos(*iter,histos);
    SiStripApvGain::Range range=SiStripApvGain::Range(p.getFirstElement(iter),p.getLastElement(iter));

    edm::LogInfo("") << "[Doanalysis] detid " << *iter << " range " << range.second-range.first;
    size_t apv=0, apvE= (range.second-range.first);
    for (;apv<apvE;apv+=2){       
      value=gain.getApvGain(apv,range);
      tkmap->fill(*iter,value);
      for(size_t i=0;i<histos.size();++i)
	histos[i]->Fill(value);
    }
    
  }
}


void
SiStripPlotGain::getHistos(const uint32_t& detid,std::vector<TH1F*>& histos){
  
  histos.clear();
  
  int subdet=-999; int component=-999;
  SiStripDetId a(detid);
  if ( a.subdetId() == 3 ){
    subdet=0;
    component=TIBDetId(detid).layer();
  } else if ( a.subdetId() == 4 ) {
    subdet=1;
    component=TIDDetId(detid).side()==2?TIDDetId(detid).wheel():TIDDetId(detid).wheel()+3;
  } else if ( a.subdetId() == 5 ) {
    subdet=2;
    component=TOBDetId(detid).layer();
  } else if ( a.subdetId() == 6 ) {
    subdet=3;
    component=TECDetId(detid).side()==2?TECDetId(detid).wheel():TECDetId(detid).wheel()+9;
  } 
  
  int index=100+subdet*100+component;


  histos.push_back(getHisto(subdet+1));
  histos.push_back(getHisto(index));
  
}

TH1F*
SiStripPlotGain::getHisto(const long unsigned int& index){
  if(vTH1.size()<index+1)
    vTH1.resize(index+1,0);
  
  if(vTH1[index]==0){
    char name[128];
    sprintf(name,"%lu",index);
    edm::LogInfo("")<<"[getHisto] creating index " << index << std::endl;
    vTH1[index]=new TH1F(name,name,150,0.,5.);
  }
  
  return vTH1[index];
}

void 
SiStripPlotGain::endJob() {
  for(size_t i=0;i<vTH1.size();i++)
    if(vTH1[i]!=0)
      vTH1[i]->Write();

  file->Write();
  file->Close();

  tkmap->save(false,0,0,"testTkMap.png");

}

