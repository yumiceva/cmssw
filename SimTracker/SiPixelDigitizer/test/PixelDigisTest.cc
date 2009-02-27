// -*- C++ -*-
//
// Package:    PixelDigisTest
// Class:      PixelDigisTest
// 
/**\class PixelDigisTest PixelDigisTest.cc 

 Description: Test pixel digis. 
 Barrel & Forward digis. Uses root histos.
 Works with CMSSW_0_9_0_pre3 
 Adopted for the new simLinks. 

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  d.k.
//         Created:  Jan CET 2006
// $Id: PixelDigisTest.cc,v 1.17 2008/10/30 13:46:58 dkotlins Exp $
//
//
// system include files
#include <memory>

// user include files
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "DataFormats/Common/interface/Handle.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/InputTag.h"

// my includes
//#include "Geometry/CommonDetUnit/interface/GeomDetUnit.h"

#include "Geometry/Records/interface/TrackerDigiGeometryRecord.h" 

#include "Geometry/TrackerGeometryBuilder/interface/TrackerGeometry.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetUnit.h"
#include "Geometry/TrackerGeometryBuilder/interface/PixelGeomDetType.h"

#include "DataFormats/SiPixelDetId/interface/PixelSubdetector.h"
#include "DataFormats/SiPixelDetId/interface/PXBDetId.h"
#include "DataFormats/SiPixelDetId/interface/PXFDetId.h"

// data formats
#include "DataFormats/Common/interface/DetSetVector.h"
#include "DataFormats/SiPixelDigi/interface/PixelDigi.h"
//#include "DataFormats/SiPixelDigi/interface/PixelDigiCollection.h"
#include "DataFormats/DetId/interface/DetId.h"

#include "SimDataFormats/TrackerDigiSimLink/interface/PixelDigiSimLink.h"

// For the big pixel recongnition
#include "Geometry/TrackerTopology/interface/RectangularPixelTopology.h"

// for simulated Tracker hits
//#include "SimDataFormats/TrackingHit/interface/PSimHitContainer.h"

// To use root histos
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "PhysicsTools/UtilAlgos/interface/TFileService.h"


// For ROOT
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TF1.h>
#include <TH2F.h>
#include <TH1F.h>

using namespace std;

// Enable this to look at simlinks (link simhit->digis)
// Can be used only with simulated data.
//#define USE_SIM_LINKS

#define HISTOS

//
// class decleration
//

class PixelDigisTest : public edm::EDAnalyzer {

public:

  explicit PixelDigisTest(const edm::ParameterSet&);
  ~PixelDigisTest();
  virtual void beginJob(const edm::EventSetup& iSetup);
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  virtual void endJob(); 

private:
  // ----------member data ---------------------------
  bool PRINT;

#ifdef HISTOS

  //TFile* hFile;
  TH1F *hdetunit;
  TH1F *heloss1,*heloss2, *heloss3;
  TH1F *helossF1,*helossF2;
  TH1F *hpixid,*hpixsubid,*hlayerid,*hladder1id,*hladder2id,*hladder3id,
    *hz1id,*hz2id,*hz3id;
  TH1F *hcols1,*hcols2,*hcols3,*hrows1,*hrows2,*hrows3;
  TH1F *hcolsF1,*hcolsF2,*hcolsF3,*hrowsF1,*hrowsF2,*hrowsF3;
  TH1F *hdigisPerDet1,*hdigisPerDet2,*hdigisPerDet3;
  TH1F *hdigisPerLay1,*hdigisPerLay2,*hdigisPerLay3;
  TH1F *hdetsPerLay1,*hdetsPerLay2,*hdetsPerLay3;
  TH1F *hdigisPerDetF1,*hdigisPerDetF2,*hdigisPerDetF3;
  TH1F *hdigisPerLayF1,*hdigisPerLayF2,*hdigisPerLayF3;
  TH1F *hdetsPerLayF1,*hdetsPerLayF2,*hdetsPerLayF3;
  TH1F *hdetr, *hdetz, *hdetrF, *hdetzF;
  TH1F *hcolsB,  *hrowsB,  *hcolsF,  *hrowsF;
  TH1F *hcols1big, *hrows1big, *heloss1bigx, *heloss1bigy;
  TH1F *hsimlinks, *hfract;
  TH1F *hblade1, *hblade2;

  TH2F *htest, *htest2;
  TH2F *hdetMap3,*hdetMap2,*hdetMap1, *hpixMap1, *hpixMap2, *hpixMap3; 

#endif

  edm::InputTag src_;  

};

//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PixelDigisTest::PixelDigisTest(const edm::ParameterSet& iConfig) {
  //We put this here for the moment since there is no better place 
  //edm::Service<MonitorDaemon> daemon;
  //daemon.operator->();

  PRINT = iConfig.getUntrackedParameter<bool>("Verbosity",false);
  src_ =  iConfig.getParameter<edm::InputTag>( "src" );
  cout<<" Construct PixelDigisTest "<<endl;
}


PixelDigisTest::~PixelDigisTest() {
 
  // do anything here that needs to be done at desctruction time
  // (e.g. close files, deallocate resources etc.)
  cout<<" Destroy PixelDigisTest "<<endl;

}

//
// member functions
//
// ------------ method called at the begining   ------------
void PixelDigisTest::beginJob(const edm::EventSetup& iSetup) {

   using namespace edm;
   cout << "Initialize PixelDigisTest " <<endl;

#ifdef HISTOS

 // NEW way to use root (from 2.0.0?)
  edm::Service<TFileService> fs;

  // Histos go to a subdirectory "PixRecHits")
  //TFileDirectory subDir = fs->mkdir( "mySubDirectory" );
  //TFileDirectory subSubDir = subDir.mkdir( "mySubSubDirectory" );

   // put here whatever you want to do at the beginning of the job
  //hFile = new TFile ( "histo.root", "RECREATE" );
     
    hdetunit = fs->make<TH1F>( "hdetunit", "Det unit", 1000,
                              302000000.,302300000.);
    hpixid = fs->make<TH1F>( "hpixid", "Pix det id", 10, 0., 10.);
    hpixsubid = fs->make<TH1F>( "hpixsubid", "Pix Barrel id", 10, 0., 10.);
    hlayerid = fs->make<TH1F>( "hlayerid", "Pix layer id", 10, 0., 10.);
    hladder1id = fs->make<TH1F>( "hladder1id", "Ladder L1 id", 50, 0., 50.);
    hladder2id = fs->make<TH1F>( "hladder2id", "Ladder L2 id", 50, 0., 50.);
    hladder3id = fs->make<TH1F>( "hladder3id", "Ladder L3 id", 50, 0., 50.);
    hz1id = fs->make<TH1F>( "hz1id", "Z-index id L1", 10, 0., 10.);
    hz2id = fs->make<TH1F>( "hz2id", "Z-index id L2", 10, 0., 10.);
    hz3id = fs->make<TH1F>( "hz3id", "Z-index id L3", 10, 0., 10.);
 
    hdigisPerDet1 = fs->make<TH1F>( "hdigisPerDet1", "Digis per det l1", 
			      200, -0.5, 199.5);
    hdigisPerDet2 = fs->make<TH1F>( "hdigisPerDet2", "Digis per det l2", 
			      200, -0.5, 199.5);
    hdigisPerDet3 = fs->make<TH1F>( "hdigisPerDet3", "Digis per det l3", 
			      200, -0.5, 199.5);
    hdigisPerLay1 = fs->make<TH1F>( "hdigisPerLay1", "Digis per layer l1", 
			      2000, -0.5, 3999.5);
    hdigisPerLay2 = fs->make<TH1F>( "hdigisPerLay2", "Digis per layer l2", 
			      2000, -0.5, 3999.5);
    hdigisPerLay3 = fs->make<TH1F>( "hdigisPerLay3", "Digis per layer l3", 
			      2000, -0.5, 3999.5);
    hdetsPerLay1 = fs->make<TH1F>( "hdetsPerLay1", "Full dets per layer l1", 
			      161, -0.5, 160.5);
    hdetsPerLay3 = fs->make<TH1F>( "hdetsPerLay3", "Full dets per layer l3", 
			      353, -0.5, 352.5);
    hdetsPerLay2 = fs->make<TH1F>( "hdetsPerLay2", "Full dets per layer l2", 
			      257, -0.5, 256.5);

    hdigisPerDetF1 = fs->make<TH1F>( "hdigisPerDetF1", "Digis per det d1", 
			      200, -0.5, 199.5);
    hdigisPerDetF2 = fs->make<TH1F>( "hdigisPerDetF2", "Digis per det d2", 
			      200, -0.5, 199.5);
    hdigisPerLayF1 = fs->make<TH1F>( "hdigisPerLayF1", "Digis per layer d1", 
			      2000, -0.5, 1999.5);
    hdigisPerLayF2 = fs->make<TH1F>( "hdigisPerLayF2", "Digis per layer d2", 
			      2000, -0.5, 1999.5);
    hdetsPerLayF1 = fs->make<TH1F>( "hdetsPerLayF1", "Full dets per layer d1", 
			      161, -0.5, 160.5);
    hdetsPerLayF2 = fs->make<TH1F>( "hdetsPerLayF2", "Full dets per layer d2", 
			      257, -0.5, 256.5);

    heloss1 = fs->make<TH1F>( "heloss1", "Pix charge l1", 100, 0., 300.);
    heloss2 = fs->make<TH1F>( "heloss2", "Pix charge l2", 100, 0., 300.);
    heloss3 = fs->make<TH1F>( "heloss3", "Pix charge l3", 100, 0., 300.);
    heloss1bigx = fs->make<TH1F>( "heloss1bigx", "L1 big-x pix", 100, 0., 300.);
    heloss1bigy = fs->make<TH1F>( "heloss1bigy", "L1 big-y pix", 100, 0., 300.);

    hcols1 = fs->make<TH1F>( "hcols1", "Layer 1 cols", 500,-1.5,498.5);
    hcols2 = fs->make<TH1F>( "hcols2", "Layer 2 cols", 500,-1.5,498.5);
    hcols3 = fs->make<TH1F>( "hcols3", "Layer 3 cols", 500,-1.5,498.5);
    hcols1big = fs->make<TH1F>( "hcols1big", "Layer 1 big cols", 500,-1.5,498.5);
 
    hrows1 = fs->make<TH1F>( "hrows1", "Layer 1 rows", 200,-1.5,198.5);
    hrows2 = fs->make<TH1F>( "hrows2", "Layer 2 rows", 200,-1.5,198.5);
    hrows3 = fs->make<TH1F>( "hrows3", "layer 3 rows", 200,-1.5,198.5);
    hrows1big = fs->make<TH1F>( "hrows1big", "Layer 1 big rows", 200,-1.5,198.5);
 
    hblade1 = fs->make<TH1F>( "hblade1", "blade num, disk1", 24, 0., 24.);
    hblade2 = fs->make<TH1F>( "hblade2", "blade num, disk2", 24, 0., 24.);

    helossF1 = fs->make<TH1F>( "helossF1", "Pix charge d1", 100, 0., 300.);
    helossF2 = fs->make<TH1F>( "helossF2", "Pix charge d2", 100, 0., 300.);
    hcolsF1 = fs->make<TH1F>( "hcolsF1", "Disk 1 cols", 500,-1.5,498.5);
    hcolsF2 = fs->make<TH1F>( "hcolsF2", "Disk 2 cols", 500,-1.5,498.5);
    hrowsF1 = fs->make<TH1F>( "hrowsF1", "Disk 1 rows", 200,-1.5,198.5);
    hrowsF2 = fs->make<TH1F>( "hrowsF2", "Disk 2 rows", 200,-1.5,198.5);



    hdetr = fs->make<TH1F>("hdetr","det r",150,0.,15.);
    hdetz = fs->make<TH1F>("hdetz","det z",520,-26.,26.);
    hdetrF = fs->make<TH1F>("hdetrF","det r",150,0.,15.);
    hdetzF = fs->make<TH1F>("hdetzF","det z",700,-70.,70.);

    hcolsB = fs->make<TH1F>("hcolsB","cols per bar det",450,0.,450.);
    hrowsB = fs->make<TH1F>("hrowsB","rows per bar det",200,0.,200.);
    hcolsF = fs->make<TH1F>("hcolsF","cols per for det",300,0.,300.);
    hrowsF = fs->make<TH1F>("hrowsF","rows per for det",200,0.,200.);

    hsimlinks = fs->make<TH1F>("hsimlinks"," track ids",200,0.,200.);
    hfract = fs->make<TH1F>("hfract"," track rractions",100,0.,1.);

    hdetMap1 = fs->make<TH2F>("hdetMap1"," ",22,0.,22.,10,0.,10.);
    hdetMap1->SetOption("colz");
    hdetMap2 = fs->make<TH2F>("hdetMap2"," ",34,0.,34.,10,0.,10.);
    hdetMap2->SetOption("colz");
    hdetMap3 = fs->make<TH2F>("hdetMap3"," ",46,0.,46.,10,0.,10.);
    hdetMap3->SetOption("colz");
    hpixMap1 = fs->make<TH2F>("hpixMap1"," ",416,0.,416.,160,0.,160.);
    hpixMap1->SetOption("colz");
    hpixMap2 = fs->make<TH2F>("hpixMap2"," ",416,0.,416.,160,0.,160.);
    hpixMap2->SetOption("colz");
    hpixMap3 = fs->make<TH2F>("hpixMap3"," ",416,0.,416.,160,0.,160.);
    hpixMap3->SetOption("colz");

    htest = fs->make<TH2F>("htest"," ",10,0.,10.,20,0.,20.);
    htest2 = fs->make<TH2F>("htest2"," ",10,0.,10.,300,0.,300.);
    htest->SetOption("colz");
    htest2->SetOption("colz");

#endif

}

// ------------ method called to produce the data  ------------
void PixelDigisTest::analyze(const edm::Event& iEvent, 
			   const edm::EventSetup& iSetup) {
  using namespace edm;
  if(PRINT) cout<<" Analyze PixelDigisTest "<<endl;

    // Get digis
  edm::Handle< edm::DetSetVector<PixelDigi> > pixelDigis;
  iEvent.getByLabel( src_ , pixelDigis);

#ifdef USE_SIM_LINKS
  // Get simlink data
  edm::Handle< edm::DetSetVector<PixelDigiSimLink> > pixelSimLinks;
  iEvent.getByLabel( src_ ,   pixelSimLinks);
#endif

  // Get event setup (to get global transformation)
  edm::ESHandle<TrackerGeometry> geom;
  iSetup.get<TrackerDigiGeometryRecord>().get( geom );
  const TrackerGeometry& theTracker(*geom);

  int numberOfDetUnits = 0;
  int totalNumOfDigis = 0;

  int numberOfDetUnits1 = 0;
  int totalNumOfDigis1 = 0;
  int numberOfDetUnits2 = 0;
  int totalNumOfDigis2 = 0;
  int numberOfDetUnits3 = 0;
  int totalNumOfDigis3 = 0;
  int numOfDigisPerDet1 = 0;
  int numOfDigisPerDet2 = 0;
  int numOfDigisPerDet3 = 0;

  int numberOfDetUnitsF1 = 0;
  int totalNumOfDigisF1 = 0;
  int numberOfDetUnitsF2 = 0;
  int totalNumOfDigisF2 = 0;
  int numOfDigisPerDetF1 = 0;
  int numOfDigisPerDetF2 = 0;

  // Iterate on detector units
  edm::DetSetVector<PixelDigi>::const_iterator DSViter;
  for(DSViter = pixelDigis->begin(); DSViter != pixelDigis->end(); DSViter++) {

    unsigned int detid = DSViter->id; // = rawid
    DetId detId(detid);
    //const GeomDetUnit      * geoUnit = geom->idToDetUnit( detId );
    //const PixelGeomDetUnit * pixDet  = dynamic_cast<const PixelGeomDetUnit*>(geoUnit);
    unsigned int detType=detId.det(); // det type, tracker=1
    unsigned int subid=detId.subdetId(); //subdetector type, barrel=1
    
    if(PRINT) 
      cout<<"Det: "<<detId.rawId()<<" "<<detId.null()<<" "<<detType<<" "<<subid<<endl;
    
#ifdef HISTOS
    hdetunit->Fill(float(detid));
    hpixid->Fill(float(detType));
    hpixsubid->Fill(float(subid));
#endif
    
    if(detType!=1) continue; // look only at tracker
    ++numberOfDetUnits;
    
    // Get the geom-detector 
    const PixelGeomDetUnit * theGeomDet = 
      dynamic_cast<const PixelGeomDetUnit*> ( theTracker.idToDet(detId) );
    double detZ = theGeomDet->surface().position().z();
    double detR = theGeomDet->surface().position().perp();
    //const BoundPlane plane = theGeomDet->surface(); // does not work
    
    int cols = theGeomDet->specificTopology().ncolumns();
    int rows = theGeomDet->specificTopology().nrows();
    float pitchX = theGeomDet->specificTopology().pitch().first;
    float pitchY = theGeomDet->specificTopology().pitch().second;
    
    unsigned int layer=0;
    unsigned int ladder=0;
    unsigned int zindex=0;
    unsigned int disk=0; //1,2,3

    unsigned int blade=0; //1-24
    unsigned int side=0; //size=1 for -z, 2 for +z
    unsigned int panel=0; //panel=1
 
   // Subdet it, pix barrel=1, forward=2
    if(subid==2) {   // forward

#ifdef HISTOS
      hdetrF->Fill(detR);
      hdetzF->Fill(detZ);
      hcolsF->Fill(float(cols));
      hrowsF->Fill(float(rows));
#endif

      PXFDetId pdetId = PXFDetId(detid);
      disk=pdetId.disk(); //1,2,3
      blade=pdetId.blade(); //1-24
      zindex=pdetId.module(); //
      side=pdetId.side(); //size=1 for -z, 2 for +z
      panel=pdetId.panel(); //panel=1
      
      if(PRINT) {
	cout<<"Forward det "<<subid<<", disk "<<disk<<", blade "
		    <<blade<<", module "<<zindex<<", side "<<side<<", panel "
		    <<panel<<endl;
	cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
		    <<pitchX<<" "<<pitchY<<endl;
      }

    } else if(subid == 1) { // Barrel 
      
#ifdef HISTOS
      hdetr->Fill(detR);
      hdetz->Fill(detZ);
      hcolsB->Fill(float(cols));
      hrowsB->Fill(float(rows));
#endif
      
      PXBDetId pdetId = PXBDetId(detid);
      // Barell layer = 1,2,3
      layer=pdetId.layer();
      // Barrel ladder id 1-20,32,44.
      ladder=pdetId.ladder();
      // Barrel Z-index=1,8
      zindex=pdetId.module();
      if(PRINT) { 
	cout<<" Barrel layer, ladder, module "
	    <<layer<<" "<<ladder<<" "<<zindex<<endl;
	//cout<<" Barrel det, thick "<<detThick<<" "
	//  <<" layer, ladder, module "
	//  <<layer<<" "<<ladder<<" "<<zindex<<endl;
	//cout<<" col/row, pitch "<<cols<<" "<<rows<<" "
	//  <<pitchX<<" "<<pitchY<<endl;
      }      
#ifdef HISTOS
      hlayerid->Fill(float(layer));
#endif

    } // end fb-bar

#ifdef HISTOS
    // Some histos
    if(layer==1) {
      hladder1id->Fill(float(ladder));
      hz1id->Fill(float(zindex));
      hdetMap1->Fill(float(ladder),float(zindex));
      ++numberOfDetUnits1;
      numOfDigisPerDet1=0;
      
    } else if(layer==2) {
      hladder2id->Fill(float(ladder));
      hz2id->Fill(float(zindex));
      hdetMap2->Fill(float(ladder),float(zindex));
      ++numberOfDetUnits2;
      numOfDigisPerDet2=0;

    } else if(layer==3) {
      hladder3id->Fill(float(ladder));
      hz3id->Fill(float(zindex));
      hdetMap3->Fill(float(ladder),float(zindex));
      ++numberOfDetUnits3;
      numOfDigisPerDet3=0;

    } else if(disk==1) {
      hblade1->Fill(float(blade));
      ++numberOfDetUnitsF1;
      numOfDigisPerDetF1=0;
 
   } else if(disk==2) {
      hblade2->Fill(float(blade));
      ++numberOfDetUnitsF2;
      numOfDigisPerDetF2=0;
   }
#endif
      

#ifdef USE_SIM_LINKS
    // Look at simlink information (simulated data only)

    int numberOfSimLinks = 0;
    edm::DetSetVector<PixelDigiSimLink>::const_iterator 
      isearch = pixelSimLinks->find(detid);

    if(isearch != pixelSimLinks->end()) {      //if it is not empty
      edm::DetSet<PixelDigiSimLink> link_detset = (*pixelSimLinks)[detid];
      edm::DetSet<PixelDigiSimLink>::const_iterator it;
      // Loop over DigisSimLink in this det unit
      for(it = link_detset.data.begin(); 
	  it != link_detset.data.end(); it++) {
	
	numberOfSimLinks++;
	// these methods should be declared const, fixed by M.P.
	// wait for next releasse and then uncomment
	unsigned int chan = it->channel();
	unsigned int simTrack = it->SimTrackId();
	float frac = it->fraction();
#ifdef HISTOS
	hsimlinks->Fill(float(simTrack));
	hfract->Fill(frac);
#endif

	if(PRINT) cout<<" Sim link "<<numberOfSimLinks<<" "<<chan<<" "
		      <<simTrack<<" "<<frac<<endl;
      } // end simlink det loop

    } // end simlink if

#endif  // USE_SIM_LINKS

    unsigned int numberOfDigis = 0;

    // Look at digis now
    edm::DetSet<PixelDigi>::const_iterator  di;
      for(di = DSViter->data.begin(); di != DSViter->data.end(); di++) {
	//for(di = begin; di != end; di++) {
	
	numberOfDigis++;
	totalNumOfDigis++;
       int adc = di->adc();    // charge, modifued to unsiged short 
       int col = di->column(); // column 
       int row = di->row();    // row
       //int tof = di->time();    // tof always 0, method deleted

       // channel index needed to look for the simlink to simtracks
       int channel = PixelChannelIdentifier::pixelToChannel(row,col);
       if(PRINT) cout <<numberOfDigis<< " Col: " << col << " Row: " << row 
		      << " ADC: " << adc <<" channel = "<<channel<<endl;

       if(col>415) cout<<" Error: column index too large "<<col<<" Barrel layer, ladder, module "
		       <<layer<<" "<<ladder<<" "<<zindex<<endl;
       if(row>159) cout<<" Error: row index too large "<<row<<endl;

#ifdef HISTOS
       if(layer==1) {
	 heloss1->Fill(float(adc));
	 hcols1->Fill(float(col));
	 hrows1->Fill(float(row));
	 hpixMap1->Fill(float(col),float(row));
	 totalNumOfDigis1++;
	 htest2->Fill(float(zindex),float(adc));
	 numOfDigisPerDet1++;
	 if(RectangularPixelTopology::isItBigPixelInX(row)) {
	   //cout<<" big in X "<<row<<endl;
	   heloss1bigx->Fill(float(adc));
	   hrows1big->Fill(float(row));
	 }
	 if(RectangularPixelTopology::isItBigPixelInY(col)) {
	   //cout<<" big in Y "<<col<<endl;
	   heloss1bigy->Fill(float(adc));
	   hcols1big->Fill(float(col));
	 }

       } else if(layer==2) {
	 heloss2->Fill(float(adc));
	 hcols2->Fill(float(col));
	 hrows2->Fill(float(row));
	 hpixMap2->Fill(float(col),float(row));
	 totalNumOfDigis2++;
	 numOfDigisPerDet2++;

       } else if(layer==3) {
	 heloss3->Fill(float(adc));
	 hcols3->Fill(float(col));
	 hrows3->Fill(float(row));
	 hpixMap3->Fill(float(col),float(row));
	 if(ladder==11 && zindex==5 )  // r367 
	   //if(ladder==8 && zindex==5 ) //r371
	   hpixMap3->Fill(float(col),float(row));  
	 totalNumOfDigis3++;
	 numOfDigisPerDet3++;

       } else if(disk==1) {
	 helossF1->Fill(float(adc));
	 hcolsF1->Fill(float(col));
	 hrowsF1->Fill(float(row));
	 totalNumOfDigisF1++;
	 numOfDigisPerDetF1++;

       } else if(disk==2) {
	 helossF2->Fill(float(adc));
	 hcolsF2->Fill(float(col));
	 hrowsF2->Fill(float(row));
	 totalNumOfDigisF2++;
	 numOfDigisPerDetF2++;
       } // end if layer
#endif
        
     } // end for digis
      //if(PRINT) 
      //cout<<" for det "<<detid<<" digis = "<<numberOfDigis<<endl;

#ifdef HISTOS
     if(layer==1) {
       hdigisPerDet1->Fill(float(numOfDigisPerDet1));
       htest->Fill(float(zindex),float(numOfDigisPerDet1));
     } else if(layer==2) hdigisPerDet2->Fill(float(numOfDigisPerDet2));
     else if(layer==3) hdigisPerDet3->Fill(float(numOfDigisPerDet3));
     else if(disk==1) hdigisPerDetF1->Fill(float(numOfDigisPerDetF1));
     else if(disk==2) hdigisPerDetF2->Fill(float(numOfDigisPerDetF2));
#endif

   } // end for det-units

   if(PRINT) 
     cout << " Number of full det-units = " <<numberOfDetUnits
	  <<" total digis = "<<totalNumOfDigis<<endl;

#ifdef HISTOS
   hdigisPerLay1 ->Fill(float(totalNumOfDigis1));
   hdigisPerLay2 ->Fill(float(totalNumOfDigis2));
   hdigisPerLay3 ->Fill(float(totalNumOfDigis3));
   hdetsPerLay1 ->Fill(float(numberOfDetUnits1));
   hdetsPerLay2 ->Fill(float(numberOfDetUnits2));
   hdetsPerLay3 ->Fill(float(numberOfDetUnits3));
   hdigisPerLayF1 ->Fill(float(totalNumOfDigisF1));
   hdigisPerLayF2 ->Fill(float(totalNumOfDigisF2));
   hdetsPerLayF1 ->Fill(float(numberOfDetUnitsF1));
   hdetsPerLayF2 ->Fill(float(numberOfDetUnitsF2));
#endif

}
// ------------ method called to at the end of the job  ------------
void PixelDigisTest::endJob(){
  cout << " End PixelDigisTest " << endl;
  //hFile->Write();
  //hFile->Close();
}

//define this as a plug-in
DEFINE_FWK_MODULE(PixelDigisTest);
