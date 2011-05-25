#ifndef CL_EcnaAnalyzer_H
#define CL_EcnaAnalyzer_H

// -*- C++ -*-
//
// Package:    EcalCorrelatedNoiseAnalysisModules
// Class:      EcnaAnalyzer
// 
/**\class EcnaAnalyzer EcnaAnalyzer.cc CalibCalorimetry/EcalCorrelatedNoiseAnalysisModules/src/EcnaAnalyzer.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Bernard Fabbro
//         Created:  Fri Jun  2 10:27:01 CEST 2006
// $Id: EcnaAnalyzer.h,v 1.2 2007/07/13 12:11:58 fabbro Exp $
//
//


// system include files
#include <memory>
#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <time.h>
#include "Riostream.h"

#include <sys/time.h>

// ROOT include files
#include "TObject.h"
#include "TSystem.h"
#include "TString.h"
#include "TVectorD.h"
#include "TTreeIndex.h"

// CMSSW include files
#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "CondCore/DBCommon/interface/Time.h"

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "DataFormats/EcalDigi/interface/EBDataFrame.h"
#include "DataFormats/EcalDigi/interface/EcalMGPASample.h"
#include "DataFormats/EcalDigi/interface/EcalDigiCollections.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DataFormats/Provenance/interface/EventID.h"
#include "DataFormats/EcalDetId/interface/EBDetId.h"
#include "DataFormats/EcalDetId/interface/EEDetId.h"

#include "DataFormats/Provenance/interface/Timestamp.h"
//#include "Geometry/EcalMapping/interface/EcalElectronicsMapping.h"
//#include "Geometry/EcalMapping/interface/EcalMappingRcd.h"

#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"

// user include files
#include "CalibCalorimetry/EcalCorrelatedNoiseAnalysisAlgos/interface/TEcnaRun.h"
#include "CalibCalorimetry/EcalCorrelatedNoiseAnalysisAlgos/interface/TEcnaParEcal.h"
#include "CalibCalorimetry/EcalCorrelatedNoiseAnalysisAlgos/interface/TEcnaNumbering.h"

//
// class declaration
//

class EcnaAnalyzer : public edm::EDAnalyzer {

 public:
  
  enum { kChannels = 1700, kGains = 3, kFirstGainId = 1 };
  
  explicit EcnaAnalyzer(const edm::ParameterSet&);
  ~EcnaAnalyzer();  
  
  virtual void analyze(const edm::Event&, const edm::EventSetup&);
  TString runtype(const Int_t&);
  Int_t   gainvalue(const Int_t&);
  void    CheckMsg(const Int_t&, const Int_t&);
  void    CheckMsg(const Int_t&);
  Bool_t  AnalysisOutcome(const TString);

 private:

  Int_t   fgMaxCar;   // Max nb of caracters for char*
  TString fTTBELL;

  // ----------member data ---------------------------
  unsigned int verbosity_;
  Int_t  nChannels_;
  Int_t  iEvent_; // should be removed when we can access class EventID
  string eventHeaderProducer_;
  string digiProducer_;
  string eventHeaderCollection_;
  string EBdigiCollection_;
  string EEdigiCollection_;

  TString  sAnalysisName_;
  TString  sNbOfSamples_;
  TString  sFirstReqEvent_;
  TString  sLastReqEvent_;
  TString  sReqNbOfEvts_;
  TString  sStexName_;
  TString  sStexNumber_;

  Bool_t   fOutcomeError;

  Int_t   fEvtNumber;
  Int_t   fEvtNumberMemo;
  Int_t   fRecNumber;
  Int_t   fCurrentEventNumber;
  Int_t   fNbOfSelectedEvents;

  Int_t   fBadBuildCnaRun;

  TString  fCfgAnalyzerParametersFilePath;  // absolute path for the analyzer parameters files (/afs/etc...)
  TString  fCfgAnalyzerParametersFileName;  // name of the analyzer parameters file 
  ifstream fFcin_f;

  TString fAnalysisName;
  Int_t   fChozenGainNumber;     // determined from fAnalysisName
  Int_t   fChozenRunTypeNumber;  // determined from fAnalysisName
  TString fDynBaseLineSub;       // determined from fAnalysisName

  Int_t   fNbOfSamples;
  Int_t   fRunNumber;
  Int_t   fRunTypeNumber;
  Int_t   fFirstReqEvent;
  Int_t   fLastReqEvent;
  TString fStexName;
  Int_t   fStexNumber;

  Int_t fReqNbOfEvts;
  Int_t fMgpaGainNumber;

  Int_t fSMIndexBegin;
  Int_t fSMIndexStop;
  Int_t fDeeIndexBegin;
  Int_t fDeeIndexStop;
  Int_t fStexIndexBegin;
  Int_t fStexIndexStop;

  Int_t    fFedTcc;
  Int_t*   fSMFromFedTcc;
  Int_t*   fESFromFedTcc;
  Int_t*   fDeeFromFedTcc;
  Int_t    fTreatedFedOrder;
  Int_t*   fFedStatusOrder;
  Int_t    fFedId;
  TString* fDeeNumberString;

  Int_t  fMaxTreatedStexCounter;
  Int_t  fDeeDS5Memo1;
  Int_t  fDeeDS5Memo2;
  Int_t* fStexDigiOK;
  Int_t* fStexNbOfTreatedEvents;
  Int_t* fStexStatus;

  Int_t  fMaxFedUnitCounter;
  Int_t* fFedStatus;
  Int_t* fFedDigiOK;
  Int_t* fFedNbOfTreatedEvents;

  Int_t  fMemoCutOK;
  Int_t  fNbOfTreatedStexs;
  Int_t* fNbOfTreatedFedsInDee;
  Int_t* fNbOfTreatedFedsInStex;

  Int_t fPEDESTAL_STD;
  Int_t fPEDESTAL_GAP;
  Int_t fLASER_STD;
  Int_t fPEDSIM;

  time_t*  fTimeFirst;
  time_t*  fTimeLast;  
  TString* fDateFirst;
  TString* fDateLast;

  Int_t* fMemoDateFirstEvent;

  TEcnaRun** fMyCnaEBSM;
  TEcnaRun** fMyCnaEEDee;

  TEcnaNumbering* fMyEBNumbering; 
  TEcnaParEcal*   fMyEBEcal;

  TEcnaNumbering* fMyEENumbering; 
  TEcnaParEcal*   fMyEEEcal; 

  TEcnaParPaths*  fCnaParPaths;

  //  Int_t** fT2d_LastEvt; // 2D array[channel][sample] max nb of evts read for a given (channel,sample) 
  //  Int_t*  fT1d_LastEvt;

  Int_t  fMaxRunTypeCounter;
  Int_t* fRunTypeCounter;

  Int_t  fMaxMgpaGainCounter;
  Int_t* fMgpaGainCounter;

  Int_t  fMaxFedIdCounter;
  Int_t* fFedIdCounter;

  Int_t  fMaxCounterQuad;
  Int_t* fCounterQuad;
};

#endif
