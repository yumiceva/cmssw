#ifndef HcalZeroSuppression_H
#define HcalZeroSuppression_H

#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/Common/interface/Handle.h"
#include "DataFormats/HcalDetId/interface/HcalGenericDetId.h"
#include "DataFormats/HcalDetId/interface/HcalElectronicsId.h"
#include "DataFormats/HcalDetId/interface/HcalDetId.h"
#include "DataFormats/HcalDigi/interface/HcalDigiCollections.h"
#include "DataFormats/DetId/interface/DetId.h"
#include "CondFormats/HcalObjects/interface/HcalPedestals.h"
#include "CondFormats/HcalObjects/interface/HcalPedestalWidths.h"
#include "CondFormats/HcalObjects/interface/HcalQIECoder.h"
#include "CondFormats/HcalObjects/interface/HcalQIEData.h"
#include "CondFormats/HcalObjects/interface/HcalQIEShape.h"
#include "CondFormats/HcalObjects/interface/HcalElectronicsMap.h"
#include "CondFormats/HcalObjects/interface/AllObjects.h"

#include "CalibFormats/HcalObjects/interface/HcalDbRecord.h"
#include "CalibFormats/HcalObjects/interface/HcalDbService.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrations.h"
#include "CalibFormats/HcalObjects/interface/HcalCalibrationWidths.h"

#include "CondTools/Hcal/interface/HcalDbOnline.h"

#include "CalibCalorimetry/HcalAlgos/interface/HcalDbASCIIIO.h"
//#include "CalibCalorimetry/HcalStandardModules/interface/HcalDbXmlTwo.h"
#include "CalibCalorimetry/HcalStandardModules/interface/HcalCondXML.h"
#include "TBDataFormats/HcalTBObjects/interface/HcalTBTriggerData.h"

#include "TFile.h"
#include "TProfile.h"
#include "TH1.h"
#include "TH2.h"
#include "TStyle.h"
#include "TCanvas.h"
#include <math.h>
#include <iostream>
#include <map>
#include <iomanip>
#include <fstream>
#include <vector>
#include <string>

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

   struct ZSBunch
   {
      HcalDetId detid;
      bool usedflag;
      float cap[4];
      float sig[4][4];
      float prod[4][4];
      int num[4][4];
   };

class HcalZeroSuppression : public edm::EDAnalyzer
{
   public:
   //Constructor
   HcalZeroSuppression(const edm::ParameterSet& ps);
   //Destructor
   virtual ~HcalZeroSuppression();
   //Analysis
   void analyze(const edm::Event & event, const edm::EventSetup& eventSetup);

   private:
   //Container for data, 1 per channel
   std::vector<ZSBunch> Bunches;
   //Flag for saving histos
   bool verboseflag;
   int runnum;
   int firstTS; //TIMESLICE SELECTION IS HARDCODED
   int lastTS;
   std::string ROOTfilename;
   std::string ZSfilename;
   std::string tag;

   TH1F *HBMeans;
   TH1F *HBWidths;
   TH1F *HEMeans;
   TH1F *HEWidths;
   TH1F *HFMeans;
   TH1F *HFWidths;
   TH1F *HOMeans;
   TH1F *HOWidths;
   TH1F *ZSHist[4];
   TH2F *depthhist[4];

   TFile *theFile;
   bool firsttime;
};
#endif

