#include <iostream>
#include <fstream>
#include <vector>
#include <math.h>

#include "FWCore/Framework/interface/Run.h"
#include "FWCore/ServiceRegistry/interface/Service.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/MakerMacros.h"
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "DataFormats/EcalDetId/interface/ESDetId.h"
#include "DataFormats/EcalRecHit/interface/EcalRecHitCollections.h"
#include "DQMServices/Core/interface/MonitorElement.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQM/EcalPreshowerMonitorModule/interface/ESTrendTask.h"
#include "DataFormats/EcalRawData/interface/ESDCCHeaderBlock.h"
#include "DataFormats/EcalRawData/interface/ESKCHIPBlock.h"
#include "DataFormats/EcalRawData/interface/EcalRawDataCollections.h"

using namespace cms;
using namespace edm;
using namespace std;

ESTrendTask::ESTrendTask(const ParameterSet& ps) {

  init_ = false;

  dqmStore_ = Service<DQMStore>().operator->();

  prefixME_       = ps.getUntrackedParameter<string>("prefixME", "EcalPreshower"); 
  enableCleanup_  = ps.getUntrackedParameter<bool>("enableCleanup", false);
  mergeRuns_      = ps.getUntrackedParameter<bool>("mergeRuns", false);
  rechitlabel_    = ps.getParameter<InputTag>("RecHitLabel");
  dccCollections_ = ps.getParameter<InputTag>("ESDCCCollections");

  for (int i=0; i<2; ++i)
    for (int j=0; j<2; ++j) {
      hESRecHitTrend_[i][j] = 0;
      hESRecHitTrendHr_[i][j] = 0;
    }

  hESSLinkErrTrend_ = 0;
  hESFiberErrTrend_ = 0;
  hESSLinkErrTrendHr_ = 0;
  hESFiberErrTrendHr_ = 0;

  start_time_ = 0;
  current_time_ = 0;
  last_time_ = 0;

}

ESTrendTask::~ESTrendTask() {
}

void ESTrendTask::beginJob(void) {

  ievt_ = 0;

  if ( dqmStore_ ) {
    dqmStore_->setCurrentFolder(prefixME_ + "/ESTrendTask");
    dqmStore_->rmdir(prefixME_ + "/ESTrendTask");
  }

}

void ESTrendTask::beginRun(const Run& r, const EventSetup& c) {

  if ( ! mergeRuns_ ) this->reset();

  start_time_ = (r.beginTime()).unixTime();

  //std::cout << "start time : " << start_time_ << std::endl;

}

void ESTrendTask::endRun(const Run& r, const EventSetup& c) {

}

void ESTrendTask::reset(void) {

  for (int i=0 ; i<2; ++i)  
    for (int j=0 ; j<2; ++j) {
      if (hESRecHitTrend_[i][j]) hESRecHitTrend_[i][j]->Reset();
      if (hESRecHitTrendHr_[i][j]) hESRecHitTrendHr_[i][j]->Reset();
    }

  if (hESSLinkErrTrend_) hESSLinkErrTrend_->Reset();
  if (hESFiberErrTrend_) hESFiberErrTrend_->Reset();
  if (hESSLinkErrTrendHr_) hESSLinkErrTrendHr_->Reset();
  if (hESFiberErrTrendHr_) hESFiberErrTrendHr_->Reset();

}

void ESTrendTask::setup(void) {

  init_ = true;

  char histo[200];

  if ( dqmStore_ ) {
    dqmStore_->setCurrentFolder(prefixME_ + "/ESTrendTask");

    for (int i=0 ; i<2; ++i)  
      for (int j=0 ; j<2; ++j) {
	int iz = (i==0)? 1:-1;
	sprintf(histo, "ES Trending RH Occ per 5 mins Z %d P %d", iz, j+1);
	hESRecHitTrend_[i][j] = dqmStore_->bookProfile(histo, histo, 36, 0.0, 180.0, 100, 0.0, 1.0e6, "s");
	hESRecHitTrend_[i][j]->setAxisTitle("Elapse time (Minutes)", 1);
	hESRecHitTrend_[i][j]->setAxisTitle("ES RecHit Occupancy / 5 minutes", 2);

	sprintf(histo, "ES Trending RH Occ per hour Z %d P %d", iz, j+1);
	hESRecHitTrendHr_[i][j] = dqmStore_->bookProfile(histo, histo, 24, 0.0, 24.0, 100, 0.0, 1.0e6, "s");
	hESRecHitTrendHr_[i][j]->setAxisTitle("Elapse time (Hours)", 1);
	hESRecHitTrendHr_[i][j]->setAxisTitle("ES RecHit Occupancy / hour", 2);
      }

    sprintf(histo, "ES Trending SLink CRC Error per 5 mins");
    hESSLinkErrTrend_ = dqmStore_->bookProfile(histo, histo, 36, 0.0, 180.0, 100, 0.0, 1.0e6, "s");
    hESSLinkErrTrend_->setAxisTitle("Elapse time (Minutes)", 1);
    hESSLinkErrTrend_->setAxisTitle("ES SLink CRC Err / 5 minutes", 2);

    sprintf(histo, "ES Trending Fiber Error per 5 mins");
    hESFiberErrTrend_ = dqmStore_->bookProfile(histo, histo, 36, 0.0, 180.0, 100, 0.0, 1.0e6, "s");
    hESFiberErrTrend_->setAxisTitle("Elapse time (Minutes)", 1);
    hESFiberErrTrend_->setAxisTitle("ES Fiber Err / 5 minutes", 2);

    sprintf(histo, "ES Trending SLink CRC Error per hour");
    hESSLinkErrTrendHr_ = dqmStore_->bookProfile(histo, histo, 24, 0.0, 24.0, 100, 0.0, 1.0e6, "s");
    hESSLinkErrTrendHr_->setAxisTitle("Elapse time (Hours)", 1);
    hESSLinkErrTrendHr_->setAxisTitle("ES SLink CRC Err / hour", 2);

    sprintf(histo, "ES Trending Fiber Error per hour");
    hESFiberErrTrendHr_ = dqmStore_->bookProfile(histo, histo, 24, 0.0, 24.0, 100, 0.0, 1.0e6, "s");
    hESFiberErrTrendHr_->setAxisTitle("Elapse time (Hours)", 1);
    hESFiberErrTrendHr_->setAxisTitle("ES Fiber Err / hour", 2);
  }

}

void ESTrendTask::cleanup(void) {

  if ( ! init_ ) return;

  if ( dqmStore_ ) {
    dqmStore_->setCurrentFolder(prefixME_ + "/ESTrendTask");

    for (int i=0 ; i<2; ++i)  
      for (int j=0 ; j<2; ++j) {
	if (hESRecHitTrend_[i][j]) dqmStore_->removeElement(hESRecHitTrend_[i][j]->getName());
	hESRecHitTrend_[i][j] = 0;
	if (hESRecHitTrendHr_[i][j]) dqmStore_->removeElement(hESRecHitTrendHr_[i][j]->getName());
	hESRecHitTrendHr_[i][j] = 0;
      }

    if (hESSLinkErrTrend_) dqmStore_->removeElement(hESSLinkErrTrend_->getName());
    hESSLinkErrTrend_ = 0;
    if (hESFiberErrTrend_) dqmStore_->removeElement(hESFiberErrTrend_->getName());
    hESFiberErrTrend_ = 0;
    if (hESSLinkErrTrendHr_) dqmStore_->removeElement(hESSLinkErrTrendHr_->getName());
    hESSLinkErrTrendHr_ = 0;
    if (hESFiberErrTrendHr_) dqmStore_->removeElement(hESFiberErrTrendHr_->getName());
    hESFiberErrTrendHr_ = 0;
  }

  init_ = false;

}

void ESTrendTask::endJob(void) {

  LogInfo("ESTrendTask") << "analyzed " << ievt_ << " events";

  if ( enableCleanup_ ) this->cleanup();

}

void ESTrendTask::analyze(const Event& e, const EventSetup& c) {

  if ( ! init_ ) this->setup();

  ievt_++;

  // Collect time information
  updateTime(e);

  long int diff_current_start = current_time_ - start_time_;
  long int diff_last_start    = last_time_ - start_time_;
  //LogInfo("ESTrendTask") << "time difference is negative in " << ievt_ << " events\n"
  //<< "\tcurrent - start time = " << diff_current_start
  //<< ", \tlast - start time = " << diff_last_start << endl;

  //  std::cout << "current_time : " << current_time_ << ", diff : " << diff_current_start << std::endl;

  // Calculate time interval and bin width
  //  int minuteBinWidth = int(nBasicClusterMinutely_->getTProfile()->GetXaxis()->GetBinWidth(1));
  int minuteBinWidth = 5;
  long int minuteBinDiff = diff_current_start/60/minuteBinWidth - diff_last_start/60/minuteBinWidth;
  long int minuteDiff = (current_time_ - last_time_)/60;

  //  int hourBinWidth = int(nBasicClusterHourly_->getTProfile()->GetXaxis()->GetBinWidth(1));
  int hourBinWidth = 1;
  long int hourBinDiff = diff_current_start/3600/hourBinWidth - diff_last_start/3600/hourBinWidth;
  long int hourDiff = (current_time_ - last_time_)/3600;

  if (minuteDiff >= minuteBinWidth) {
    while (minuteDiff >= minuteBinWidth) minuteDiff -= minuteBinWidth;
  }
  if (hourDiff >= hourBinWidth) {
    while (hourDiff >= hourBinWidth) hourDiff -= hourBinWidth;
  }

  // ES DCC
  Int_t slinkCRCErr = 0;
  Int_t fiberErr = 0;
  vector<int> fiberStatus;
  Handle<ESRawDataCollection> dccs;
  if ( e.getByLabel(dccCollections_, dccs) ) {
    for (ESRawDataCollection::const_iterator dccItr = dccs->begin(); dccItr != dccs->end(); ++dccItr) {
      ESDCCHeaderBlock dcc = (*dccItr);

      if (dcc.getDCCErrors() == 101) slinkCRCErr++;

      fiberStatus = dcc.getFEChannelStatus();
      for (unsigned int i=0; i<fiberStatus.size(); ++i) {
	if (fiberStatus[i]==4 || fiberStatus[i]==8 || fiberStatus[i]==10 || fiberStatus[i]==11 || fiberStatus[i]==12)
	  fiberErr++;
      }
      
    }
  }

  shift2Right(hESSLinkErrTrend_->getTProfile(), minuteBinDiff);
  hESSLinkErrTrend_->Fill(minuteDiff, slinkCRCErr);

  shift2Right(hESFiberErrTrend_->getTProfile(), minuteBinDiff);
  hESFiberErrTrend_->Fill(minuteDiff, fiberErr);

  shift2Right(hESSLinkErrTrendHr_->getTProfile(), hourBinDiff);
  hESSLinkErrTrendHr_->Fill(hourDiff, slinkCRCErr);

  shift2Right(hESFiberErrTrendHr_->getTProfile(), hourBinDiff);
  hESFiberErrTrendHr_->Fill(hourDiff, fiberErr);

  // ES RecHits
  int zside, plane, ix, iy, strip;
  int nrh[2][2];
  for (int i = 0; i < 2; i++ ) 
    for( int j = 0; j < 2; j++) {
      nrh[i][j] = 0;
    }

  Handle<EcalRecHitCollection> ESRecHit;
  if ( e.getByLabel(rechitlabel_, ESRecHit) ) {
    
    for (ESRecHitCollection::const_iterator hitItr = ESRecHit->begin(); hitItr != ESRecHit->end(); ++hitItr) {
      
      ESDetId id = ESDetId(hitItr->id());
      
      zside = id.zside();
      plane = id.plane();
      ix    = id.six();
      iy    = id.siy();
      strip = id.strip();
      
      int i = (zside==1)? 0:1;
      int j = plane-1;
      
      nrh[i][j]++;
    }
  } else {
    LogWarning("ESTrendTask") << rechitlabel_ << " not available";
  }
  
  for (int i=0; i<2; ++i) 
    for (int j=0; j<2; ++j) {
      shift2Right(hESRecHitTrend_[i][j]->getTProfile(), minuteBinDiff);
      hESRecHitTrend_[i][j]->Fill(minuteDiff, nrh[i][j]/(1072*32.));

      shift2Right(hESRecHitTrendHr_[i][j]->getTProfile(), hourBinDiff);
      hESRecHitTrendHr_[i][j]->Fill(hourDiff, nrh[i][j]/(1072*32.));
    }

}

void ESTrendTask::updateTime(const edm::Event& e) {

  last_time_ = current_time_;
  current_time_ = e.time().unixTime();

}

void ESTrendTask::shift2Right(TProfile* p, int bins) {

  if(bins <= 0) return;

  if(!p->GetSumw2()) p->Sumw2();
  int nBins = p->GetXaxis()->GetNbins();

  // by shifting n bin to the right, the number of entries are
  // reduced by the number in n bins including the overflow bin.
  double nentries = p->GetEntries();
  for(int i=0; i<bins; i++) nentries -= p->GetBinEntries(nBins+1-bins);
  p->SetEntries(nentries);
  
  // the last bin goes to overflow
  // each bin moves to the right

  TArrayD* sumw2 = p->GetSumw2();

  for(int i=nBins+1; i>bins; i--) {
    // GetBinContent return binContent/binEntries
    p->SetBinContent(i, p->GetBinContent(i-bins)*p->GetBinEntries(i-bins));
    p->SetBinEntries(i,p->GetBinEntries(i-bins));
    sumw2->SetAt(sumw2->GetAt(i-bins),i);
  }

}

void ESTrendTask::shift2Left(TProfile* p, int bins) {

  if(bins <= 0) return;

  if(!p->GetSumw2()) p->Sumw2();
  int nBins = p->GetXaxis()->GetNbins();

  // by shifting n bin to the left, the number of entries are
  // reduced by the number in n bins including the underflow bin.
  double nentries = p->GetEntries();
  for(int i=0; i<bins; i++) nentries -= p->GetBinEntries(i);
  p->SetEntries(nentries);
  
  // the first bin goes to underflow
  // each bin moves to the right

  TArrayD* sumw2 = p->GetSumw2();

  for(int i=0; i<=nBins+1-bins; i++) {
    // GetBinContent return binContent/binEntries
    p->SetBinContent(i, p->GetBinContent(i+bins)*p->GetBinEntries(i+bins));
    p->SetBinEntries(i,p->GetBinEntries(i+bins));
    sumw2->SetAt(sumw2->GetAt(i+bins),i);
  }

}

DEFINE_FWK_MODULE(ESTrendTask);
