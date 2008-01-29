// -*- C++ -*-
//
// Package:    SiPixelGainCalibrationAnalysis
// Class:      SiPixelGainCalibrationAnalysis
// 
/**\class SiPixelGainCalibrationAnalysis SiPixelGainCalibrationAnalysis.cc CalibTracker/SiPixelGainCalibrationAnalysis/src/SiPixelGainCalibrationAnalysis.cc

 Description: <one line class summary>

 Implementation:
     <Notes on implementation>
*/
//
// Original Author:  Freya Blekman
//         Created:  Wed Nov 14 15:02:06 CET 2007
// $Id: SiPixelGainCalibrationAnalysis.cc,v 1.6 2008/01/23 20:21:03 fblekman Exp $
//
//

// user include files
#include "CondCore/DBOutputService/interface/PoolDBOutputService.h"

#include "SiPixelGainCalibrationAnalysis.h"
#include <sstream>
//
// constructors and destructor
//
SiPixelGainCalibrationAnalysis::SiPixelGainCalibrationAnalysis(const edm::ParameterSet& iConfig):
  SiPixelOfflineCalibAnalysisBase(iConfig),
  conf_(iConfig),
  bookkeeper_(),
  bookkeeper_pixels_(),
  nfitparameters_(iConfig.getUntrackedParameter<int>("numberOfFitParameters",2)),
  fitfunction_(iConfig.getUntrackedParameter<std::string>("fitFunctionRootFormula","[1]*(x+[0])")),
  reject_plateaupoints_(iConfig.getUntrackedParameter<bool>("suppressPlateauInFit",true)),
  reject_single_entries_(iConfig.getUntrackedParameter<bool>("suppressPointsWithOneEntryOrLess",true)),
  reject_badpoints_frac_(iConfig.getUntrackedParameter<double>("suppressZeroAndPlateausInFitFrac",0)),
  chi2Threshold_(iConfig.getUntrackedParameter<double>("minChi2forHistSave",10)),
  maxGainInHist_(iConfig.getUntrackedParameter<double>("maxGainInHist",10)),
  maxChi2InHist_(iConfig.getUntrackedParameter<double>("maxChi2InHist",25)),
  saveALLHistograms_(iConfig.getUntrackedParameter<bool>("saveAllHistograms",false)),
  filldb_(iConfig.getUntrackedParameter<bool>("writeDatabase",false)),
  recordName_(conf_.getParameter<std::string>("record")),
  appendMode_(conf_.getUntrackedParameter<bool>("appendMode",true)),
  theGainCalibrationDbInput_(0),
  theGainCalibrationDbInputService_(iConfig)
{
  ::putenv("CORAL_AUTH_USER=me");
  ::putenv("CORAL_AUTH_PASSWORD=test");   
  fitter_=new TLinearFitter(nfitparameters_,fitfunction_.c_str());
  edm::LogInfo("SiPixelGainCalibrationAnalysis") << "now using fit function " << fitfunction_ << ", which has " << nfitparameters_ << " free parameters. " << std::endl;
}

SiPixelGainCalibrationAnalysis::~SiPixelGainCalibrationAnalysis()
{
  delete fitter_;
}
// member functions
//
// ------------ method called once each job just before starting event loop  ------------

std::vector<float> SiPixelGainCalibrationAnalysis::CalculateAveragePerColumn(uint32_t detid, std::string label){
  std::vector<float> result;
  int ncols= bookkeeper_[detid][label]->getNbinsX();
  int nrows= bookkeeper_[detid][label]->getNbinsY();
  for(int icol=1; icol<=ncols; ++icol){
    float val=0;
    float ntimes =0;
    for(int irow=1; irow<=nrows; ++irow){
      val+= bookkeeper_[detid][label]->getBinContent(icol,irow);
      ntimes++;
    }
    val/= ntimes;
    result.push_back(val);
  }
  return result;
}

bool
SiPixelGainCalibrationAnalysis::checkCorrectCalibrationType()
{
  if(calibrationMode_=="Gain")
    return true;
  else if(calibrationMode_=="unknown"){
    edm::LogInfo("SiPixelGainCalibrationAnalysis") <<  "calibration mode is: " << calibrationMode_ << ", continuing anyway..." ;
    return true;
  }
  else
    edm::LogError("SiPixelGainCalibrationAnalysis") << "unknown calibration mode for Gain calibration, should be \"Gain\" and is \"" << calibrationMode_ << "\"";
  return false;
}

void SiPixelGainCalibrationAnalysis::calibrationSetup(const edm::EventSetup&)
{
}

// ------------ method called once each job just after ending the event loop  ------------
void 
SiPixelGainCalibrationAnalysis::calibrationEnd() {

  // this is where we loop over all histograms and save the database objects
  uint32_t detid=0;
  for(std::map<uint32_t,std::map<std::string,MonitorElement *> >::const_iterator idet = bookkeeper_.begin(); idet != bookkeeper_.end(); ++idet){
    if(detid==idet->first)
      continue;// only do things once per detid
    detid=idet->first;
    std::vector<float> gainvec=CalculateAveragePerColumn(detid,"gain_2d");
    std::vector<float> pedvec =CalculateAveragePerColumn(detid,"ped_2d");
    std::vector<float> chi2vec = CalculateAveragePerColumn(detid,"chi2_2d");
    std::ostringstream summarytext;

    summarytext << "Summary for det ID " << detid << "(" << translateDetIdToString(detid) << ")\n";
    summarytext << "\t Following: values per column: column #, gain, pedestal, chi2\n";
    for(uint32_t i=0; i<gainvec.size(); i++)
      summarytext << "\t " << i << " \t" << gainvec[i] << " \t" << pedvec[i] << " \t" << chi2vec[i] << "\n";
    summarytext << "\t list of pixels with high chi2 (chi2> " << chi2Threshold_ << "): \n";

    
    for(std::map<std::string, MonitorElement *>::const_iterator ipix = bookkeeper_pixels_[detid].begin(); ipix!=bookkeeper_pixels_[detid].end(); ++ipix)
      summarytext << "\t " << ipix->first << "\n";
    edm::LogInfo("SiPixelGainCalibrationAnalysis") << summarytext.str() << std::endl;
  }
  if(filldb_)
    fillDatabase();
}
//-----------method to fill the database
void SiPixelGainCalibrationAnalysis::fillDatabase(){

  uint32_t nchannels=0;
  uint32_t nmodules=0;
  for(std::map<uint32_t,std::map<std::string, MonitorElement *> >::const_iterator idet=bookkeeper_.begin(); idet!= bookkeeper_.end(); ++idet){
    uint32_t detid=idet->first;
    // Get the module sizes.
    int nrows = bookkeeper_[detid]["gain2d"]->getNbinsY();
    int ncols = bookkeeper_[detid]["ped2d"]->getNbinsX();   
    
    std::vector<char> theSiPixelGainCalibration;

    // Loop over columns and rows of this DetID
    for(int i=1; i<=ncols; i++) {
      for(int j=1; j<=nrows; j++) {
	nchannels++;
	     
	float ped = bookkeeper_[detid]["ped2d"]->getBinContent(i,j);
	float gain = bookkeeper_[detid]["gain2d"]->getBinContent(i,j);

	theGainCalibrationDbInput_->setData( ped , gain , theSiPixelGainCalibration);
      }
    }

    SiPixelGainCalibration::Range range(theSiPixelGainCalibration.begin(),theSiPixelGainCalibration.end());
    if( !theGainCalibrationDbInput_->put(detid,range,ncols) )
      edm::LogError("SiPixelGainCalibrationAnalysis")<<"warning: detid already exists"<<std::endl;
  }
  edm::LogInfo("SiPixelGainCalibrationAnalysis") << " ---> PIXEL Modules  " << nmodules  << "\n"
						 << " ---> PIXEL Channels " << nchannels << std::endl;

  edm::LogInfo(" --- writing to DB!");
  edm::Service<cond::service::PoolDBOutputService> mydbservice;
  if(!mydbservice.isAvailable() ){
    edm::LogError("db service unavailable");
    return;
    if( mydbservice->isNewTagRequest(recordName_) ){
      mydbservice->createNewIOV<SiPixelGainCalibration>(
							theGainCalibrationDbInput_, mydbservice->endOfTime(),recordName_);
    } else {
      mydbservice->appendSinceTime<SiPixelGainCalibration>(
							   theGainCalibrationDbInput_, mydbservice->currentTime(),recordName_);
    }
    edm::LogInfo(" --- all OK");
  } 
}
// ------------ method called to do fits to all objects available  ------------
bool
SiPixelGainCalibrationAnalysis::doFits(uint32_t detid, std::vector<SiPixelCalibDigi>::const_iterator ipix)
{

  // first, fill the input arrays to the TLinearFitter.
  double xvals[100];
  double yvals[100];
  double yerrvals[100];
  double xvalsall[200];
  float  xvalsasfloatsforDQM[200];
  double yvalsall[200];
  double yerrvalsall[200];
  int npoints=0;
  float maxrange = 255.*(1-reject_badpoints_frac_);
  float lastpointval = ipix->getsum(ipix->getnpoints()-1);
  if(ipix->getnentries(ipix->getnpoints()-1)>0)
    lastpointval/=(float)ipix->getnentries(ipix->getnpoints()-1);
  if(lastpointval>100 && reject_plateaupoints_)
    maxrange = (1-reject_badpoints_frac_)*lastpointval;


  // now do the loop...
  for(uint32_t ipoint = 0; ipoint < ipix->getnpoints(); ++ipoint){
    //    float minrange = 255.*reject_badpoints_frac_;
    if(ipoint>200)
      continue;

    xvalsasfloatsforDQM[ipoint]=xvalsall[ipoint]=vCalValues_[ipoint];
    yerrvalsall[ipoint]=yvalsall[ipoint]=0;
    if(ipix->getnentries(ipoint)>0){
      yvalsall[ipoint]=ipix->getsum(ipoint)/(float)ipix->getnentries(ipoint);
      yerrvalsall[ipoint]=(ipix->getsumsquares(ipoint)/(float)ipix->getnentries(ipoint))-pow(yvalsall[ipoint],2);
      yerrvalsall[ipoint]=sqrt(yerrvalsall[ipoint]);
    }
    
    if(yvalsall[ipoint]<maxrange && reject_badpoints_ && npoints<100){
      if(ipix->getnentries(ipoint)>1 || !reject_single_entries_){
	xvals[npoints]=xvalsall[ipoint];
	yvals[npoints]=yvalsall[ipoint];
	yerrvals[npoints]=yerrvalsall[ipoint];
	//std::cout << xvals[npoints] << " "<< yvals[npoints] << " " << yerrvals[npoints] << std::endl;
	npoints++;
      }
    }
    
  }
  bool makehistopersistent=saveALLHistograms_;
  if(npoints>2)
    fitter_->AssignData(npoints,2,xvals,yvals,yerrvals);
  else{
    int nallpoints = ipix->getnpoints();
    if(nallpoints>200)
      nallpoints=200;
    fitter_->AssignData(nallpoints,2,xvalsall,yvalsall,yerrvalsall);
    makehistopersistent=true;
  }


  // and do the fit:
  int result = fitter_->Eval();
  float chi2,slope,intercept,prob;
  prob=chi2=-1;
  slope=intercept=0;
  if(result==1)
    makehistopersistent=true;
  if(result==0){
    slope = fitter_->GetParameter(1);
    intercept = fitter_->GetParameter(0);
    chi2 = fitter_->GetChisquare()/fitter_->GetNumberFreeParameters();
    prob = TMath::Prob(fitter_->GetChisquare(),fitter_->GetNumberFreeParameters());
      
    if(slope<0)
      makehistopersistent=true;
    if(chi2>chi2Threshold_ && chi2Threshold_>=0.)
      makehistopersistent=true;
    if(chi2Threshold_==0.)
      makehistopersistent=true;

    //    std::cout << "fit successful! gain " << slope << " intercept " << intercept << " prob " << prob << std::endl;
    bookkeeper_[detid]["gain_1d"]->Fill(slope);
    bookkeeper_[detid]["gain_2d"]->Fill(ipix->col(),ipix->row(),slope);
    bookkeeper_[detid]["ped_1d"]->Fill(intercept);
    bookkeeper_[detid]["ped_2d"]->Fill(ipix->col(),ipix->row(),intercept);
    bookkeeper_[detid]["chi2_1d"]->Fill(chi2);
    bookkeeper_[detid]["chi2_2d"]->Fill(ipix->col(),ipix->row(),chi2);
    bookkeeper_[detid]["prob_1d"]->Fill(prob);
    bookkeeper_[detid]["prob_2d"]->Fill(ipix->col(),ipix->row(),prob);
    //  std::cout << "leaving doFits" << std::endl;
  }
  if(makehistopersistent){
    setDQMDirectory(detid);
    std::ostringstream pixelinfo;
    pixelinfo << "row_" << ipix->row() << "_col_" << ipix->col();
    std::string tempname=translateDetIdToString(detid);
    tempname+="_";
    tempname+=pixelinfo.str();
     // and book the histo
    int nallpoints = ipix->getnpoints();
    if(nallpoints>200)
      nallpoints=200;
    bookkeeper_pixels_[detid][pixelinfo.str()] =  bookDQMHistogram1D(pixelinfo.str(),tempname,nallpoints-1,xvalsasfloatsforDQM);
    for(int ii=0; ii<nallpoints; ++ii){
      std::cout << "value for point " << ii << " " << xvalsall[ii] << " " << yvalsall[ii] << " " << yerrvalsall[ii] << std::endl;
      bookkeeper_pixels_[detid][pixelinfo.str()]->setBinContent(ii+1,yvalsall[ii]);
      bookkeeper_pixels_[detid][pixelinfo.str()]->setBinError(ii+1,yerrvalsall[ii]);
    }
  }
  //  edm::LogInfo("SiPixelGainCalibrationAnalysis") << "looking at Det ID " << detid << " "<< translateDetIdToString(detid) << ", pixel " << ipix->row() << " , " << ipix->col() << " gain= " << slope << ", pedestal= " << intercept << " chi2/NDOF= " << chi2 << " fit result value " << result << std::endl;
  return true;
}
// ------------ method called to do fill new detids  ------------
void 
SiPixelGainCalibrationAnalysis::newDetID(uint32_t detid)
{
  setDQMDirectory(detid);
  std::string tempname=translateDetIdToString(detid);
  //std::cout << "creating new histograms..."<< tempname << std::endl;
  bookkeeper_[detid]["gain_1d"] = bookDQMHistogram1D("gain_1d","gain for "+tempname,100,0.,maxGainInHist_);
  bookkeeper_[detid]["gain_2d"] = bookDQMHistoPlaquetteSummary2D("gain_2d","gain for "+tempname,detid);
  bookkeeper_[detid]["ped_1d"] = bookDQMHistogram1D("pedestal_1d","pedestal for "+tempname,256,0.,256.);
  bookkeeper_[detid]["ped_2d"] = bookDQMHistoPlaquetteSummary2D("pedestal_2d","pedestal for "+tempname,detid);
  bookkeeper_[detid]["chi2_1d"] = bookDQMHistogram1D("chi2_1d","#chi^{2}/NDOF for "+tempname,100,0.,maxChi2InHist_);
  bookkeeper_[detid]["chi2_2d"] = bookDQMHistoPlaquetteSummary2D("chi2_2d","#chi^{2}/NDOF for "+tempname,detid);
  bookkeeper_[detid]["prob_1d"] = bookDQMHistogram1D("prob_1d","P(#chi^{2},NDOF) for "+tempname,100,0.,1.0);
  bookkeeper_[detid]["prob_2d"] = bookDQMHistoPlaquetteSummary2D("prob_2d","P(#chi^{2},NDOF) for "+tempname,detid);

  //std::cout << "leaving new detid" << std::endl;
}
//define this as a plug-in
DEFINE_FWK_MODULE(SiPixelGainCalibrationAnalysis);
