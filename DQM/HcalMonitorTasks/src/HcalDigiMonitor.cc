#include "DQM/HcalMonitorTasks/interface/HcalDigiMonitor.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

HcalDigiMonitor::HcalDigiMonitor() {
  doPerChannel_ = false;
  occThresh_ = 1;
  ievt_=0;
}

HcalDigiMonitor::~HcalDigiMonitor() {}

namespace HcalDigiPerChan{
  template<class Digi>
  inline void perChanHists(int id, const Digi& digi, float* ampl,std::map<HcalDetId, MonitorElement*> &tool, DQMStore* dbe, string baseFolder) {
    
    std::map<HcalDetId,MonitorElement*>::iterator _mei;

    string type = "HB";
    if(id==1) type = "HE"; 
    else if(id==2) type = "HO"; 
    else if(id==3) type = "HF"; 
    
    if(dbe) dbe->setCurrentFolder(baseFolder+"/"+type);
    
    ///shapes by channel
    _mei=tool.find(digi.id()); // look for a histogram with this hit's id
    if (_mei!=tool.end()){
      if (_mei->second==0) cout << "HcalDigiMonitor::perChanHists, Found the histo, but it's null??";
      else{
	for (int i=0; i<digi.size(); i++) tool[digi.id()]->Fill(i,ampl[i]);
      }
    }
    else{
      if(dbe){
	char name[1024];
	sprintf(name,"%s Digi Shape ieta=%d iphi=%d depth=%d",type.c_str(),digi.id().ieta(),digi.id().iphi(),digi.id().depth());
	tool[digi.id()] =  dbe->book1D(name,name,11,-0.5,10.5); 
	for (int i=0; i<digi.size(); i++) tool[digi.id()]->Fill(i,ampl[i]);
      }
    }
  }
}

void HcalDigiMonitor::reset(){}

static bool bitUpset(int last, int now){
  if(last ==-1) return false;
  int v = last+1; if(v==4) v=0;
  if(v==now) return false;
  return true;
}

namespace HcalDigiMap{
  template<class Digi>
  inline void fillErrors(const Digi& digi, float* vals,
			 MonitorElement* mapGEO, MonitorElement* mapVME, 
			 MonitorElement* mapDCC){
    mapGEO->Fill(digi.id().ieta(),digi.id().iphi());
    float slotnum = digi.elecId().htrSlot() + 0.5*digi.elecId().htrTopBottom();
    mapVME->Fill(slotnum,digi.elecId().readoutVMECrateId());
    mapDCC->Fill(digi.elecId().spigot(),digi.elecId().dccid());
    return;
  }

  template<class Digi>
  inline void fillOccupancy(const Digi& digi, float* vals, 
			    MonitorElement* mapG1, MonitorElement* mapG2,
			    MonitorElement* mapG3, MonitorElement* mapG4,  
			    MonitorElement* mapVME, MonitorElement* mapDCC, 
			    MonitorElement* mapEta, MonitorElement* mapPhi){
    if(digi.id().depth()==1) mapG1->Fill(digi.id().ieta(),digi.id().iphi());
    else if(digi.id().depth()==2) mapG2->Fill(digi.id().ieta(),digi.id().iphi());
    else if(digi.id().depth()==3) mapG3->Fill(digi.id().ieta(),digi.id().iphi());
    else if(digi.id().depth()==4) mapG4->Fill(digi.id().ieta(),digi.id().iphi());
    float slotnum = digi.elecId().htrSlot() + 0.5*digi.elecId().htrTopBottom();
    mapVME->Fill(slotnum,digi.elecId().readoutVMECrateId());
    mapDCC->Fill(digi.elecId().spigot(),digi.elecId().dccid());
    mapEta->Fill(digi.id().ieta());
    mapPhi->Fill(digi.id().iphi());
    return;
  }

  template<class Digi>
  inline bool digiStats(const Digi& digi, HcalCalibrations calibs, float occThr, 
			float* vals,bool& err, bool& occ, bool& bitUp){
    int last = -1; float pval = -1;
    bitUp=false; err=false; occ=false;
    
    for (int i=0; i<digi.size(); i++) {
      int thisCapid = digi.sample(i).capid();
      if(bitUpset(last,thisCapid)) bitUp=true;
      last = thisCapid;
      if(digi.sample(i).er()) err=true;
      if(!digi.sample(i).dv()) err=true;
      pval = digi.sample(i).adc()-calibs.pedestal(thisCapid);
      vals[i] = pval;
      if(pval>occThr) occ=true;
    }
    if(bitUp) err=true;
    
    return err;
  }
  
}

void HcalDigiMonitor::setup(const edm::ParameterSet& ps, 
			    DQMStore* dbe){
  HcalBaseMonitor::setup(ps,dbe);
  baseFolder_ = rootFolder_+"DigiMonitor";

  occThresh_ = ps.getUntrackedParameter<int>("DigiOccThresh", 10);
  cout << "Digi occupancy threshold set to " << occThresh_ << endl;

  if ( ps.getUntrackedParameter<bool>("DigisPerChannel", false) ) doPerChannel_ = true;  

  etaMax_ = ps.getUntrackedParameter<double>("MaxEta", 42.5);
  etaMin_ = ps.getUntrackedParameter<double>("MinEta", -42.5);
  etaBins_ = (int)(etaMax_ - etaMin_);
  cout << "Digi eta min/max set to " << etaMin_ << "/" <<etaMax_ << endl;

  phiMax_ = ps.getUntrackedParameter<double>("MaxPhi", 73.5);
  phiMin_ = ps.getUntrackedParameter<double>("MinPhi", -0.5);
  phiBins_ = (int)(phiMax_ - phiMin_);
  cout << "Digi phi min/max set to " << phiMin_ << "/" <<phiMax_ << endl;

  ievt_=0;
  
  if ( m_dbe ) {

    m_dbe->setCurrentFolder(baseFolder_);
    meEVT_ = m_dbe->bookInt("Digi Task Event Number");    
    meEVT_->Fill(ievt_);
    
    OCC_L1 = m_dbe->book2D("Digi Depth 1 Occupancy Map","Digi Depth 1 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    OCC_L2 = m_dbe->book2D("Digi Depth 2 Occupancy Map","Digi Depth 2 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    OCC_L3 = m_dbe->book2D("Digi Depth 3 Occupancy Map","Digi Depth 3 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    OCC_L4 = m_dbe->book2D("Digi Depth 4 Occupancy Map","Digi Depth 4 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    OCC_ETA = m_dbe->book1D("Digi Eta Occupancy Map","Digi Eta Occupancy Map",etaBins_,etaMin_,etaMax_);
    OCC_PHI = m_dbe->book1D("Digi Phi Occupancy Map","Digi Phi Occupancy Map",phiBins_,phiMin_,phiMax_);

    OCC_ELEC_VME = m_dbe->book2D("Digi VME Occupancy Map","Digi VME Occupancy Map",40,-0.25,19.75,18,-0.5,17.5);

    OCC_ELEC_DCC = m_dbe->book2D("Digi Spigot Occupancy Map","Digi Spigot Occupancy Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);
    ERR_MAP_GEO = m_dbe->book2D("Digi Geo Error Map","Digi Geo Error Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    ERR_MAP_VME = m_dbe->book2D("Digi VME Error Map","Digi VME Error Map",40,-0.25,19.75,18,-0.5,17.5);

    ERR_MAP_DCC = m_dbe->book2D("Digi Spigot Error Map","Digi Spigot Error Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);
    CAPID_T0 = m_dbe->book1D("Capid 1st Time Slice","Capid for 1st Time Slice",7,-3.5,3.5); 
    DIGI_NUM = m_dbe->book1D("# of Digis","# of Digis",910,-0.5,9099.5);
    BQDIGI_NUM = m_dbe->book1D("# Bad Qual Digis","# Bad Qual Digis",910,-0.5,9099.5);
    BQDIGI_FRAC =  m_dbe->book1D("Bad Digi Fraction","Bad Digi Fraction",220,-0.05,1.05);

    m_dbe->setCurrentFolder(baseFolder_+"/HB");
    hbHists.SHAPE_tot =  m_dbe->book1D("HB Digi Shape","HB Digi Shape",11,-0.5,10.5);
    hbHists.SHAPE_THR_tot =  m_dbe->book1D("HB Digi Shape - over thresh","HB Digi Shape - over thresh",11,-0.5,10.5);

    hbHists.DIGI_NUM =  m_dbe->book1D("HB # of Digis","HB # of Digis",2700,-0.5,2699.5);
    hbHists.BQDIGI_NUM =  m_dbe->book1D("HB # Bad Qual Digis","HB # Bad Qual Digis",2700,-0.5,2699.5);
    hbHists.BQDIGI_FRAC =  m_dbe->book1D("HB Bad Digi Fraction","HB Bad Digi Fraction",220,-0.05,1.05);
    hbHists.CAPID_T0 = m_dbe->book1D("HB Capid 1st Time Slice","HB Capid for 1st Time Slice",7,-3.5,3.5);

    hbHists.DIGI_SIZE =  m_dbe->book1D("HB Digi Size","HB Digi Size",50,0,50);
    hbHists.DIGI_PRESAMPLE =  m_dbe->book1D("HB Digi Presamples","HB Digi Presamples",50,0,50);
    hbHists.QIE_CAPID =  m_dbe->book1D("HB QIE Cap-ID","HB QIE Cap-ID",6,-0.5,5.5);
    hbHists.QIE_ADC = m_dbe->book1D("HB QIE ADC Value","HB QIE ADC Value",100,0,200);
    hbHists.QIE_DV = m_dbe->book1D("HB QIE Data Valid Err Bits","HB QIE Data Valid Err Bits",4,-0.5,3.5);
    
    hbHists.ERR_MAP_GEO = m_dbe->book2D("HB Digi Geo Error Map","HB Digi Geo Error Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hbHists.ERR_MAP_VME = m_dbe->book2D("HB Digi VME Error Map","HB Digi VME Error Map",40,-0.25,19.75,18,-0.5,17.5);

    hbHists.ERR_MAP_DCC = m_dbe->book2D("HB Digi Spigot Error Map","HB Digi Spigot Error Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);

    hbHists.OCC_MAP_GEO1 = m_dbe->book2D("HB Digi Depth 1 Occupancy Map","HB Digi Depth 1 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hbHists.OCC_MAP_GEO2 = m_dbe->book2D("HB Digi Depth 2 Occupancy Map","HB Digi Depth 2 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hbHists.OCC_MAP_GEO3 = m_dbe->book2D("HB Digi Depth 3 Occupancy Map","HB Digi Depth 3 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hbHists.OCC_MAP_GEO4 = m_dbe->book2D("HB Digi Depth 4 Occupancy Map","HB Digi Depth 4 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hbHists.OCC_ETA = m_dbe->book1D("HB Digi Eta Occupancy Map","HB Digi Eta Occupancy Map",etaBins_,etaMin_,etaMax_);
    hbHists.OCC_PHI = m_dbe->book1D("HB Digi Phi Occupancy Map","HB Digi Phi Occupancy Map",phiBins_,phiMin_,phiMax_);

    hbHists.OCC_MAP_VME = m_dbe->book2D("HB Digi VME Occupancy Map","HB Digi VME Occupancy Map",40,-0.25,19.75,18,-0.5,17.5);

    hbHists.OCC_MAP_DCC = m_dbe->book2D("HB Digi Spigot Occupancy Map","HB Digi Spigot Occupancy Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);



    m_dbe->setCurrentFolder(baseFolder_+"/HE");
    heHists.SHAPE_tot =  m_dbe->book1D("HE Digi Shape","HE Digi Shape",11,-0.5,10.5);
    heHists.SHAPE_THR_tot =  m_dbe->book1D("HE Digi Shape - over thresh","HE Digi Shape - over thresh",11,-0.5,10.5);
    heHists.DIGI_NUM =  m_dbe->book1D("HE # of Digis","HE # of Digis",2700,-0.5,2699.5);
    heHists.DIGI_SIZE =  m_dbe->book1D("HE Digi Size","HE Digi Size",50,0,50);
    heHists.DIGI_PRESAMPLE =  m_dbe->book1D("HE Digi Presamples","HE Digi Presamples",50,0,50);
    heHists.QIE_CAPID =  m_dbe->book1D("HE QIE Cap-ID","HE QIE Cap-ID",6,-0.5,5.5);
    heHists.QIE_ADC = m_dbe->book1D("HE QIE ADC Value","HE QIE ADC Value",100,0,200);
    heHists.QIE_DV = m_dbe->book1D("HE QIE Data Valid Err Bits","HE QIE Data Valid Err Bits",4,-0.5,3.5);

    heHists.BQDIGI_NUM =  m_dbe->book1D("HE # Bad Qual Digis","HE # Bad Qual Digis",2700,-0.5,2699.5);
    heHists.BQDIGI_FRAC =  m_dbe->book1D("HE Bad Digi Fraction","HE Bad Digi Fraction",220,-0.05,1.05);
    heHists.CAPID_T0 = m_dbe->book1D("HE Capid 1st Time Slice","HE Capid for 1st Time Slice",7,-3.5,3.5);

    heHists.ERR_MAP_GEO = m_dbe->book2D("HE Digi Geo Error Map","HE Digi Geo Error Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    heHists.ERR_MAP_VME = m_dbe->book2D("HE Digi VME Error Map","HE Digi VME Error Map",40,-0.25,19.75,18,-0.5,17.5);

    heHists.ERR_MAP_DCC = m_dbe->book2D("HE Digi Spigot Error Map","HE Digi Spigot Error Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);

    heHists.OCC_MAP_GEO1 = m_dbe->book2D("HE Digi Depth 1 Occupancy Map","HE Digi Depth 1 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    heHists.OCC_MAP_GEO2 = m_dbe->book2D("HE Digi Depth 2 Occupancy Map","HE Digi Depth 2 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    heHists.OCC_MAP_GEO3 = m_dbe->book2D("HE Digi Depth 3 Occupancy Map","HE Digi Depth 3 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    heHists.OCC_MAP_GEO4 = m_dbe->book2D("HE Digi Depth 4 Occupancy Map","HE Digi Depth 4 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    heHists.OCC_ETA = m_dbe->book1D("HE Digi Eta Occupancy Map","HE Digi Eta Occupancy Map",etaBins_,etaMin_,etaMax_);
    heHists.OCC_PHI = m_dbe->book1D("HE Digi Phi Occupancy Map","HE Digi Phi Occupancy Map",phiBins_,phiMin_,phiMax_);

    heHists.OCC_MAP_VME = m_dbe->book2D("HE Digi VME Occupancy Map","HE Digi VME Occupancy Map",40,-0.25,19.75,18,-0.5,17.5);

    heHists.OCC_MAP_DCC = m_dbe->book2D("HE Digi Spigot Occupancy Map","HE Digi Spigot Occupancy Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);



    m_dbe->setCurrentFolder(baseFolder_+"/HF");
    hfHists.SHAPE_tot =  m_dbe->book1D("HF Digi Shape","HF Digi Shape",11,-0.5,10.5);
    hfHists.SHAPE_THR_tot =  m_dbe->book1D("HF Digi Shape - over thresh","HF Digi Shape - over thresh",11,-0.5,10.5);
    hfHists.DIGI_NUM =  m_dbe->book1D("HF # of Digis","HF # of Digis",1800,-0.5,1799.5);
    hfHists.DIGI_SIZE =  m_dbe->book1D("HF Digi Size","HF Digi Size",50,0,50);
    hfHists.DIGI_PRESAMPLE =  m_dbe->book1D("HF Digi Presamples","HF Digi Presamples",50,0,50);
    hfHists.QIE_CAPID =  m_dbe->book1D("HF QIE Cap-ID","HF QIE Cap-ID",6,-0.5,5.5);
    hfHists.QIE_ADC = m_dbe->book1D("HF QIE ADC Value","HF QIE ADC Value",100,0,200);
    hfHists.QIE_DV = m_dbe->book1D("HF QIE Data Valid Err Bits","HF QIE Data Valid Err Bits",4,-0.5,3.5);

    hfHists.BQDIGI_NUM =  m_dbe->book1D("HF # Bad Qual Digis","HF # Bad Qual Digis",1800,-0.5,1799.5);
    hfHists.BQDIGI_FRAC =  m_dbe->book1D("HF Bad Digi Fraction","HF Bad Digi Fraction",220,-0.05,1.05);
    hfHists.CAPID_T0 = m_dbe->book1D("HF Capid 1st Time Slice","HF Capid for 1st Time Slice",7,-3.5,3.5);

    hfHists.ERR_MAP_GEO = m_dbe->book2D("HF Digi Geo Error Map","HF Digi Geo Error Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hfHists.ERR_MAP_VME = m_dbe->book2D("HF Digi VME Error Map","HF Digi VME Error Map",40,-0.25,19.75,18,-0.5,17.5);

    hfHists.ERR_MAP_DCC = m_dbe->book2D("HF Digi Spigot Error Map","HF Digi Spigot Error Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);

    hfHists.OCC_MAP_GEO1 = m_dbe->book2D("HF Digi Depth 1 Occupancy Map","HF Digi Depth 1 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hfHists.OCC_MAP_GEO2 = m_dbe->book2D("HF Digi Depth 2 Occupancy Map","HF Digi Depth 2 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hfHists.OCC_MAP_GEO3 = m_dbe->book2D("HF Digi Depth 3 Occupancy Map","HF Digi Depth 3 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hfHists.OCC_MAP_GEO4 = m_dbe->book2D("HF Digi Depth 4 Occupancy Map","HF Digi Depth 4 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hfHists.OCC_ETA = m_dbe->book1D("HF Digi Eta Occupancy Map","HF Digi Eta Occupancy Map",etaBins_,etaMin_,etaMax_);
    hfHists.OCC_PHI = m_dbe->book1D("HF Digi Phi Occupancy Map","HF Digi Phi Occupancy Map",phiBins_,phiMin_,phiMax_);

    hfHists.OCC_MAP_VME = m_dbe->book2D("HF Digi VME Occupancy Map","HF Digi VME Occupancy Map",40,-0.25,19.75,18,-0.5,17.5);

    hfHists.OCC_MAP_DCC = m_dbe->book2D("HF Digi Spigot Occupancy Map","HF Digi Spigot Occupancy Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);



    m_dbe->setCurrentFolder(baseFolder_+"/HO");
    hoHists.SHAPE_tot =  m_dbe->book1D("HO Digi Shape","HO Digi Shape",11,-0.5,10.5);
    hoHists.SHAPE_THR_tot =  m_dbe->book1D("HO Digi Shape - over thresh","HO Digi Shape - over thresh",11,-0.5,10.5);
    hoHists.DIGI_NUM =  m_dbe->book1D("HO # of Digis","HO # of Digis",2200,-0.5,2199.5);
    hoHists.DIGI_SIZE =  m_dbe->book1D("HO Digi Size","HO Digi Size",50,0,50);
    hoHists.DIGI_PRESAMPLE =  m_dbe->book1D("HO Digi Presamples","HO Digi Presamples",50,0,50);
    hoHists.QIE_CAPID =  m_dbe->book1D("HO QIE Cap-ID","HO QIE Cap-ID",6,-0.5,5.5);
    hoHists.QIE_ADC = m_dbe->book1D("HO QIE ADC Value","HO QIE ADC Value",100,0,200);
    hoHists.QIE_DV = m_dbe->book1D("HO QIE Data Valid Err Bits","HO QIE Data Valid Err Bits",4,-0.5,3.5);

    hoHists.BQDIGI_NUM =  m_dbe->book1D("HO # Bad Qual Digis","HO # Bad Qual Digis",2200,-0.5,2199.5);
    hoHists.BQDIGI_FRAC =  m_dbe->book1D("HO Bad Digi Fraction","HO Bad Digi Fraction",220,-0.05,1.05);
    hoHists.CAPID_T0 = m_dbe->book1D("HO Capid 1st Time Slice","HO Capid for 1st Time Slice",7,-3.5,3.5);

    hoHists.ERR_MAP_GEO = m_dbe->book2D("HO Digi Geo Error Map","HO Digi Geo Error Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hoHists.ERR_MAP_VME = m_dbe->book2D("HO Digi VME Error Map","HO Digi VME Error Map",40,-0.25,19.75,18,-0.5,17.5);

    hoHists.ERR_MAP_DCC = m_dbe->book2D("HO Digi Spigot Error Map","HO Digi Spigot Error Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);

    hoHists.OCC_MAP_GEO1 = m_dbe->book2D("HO Digi Depth 1 Occupancy Map","HO Digi Depth 1 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hoHists.OCC_MAP_GEO2 = m_dbe->book2D("HO Digi Depth 2 Occupancy Map","HO Digi Depth 2 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hoHists.OCC_MAP_GEO3 = m_dbe->book2D("HO Digi Depth 3 Occupancy Map","HO Digi Depth 3 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hoHists.OCC_MAP_GEO4 = m_dbe->book2D("HO Digi Depth 4 Occupancy Map","HO Digi Depth 4 Occupancy Map",etaBins_,etaMin_,etaMax_,phiBins_,phiMin_,phiMax_);
    hoHists.OCC_ETA = m_dbe->book1D("HO Digi Eta Occupancy Map","HO Digi Eta Occupancy Map",etaBins_,etaMin_,etaMax_);
    hoHists.OCC_PHI = m_dbe->book1D("HO Digi Phi Occupancy Map","HO Digi Phi Occupancy Map",phiBins_,phiMin_,phiMax_);

    hoHists.OCC_MAP_VME = m_dbe->book2D("HO Digi VME Occupancy Map","HO Digi VME Occupancy Map",40,-0.25,19.75,18,-0.5,17.5);

    hoHists.OCC_MAP_DCC = m_dbe->book2D("HO Digi Spigot Occupancy Map","HO Digi Spigot Occupancy Map",
					HcalDCCHeader::SPIGOT_COUNT,-0.5,HcalDCCHeader::SPIGOT_COUNT-0.5,
					36,-0.5,35.5);
    
}

  return;
}

void HcalDigiMonitor::processEvent(const HBHEDigiCollection& hbhe,
				   const HODigiCollection& ho,
				   const HFDigiCollection& hf,
				   const HcalDbService& cond,
				   const HcalUnpackerReport& report){
  
  if(!m_dbe) { 
    if(fVerbosity) printf("HcalDigiMonitor::processEvent   DQMStore not instantiated!!!\n");  
    return; 
  }
  
  ievt_++;
  meEVT_->Fill(ievt_);

  int nbqdigi_report = report.badQualityDigis();
  if (nbqdigi_report != 0)BQDIGI_NUM->Fill(nbqdigi_report);

  float normVals[10]; bool digiErr=false;
  bool digiOcc=false; bool digiUpset=false;
  int ndigi = 0;  int nbqdigi = 0;
	  
  try{
    int nhedigi = 0;   int nhbdigi = 0;
    int nhbbqdigi = 0;  int nhebqdigi = 0;
    int firsthbcap = -1; int firsthecap = -1;
    for (HBHEDigiCollection::const_iterator j=hbhe.begin(); j!=hbhe.end(); j++){
      const HBHEDataFrame digi = (const HBHEDataFrame)(*j);
      cond.makeHcalCalibration(digi.id(), &calibs_);

      HcalDigiMap::digiStats(digi, calibs_, occThresh_, normVals, digiErr, digiOcc, digiUpset);      
      if((HcalSubdetector)(digi.id().subdet())==HcalBarrel){	
	nhbdigi++;  ndigi++;
	
	if(digiErr){
	  nhbbqdigi++; nbqdigi++;
	  HcalDigiMap::fillErrors<HBHEDataFrame>(digi,normVals,
						 hbHists.ERR_MAP_GEO,hbHists.ERR_MAP_VME,
						 hbHists.ERR_MAP_DCC);	  

	  HcalDigiMap::fillErrors<HBHEDataFrame>(digi,normVals,
						 ERR_MAP_GEO,ERR_MAP_VME,
						 ERR_MAP_DCC);	  
	}

	if(digiOcc){
	  HcalDigiMap::fillOccupancy<HBHEDataFrame>(digi,normVals,
						    hbHists.OCC_MAP_GEO1,hbHists.OCC_MAP_GEO2,
						    hbHists.OCC_MAP_GEO3,hbHists.OCC_MAP_GEO4,
						    hbHists.OCC_MAP_VME, 
						    hbHists.OCC_MAP_DCC,
						    hbHists.OCC_ETA,hbHists.OCC_PHI);
	  
	  HcalDigiMap::fillOccupancy<HBHEDataFrame>(digi,normVals,
						    OCC_L1,OCC_L2,OCC_L3,OCC_L4,
						    OCC_ELEC_VME,OCC_ELEC_DCC,
						    OCC_ETA,OCC_PHI);	  
	}

	hbHists.DIGI_SIZE->Fill(digi.size());
	hbHists.DIGI_PRESAMPLE->Fill(digi.presamples());

	if (firsthbcap == -1) firsthbcap = digi.sample(0).capid();
	int capdif = digi.sample(0).capid() - firsthbcap;
	hbHists.CAPID_T0->Fill(capdif);
	CAPID_T0->Fill(capdif);

	for (int i=0; i<digi.size(); i++) {	    
	  hbHists.QIE_CAPID->Fill(digi.sample(i).capid());
	  hbHists.QIE_ADC->Fill(digi.sample(i).adc());
	  hbHists.SHAPE_tot->Fill(i,normVals[i]);
	  
	  if(digiOcc) hbHists.SHAPE_THR_tot->Fill(i,normVals[i]);
	  if(digiUpset) hbHists.QIE_CAPID->Fill(5);
	  int dver = 2*digi.sample(i).er() + digi.sample(i).dv();
	  hbHists.QIE_DV->Fill(dver);
	}    
	
	if(doPerChannel_)	  
	  HcalDigiPerChan::perChanHists<HBHEDataFrame>(0,digi,normVals,hbHists.SHAPE,m_dbe,baseFolder_);

      }
      else if((HcalSubdetector)(digi.id().subdet())==HcalEndcap){	
	nhedigi++;  ndigi++;

	if(digiErr){
	  nhebqdigi++; nbqdigi++;
	  HcalDigiMap::fillErrors<HBHEDataFrame>(digi,normVals,
						 heHists.ERR_MAP_GEO,heHists.ERR_MAP_VME,
						 heHists.ERR_MAP_DCC);	  

	  HcalDigiMap::fillErrors<HBHEDataFrame>(digi,normVals,
						 ERR_MAP_GEO,ERR_MAP_VME,
						 ERR_MAP_DCC);	  
	}

	if(digiOcc){
	  HcalDigiMap::fillOccupancy<HBHEDataFrame>(digi,normVals,
						    heHists.OCC_MAP_GEO1,heHists.OCC_MAP_GEO2,
						    heHists.OCC_MAP_GEO3,heHists.OCC_MAP_GEO4,
						    heHists.OCC_MAP_VME, 
						    heHists.OCC_MAP_DCC,
						    heHists.OCC_ETA,heHists.OCC_PHI);
	  
	  HcalDigiMap::fillOccupancy<HBHEDataFrame>(digi,normVals,
						    OCC_L1,OCC_L2,OCC_L3,OCC_L4,
						    OCC_ELEC_VME,OCC_ELEC_DCC,
						    OCC_ETA,OCC_PHI);	  
	}
	
	heHists.DIGI_SIZE->Fill(digi.size());
	heHists.DIGI_PRESAMPLE->Fill(digi.presamples());

	if (firsthecap == -1) firsthecap = digi.sample(0).capid();
	int capdif = digi.sample(0).capid() - firsthecap;
	heHists.CAPID_T0->Fill(capdif);
	CAPID_T0->Fill(capdif);

	for (int i=0; i<digi.size(); i++) {	    
	  heHists.QIE_CAPID->Fill(digi.sample(i).capid());
	  heHists.QIE_ADC->Fill(digi.sample(i).adc());
	  heHists.SHAPE_tot->Fill(i,normVals[i]);
	  
	  if(digiOcc) heHists.SHAPE_THR_tot->Fill(i,normVals[i]);
	  if(digiUpset) heHists.QIE_CAPID->Fill(5);
	  int dver = 2*digi.sample(i).er() + digi.sample(i).dv();
	  heHists.QIE_DV->Fill(dver);
	}    
	
	if(doPerChannel_)
	  HcalDigiPerChan::perChanHists<HBHEDataFrame>(1,digi,normVals,heHists.SHAPE,m_dbe,baseFolder_);

      }
    }
    
    hbHists.DIGI_NUM->Fill(nhbdigi);
    hbHists.BQDIGI_NUM->Fill(nhbbqdigi);
    if (nhbdigi != 0)hbHists.BQDIGI_FRAC->Fill((1.0*nhbbqdigi)/(1.0*nhbdigi));

    heHists.DIGI_NUM->Fill(nhedigi);
    heHists.BQDIGI_NUM->Fill(nhebqdigi);
    if (nhedigi != 0)heHists.BQDIGI_FRAC->Fill((1.0*nhebqdigi)/(1.0*nhedigi));
    
  } catch (...) {    
    if(fVerbosity) printf("HcalDigiMonitor::processEvent  No HBHE Digis.\n");
  }
  
  try{
    int firsthocap = -1; int nhobqdigi = 0;
    int nhodigi = ho.size();
    //    hoHists.DIGI_NUM->Fill(ho.size());
    for (HODigiCollection::const_iterator j=ho.begin(); j!=ho.end(); j++){
      const HODataFrame digi = (const HODataFrame)(*j);	
      HcalDigiMap::digiStats(digi, calibs_, occThresh_, normVals, digiErr, digiOcc, digiUpset);      
	
      if(digiErr){
	nhobqdigi++; nbqdigi++;
	HcalDigiMap::fillErrors<HODataFrame>(digi,normVals,
					       hoHists.ERR_MAP_GEO,hoHists.ERR_MAP_VME,
					       hoHists.ERR_MAP_DCC);	  
	
	HcalDigiMap::fillErrors<HODataFrame>(digi,normVals,
					       ERR_MAP_GEO,ERR_MAP_VME,
					       ERR_MAP_DCC);	  
      }
      
      if(digiOcc){
	HcalDigiMap::fillOccupancy<HODataFrame>(digi,normVals,
						  hoHists.OCC_MAP_GEO1,hoHists.OCC_MAP_GEO2,
						  hoHists.OCC_MAP_GEO3,hoHists.OCC_MAP_GEO4,
						hoHists.OCC_MAP_VME, 
						  hoHists.OCC_MAP_DCC,
						  hoHists.OCC_ETA,hoHists.OCC_PHI);
	
	HcalDigiMap::fillOccupancy<HODataFrame>(digi,normVals,
						  OCC_L1,OCC_L2,OCC_L3,OCC_L4,
						  OCC_ELEC_VME,OCC_ELEC_DCC,
						  OCC_ETA,OCC_PHI);	  
      }
      
      hoHists.DIGI_SIZE->Fill(digi.size());
      hoHists.DIGI_PRESAMPLE->Fill(digi.presamples());

      if (firsthocap == -1) firsthocap = digi.sample(0).capid();
      int capdif = digi.sample(0).capid() - firsthocap;
      hoHists.CAPID_T0->Fill(capdif);
      CAPID_T0->Fill(capdif);
      
      for (int i=0; i<digi.size(); i++) {	    
	hoHists.QIE_CAPID->Fill(digi.sample(i).capid());
	hoHists.QIE_ADC->Fill(digi.sample(i).adc());
	hoHists.SHAPE_tot->Fill(i,normVals[i]);
	
	if(digiOcc) hoHists.SHAPE_THR_tot->Fill(i,normVals[i]);
	if(digiUpset) hoHists.QIE_CAPID->Fill(5);
	int dver = 2*digi.sample(i).er() + digi.sample(i).dv();
	hoHists.QIE_DV->Fill(dver);
      }    
      
      if(doPerChannel_)	  
	HcalDigiPerChan::perChanHists<HODataFrame>(2,digi,normVals,hoHists.SHAPE,m_dbe, baseFolder_);
    }
    hoHists.DIGI_NUM->Fill(nhodigi);
    hoHists.BQDIGI_NUM->Fill(nhobqdigi);
    if (nhodigi != 0)hoHists.BQDIGI_FRAC->Fill((1.0*nhobqdigi)/(1.0*nhodigi));
    ndigi = ndigi + nhodigi;
  }
  catch (...) {
    if(fVerbosity) cout << "HcalDigiMonitor::processEvent  No HO Digis." << endl;
  }
  
  try{
    int firsthfcap = -1; int nhfbqdigi = 0;
    int nhfdigi = hf.size();
    //    hfHists.DIGI_NUM->Fill(hf.size());
    for (HFDigiCollection::const_iterator j=hf.begin(); j!=hf.end(); j++){
      const HFDataFrame digi = (const HFDataFrame)(*j);	
      HcalDigiMap::digiStats(digi, calibs_, occThresh_, normVals, digiErr, digiOcc, digiUpset);      
	
      if(digiErr){
	nhfbqdigi++;  nbqdigi++;
	HcalDigiMap::fillErrors<HFDataFrame>(digi,normVals,
					       hfHists.ERR_MAP_GEO,hfHists.ERR_MAP_VME,
					       hfHists.ERR_MAP_DCC);	  
	
	HcalDigiMap::fillErrors<HFDataFrame>(digi,normVals,
					       ERR_MAP_GEO,ERR_MAP_VME,
					       ERR_MAP_DCC);	  
      }
      
      if(digiOcc){
	HcalDigiMap::fillOccupancy<HFDataFrame>(digi,normVals,
						  hfHists.OCC_MAP_GEO1,hfHists.OCC_MAP_GEO2,
						  hfHists.OCC_MAP_GEO3,hfHists.OCC_MAP_GEO4,
						  hfHists.OCC_MAP_VME, 
						  hfHists.OCC_MAP_DCC,
						  hfHists.OCC_ETA,hfHists.OCC_PHI);
	
	HcalDigiMap::fillOccupancy<HFDataFrame>(digi,normVals,
						  OCC_L1,OCC_L2,OCC_L3,OCC_L4,
						  OCC_ELEC_VME,OCC_ELEC_DCC,
						  OCC_ETA,OCC_PHI);	  
      }
      
      hfHists.DIGI_SIZE->Fill(digi.size());
      hfHists.DIGI_PRESAMPLE->Fill(digi.presamples());

      if (firsthfcap == -1) firsthfcap = digi.sample(0).capid();
      int capdif = digi.sample(0).capid() - firsthfcap;
      hfHists.CAPID_T0->Fill(capdif);
      CAPID_T0->Fill(capdif);
      
      for (int i=0; i<digi.size(); i++) {	    
	hfHists.QIE_CAPID->Fill(digi.sample(i).capid());
	hfHists.QIE_ADC->Fill(digi.sample(i).adc());
	hfHists.SHAPE_tot->Fill(i,normVals[i]);
	
	if(digiOcc) hfHists.SHAPE_THR_tot->Fill(i,normVals[i]);
	if(digiUpset) hfHists.QIE_CAPID->Fill(5);
	int dver = 2*digi.sample(i).er() + digi.sample(i).dv();
	hfHists.QIE_DV->Fill(dver);
      }    
      
      if(doPerChannel_)	  
	HcalDigiPerChan::perChanHists<HFDataFrame>(3,digi,normVals,hfHists.SHAPE,m_dbe, baseFolder_);

    }
    hfHists.DIGI_NUM->Fill(nhfdigi);
    hfHists.BQDIGI_NUM->Fill(nhfbqdigi);
    if (nhfdigi != 0)hfHists.BQDIGI_FRAC->Fill((1.0*nhfbqdigi)/(1.0*nhfdigi));
    ndigi = ndigi + nhfdigi;
  } catch (...) {
    if(fVerbosity) cout << "HcalDigiMonitor::processEvent  No HF Digis." << endl;
  }
  DIGI_NUM->Fill(ndigi);
  if (nbqdigi !=0) BQDIGI_NUM->Fill(nbqdigi);
  if (ndigi !=0){
    double bqfrac;
    if (nbqdigi == 0){
      bqfrac = (1.0*nbqdigi_report)/(1.0*(ndigi+nbqdigi_report));
      cout << "Bad Digis from report, bqfrac:  " << nbqdigi_report << "  "<< bqfrac<< endl; 
    }
    else{
      bqfrac = (1.0*nbqdigi)/(1.0*ndigi);
      cout << "Bad Digis counted, bqfrac:  " << nbqdigi << "  "<< bqfrac<< endl;
    }
    BQDIGI_FRAC->Fill(bqfrac);
  }
  return;
}
