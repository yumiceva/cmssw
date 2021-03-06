#define JPTRootAnalysisEta_cxx
#include "JPTRootAnalysisEta.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>

Float_t JPTRootAnalysisEta::deltaPhi(Float_t phi1, Float_t phi2)
{
  Float_t pi = 3.1415927;
  Float_t dphi = fabs(phi1 - phi2);
  if(dphi >= pi) dphi = 2. * pi - dphi; 
  return dphi;
}

Float_t JPTRootAnalysisEta::deltaEta(Float_t eta1, Float_t eta2)
{
  Float_t deta = fabs(eta1-eta2);
  return deta;
}

Float_t JPTRootAnalysisEta::deltaR(Float_t eta1, Float_t eta2,
		                Float_t phi1, Float_t phi2)
{
  Float_t dr = sqrt( deltaEta(eta1, eta2) * deltaEta(eta1, eta2) +
		     deltaPhi(phi1, phi2) * deltaPhi(phi1, phi2) );
  return dr;
}

void JPTRootAnalysisEta::setTDRStyle(Int_t ylog) {

  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");

// For the canvas:
  tdrStyle->SetCanvasBorderMode(0);
  tdrStyle->SetCanvasColor(kWhite);
  tdrStyle->SetCanvasDefH(600); //Height of canvas
  tdrStyle->SetCanvasDefW(600); //Width of canvas
  tdrStyle->SetCanvasDefX(0);   //POsition on screen
  tdrStyle->SetCanvasDefY(0);

// For the Pad:
  tdrStyle->SetPadBorderMode(0);
  // tdrStyle->SetPadBorderSize(Width_t size = 1);
  tdrStyle->SetPadColor(kWhite);
  tdrStyle->SetPadGridX(false);
  tdrStyle->SetPadGridY(false);
  tdrStyle->SetGridColor(0);
  tdrStyle->SetGridStyle(3);
  tdrStyle->SetGridWidth(1);

// For the frame:
  tdrStyle->SetFrameBorderMode(0);
  tdrStyle->SetFrameBorderSize(1);
  tdrStyle->SetFrameFillColor(0);
  tdrStyle->SetFrameFillStyle(0);
  tdrStyle->SetFrameLineColor(1);
  tdrStyle->SetFrameLineStyle(1);
  tdrStyle->SetFrameLineWidth(1);

// For the histo:
  // tdrStyle->SetHistFillColor(1);
  // tdrStyle->SetHistFillStyle(0);
  tdrStyle->SetHistLineColor(1);
  tdrStyle->SetHistLineStyle(0);
  tdrStyle->SetHistLineWidth(2);
  // tdrStyle->SetLegoInnerR(Float_t rad = 0.5);
  // tdrStyle->SetNumberContours(Int_t number = 20);

  tdrStyle->SetEndErrorSize(4);
  //  tdrStyle->SetErrorMarker(20);
  //  tdrStyle->SetErrorX(0.);
  
  tdrStyle->SetMarkerStyle(20);

//For the fit/function:
  tdrStyle->SetOptFit(1);
  tdrStyle->SetFitFormat("5.4g");
  tdrStyle->SetFuncColor(1);
  tdrStyle->SetFuncStyle(1);
  tdrStyle->SetFuncWidth(1);

//For the date:
  tdrStyle->SetOptDate(0);
  // tdrStyle->SetDateX(Float_t x = 0.01);
  // tdrStyle->SetDateY(Float_t y = 0.01);

// For the statistics box:
  tdrStyle->SetOptFile(0);
  tdrStyle->SetOptStat(0); // To display the mean and RMS:   SetOptStat("mr");
  tdrStyle->SetStatColor(kWhite);
  tdrStyle->SetStatFont(42);
  tdrStyle->SetStatFontSize(0.025);
  tdrStyle->SetStatTextColor(1);
  tdrStyle->SetStatFormat("6.4g");
  tdrStyle->SetStatBorderSize(1);
  tdrStyle->SetStatH(0.1);
  tdrStyle->SetStatW(0.15);
  // tdrStyle->SetStatStyle(Style_t style = 1001);
  // tdrStyle->SetStatX(Float_t x = 0);
  // tdrStyle->SetStatY(Float_t y = 0);

// Margins:
  tdrStyle->SetPadTopMargin(0.05);
  tdrStyle->SetPadBottomMargin(0.13);
  tdrStyle->SetPadLeftMargin(0.13);
  tdrStyle->SetPadRightMargin(0.05);

// For the Global title:

  tdrStyle->SetOptTitle(0);
  tdrStyle->SetTitleFont(42);
  tdrStyle->SetTitleColor(1);
  tdrStyle->SetTitleTextColor(1);
  tdrStyle->SetTitleFillColor(10);
  tdrStyle->SetTitleFontSize(0.05);
  // tdrStyle->SetTitleH(0); // Set the height of the title box
  // tdrStyle->SetTitleW(0); // Set the width of the title box
  // tdrStyle->SetTitleX(0); // Set the position of the title box
  // tdrStyle->SetTitleY(0.985); // Set the position of the title box
  // tdrStyle->SetTitleStyle(Style_t style = 1001);
  // tdrStyle->SetTitleBorderSize(2);

// For the axis titles:

  tdrStyle->SetTitleColor(1, "XYZ");
  tdrStyle->SetTitleFont(42, "XYZ");
  tdrStyle->SetTitleSize(0.06, "XYZ");
  // tdrStyle->SetTitleXSize(Float_t size = 0.02); // Another way to set the size?
  // tdrStyle->SetTitleYSize(Float_t size = 0.02);
  tdrStyle->SetTitleXOffset(0.9);
  tdrStyle->SetTitleYOffset(1.05);
  // tdrStyle->SetTitleOffset(1.1, "Y"); // Another way to set the Offset

// For the axis labels:

  tdrStyle->SetLabelColor(1, "XYZ");
  tdrStyle->SetLabelFont(42, "XYZ");
  tdrStyle->SetLabelOffset(0.007, "XYZ");
  tdrStyle->SetLabelSize(0.05, "XYZ");

// For the axis:

  tdrStyle->SetAxisColor(1, "XYZ");
  tdrStyle->SetStripDecimals(kTRUE);
  tdrStyle->SetTickLength(0.03, "XYZ");
  tdrStyle->SetNdivisions(510, "XYZ");
  tdrStyle->SetPadTickX(1);  // To get tick marks on the opposite side of the frame
  tdrStyle->SetPadTickY(1);

// Change for log plots:
  tdrStyle->SetOptLogx(0);
  tdrStyle->SetOptLogy(ylog);
  tdrStyle->SetOptLogz(0);

// Postscript options:

//  tdrStyle->SetPaperSize(7.5,7.5);

  tdrStyle->SetPaperSize(15.,15.);

//  tdrStyle->SetPaperSize(20.,20.);

  // tdrStyle->SetLineScalePS(Float_t scale = 3);
  // tdrStyle->SetLineStyleString(Int_t i, const char* text);
  // tdrStyle->SetHeaderPS(const char* header);
  // tdrStyle->SetTitlePS(const char* pstitle);

  // tdrStyle->SetBarOffset(Float_t baroff = 0.5);
  // tdrStyle->SetBarWidth(Float_t barwidth = 0.5);
  // tdrStyle->SetPaintTextFormat(const char* format = "g");
  // tdrStyle->SetPalette(Int_t ncolors = 0, Int_t* colors = 0);
  // tdrStyle->SetTimeOffset(Double_t toffset);
  // tdrStyle->SetHistMinimumZero(kTRUE);

  tdrStyle->cd();
}

void JPTRootAnalysisEta::Loop()
{
//   In a ROOT session, you can do:
//      Root > .L JPTRootAnalysisEta.C
//      Root > JPTRootAnalysisEta t
//      Root > t.GetEntry(12); // Fill t data members with entry number 12
//      Root > t.Show();       // Show values of entry 12
//      Root > t.Show(16);     // Read and show values of entry 16
//      Root > t.Loop();       // Loop on all entries
//

//     This is the loop skeleton where:
//    jentry is the global entry number in the chain
//    ientry is the entry number in the current Tree
//  Note that the argument to GetEntry must be:
//    jentry for TChain::GetEntry
//    ientry for TTree::GetEntry and TBranch::GetEntry
//
//       To read only selected branches, Insert statements like:
// METHOD1:
//    fChain->SetBranchStatus("*",0);  // disable all branches
//    fChain->SetBranchStatus("branchname",1);  // activate branchname
// METHOD2: replace line
//    fChain->GetEntry(jentry);       //read all branches
//by  b_branchname->GetEntry(ientry); //read only this branch
   if (fChain == 0) return;

   // resolution plot
   const Int_t nbins = 10;
   const Int_t nh = 10;

   const Int_t nbx = nbins+1;
   const Float_t xbins[nbx]={0.0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2.0};
   // scale plot
   TH1F* hScaleRaw = new TH1F("hScaleRaw", "ScaleRaw", nbins, xbins);
   TH1F* hScaleJPTInCone = new TH1F("hScaleJPTInCone", "ScaleJPTInCone", nbins, xbins);
   TH1F* hScaleZSP = new TH1F("hScaleZSP", "ScaleZSP", nbins, xbins);
   TH1F* hScaleJPT = new TH1F("hScaleJPT", "ScaleJPT", nbins, xbins);

   // histogramms

   TH1F* hEtRaw[nh];
   TH1F* hEtJPTInCone[nh]; 
   TH1F* hEtZSP[nh];
   TH1F* hEtJPT[nh];

   const char* namesEtRaw[nh] = {"hEtRaw1","hEtRaw2","hEtRaw3","hEtRaw4","hEtRaw5","hEtRaw6","hEtRaw7","hEtRaw8","hEtRaw9","hEtRaw10",};
   const char* titleEtRaw[nh] = {"EtRaw1","EtRaw2","EtRaw2","EtRaw4","EtRaw5","EtRaw6","EtRaw7","EtRaw8","EtRaw9","EtRaw10"};

   const char* namesEtJPTInCone[nh] = {"hEtJPTInCone1","hEtJPTInCone2","hEtJPTInCone3","hEtJPTInCone4","hEtJPTInCone5","hEtJPTInCone6","hEtJPTInCone7","hEtJPTInCone8","hEtJPTInCone9","hEtJPTInCone10"};
   const char* titleEtJPTInCone[nh] = {"EtJPTInCone1","EtJPTInCone2","EtJPTInCone3","EtJPTInCone4","EtJPTInCone5","EtJPTInCone6","EtJPTInCone7","EtJPTInCone8","EtJPTInCone9","EtJPTInCone10"};

   const char* namesEtZSP[nh] = {"hEtZSP1","hEtZSP2","hEtZSP3","hEtZSP4","hEtZSP5","hEtZSP6","hEtZSP7","hEtZSP8","hEtZSP9","hEtZSP10"};
   const char* titleEtZSP[nh] = {"EtZSP1","EtZSP2","EtZSP3","EtZSP4","EtZSP5","EtZSP6","EtZSP7","EtZSP8","EtZSP9","EtZSP10"};

   const char* namesEtJPT[nh] = {"hEtJPT1","hEtJPT2","hEtJPT3","hEtJPT4","hEtJPT5","hEtJPT6","hEtJPT7","hEtJPT8","hEtJPT9","hEtJPT10"};
   const char* titleEtJPT[nh] = {"EtJPT1","EtJPT2","EtJPT3","EtJPT4","EtJPT5","EtJPT6","EtJPT7","EtJPT8","EtJPT9","EtJPT10"};

   for(Int_t ih=0; ih < nh; ih++) { 
     cout <<" ih = " << ih <<" namesEtRaw[nh] = " << namesEtRaw[ih] <<" titleEtRaw[ih]= " << titleEtRaw[ih] << endl; 
     hEtRaw[ih]  = new TH1F(namesEtRaw[ih], titleEtRaw[ih], 60, 0., 3.);
     hEtJPTInCone[ih]  = new TH1F(namesEtJPTInCone[ih], titleEtJPTInCone[ih], 60, 0., 3.);
     hEtZSP[ih]  = new TH1F(namesEtZSP[ih], titleEtZSP[ih], 60, 0., 3.);
     hEtJPT[ih]  = new TH1F(namesEtJPT[ih], titleEtJPT[ih], 60, 0., 3.);
   }

   TH1F * hEtGen  = new TH1F( "hEtGen", "EtGen", 20, 0., 200.);
   TH1F * hEtaGen = new TH1F( "hEtaGen", "EtaGen", 16, 0., 2.1);
   TH1F * hDR     = new TH1F( "hDR", "DR", 100, 0., 10.);

   // separate jets by DR
   Float_t DR;
   Float_t DRcut = 2.0;
   Float_t etaMin = 0.0;
   Float_t etaMax = 2.2;

   Long64_t nentries = fChain->GetEntriesFast();

   Long64_t nbytes = 0, nb = 0;
   for (Long64_t jentry=0; jentry<nentries;jentry++) {
      Long64_t ientry = LoadTree(jentry);
      if (ientry < 0) break;
      nb = fChain->GetEntry(jentry);   nbytes += nb;
      // if (Cut(ientry) < 0) continue;
      for(Int_t ih = 0; ih < nh; ih++) {
	if(fabs(EtaGen1) >= xbins[ih] && fabs(EtaGen1) < xbins[ih+1] 
	   && DRMAXgjet1 < 0.3 && EtGen1 > 30 && EtGen1 < 150
	   && fabs(EtaGen1) > etaMin && fabs(EtaGen1) <= etaMax) {
	  if(EtRaw1/EtGen1 > 0.1) {


	    if(EtGen2 < 20.) {
	      hEtRaw[ih]->Fill(EtRaw1/EtGen1);
	      hEtZSP[ih]->Fill(EtZSP1/EtGen1);
	      hEtJPT[ih]->Fill(EtJPT1/EtGen1);
	    } 

	    if(EtGen2 > 20.) {
	      DR = deltaR(EtaGen1, EtaGen2, PhiGen1, PhiGen2);
	      if(DR > DRcut) {
		hDR->Fill(DR);
		hEtRaw[ih]->Fill(EtRaw1/EtGen1);
		hEtZSP[ih]->Fill(EtZSP1/EtGen1);
		hEtJPT[ih]->Fill(EtJPT1/EtGen1);
		hEtGen->Fill(EtGen1);
		hEtaGen->Fill(EtaGen1);
	      }
	    }
	  }
	}
	if(fabs(EtaGen2) >= xbins[ih] && fabs(EtaGen2) < xbins[ih+1] 
	   && DRMAXgjet2 < 0.3 && EtGen2 > 30 && EtGen2 < 150
	   && fabs(EtaGen2) > etaMin && fabs(EtaGen2) <= etaMax) {
	  if(EtRaw2/EtGen2 > 0.1) {

	    DR = deltaR(EtaGen1, EtaGen2, PhiGen1, PhiGen2);
	    if(DR > DRcut) {
	      hDR->Fill(DR);
	      hEtRaw[ih]->Fill(EtRaw2/EtGen2);
	      hEtZSP[ih]->Fill(EtZSP2/EtGen2);
	      hEtJPT[ih]->Fill(EtJPT2/EtGen2);
	      hEtGen->Fill(EtGen2);
	      hEtaGen->Fill(EtaGen2);
	    }
	  }
	}
      }
   }

   setTDRStyle(0);
   gStyle->SetOptFit();

   // plot histos
   /*
   TCanvas* cScape = new TCanvas("X","Y",1);
   // raw
   hEtRaw[0]->GetXaxis()->SetTitle("Et reco / Et gen");
   hEtRaw[0]->GetYaxis()->SetTitle("Nev");
   hEtRaw[0]->SetMaximum(100.);
   hEtRaw[0]->Draw("hist");
   //
   // MC
   hEtJPTInCone[0]->SetLineStyle(2);
   hEtJPTInCone[0]->SetLineWidth(1);
   hEtJPTInCone[0]->Draw("same");
   // ZSP
   hEtZSP[0]->SetLineStyle(3);
   hEtZSP[0]->SetLineWidth(4);
   hEtZSP[0]->Draw("same");
   // JPT
   hEtJPT[0]->SetLineStyle(4);
   hEtJPT[0]->SetLineWidth(4);
   hEtJPT[0]->Draw("same");
  // legend
   TLatex *t = new TLatex();
   t->SetTextSize(0.042);
   TLegend *leg = new TLegend(0.45,0.4,0.85,0.8,NULL,"brNDC");
   leg->SetFillColor(10);
   leg->AddEntry(hEtRaw[0],"Raw jets; #mu = 0.60, #sigma = 0.16","L");
   leg->AddEntry(hEtJPTInCone[0],"MC corr; #mu = 1.05, #sigma = 0.17","L");
   leg->AddEntry(hEtZSP[0],"ZSP corr; #mu = 0.73, #sigma = 0.15","L");
   leg->AddEntry(hEtJPT[0],"JPT corr; #mu = 0.89, #sigma = 0.11","L");
   leg->Draw();  
   t->DrawLatex(0.1,450.,"CMSSW160, Z+jets. |#eta ^{jet}|< 1.4, E_{T}^{gen.jet}>20 GeV");
   */

   //fit JPT
   TCanvas* c1 = new TCanvas("X","Y",1);
   c1->Divide(2,2);
   for(Int_t ih = 0; ih < nh; ++ih) {
   //   for(Int_t ih = 1; ih < 2; ++ih) {

     // gen energy bin center
     TAxis* xaxisRes = hScaleJPT->GetXaxis();
     Double_t EbinCenter = xaxisRes->GetBinCenter(ih+1);
     cout <<" bin center = " << EbinCenter << endl;

     c1->cd(1);
     // JPT
     // get bin with max content
     Int_t binMax = hEtJPT[ih]->GetMaximumBin();
     TAxis* xaxis = hEtJPT[ih]->GetXaxis();
     Double_t binCenter = xaxis->GetBinCenter(binMax);
     Double_t rms = hEtJPT[ih]->GetRMS();
     Double_t rFitMin = binCenter - 2.0 * rms; 
     Double_t rFitMax = binCenter + 2.0 * rms;
     hEtJPT[ih]->Fit("gaus","","",rFitMin,rFitMax);
     TF1 *fit = hEtJPT[ih]->GetFunction("gaus"); 
     gStyle->SetOptFit();
     Double_t mean  = fit->GetParameter(1);
     Double_t meanErr  = fit->GetParError(1);
     Double_t sigma = fit->GetParameter(2);
     Double_t sigmaErr = fit->GetParError(2);
     Float_t resolution = sigma/mean;
     Float_t resolutionErr = resolution * sqrt((meanErr/mean)*(meanErr/mean) + (sigmaErr/sigma)*(sigmaErr/sigma));
     hScaleJPT->Fill(EbinCenter,mean);
     hScaleJPT->SetBinError(ih+1,meanErr);    

     c1->cd(2);
     // ZSP
     // get bin with max content
     binMax = hEtZSP[ih]->GetMaximumBin();
     xaxis = hEtZSP[ih]->GetXaxis();
     binCenter = xaxis->GetBinCenter(binMax);
     rms = hEtZSP[ih]->GetRMS();
     rFitMin = binCenter - 2.0 * rms; 
     rFitMax = binCenter + 2.0 * rms;
     hEtZSP[ih]->Fit("gaus","","",rFitMin,rFitMax);
     fit = hEtZSP[ih]->GetFunction("gaus"); 
     mean  = fit->GetParameter(1);
     meanErr  = fit->GetParError(1);
     sigma = fit->GetParameter(2);
     sigmaErr = fit->GetParError(2);
     resolution = sigma/mean;
     resolutionErr = resolution * sqrt((meanErr/mean)*(meanErr/mean) + (sigmaErr/sigma)*(sigmaErr/sigma));
     hScaleZSP->Fill(EbinCenter,mean);
     hScaleZSP->SetBinError(ih+1,meanErr);    

     c1->cd(3);
     // JPTInCone
     // get bin with max content
     binMax = hEtJPTInCone[ih]->GetMaximumBin();
     xaxis = hEtJPTInCone[ih]->GetXaxis();
     binCenter = xaxis->GetBinCenter(binMax);
     rms = hEtJPTInCone[ih]->GetRMS();
     rFitMin = binCenter - 2.0 * rms; 
     rFitMax = binCenter + 2.0 * rms;
     hEtJPTInCone[ih]->Fit("gaus","","",rFitMin,rFitMax);
     fit = hEtJPTInCone[ih]->GetFunction("gaus"); 
     mean  = fit->GetParameter(1);
     meanErr  = fit->GetParError(1);
     sigma = fit->GetParameter(2);
     sigmaErr = fit->GetParError(2);
     resolution = sigma/mean;
     resolutionErr = resolution * sqrt((meanErr/mean)*(meanErr/mean) + (sigmaErr/sigma)*(sigmaErr/sigma));
     hScaleJPTInCone->Fill(EbinCenter,mean);
     hScaleJPTInCone->SetBinError(ih+1,meanErr);    
 
     c1->cd(4);
     // RAW
     // get bin with max content
     binMax = hEtRaw[ih]->GetMaximumBin();
     xaxis = hEtRaw[ih]->GetXaxis();
     binCenter = xaxis->GetBinCenter(binMax);
     rms = hEtRaw[ih]->GetRMS();
     rFitMin = binCenter - 2.0 * rms; 
     rFitMax = binCenter + 2.0 * rms;
     hEtRaw[ih]->Fit("gaus","","",rFitMin,rFitMax);
     fit = hEtRaw[ih]->GetFunction("gaus"); 
     mean  = fit->GetParameter(1);
     meanErr  = fit->GetParError(1);
     sigma = fit->GetParameter(2);
     sigmaErr = fit->GetParError(2);
     resolution = sigma/mean;
     resolutionErr = resolution * sqrt((meanErr/mean)*(meanErr/mean) + (sigmaErr/sigma)*(sigmaErr/sigma));
     hScaleRaw->Fill(EbinCenter,mean);
     hScaleRaw->SetBinError(ih+1,meanErr);    
   }

   // save histo on disk

   //   TFile efile("CTF.root","recreate");

   TFile efile("testVSeta.root","recreate");
   hScaleRaw->Write();
   hScaleJPT->Write();
   efile.Close();

   TCanvas* c2 = new TCanvas("X","Y",1);

   hScaleJPT->GetXaxis()->SetTitle("#eta");
   hScaleJPT->GetYaxis()->SetTitle("E_{T}^{reco}/E_{T}^{gen}");

   hScaleJPT->SetMaximum(1.2);
   hScaleJPT->SetMinimum(0.2);
   hScaleJPT->SetMarkerStyle(21);
   hScaleJPT->SetMarkerSize(1.2);
   hScaleJPT->Draw("histPE1");

   //   hScaleJPTInCone->SetMarkerSize(1.0);
   //   hScaleJPTInCone->SetMarkerStyle(24);
   //   hScaleJPTInCone->Draw("samePE1");

   hScaleZSP->SetMarkerSize(1.0);
   hScaleZSP->SetMarkerStyle(24);
   hScaleZSP->Draw("samePE1");

   hScaleRaw->SetMarkerSize(1.5);
   hScaleRaw->SetMarkerStyle(22);
   hScaleRaw->Draw("samePE1");

   TLatex *t = new TLatex();
   t->SetTextSize(0.042);
   TLegend *leg = new TLegend(0.5,0.15,0.9,0.30,NULL,"brNDC");
   leg->SetFillColor(10);
   leg->AddEntry(hScaleRaw,"Raw calo jets","P");
   leg->AddEntry(hScaleZSP,"ZSP corr","P");
   leg->AddEntry(hScaleJPT,"ZSP+JPT corr","P");
   leg->Draw();  
   t->DrawLatex(0.4,1.1,"CMSSW219, RelVal QCD 80-120 GeV");

   c2->SaveAs("ScaleJPTEta219.gif");
}
