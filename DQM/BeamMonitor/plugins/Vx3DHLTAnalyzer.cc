// -*- C++ -*-
//
// Package:    Vx3DHLTAnalyzer
// Class:      Vx3DHLTAnalyzer
// 
/**\class Vx3DHLTAnalyzer Vx3DHLTAnalyzer.cc plugins/Vx3DHLTAnalyzer.cc

 Description: [one line class summary]

 Implementation:
     [Notes on implementation]
*/
//
// Original Author:  Mauro Dinardo,28 S-020,+41227673777,
//         Created:  Tue Feb 23 13:15:31 CET 2010
// $Id: Vx3DHLTAnalyzer.cc,v 1.14 2010/03/06 21:38:39 dinardo Exp $
//
//


#include "DQM/BeamMonitor/interface/Vx3DHLTAnalyzer.h"

#include "FWCore/ServiceRegistry/interface/Service.h"

#include "DataFormats/TrackerRecHit2D/interface/SiPixelRecHitCollection.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/VertexReco/interface/VertexFwd.h"
#include "DataFormats/VertexReco/interface/Vertex.h"

#include <iostream>
#include <fstream>

#include <TFitterMinuit.h>


using namespace std;
using namespace reco;
using namespace edm;


Vx3DHLTAnalyzer::Vx3DHLTAnalyzer(const ParameterSet& iConfig)
{
  vertexCollection = edm::InputTag("pixelVertices");
  nLumiReset       = 1;
  dataFromFit      = false;
  minNentries      = 100;
  xRange           = 0.;
  xStep            = 1.;
  yRange           = 0.;
  yStep            = 1.;
  zRange           = 0.;
  zStep            = 1.;
  fileName         = "BeamSpot_3DVxPixels.txt";

  vertexCollection = iConfig.getParameter<InputTag>("vertexCollection");
  nLumiReset       = iConfig.getParameter<unsigned int>("nLumiReset");
  dataFromFit      = iConfig.getParameter<bool>("dataFromFit");
  minNentries      = iConfig.getParameter<int>("minNentries");
  xRange           = iConfig.getParameter<double>("xRange");
  xStep            = iConfig.getParameter<double>("xStep");
  yRange           = iConfig.getParameter<double>("yRange");
  yStep            = iConfig.getParameter<double>("yStep");
  zRange           = iConfig.getParameter<double>("zRange");
  zStep            = iConfig.getParameter<double>("zStep");
  fileName         = iConfig.getParameter<string>("fileName");
}


Vx3DHLTAnalyzer::~Vx3DHLTAnalyzer()
{
}


void Vx3DHLTAnalyzer::analyze(const Event& iEvent, const EventSetup& iSetup)
{
  Handle<VertexCollection> Vx3DCollection;
  iEvent.getByLabel(vertexCollection,Vx3DCollection);

  if (beginTimeOfFit != 0)
    {
      for (vector<Vertex>::const_iterator it3DVx = Vx3DCollection->begin(); it3DVx != Vx3DCollection->end(); it3DVx++) {
	
	if ((it3DVx->isValid() == true) && (it3DVx->isFake() == false))
	  {
	    xVxValues.push_back(it3DVx->x());
	    yVxValues.push_back(it3DVx->y());
	    zVxValues.push_back(it3DVx->z());

	    Vx_X->Fill(it3DVx->x());
	    Vx_Y->Fill(it3DVx->y());
	    Vx_Z->Fill(it3DVx->z());
	    
	    Vx_ZX->Fill(it3DVx->z(), it3DVx->x());
	    Vx_ZY->Fill(it3DVx->z(), it3DVx->y());
	    Vx_XY->Fill(it3DVx->x(), it3DVx->y());
	   
	    Vx_ZX_profile->Fill(it3DVx->z(), it3DVx->x());
	    Vx_ZY_profile->Fill(it3DVx->z(), it3DVx->y());
	   
	    reportSummary->Fill(1.0);
	    reportSummaryMap->Fill(0.5, 0.5, 1.0);
	  }
      }

      runNumber = iEvent.id().run();
    }
}


void Gauss3DFunc(int& /*npar*/, double* /*gin*/, double& fval, double* par, int /*iflag*/)
{
  double a,b,c,d,e,f;
  double coef,det;
  double tmp,sumlog = 0.;
  double dim = 3.;
  double pi = 3.141592653589793238;

//   par[0] = K(0,0)
//   par[1] = K(1,1)
//   par[2] = K(2,2)
//   par[3] = K(0,1) = K(1,0)
//   par[4] = K(1,2) = K(2,1)
//   par[5] = K(0,2) = K(2,0)

//   a = M(0,0)
//   b = M(1,1)
//   c = M(2,2)
//   d = M(0,1) = M(1,0)
//   e = M(1,2) = M(2,1)
//   f = M(0,2) = M(2,0)

// K = Covariance Matrix
// M = K^-1

  det = fabs(par[0])*(fabs(par[1])*fabs(par[2])-par[4]*par[4]) - par[3]*(par[3]*fabs(par[2])-par[5]*par[4]) + par[5]*(par[3]*par[4]-par[5]*fabs(par[1]));
  a   = (fabs(par[1]*par[2]) - par[4]*par[4]) / det;
  b   = (fabs(par[0]*par[2]) - par[5]*par[5]) / det;
  c   = (fabs(par[0]*par[1]) - par[3]*par[3]) / det;
  d   = (par[5]*par[4] - par[3]*fabs(par[2])) / det;
  e   = (par[5]*par[3] - par[4]*fabs(par[0])) / det;
  f   = (par[3]*par[4] - par[5]*fabs(par[1])) / det;
  coef = 1. / sqrt(powf(2.*pi,dim)*fabs(det));

  for (unsigned int i = 0; i < xVxValues.size(); i++)
    {
      tmp = coef * exp(-1./2.*(a*(xVxValues[i]-par[6])*(xVxValues[i]-par[6]) + b*(yVxValues[i]-par[7])*(yVxValues[i]-par[7]) + c*(zVxValues[i]-par[8])*(zVxValues[i]-par[8]) +
			       2.*d*(xVxValues[i]-par[6])*(yVxValues[i]-par[7]) + 2.*e*(yVxValues[i]-par[7])*(zVxValues[i]-par[8]) + 2.*f*(xVxValues[i]-par[6])*(zVxValues[i]-par[8])));
      (tmp != 0.) ? sumlog += log(tmp) : sumlog += 0.;
    }
  
  fval = -1./2. * sumlog; 
}


void Vx3DHLTAnalyzer::writeToFile(vector<double>* vals,
				  string fileName,
				  edm::TimeValue_t BeginTimeOfFit,
				  edm::TimeValue_t EndTimeOfFit,
				  unsigned int BeginLumiOfFit,
				  unsigned int EndLumiOfFit)
{
  ofstream outputFile;
  outputFile.open(fileName.c_str(), ios::app); // To append: ios::app, to overwrite ios::out

  stringstream BufferString;
  BufferString.precision(5);
  
  if ((outputFile.is_open() == true) && (vals != NULL) && (vals->size() == 8))
    {
      vector<double>::const_iterator it = vals->begin();

      outputFile << "Runnumber " << runNumber << endl;
      outputFile << "BeginTimeOfFit ";
      outputFile << gmtime((const time_t*)&beginTimeOfFit)->tm_year << "-";
      outputFile << gmtime((const time_t*)&beginTimeOfFit)->tm_mon+1 << "-";
      outputFile << gmtime((const time_t*)&beginTimeOfFit)->tm_mday << " ";
      outputFile << (gmtime((const time_t*)&beginTimeOfFit)->tm_hour+1)%24 << ":";
      outputFile << gmtime((const time_t*)&beginTimeOfFit)->tm_min << ":";
      outputFile << gmtime((const time_t*)&beginTimeOfFit)->tm_sec << endl;
      outputFile << "EndTimeOfFit ";
      outputFile << gmtime((const time_t*)&endTimeOfFit)->tm_year << "-";
      outputFile << gmtime((const time_t*)&endTimeOfFit)->tm_mon+1 << "-";
      outputFile << gmtime((const time_t*)&endTimeOfFit)->tm_mday << " ";
      outputFile << (gmtime((const time_t*)&endTimeOfFit)->tm_hour+1)%24 << ":";
      outputFile << gmtime((const time_t*)&endTimeOfFit)->tm_min << ":";
      outputFile << gmtime((const time_t*)&endTimeOfFit)->tm_sec << endl;
      outputFile << "LumiRange " << beginLumiOfFit << " - " << endLumiOfFit << endl;
      outputFile << "Type 3" << endl; // 3D Vertexing with Pixel Tracks = type 3

      BufferString << *(it+0);
      outputFile << "X0 " << BufferString.str().c_str() << endl;
      BufferString.str("");

      BufferString << *(it+1);
      outputFile << "Y0 " << BufferString.str().c_str() << endl;
      BufferString.str("");

      BufferString << *(it+2);
      outputFile << "Z0 " << BufferString.str().c_str() << endl;
      BufferString.str("");

      BufferString << *(it+5);
      outputFile << "sigmaZ0 " << BufferString.str().c_str() << endl;
      BufferString.str("");

      BufferString << *(it+6);
      outputFile << "dxdz " << BufferString.str().c_str() << endl;
      BufferString.str("");

      BufferString << *(it+7);
      outputFile << "dydz " << BufferString.str().c_str() << endl;
      BufferString.str("");

      BufferString << *(it+3);
      outputFile << "BeamWidthX " << BufferString.str().c_str() << endl;
      BufferString.str("");

      BufferString << *(it+4);
      outputFile << "BeamWidthY " << BufferString.str().c_str() << endl;
      BufferString.str("");

      outputFile << "Cov(0,j) 0.0 0.0 0.0 0.0 0.0 0.0 0.0" << endl;
      outputFile << "Cov(1,j) 0.0 0.0 0.0 0.0 0.0 0.0 0.0" << endl;
      outputFile << "Cov(2,j) 0.0 0.0 0.0 0.0 0.0 0.0 0.0" << endl;
      outputFile << "Cov(3,j) 0.0 0.0 0.0 0.0 0.0 0.0 0.0" << endl;
      outputFile << "Cov(4,j) 0.0 0.0 0.0 0.0 0.0 0.0 0.0" << endl;
      outputFile << "Cov(5,j) 0.0 0.0 0.0 0.0 0.0 0.0 0.0" << endl;
      outputFile << "Cov(6,j) 0.0 0.0 0.0 0.0 0.0 0.0 0.0" << endl;

      outputFile << "EmittanceX 0.0" << endl;
      outputFile << "EmittanceY 0.0" << endl;
      outputFile << "BetaStar 0.0" << endl;
      outputFile << "\n" << endl;

      outputFile.close();
    }
}


void Vx3DHLTAnalyzer::beginLuminosityBlock(const LuminosityBlock& lumiBlock, 
					   const EventSetup& iSetup)
{
  if (lumiCounter == 0)
    {
      beginTimeOfFit = lumiBlock.beginTime().value();
      beginLumiOfFit = lumiBlock.luminosityBlock();
    }
}


void Vx3DHLTAnalyzer::endLuminosityBlock(const LuminosityBlock& lumiBlock,
					 const EventSetup& iSetup)
{
  lumiCounter++;
  int goodData = -1;

  if ((lumiCounter == nLumiReset) && (nLumiReset != 0) && (beginTimeOfFit != 0))
    {
      TF1* Gauss = new TF1("Gauss", "gaus");

      mXlumi->ShiftFillLast(Vx_X->getTH1F()->GetMean(), Vx_X->getTH1F()->GetMeanError(), nLumiReset);
      mYlumi->ShiftFillLast(Vx_Y->getTH1F()->GetMean(), Vx_Y->getTH1F()->GetMeanError(), nLumiReset);
      mZlumi->ShiftFillLast(Vx_Z->getTH1F()->GetMean(), Vx_Z->getTH1F()->GetMeanError(), nLumiReset);
      
      sXlumi->ShiftFillLast(Vx_X->getTH1F()->GetRMS(), Vx_X->getTH1F()->GetRMSError(), nLumiReset);
      sYlumi->ShiftFillLast(Vx_Y->getTH1F()->GetRMS(), Vx_Y->getTH1F()->GetRMSError(), nLumiReset);
      sZlumi->ShiftFillLast(Vx_Z->getTH1F()->GetRMS(), Vx_Z->getTH1F()->GetRMSError(), nLumiReset);
      
      Gauss->SetParameters(Vx_X->getTH1()->GetMaximum(), Vx_X->getTH1()->GetMean(), Vx_X->getTH1()->GetRMS());
      Vx_X->getTH1()->Fit("Gauss","LQN0");
      RMSsigXlumi->ShiftFillLast(Vx_X->getTH1F()->GetRMS() / Gauss->GetParameter(2),
				 (Vx_X->getTH1F()->GetRMS() / Gauss->GetParameter(2)) * sqrt(powf(Vx_X->getTH1F()->GetRMSError() / Vx_X->getTH1F()->GetRMS(),2.) +
											     powf(Gauss->GetParError(2) / Gauss->GetParError(2),2.)), nLumiReset);
      Gauss->SetParameters(Vx_Y->getTH1()->GetMaximum(), Vx_Y->getTH1()->GetMean(), Vx_Y->getTH1()->GetRMS());
      Vx_Y->getTH1()->Fit("Gauss","LQN0");
      RMSsigYlumi->ShiftFillLast(Vx_Y->getTH1F()->GetRMS() / Gauss->GetParameter(2),
				 (Vx_Y->getTH1F()->GetRMS() / Gauss->GetParameter(2)) * sqrt(powf(Vx_Y->getTH1F()->GetRMSError() / Vx_Y->getTH1F()->GetRMS(),2.) +
											     powf(Gauss->GetParError(2) / Gauss->GetParError(2),2.)), nLumiReset);
      Gauss->SetParameters(Vx_Z->getTH1()->GetMaximum(), Vx_Z->getTH1()->GetMean(), Vx_Z->getTH1()->GetRMS());
      Vx_Z->getTH1()->Fit("Gauss","LQN0");
      RMSsigXlumi->ShiftFillLast(Vx_Z->getTH1F()->GetRMS() / Gauss->GetParameter(2),
				 (Vx_Z->getTH1F()->GetRMS() / Gauss->GetParameter(2)) * sqrt(powf(Vx_Z->getTH1F()->GetRMSError() / Vx_Z->getTH1F()->GetRMS(),2.) +
											     powf(Gauss->GetParError(2) / Gauss->GetParError(2),2.)), nLumiReset);

      // Write the 8 beam spot parameters into a file
      endTimeOfFit = lumiBlock.endTime().value();
      endLumiOfFit = lumiBlock.luminosityBlock();
      vector<double> vals;

      if (dataFromFit == true)
	{
	  double pi = 3.141592653589793238;
	  unsigned int nParams = 9;

	  TFitterMinuit* Gauss3D = new TFitterMinuit(nParams);
	  Gauss3D->SetPrintLevel(0);
// 	  Gauss3D->SetStrategy(0);
	  Gauss3D->SetFCN(Gauss3DFunc);
      
	  // arg3 - first guess of parameter value
	  // arg4 – step of the parameter
	  Gauss3D->SetParameter(0,"var x ", Vx_X->getTH1()->GetRMS()*Vx_X->getTH1()->GetRMS(), 0.0005, 0, 0);
	  Gauss3D->SetParameter(1,"var y ", Vx_Y->getTH1()->GetRMS()*Vx_Y->getTH1()->GetRMS(), 0.0005, 0, 0);
	  Gauss3D->SetParameter(2,"var z ", Vx_Z->getTH1()->GetRMS()*Vx_Z->getTH1()->GetRMS(), 0.0005, 0, 0);
	  Gauss3D->SetParameter(3,"cov xy", 0.0, 0.0001, 0, 0);
	  Gauss3D->SetParameter(4,"cov yz", 0.0, 0.0001, 0, 0);
	  Gauss3D->SetParameter(5,"cov xz", 0.0, 0.0001, 0, 0);
	  Gauss3D->SetParameter(6,"mean x", Vx_X->getTH1()->GetMean(), 0.0005, 0, 0);
	  Gauss3D->SetParameter(7,"mean y", Vx_Y->getTH1()->GetMean(), 0.0005, 0, 0);
	  Gauss3D->SetParameter(8,"mean z", Vx_Z->getTH1()->GetMean(), 0.0005, 0, 0);

	  double arglist[2];
	  arglist[0] = 10000; // Number of function calls
	  arglist[1] = 1e-5;  // Tolerance on likelihood
	  goodData = Gauss3D->ExecuteCommand("MIGRAD",arglist,2);
// 	  cout << "goodData: " << goodData << endl;
// 	  cout << "sigma x: " << sqrt(Gauss3D->GetParameter(0)) << endl;
// 	  cout << "sigma y: " << sqrt(Gauss3D->GetParameter(1)) << endl;
// 	  cout << "sigma z: " << sqrt(Gauss3D->GetParameter(2)) << endl;
// 	  cout << "dxdz --> " << 90 - 180. / pi * acos(Gauss3D->GetParameter(5)/sqrt(Gauss3D->GetParameter(0)*Gauss3D->GetParameter(2))) << endl;
// 	  cout << "dydz --> " << 90 - 180. / pi * acos(Gauss3D->GetParameter(4)/sqrt(Gauss3D->GetParameter(1)*Gauss3D->GetParameter(2))) << endl;

	  dxdzlumi->ShiftFillLast(90 - 180. / pi * acos(Gauss3D->GetParameter(5)/sqrt(Gauss3D->GetParameter(0)*Gauss3D->GetParameter(2))), 0.0, nLumiReset);
	  dydzlumi->ShiftFillLast(90 - 180. / pi * acos(Gauss3D->GetParameter(4)/sqrt(Gauss3D->GetParameter(1)*Gauss3D->GetParameter(2))), 0.0, nLumiReset);

	  vals.push_back(Gauss3D->GetParameter(6));
	  vals.push_back(Gauss3D->GetParameter(7));
	  vals.push_back(Gauss3D->GetParameter(8));
	  vals.push_back(Gauss3D->GetParameter(0));
	  vals.push_back(Gauss3D->GetParameter(1));
	  vals.push_back(Gauss3D->GetParameter(2));
	  vals.push_back(90 - 180. / pi * acos(Gauss3D->GetParameter(5)/sqrt(Gauss3D->GetParameter(0)*Gauss3D->GetParameter(2))));
	  vals.push_back(90 - 180. / pi * acos(Gauss3D->GetParameter(4)/sqrt(Gauss3D->GetParameter(1)*Gauss3D->GetParameter(2))));

	  delete Gauss3D;

	  if (Vx_X->getTH1F()->GetEntries() < minNentries) goodData = -1;
	}
      else
	{
	  if (Vx_X->getTH1F()->GetEntries() >= minNentries) goodData = 0;
	  
	  vals.push_back(Vx_X->getTH1F()->GetMean());
	  vals.push_back(Vx_Y->getTH1F()->GetMean());
	  vals.push_back(Vx_Z->getTH1F()->GetMean());
	  vals.push_back(Vx_X->getTH1F()->GetRMS());
	  vals.push_back(Vx_Y->getTH1F()->GetRMS());
	  vals.push_back(Vx_Z->getTH1F()->GetRMS());
	  vals.push_back(0.);
	  vals.push_back(0.);
	}

      if (goodData == 0)
	{
	  writeToFile(&vals, fileName, beginTimeOfFit, endTimeOfFit, beginLumiOfFit, endLumiOfFit);

	  Vx_X->Reset();
	  Vx_Y->Reset();
	  Vx_Z->Reset();
	  
	  Vx_ZX->Reset();
	  Vx_ZY->Reset();
	  Vx_XY->Reset();
	  
	  Vx_ZX_profile->Reset();
	  Vx_ZY_profile->Reset();

	  xVxValues.clear();
	  yVxValues.clear();
	  zVxValues.clear();

	  reportSummary->Fill(0.5);
	  reportSummaryMap->Fill(0.5, 0.5, 0.5);
	}
      else
	{
	  reportSummary->Fill(0.25);
	  reportSummaryMap->Fill(0.5, 0.5, 0.25);
	}

      lumiCounter = 0;

      vals.clear();
      delete Gauss;
    }
  else if (nLumiReset == 0)
    {
      reportSummary->Fill(0.5);
      reportSummaryMap->Fill(0.5, 0.5, 0.5);

      lumiCounter = 0;
    }
}


void Vx3DHLTAnalyzer::beginJob()
{
  DQMStore* dbe = 0;
  dbe = Service<DQMStore>().operator->();
 
  if ( dbe ) 
    {
      dbe->setCurrentFolder("BeamPixel");

      Vx_X = dbe->book1D("Vertex_X", "Primary Vertex X Coordinate Distribution", (int)(xRange/xStep), -xRange/2., xRange/2.);
      Vx_Y = dbe->book1D("Vertex_Y", "Primary Vertex Y Coordinate Distribution", (int)(yRange/yStep), -yRange/2., yRange/2.);
      Vx_Z = dbe->book1D("Vertex_Z", "Primary Vertex Z Coordinate Distribution", (int)(zRange/zStep), -zRange/2., zRange/2.);

      Vx_X->setAxisTitle("Primary Vertices X [cm]",1);
      Vx_X->setAxisTitle("Entries [#]",2);
      Vx_Y->setAxisTitle("Primary Vertices Y [cm]",1);
      Vx_Y->setAxisTitle("Entries [#]",2);
      Vx_Z->setAxisTitle("Primary Vertices Z [cm]",1);
      Vx_Z->setAxisTitle("Entries [#]",2);
 
      mXlumi = dbe->book1D("muX vs lumi", "\\mu_{x} vs. Lumisection", 50, 0.5, 50.5);
      mYlumi = dbe->book1D("muY vs lumi", "\\mu_{y} vs. Lumisection", 50, 0.5, 50.5);
      mZlumi = dbe->book1D("muZ vs lumi", "\\mu_{z} vs. Lumisection", 50, 0.5, 50.5);

      mXlumi->setAxisTitle("Lumisection [#]",1);
      mXlumi->setAxisTitle("\\mu_{x} [cm]",2);
      mXlumi->getTH1()->SetOption("E1");
      mYlumi->setAxisTitle("Lumisection [#]",1);
      mYlumi->setAxisTitle("\\mu_{y} [cm]",2);
      mYlumi->getTH1()->SetOption("E1");
      mZlumi->setAxisTitle("Lumisection [#]",1);
      mZlumi->setAxisTitle("\\mu_{z} [cm]",2);
      mZlumi->getTH1()->SetOption("E1");

      sXlumi = dbe->book1D("sigmaX vs lumi", "\\sigma_{x} vs. Lumisection", 50, 0.5, 50.5);
      sYlumi = dbe->book1D("sigmaY vs lumi", "\\sigma_{y} vs. Lumisection", 50, 0.5, 50.5);
      sZlumi = dbe->book1D("sigmaZ vs lumi", "\\sigma_{z} vs. Lumisection", 50, 0.5, 50.5);

      sXlumi->setAxisTitle("Lumisection [#]",1);
      sXlumi->setAxisTitle("\\sigma_{x} [cm]",2);
      sXlumi->getTH1()->SetOption("E1");
      sYlumi->setAxisTitle("Lumisection [#]",1);
      sYlumi->setAxisTitle("\\sigma_{y} [cm]",2);
      sYlumi->getTH1()->SetOption("E1");
      sZlumi->setAxisTitle("Lumisection [#]",1);
      sZlumi->setAxisTitle("\\sigma_{z} [cm]",2);
      sZlumi->getTH1()->SetOption("E1");

      dxdzlumi = dbe->book1D("dxdz vs lumi", "dxdz vs. Lumisection", 50, 0.5, 50.5);
      dydzlumi = dbe->book1D("dydz vs lumi", "dydz vs. Lumisection", 50, 0.5, 50.5);

      dxdzlumi->setAxisTitle("Lumisection [#]",1);
      dxdzlumi->setAxisTitle("dxdz [deg]",2);
      dxdzlumi->getTH1()->SetOption("E1");
      dydzlumi->setAxisTitle("Lumisection [#]",1);
      dydzlumi->setAxisTitle("dydz [deg]",2);
      dydzlumi->getTH1()->SetOption("E1");

      RMSsigXlumi = dbe->book1D("RMS_over_sigma X vs lumi", "RMS_{x}/\\sigma_{x} vs. Lumisection", 50, 0.5, 50.5);
      RMSsigYlumi = dbe->book1D("RMS_over_sigma Y vs lumi", "RMS_{y}/\\sigma_{y} vs. Lumisection", 50, 0.5, 50.5);
      RMSsigZlumi = dbe->book1D("RMS_over_sigma Z vs lumi", "RMS_{z}/\\sigma_{z} vs. Lumisection", 50, 0.5, 50.5);

      RMSsigXlumi->setAxisTitle("Lumisection [#]",1);
      RMSsigXlumi->setAxisTitle("RMS_{x}/\\sigma_{x}",2);
      RMSsigXlumi->getTH1()->SetOption("E1");
      RMSsigYlumi->setAxisTitle("Lumisection [#]",1);
      RMSsigYlumi->setAxisTitle("RMS_{y}/\\sigma_{y}",2);
      RMSsigYlumi->getTH1()->SetOption("E1");
      RMSsigZlumi->setAxisTitle("Lumisection [#]",1);
      RMSsigZlumi->setAxisTitle("RMS_{z}/\\sigma_{z}",2);
      RMSsigZlumi->getTH1()->SetOption("E1");

      Vx_ZX = dbe->book2D("Vertex_ZX", "Primary Vertex ZX Coordinate Distributions", (int)(zRange/zStep/10.), -zRange/2., zRange/2., (int)(xRange/xStep/10.), -xRange/2., xRange/2.);
      Vx_ZY = dbe->book2D("Vertex_ZY", "Primary Vertex ZY Coordinate Distributions", (int)(zRange/zStep/10.), -zRange/2., zRange/2., (int)(yRange/yStep/10.), -yRange/2., yRange/2.);
      Vx_XY = dbe->book2D("Vertex_XY", "Primary Vertex XY Coordinate Distributions", (int)(xRange/xStep/10.), -xRange/2., xRange/2., (int)(yRange/yStep/10.), -yRange/2., yRange/2.);

      Vx_ZX->setAxisTitle("Primary Vertices Z [cm]",1);
      Vx_ZX->setAxisTitle("Primary Vertices X [cm]",2);
      Vx_ZX->setAxisTitle("Entries [#]",3);
      Vx_ZY->setAxisTitle("Primary Vertices Z [cm]",1);
      Vx_ZY->setAxisTitle("Primary Vertices Y [cm]",2);
      Vx_ZY->setAxisTitle("Entries [#]",3);
      Vx_XY->setAxisTitle("Primary Vertices X [cm]",1);
      Vx_XY->setAxisTitle("Primary Vertices Y [cm]",2);
      Vx_XY->setAxisTitle("Entries [#]",3);

      Vx_ZX_profile = dbe->bookProfile("ZX profile","ZX Profile", (int)(zRange/zStep/20.), -zRange/2., zRange/2., (int)(xRange/xStep/20.), -xRange/2., xRange/2., "");
      Vx_ZX_profile->setAxisTitle("Primary Vertices Z [cm]",1);
      Vx_ZX_profile->setAxisTitle("Primary Vertices X [cm]",2);

      Vx_ZY_profile = dbe->bookProfile("ZY profile","ZY Profile", (int)(zRange/zStep/20.), -zRange/2., zRange/2., (int)(yRange/yStep/20.), -yRange/2., yRange/2., "");
      Vx_ZY_profile->setAxisTitle("Primary Vertices Z [cm]",1);
      Vx_ZY_profile->setAxisTitle("Primary Vertices Y [cm]",2);

      dbe->setCurrentFolder("BeamPixel/EventInfo");
      reportSummary = dbe->bookFloat("reportSummary");
      reportSummary->Fill(0.);
      reportSummaryMap = dbe->book2D("reportSummaryMap","Beam Pixel Summary Map", 1, 0., 1., 1, 0., 1.);
      reportSummaryMap->Fill(0.5, 0.5, 0.);
      dbe->setCurrentFolder("BeamPixel/EventInfo/reportSummaryContents");
    }

  runNumber = 0;
  lumiCounter = 0;
  beginTimeOfFit = 0;
  endTimeOfFit = 0;
  beginLumiOfFit = 0;
  endLumiOfFit = 0;
}


void Vx3DHLTAnalyzer::endJob()
{
}


// Define this as a plug-in
DEFINE_FWK_MODULE(Vx3DHLTAnalyzer);
