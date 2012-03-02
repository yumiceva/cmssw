#include "DQM/SiPixelMonitorDigi/interface/SiPixelDigiModule.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQM/SiPixelCommon/interface/SiPixelHistogramId.h"
/// Framework
#include "FWCore/ServiceRegistry/interface/Service.h"
// STL
#include <vector>
#include <memory>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <sstream>
#include <cstdio>

// Data Formats
#include "DataFormats/SiPixelDetId/interface/PixelBarrelName.h"
#include "DataFormats/SiPixelDetId/interface/PixelEndcapName.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"

//
// Constructors
//
SiPixelDigiModule::SiPixelDigiModule() : id_(0),
					 ncols_(416),
					 nrows_(160) 
{
}
///
SiPixelDigiModule::SiPixelDigiModule(const uint32_t& id) : 
  id_(id),
  ncols_(416),
  nrows_(160)
{ 
}
///
SiPixelDigiModule::SiPixelDigiModule(const uint32_t& id, const int& ncols, const int& nrows) : 
  id_(id),
  ncols_(ncols),
  nrows_(nrows)
{ 
}
//
// Destructor
//
SiPixelDigiModule::~SiPixelDigiModule() {}
//
// Book histograms
//
void SiPixelDigiModule::book(const edm::ParameterSet& iConfig, int type, bool twoD, bool hiRes, bool reducedSet, bool additInfo) {
  bool barrel = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
  bool endcap = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);
  bool isHalfModule = false;
  if(barrel){
    isHalfModule = PixelBarrelName(DetId(id_)).isHalfModule(); 
  }

  std::string hid;
  // Get collection name and instantiate Histo Id builder
  edm::InputTag src = iConfig.getParameter<edm::InputTag>( "src" );
  

  // Get DQM interface
  DQMStore* theDMBE = edm::Service<DQMStore>().operator->();

  int nbinx=ncols_/2, nbiny=nrows_/2;
  std::string twodtitle = "Number of Digis (1bin=four pixels)"; 
  std::string pxtitle = "Number of Digis (1bin=two columns)";
  std::string pytitle = "Number of Digis (1bin=two rows)";
  if(hiRes){
    nbinx = ncols_;
    nbiny = nrows_;
    twodtitle = "Number of Digis (1bin=one pixel)";
    pxtitle = "Number of Digis (1bin=one column)";
    pytitle = "Number of Digis (1bin=one row)";
  }
  
  if(type==0){
    SiPixelHistogramId* theHistogramId = new SiPixelHistogramId( src.label() );
    // Number of digis
    hid = theHistogramId->setHistoId("ndigis",id_);
    meNDigis_ = theDMBE->book1D(hid,"Number of Digis",25,0.,25.);
    meNDigis_->setAxisTitle("Number of digis",1);
    // Charge in ADC counts
    hid = theHistogramId->setHistoId("adc",id_);
    meADC_ = theDMBE->book1D(hid,"Digi charge",128,0.,256.);
    meADC_->setAxisTitle("ADC counts",1);
	if(!reducedSet)
	{
    if(twoD){
      if(additInfo){
	// 2D hit map
	hid = theHistogramId->setHistoId("hitmap",id_);
	mePixDigis_ = theDMBE->book2D(hid,twodtitle,nbinx,0.,float(ncols_),nbiny,0.,float(nrows_));
	mePixDigis_->setAxisTitle("Columns",1);
	mePixDigis_->setAxisTitle("Rows",2);
      }
    }
    else{
      // projections of 2D hit map
      hid = theHistogramId->setHistoId("hitmap",id_);
      mePixDigis_px_ = theDMBE->book1D(hid+"_px",pxtitle,nbinx,0.,float(ncols_));
      mePixDigis_py_ = theDMBE->book1D(hid+"_py",pytitle,nbiny,0.,float(nrows_));
      mePixDigis_px_->setAxisTitle("Columns",1);
      mePixDigis_py_->setAxisTitle("Rows",1);
    }
	}
    delete theHistogramId;

  }
  
  if(type==1 && barrel){
    uint32_t DBladder = PixelBarrelName(DetId(id_)).ladderName();
    char sladder[80]; sprintf(sladder,"Ladder_%02i",DBladder);
    hid = src.label() + "_" + sladder;
    if(isHalfModule) hid += "H";
    else hid += "F";
    // Number of digis
    meNDigisLad_ = theDMBE->book1D("ndigis_"+hid,"Number of Digis",25,0.,25.);
    meNDigisLad_->setAxisTitle("Number of digis",1);
    // Charge in ADC counts
    meADCLad_ = theDMBE->book1D("adc_" + hid,"Digi charge",128,0.,256.);
    meADCLad_->setAxisTitle("ADC counts",1);
	if(!reducedSet)
	{
    if(twoD){
      // 2D hit map
      mePixDigisLad_ = theDMBE->book2D("hitmap_"+hid,twodtitle,nbinx,0.,float(ncols_),nbiny,0.,float(nrows_));
      mePixDigisLad_->setAxisTitle("Columns",1);
      mePixDigisLad_->setAxisTitle("Rows",2);
    }
    else{
      // projections of 2D hit map
      mePixDigisLad_px_ = theDMBE->book1D("hitmap_"+hid+"_px",pxtitle,nbinx,0.,float(ncols_));
      mePixDigisLad_py_ = theDMBE->book1D("hitmap_"+hid+"_py",pytitle,nbiny,0.,float(nrows_));
      mePixDigisLad_px_->setAxisTitle("Columns",1);
      mePixDigisLad_py_->setAxisTitle("Rows",1);	
    }
	}
  }
  if(type==2 && barrel){
    uint32_t DBlayer = PixelBarrelName(DetId(id_)).layerName();
    char slayer[80]; sprintf(slayer,"Layer_%i",DBlayer);
    hid = src.label() + "_" + slayer;
    if(!additInfo){
      // Number of digis
      meNDigisLay_ = theDMBE->book1D("ndigis_"+hid,"Number of Digis",25,0.,25.);
      meNDigisLay_->setAxisTitle("Number of digis",1);
    // Charge in ADC counts
      meADCLay_ = theDMBE->book1D("adc_" + hid,"Digi charge",128,0.,256.);
      meADCLay_->setAxisTitle("ADC counts",1);
    }
    if(!reducedSet){
      if(twoD || additInfo){
	// 2D hit map
	if(isHalfModule){
	  mePixDigisLay_ = theDMBE->book2D("hitmap_"+hid,twodtitle,nbinx,0.,float(ncols_),2*nbiny,0.,float(2*nrows_));
	}
	else{
	  mePixDigisLay_ = theDMBE->book2D("hitmap_"+hid,twodtitle,nbinx,0.,float(ncols_),nbiny,0.,float(nrows_));
	}
	mePixDigisLay_->setAxisTitle("Columns",1);
	mePixDigisLay_->setAxisTitle("Rows",2);
      }
      if(!twoD && !additInfo){
	// projections of 2D hit map
	mePixDigisLay_px_ = theDMBE->book1D("hitmap_"+hid+"_px",pxtitle,nbinx,0.,float(ncols_));
	if(isHalfModule){
	  mePixDigisLay_py_ = theDMBE->book1D("hitmap_"+hid+"_py",pytitle,2*nbiny,0.,float(2*nrows_));
	}
	else{
	  mePixDigisLay_py_ = theDMBE->book1D("hitmap_"+hid+"_py",pytitle,nbiny,0.,float(nrows_));
	}
	mePixDigisLay_px_->setAxisTitle("Columns",1);
	mePixDigisLay_py_->setAxisTitle("Rows",1);
      }
    }
  }
  if(type==3 && barrel){
    uint32_t DBmodule = PixelBarrelName(DetId(id_)).moduleName();
    char smodule[80]; sprintf(smodule,"Ring_%i",DBmodule);
    hid = src.label() + "_" + smodule;
    // Number of digis
    meNDigisPhi_ = theDMBE->book1D("ndigis_"+hid,"Number of Digis",25,0.,25.);
    meNDigisPhi_->setAxisTitle("Number of digis",1);
    // Charge in ADC counts
    meADCPhi_ = theDMBE->book1D("adc_" + hid,"Digi charge",128,0.,256.);
    meADCPhi_->setAxisTitle("ADC counts",1);
    if(!reducedSet)
      {
	if(twoD){
	  
	  // 2D hit map
	  if(isHalfModule){
	    mePixDigisPhi_ = theDMBE->book2D("hitmap_"+hid,twodtitle,nbinx,0.,float(ncols_),2*nbiny,0.,float(2*nrows_));
	  }
	  else {
	    mePixDigisPhi_ = theDMBE->book2D("hitmap_"+hid,twodtitle,nbinx,0.,float(ncols_),nbiny,0.,float(nrows_));
	  }
	  mePixDigisPhi_->setAxisTitle("Columns",1);
	  mePixDigisPhi_->setAxisTitle("Rows",2);
	}
	else{
	  // projections of 2D hit map
	  mePixDigisPhi_px_ = theDMBE->book1D("hitmap_"+hid+"_px",pxtitle,nbinx,0.,float(ncols_));
	  if(isHalfModule){
	    mePixDigisPhi_py_ = theDMBE->book1D("hitmap_"+hid+"_py",pytitle,2*nbiny,0.,float(2*nrows_));
	  }
	  else{
	    mePixDigisPhi_py_ = theDMBE->book1D("hitmap_"+hid+"_py",pytitle,nbiny,0.,float(nrows_));
	  }
	  mePixDigisPhi_px_->setAxisTitle("Columns",1);
	  mePixDigisPhi_py_->setAxisTitle("Rows",1);
	}
      }
  }
  if(type==4 && endcap){
    uint32_t blade= PixelEndcapName(DetId(id_)).bladeName();
    
    char sblade[80]; sprintf(sblade, "Blade_%02i",blade);
    hid = src.label() + "_" + sblade;
    // Number of digis
    meNDigisBlade_ = theDMBE->book1D("ndigis_"+hid,"Number of Digis",25,0.,25.);
    meNDigisBlade_->setAxisTitle("Number of digis",1);
    // Charge in ADC counts
    meADCBlade_ = theDMBE->book1D("adc_" + hid,"Digi charge",128,0.,256.);
    meADCBlade_->setAxisTitle("ADC counts",1);
  }
  if(type==5 && endcap){
    uint32_t disk = PixelEndcapName(DetId(id_)).diskName();
    
    char sdisk[80]; sprintf(sdisk, "Disk_%i",disk);
    hid = src.label() + "_" + sdisk;
    if(!additInfo){
      // Number of digis
      meNDigisDisk_ = theDMBE->book1D("ndigis_"+hid,"Number of Digis",25,0.,25.);
      meNDigisDisk_->setAxisTitle("Number of digis",1);
      // Charge in ADC counts
      meADCDisk_ = theDMBE->book1D("adc_" + hid,"Digi charge",128,0.,256.);
      meADCDisk_->setAxisTitle("ADC counts",1);
    }
    if(additInfo){
      mePixDigisDisk_ = theDMBE->book2D("hitmap_"+hid,twodtitle,260,0.,260.,160,0.,160.);
      mePixDigisDisk_->setAxisTitle("Columns",1);
      mePixDigisDisk_->setAxisTitle("Rows",2);
    }
  }
  if(type==6 && endcap){
    uint32_t panel= PixelEndcapName(DetId(id_)).pannelName();
    uint32_t module= PixelEndcapName(DetId(id_)).plaquetteName();
    char slab[80]; sprintf(slab, "Panel_%i_Ring_%i",panel, module);
    hid = src.label() + "_" + slab;
    // Number of digis
    meNDigisRing_ = theDMBE->book1D("ndigis_"+hid,"Number of Digis",25,0.,25.);
    meNDigisRing_->setAxisTitle("Number of digis",1);
    // Charge in ADC counts
    meADCRing_ = theDMBE->book1D("adc_" + hid,"Digi charge",128,0.,256.);
    meADCRing_->setAxisTitle("ADC counts",1);
	if(!reducedSet)
	{
    if(twoD){
      // 2D hit map
      mePixDigisRing_ = theDMBE->book2D("hitmap_"+hid,twodtitle,nbinx,0.,float(ncols_),nbiny,0.,float(nrows_));
      mePixDigisRing_->setAxisTitle("Columns",1);
      mePixDigisRing_->setAxisTitle("Rows",2);
    }
    else{
      // projections of 2D hit map
      mePixDigisRing_px_ = theDMBE->book1D("hitmap_"+hid+"_px",pxtitle,nbinx,0.,float(ncols_));
      mePixDigisRing_py_ = theDMBE->book1D("hitmap_"+hid+"_py",pytitle,nbiny,0.,float(nrows_));
      mePixDigisRing_px_->setAxisTitle("Columns",1);
      mePixDigisRing_py_->setAxisTitle("Rows",1);
    }
	}
  }
}


//
// Fill histograms
//
int SiPixelDigiModule::fill(const edm::DetSetVector<PixelDigi>& input, bool modon, 
								 bool ladon, bool layon, bool phion, 
								 bool bladeon, bool diskon, bool ringon, 
								 bool twoD, bool reducedSet, bool twoDimModOn, bool twoDimOnlyLayDisk,
								 int &nDigisA, int &nDigisB) {
  bool barrel = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelBarrel);
  bool endcap = DetId(id_).subdetId() == static_cast<int>(PixelSubdetector::PixelEndcap);
  bool isHalfModule = false;
  uint32_t DBladder = 0;
  if(barrel){
    isHalfModule = PixelBarrelName(DetId(id_)).isHalfModule(); 
    DBladder = PixelBarrelName(DetId(id_)).ladderName();
  }

  // Get DQM interface
  DQMStore* theDMBE = edm::Service<DQMStore>().operator->();
  //std::cout<<"id_ = "<<id_<<" , dmbe="<<theDMBE->pwd()<<std::endl;
  //std::cout<<"********************"<<std::endl;
  edm::DetSetVector<PixelDigi>::const_iterator isearch = input.find(id_); // search  digis of detid
  
  unsigned int numberOfDigisMod = 0;
  int numberOfDigis[8]; for(int i=0; i!=8; i++) numberOfDigis[i]=0; 
  nDigisA=0; nDigisB=0;
  if( isearch != input.end() ) {  // Not an empty iterator
    
    // Look at digis now
    edm::DetSet<PixelDigi>::const_iterator  di;
    for(di = isearch->data.begin(); di != isearch->data.end(); di++) {
      int adc = di->adc();    // charge
      int col = di->column(); // column 
      int row = di->row();    // row
      numberOfDigisMod++;
      bool isHalfModule = false;
      uint32_t DBladder = 0;
      if(barrel){
        isHalfModule = PixelBarrelName(DetId(id_)).isHalfModule(); 
        DBladder = PixelBarrelName(DetId(id_)).ladderName();
      }
      PixelBarrelName::Shell DBshell = PixelBarrelName(DetId(id_)).shell();
      int DBlayer = PixelBarrelName(DetId(id_)).layerName();
      if(barrel){
        if(isHalfModule){
          if(DBshell==PixelBarrelName::pI||DBshell==PixelBarrelName::pO){
	    numberOfDigis[0]++; nDigisA++;
	    if(DBlayer==1) numberOfDigis[2]++;
	    if(DBlayer==2) numberOfDigis[3]++;
	    if(DBlayer==3) numberOfDigis[4]++;
	  }
          if(DBshell==PixelBarrelName::mI||DBshell==PixelBarrelName::mO){
	    numberOfDigis[1]++; nDigisB++;
	    if(DBlayer==1) numberOfDigis[5]++;
	    if(DBlayer==2) numberOfDigis[6]++;
	    if(DBlayer==3) numberOfDigis[7]++;
	  }
        }else{
          if(row<80){
	    numberOfDigis[0]++; nDigisA++;
	    if(DBlayer==1) numberOfDigis[2]++;
	    if(DBlayer==2) numberOfDigis[3]++;
	    if(DBlayer==3) numberOfDigis[4]++;
	  }else{ 
	    numberOfDigis[1]++; nDigisB++;
	    if(DBlayer==1) numberOfDigis[5]++;
	    if(DBlayer==2) numberOfDigis[6]++;
	    if(DBlayer==3) numberOfDigis[7]++;
	  }
        }
      }
      if(modon){
	if(!reducedSet){
	  if(twoD) {
	    if(twoDimModOn) (mePixDigis_)->Fill((float)col,(float)row);
	  }
	  else {
	    (mePixDigis_px_)->Fill((float)col);
	    (mePixDigis_py_)->Fill((float)row);
	  }
	}
	(meADC_)->Fill((float)adc);
      }
      if(ladon && barrel){
	(meADCLad_)->Fill((float)adc);
	if(!reducedSet){
	  if(twoD) (mePixDigisLad_)->Fill((float)col,(float)row);
	  else {
	  (mePixDigisLad_px_)->Fill((float)col);
	  (mePixDigisLad_py_)->Fill((float)row);
	  }
	}
      }
      if((layon || twoDimOnlyLayDisk) && barrel){
	if(!twoDimOnlyLayDisk) (meADCLay_)->Fill((float)adc);
	if(!reducedSet){
	  if((layon && twoD) || twoDimOnlyLayDisk){
	    if(isHalfModule && DBladder==1){
	      (mePixDigisLay_)->Fill((float)col,(float)row+80);
	    }
	    else (mePixDigisLay_)->Fill((float)col,(float)row);
	  }
	  if((layon && !twoD) && !twoDimOnlyLayDisk){
	    (mePixDigisLay_px_)->Fill((float)col);
	    if(isHalfModule && DBladder==1) {
	      (mePixDigisLay_py_)->Fill((float)row+80);
	    }
	    else (mePixDigisLay_py_)->Fill((float)row);
	  }
	}
      }
      if(phion && barrel){
	(meADCPhi_)->Fill((float)adc);
	if(!reducedSet)
	{
	if(twoD){
	  if(isHalfModule && DBladder==1){
	    (mePixDigisPhi_)->Fill((float)col,(float)row+80);
	  }
	  else (mePixDigisPhi_)->Fill((float)col,(float)row);
	}
	else {
	  (mePixDigisPhi_px_)->Fill((float)col);
	  if(isHalfModule && DBladder==1) {
	    (mePixDigisPhi_py_)->Fill((float)row+80);
	  }
	  else (mePixDigisPhi_py_)->Fill((float)row);
	}
	}
      }
      if(bladeon && endcap){
	(meADCBlade_)->Fill((float)adc);
      }
      if((diskon || twoDimOnlyLayDisk) && endcap){
	if(!twoDimOnlyLayDisk) (meADCDisk_)->Fill((float)adc);
	if(twoDimOnlyLayDisk){
	  (mePixDigisDisk_)->Fill((float)col,(float)row);
	}
      }
      if(ringon && endcap){
	(meADCRing_)->Fill((float)adc);
	if(!reducedSet)
	{
	if(twoD) (mePixDigisRing_)->Fill((float)col,(float)row);
	else {
	  (mePixDigisRing_px_)->Fill((float)col);
	  (mePixDigisRing_py_)->Fill((float)row);
	}
	}
      }
    }
    if(modon) (meNDigis_)->Fill((float)numberOfDigisMod);
    if(ladon && barrel) (meNDigisLad_)->Fill((float)numberOfDigisMod);
    if(layon && barrel && !twoDimOnlyLayDisk) (meNDigisLay_)->Fill((float)numberOfDigisMod);
    if(phion && barrel) (meNDigisPhi_)->Fill((float)numberOfDigisMod);
    if(bladeon && endcap) (meNDigisBlade_)->Fill((float)numberOfDigisMod);
    if(diskon && endcap && !twoDimOnlyLayDisk) (meNDigisDisk_)->Fill((float)numberOfDigisMod);
    if(ringon && endcap) (meNDigisRing_)->Fill((float)numberOfDigisMod);
    if(barrel){ 
      MonitorElement* me=theDMBE->get("Pixel/Barrel/ALLMODS_ndigisCOMB_Barrel");
      if(me) me->Fill((float)numberOfDigisMod);
      me=theDMBE->get("Pixel/Barrel/ALLMODS_ndigisCHAN_Barrel");
      if(me){ if(numberOfDigis[0]>0) me->Fill((float)numberOfDigis[0]); if(numberOfDigis[1]>0) me->Fill((float)numberOfDigis[1]); }
      me=theDMBE->get("Pixel/Barrel/ALLMODS_ndigisCHAN_BarrelL1");
      if(me){ if(numberOfDigis[2]>0) me->Fill((float)numberOfDigis[2]); }
      me=theDMBE->get("Pixel/Barrel/ALLMODS_ndigisCHAN_BarrelL2");
      if(me){ if(numberOfDigis[3]>0) me->Fill((float)numberOfDigis[3]); }
      me=theDMBE->get("Pixel/Barrel/ALLMODS_ndigisCHAN_BarrelL3");
      if(me){ if(numberOfDigis[4]>0) me->Fill((float)numberOfDigis[4]); }
    }else if(endcap){
      MonitorElement* me=theDMBE->get("Pixel/Endcap/ALLMODS_ndigisCOMB_Endcap");
      if(me) me->Fill((float)numberOfDigisMod);
    }
  }
  
  //std::cout<<"numberOfDigis for this module: "<<numberOfDigis<<std::endl;
  return numberOfDigisMod;
}
