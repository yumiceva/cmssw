#include "TauAnalysis/FittingTools/plugins/PartonLuminosityAnalyzer.h"

#include "FWCore/MessageLogger/interface/MessageLogger.h"

#include "LHAPDF/LHAPDF.h"

namespace LHAPDF {
  void initPDFSet(int nset, const std::string& filename, int member);
}

#include <TCanvas.h>
#include <TH1.h>
#include <TMath.h>
#include <TLegend.h>
#include <TPaveText.h>

// global defaults
const int defaultCanvasSizeX = 800;
const int defaultCanvasSizeY = 600;

const std::string scale_linear = "linear";
const std::string scale_log = "log";
const std::string defaultXscale = scale_linear;
const std::string defaultYscale = scale_linear;

const double undefinedYmin = -1.;
const double defaultYmin_linear = 0.;
const double defaultYmin_log = 1.e-1;
const double undefinedYmax = -1.;
const double defaultYmax_linear = 1.e+2;
const double defaultYmax_log = 1.e+2;

const double massStepSize = 1.;

const double epsilon = 1.e-3;

PartonLuminosityAnalyzer::PartonLuminosityAnalyzer(const edm::ParameterSet& cfg)
  : integrator_(0),
    integrand_(0)
{
  pdfSet_ = cfg.getParameter<std::string>("pdfSet");
  
  sqrtS_TeVatron_ = cfg.getParameter<double>("sqrtS_TeVatron");
  sqrtS_LHC_ = cfg.getParameter<double>("sqrtS_LHC");

  massMin_ = cfg.getParameter<double>("massMin");
  massMax_ = cfg.getParameter<double>("massMax");

//--- configure canvas size
  canvasSizeX_ = ( cfg.exists("canvasSizeX") ) ? cfg.getParameter<int>("canvasSizeX") : defaultCanvasSizeX;
  canvasSizeY_ = ( cfg.exists("canvasSizeY") ) ? cfg.getParameter<int>("canvasSizeY") : defaultCanvasSizeY;
 
  xScale_ = cfg.exists("xScale") ? cfg.getParameter<std::string>("xScale") : defaultXscale;
  yScale_ = cfg.exists("yScale") ? cfg.getParameter<std::string>("yScale") : defaultYscale;

  yMin_ = cfg.exists("yMin") ? cfg.getParameter<double>("yMin") : undefinedYmin;
  yMax_ = cfg.exists("yMax") ? cfg.getParameter<double>("yMax") : undefinedYmax;

//--- configure output files
  outputFilePath_ = ( cfg.exists("outputFilePath") ) ? cfg.getParameter<std::string>("outputFilePath") : "";
  if ( outputFilePath_.rbegin() != outputFilePath_.rend() ) {
    if ( (*outputFilePath_.rbegin()) == '/' ) outputFilePath_.erase(outputFilePath_.length() - 1);
  }

  outputFileName_ = ( cfg.exists("outputFileName") ) ? cfg.getParameter<std::string>("outputFileName") : "";
}

PartonLuminosityAnalyzer::~PartonLuminosityAnalyzer()
{
  delete integrator_;
  delete integrand_;
}

TGraph* PartonLuminosityAnalyzer::makeGraph(int flavor1, int flavor2)
{
  std::vector<int> flavors1;
  flavors1.push_back(flavor1);

  std::vector<int> flavors2;
  flavors2.push_back(flavor2);

  return makeGraph(flavors1, flavors2);
}

double compTau(double mass, double sqrtS)
{
  double sqrtTau = mass/sqrtS;
  return sqrtTau*sqrtTau;
}

TGraph* PartonLuminosityAnalyzer::makeGraph(const std::vector<int>& flavors1, const std::vector<int>& flavors2)
{
  assert(flavors1.size() == flavors2.size());

  size_t numFlavors = flavors1.size();

  TGraph* graph = new TGraph();

  unsigned iPoint = 0;
  for ( double mass = massMin_; mass <= massMax_; mass += massStepSize ) {
    integrand_->SetParameterSqrtS(sqrtS_TeVatron_);
    integrand_->SetParameterQ(mass);
    double tau_TeVatron = compTau(mass, sqrtS_TeVatron_);
    integrand_->SetParameterTau(tau_TeVatron);
    double partonLuminosity_TeVatron = 0.;
    for ( size_t iFlavor = 0; iFlavor < numFlavors; ++iFlavor ) {
      integrand_->SetParameterFlavor1(+flavors1[iFlavor]);
      if ( LHAPDF::GLUON )
	integrand_->SetParameterFlavor2(+flavors2[iFlavor]);
      else
	integrand_->SetParameterFlavor2(-flavors2[iFlavor]); // TeVatron is ppbar collider, so need to use parton distribution function 
                                                             // for anti-flavor, since pdf(quark|proton) = pdf(anti-quark|anti-proton)
      integrator_->SetFunction(*integrand_);
      partonLuminosity_TeVatron += integrator_->Integral(tau_TeVatron, 1.); 
    }
    std::cout << " TeVatron(m = " << mass << ") = " << partonLuminosity_TeVatron << std::endl;

    integrand_->SetParameterSqrtS(sqrtS_LHC_);
    integrand_->SetParameterQ(mass);
    double tau_LHC = compTau(mass, sqrtS_LHC_);
    integrand_->SetParameterTau(tau_LHC);
    double partonLuminosity_LHC = 0.;
    for ( size_t iFlavor = 0; iFlavor < numFlavors; ++iFlavor ) {
      integrand_->SetParameterFlavor1(+flavors1[iFlavor]);
      integrand_->SetParameterFlavor2(+flavors2[iFlavor]);
      integrator_->SetFunction(*integrand_);
      partonLuminosity_LHC += integrator_->Integral(tau_LHC, 1.); 
    }
    std::cout << " LHC(m = " << mass << ") = " << partonLuminosity_LHC << std::endl;

    graph->SetPoint(iPoint, mass, partonLuminosity_LHC/partonLuminosity_TeVatron);
    ++iPoint;
  }

  return graph;
}

void PartonLuminosityAnalyzer::endJob()
{
  LHAPDF::initPDFSet(0, pdfSet_, 0); // use "best-fit" PDF values

  integrand_ = new PartonLuminosityIntegrand();
  
  integrator_ = new ROOT::Math::Integrator(*integrand_);
  integrator_->SetRelTolerance(epsilon);
  integrator_->SetFunction(*integrand_);

  TCanvas* canvas = new TCanvas("canvas", "canvas", canvasSizeX_, canvasSizeY_);
  canvas->SetFillColor(10);
  canvas->SetFrameFillColor(10);

  if ( xScale_ == scale_log ) canvas->SetLogx();
  if ( yScale_ == scale_log ) canvas->SetLogy();

  double yMin = yMin_;
  if ( yMin == undefinedYmin ) yMin = ( canvas->GetLogy() ) ? defaultYmin_log : defaultYmin_linear;
  double yMax = yMax_;
  if ( yMax == undefinedYmax ) yMax = ( canvas->GetLogy() ) ? defaultYmax_log : defaultYmax_linear;

  TH1* dummyHistogram = new TH1F("dummyHistogram", "dummyHistogram", TMath::Nint((massMax_ - massMin_)/10.), massMin_, massMax_); 
  dummyHistogram->SetTitle("Parton Luminosity LHC @ 7 TeV vs. TeVatron");
  dummyHistogram->SetStats(false);
  dummyHistogram->SetMinimum(yMin);
  dummyHistogram->SetMaximum(yMax);
  dummyHistogram->Draw();

  std::cout << "gluon gluon parton luminosities:" << std::endl;
  TGraph* graphGluonGluon = makeGraph(LHAPDF::GLUON, LHAPDF::GLUON);
  graphGluonGluon->SetLineColor(9);
  graphGluonGluon->SetLineWidth(2);
  graphGluonGluon->Draw("C");

  std::cout << "b bbar parton luminosities:" << std::endl;
  TGraph* graphBBbar = makeGraph(+LHAPDF::BOTTOM, -LHAPDF::BOTTOM);
  graphBBbar->SetLineColor(6);
  graphBBbar->SetLineWidth(2);
  graphBBbar->Draw("C");

  std::cout << "u ubar + d dbar parton luminosities:" << std::endl;
  std::vector<int> flavor1UUbarDDbar;
  flavor1UUbarDDbar.push_back(+LHAPDF::UP);
  flavor1UUbarDDbar.push_back(+LHAPDF::DOWN);
  std::vector<int> flavor2UUbarDDbar;
  flavor2UUbarDDbar.push_back(-LHAPDF::UP);
  flavor2UUbarDDbar.push_back(-LHAPDF::DOWN);
  TGraph* graphUUbarDDbar = makeGraph(flavor1UUbarDDbar, flavor2UUbarDDbar);
  graphUUbarDDbar->SetLineColor(8);
  graphUUbarDDbar->SetLineWidth(2);
  graphUUbarDDbar->Draw("C");

  TLegend* legend = new TLegend(0.14, 0.71, 0.39, 0.89, "", "brNDC"); 
  legend->SetBorderSize(0);
  legend->SetFillColor(0);
  legend->AddEntry(graphGluonGluon, "gg", "l");
  legend->AddEntry(graphBBbar, "b#bar{b}", "l");
  legend->AddEntry(graphUUbarDDbar, "u#bar{u} + d#bar{d}", "l");
  legend->Draw();

  size_t pdfSetLabel_pos0 = pdfSet_.find_last_of("/");
  if ( pdfSetLabel_pos0 != std::string::npos ) 
    pdfSetLabel_pos0 += 1;
  else 
    pdfSetLabel_pos0  = 0;
  size_t pdfSetLabel_pos1 = pdfSet_.find_last_of(".");
  TPaveText* pdfSetLabel = new TPaveText(0.71, 0.14, 0.89, 0.21, "brNDC"); 
  pdfSetLabel->SetBorderSize(0);
  pdfSetLabel->SetFillColor(0);
  pdfSetLabel->AddText(std::string(pdfSet_, pdfSetLabel_pos0, pdfSetLabel_pos1 - pdfSetLabel_pos0).data());
  pdfSetLabel->Draw();
  
  canvas->Update();

  std::string fullFileName = ( outputFilePath_ != "" ) ? 
    std::string(outputFilePath_).append("/").append(outputFileName_) : outputFileName_;
  canvas->Print(fullFileName.data());

  delete canvas;

  delete graphGluonGluon;
  delete graphBBbar;
  delete graphUUbarDDbar;

  delete legend;

  delete pdfSetLabel;
}

#include "FWCore/Framework/interface/MakerMacros.h"

DEFINE_FWK_MODULE(PartonLuminosityAnalyzer);
