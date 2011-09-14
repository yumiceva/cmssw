//----------Author's Names: B.Fabbro, FX Gentit DSM/IRFU/SPP CEA-Saclay
//----------Copyright: Those valid for CEA sofware
//----------Modified: 01/04/2010
// ROOT include files

// user's include files

#include "CalibCalorimetry/EcalCorrelatedNoiseAnalysisAlgos/interface/TEcnaRun.h"

R__EXTERN TEcnaRootFile *gCnaRootFile;

ClassImp(TEcnaRun)
//___________________________________________________________________________
//
// TEcnaRun + ECNA (Ecal Correlated Noise Analysis) instructions for use 
//           in the framework of CMSSW.
//
//==============> INTRODUCTION
//
//    The present documentation contains:
//
//    [1] a brief description of the ECNA package with instructions for use
//        in the framework of the CMS Software
//
//    [2] the documentation for the class TEcnaRun
//
//
//==[1]=====================================================================================
//
//
//         DOCUMENTATION FOR THE INTERFACE: ECNA package / CMSSW / SCRAM
//
//
//==========================================================================================
//
//  ECNA consists in 2 packages named: EcalCorrelatedNoiseAnalysisModules and
//  EcalCorrelatedNoiseAnalysisAlgos.
//
//  The directory tree is the following:
//
//      <local path>/CMSSW_a_b_c/src/----CalibCalorimetry/---EcalCorrelatedNoiseAnalysisModules/BuildFile
//                              |   |                    |                                     |---interface/
//                              |   |                    |                                     |---src/
//                                  |                    |                                     |---data/
//                                  |                    |
//                                  |                    |---EcalCorrelatedNoiseAnalysisAlgos/BuildFile
//                                  |                    |                                   |---interface/
//                                  |                    |                                   |---src/
//                                  |                    |                                   |---test/
//                                  |                    |
//                                  |                    |
//                                  |                    \--- <other packages of CalibCalorimetry> 
//                                  |
//                                  \----<other subsystems...>
//
//
//    The package EcalCorrelatedNoiseAnalysisModules contains one standard analyzer
//    (EcnaAnalyzer). The user can edit its own analyzer.
//    A detailed description is given here after in the class TEcnaRun documentation.
//    An analyzer skeleton can be obtained by means of the
//    SkeletonCodeGenerator "mkedanlzr" (see the CMSSW Framework/Edm web page).
//
//    The package EcalCorrelatedNoiseAnalysisAlgos contains the basic classes of ECNA 
//    (in src and interface) and standalone executables (in directory test).
//
// 
//==[2]======================================================================================
//
//
//                         CLASS TEcnaRun DOCUMENTATION
//
//
//===========================================================================================
//TEcnaRun.
//
//
//
// Brief and general description
// -----------------------------
//
//   This class allows the user to calculate pedestals, noises,
//   correlations and other quantities of interest for correlated
//   noise studies on the CMS/ECAL (EB and EE).
//
//   Three main operations are performed by the class TEcnaRun. Each of them is
//   associated with a specific method of the analyzer EcnaAnalyzer:
//
//    (1) Initialization and calls to "preparation methods" of the CNA.
//        This task is done in the constructor of the analyzer:
//        EcnaAnalyzer::EcnaAnalyzer(const edm::ParameterSet& pSet)
//
//    (2) Building of the event distributions (distributions of the sample ADC
//        values for each sample, each channel, etc...)
//        This task is done in the method "analyze" of the analyzer:
//        EcnaAnalyzer::analyze(const edm::Event& iEvent, const edm::EventSetup& iSetup)
//
//    (3) Calculation of the different quantities (correlations, pedestals, noises, etc...)
//        from the distributions obtained in (2) and writing of these quantities
//        in results ROOT files and also in ASCII files.
//        This task is done in the destructor of the analyzer:
//        EcnaAnalyzer::~EcnaAnalyzer()
//   
//
// Use of the class TEcnaRun by the analyzer EcnaAnalyzer
// ------------------------------------------------------
//
//           see files EcnaAnalyzer.h and EcnaAnalyzer.cc
//           in package EcalCorrelatedNoiseAnalysisModules
//
//
// More detailled description of the class TEcnaRun
// -----------------------------------------------
//
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//
//                     Declaration and Print Methods
//
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//
//     Just after the declaration with the constructor without arguments,
//     you can set a "Print Flag" by means of the following "Print Methods":
//
//     TEcnaRun* MyCnaRun = new TEcnaRun(); // declaration of the object MyCnaRun
//
//   // Print Methods: 
//
//    MyCnaRun->PrintNoComment();  // Set flag to forbid printing of all the comments
//                                 // except ERRORS.
//
//    MyCnaRun->PrintWarnings();   // (DEFAULT)
//                                 // Set flag to authorize printing of some warnings.
//                                 // WARNING/INFO: information on something unusual
//                                 // in the data.
//                                 // WARNING/CORRECTION: something wrong (but not too serious)
//                                 // in the value of some argument.
//                                 // Automatically modified to a correct value.
//
//   MyCnaRun->PrintComments();    // Set flag to authorize printing of infos
//                                 // and some comments concerning initialisations
//
//   MyCnaRun->PrintAllComments(); // Set flag to authorize printing of all the comments
//
//
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//
//           Method GetReadyToReadData(...) and associated methods
//
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//
//      MyCnaRun->GetReadyToReadData(AnalysisName,      NbOfSamples, RunNumber,
//		                     FirstReqEvtNumber, LastReqEvtNumber,  ReqNbOfEvts,
//                                   StexNumber,        [RunType]);
//
//
//   Explanations for the arguments (all of them are input arguments):
//
//      TString  AnalysisName: code for the analysis. This code is
//                             necessary to distinguish between different
//                             analyses on the same events of a same run
//                             (example: pedestal run for the 3 gains:
//                                       AnalysisName = "Ped1" or "Ped6" or "Ped12")
//                             The string AnalysisName is automatically
//                             included in the name of the results files
//                             (see below: results files paragraph)
//
//      Int_t     NbOfSamples         number of samples (=10 maximum) 
//      Int_t     RunNumber:          run number
//      Int_t     FirstReqEvtNumber:  first requested event number (numbering starting from 1)
//      Int_t     LastReqEvtNumber:   last  requested event number
//      Int_t     StexNumber:         SM or Dee number (Stex = SM if EB, Dee if EE)
//      
//        The different quantities (correlations, etc...) will be calculated
//        from the event numbered: FirstReqEvtNumber
//        to the event numbered:   LastReqEvtNumber.
//
//      Int_t     RunType [optional]: run type
//
//                          PEDESTAL_STD = 9
//                          LASER_STD    = 4
//                          PEDESTAL_GAP = 18, etc...
//      (see CMSSSW/DataFormats/EcalRawData/interface/EcalDCCHeaderBlock.h)
//
//      if RunType is specified, the run type will be displayed on the plots
//
//==============> Method to set the start and stop times of the analysis (optional)
//
//  A method can be used to set the fStartDate and fStopDate attributes
//  of the class TEcnaHeader from start and stop time given by the user provided
//  these values have been recovered from the event reading:
//
//      void  MyCnaRun->StartStopDate(TString StartDate, TString StopDate);
// 
//     // TString StartDate, StopDate:  start and stop time of the run
//     //                               in "date" format. Example: 
//     //                               Wed Oct  8 04:14:23 2003
//
//     If the method is not called, the values of the attributes
//     fStartDate and fStopDate are set to: "!Start date> no info"
//     and "!Stop date> no info" at the level of Init() method of the class TEcnaHeader.
//     The values of StartDate and StopDate are written in the header of
//     the .root result file.
//
//
//  Another similar method exists, with time_t type arguments:
//
//     void  MyCnaRun->StartStopTime(time_t  StartTime, time_t  StopTime);
//
//
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//
//                       Calculation methods
//
//&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&
//
//    The "calculation methods" are methods which compute the different
//    quantities of interest. 
//
//    List of the calculation methods:
//

//  //................... Calculation methods ( associated to GetReadyToReadData(...) )
//  void SampleValues(); // 3D histo of the sample ADC value 
//                       // for each triple (channel,sample,event)
//  void SampleMeans();  // Calculation of the expectation values over the events
//                       // for each sample and for each channel
//  void SampleSigmas(); // Calculation of the sigmas over the events
//                       // for each sample and for each channel
//  //...........................................
//  void CovariancesBetweenSamples();  // Calculation of the (s,s) covariances over the events
//                                     // for each channel
//  void CorrelationsBetweenSamples(); // Calculation of the (s,s) correlations over the events
//                                     // for each channel
//  //..........................................................
//  void LowFrequencyCovariancesBetweenChannels();
//  void HighFrequencyCovariancesBetweenChannels();
//  void LowFrequencyCorrelationsBetweenChannels();
//  void HighFrequencyCorrelationsBetweenChannels();
//  //..........................................................
//  void LowFrequencyMeanCorrelationsBetweenTowers();
//  void HighFrequencyMeanCorrelationsBetweenTowers();
//
//  void LowFrequencyMeanCorrelationsBetweenSCs();
//  void HighFrequencyMeanCorrelationsBetweenSCs();
//
// //.................................................... Quantities as a function of Xtals
//  void Pedestals();
//  void TotalNoise();
//  void LowFrequencyNoise();
//  void HighFrequencyNoise();
//  void MeanOfCorrelationsBetweenSamples();
//  void SigmaOfCorrelationsBetweenSamples();
//
//  //......................... Quantities as a function of tower (EB) or SC (EE), average over the Xtals
//  void AveragedPedestals();
//  void AveragedTotalNoise();
//  void AveragedLowFrequencyNoise();
//  void AveragedHighFrequencyNoise();
//  void AveragedMeanOfCorrelationsBetweenSamples();
//  void AveragedSigmaOfCorrelationsBetweenSamples();
//
//
//==============> RESULTS FILES
//
//  The calculation methods above provide results which can be used directly
//  in the user's code. However, these results can also be written in results
//  files by appropriate methods.
//  The names of the results files are automaticaly generated.
//
//  It is also possible to write results in ASCII files  => See TEcnaWrite and TEcnaGui
//  It is also possible to plot results in ROOT canvases => See TEcnaHistos and TEcnaGui
//                              
//
// *-----------> Codification for the name of the ROOT file:
//
//  The name of the ROOT file is the following:
//
//       aaa_S1_sss_Rrrr_fff_lll_SMnnn.root     for EB
//       aaa_S1_sss_Rrrr_fff_lll_Deennn.root    for EE
//
//  with:
//       aaa = Analysis name
//       sss = number of samples
//       rrr = Run number
//       fff = First requested event number
//       lll = Last  requested events
//       mmm = Requested number of events
//       nnn = SM number or Dee number
//
//  This name is automatically generated from the values of the arguments
//  of the method "GetReadyToReadData".
//
// *-----------> Method which writes the result in the ROOT file:
//
//       Bool_t  MyCnaRun->WriteRootFile();
//
//===================================================================================================
//
//-------------------------------------------------------------------------
//
//        For more details on other classes of the CNA package:
//
//                 http://www.cern.ch/cms-fabbro/cna
//
//-------------------------------------------------------------------------

//------------------------------ TEcnaRun.cc ------------------------------
//  
//   Creation (first version): 03 Dec 2002
//
//   For questions or comments, please send e-mail to Bernard Fabbro:
//             
//   fabbro@hep.saclay.cea.fr 
//
//------------------------------------------------------------------------

TEcnaRun::TEcnaRun()
{
//Constructor without argument: nothing special done

 // cout << "[Info Management] CLASS: TEcnaRun.           CREATE OBJECT: this = " << this << endl;
}

TEcnaRun::TEcnaRun(const TString SubDet)
{
//Constructor with argument: call to Init() and declare fEcal according to SubDet value ("EB" or "EE")

 // cout << "[Info Management] CLASS: TEcnaRun.           CREATE OBJECT: this = " << this << endl;

  Init();
  SetEcalSubDetector(SubDet.Data());
  fNbSampForFic = fEcal->MaxSampADC(); // DEFAULT Number of samples for ROOT file
}

TEcnaRun::TEcnaRun(const TString SubDet, const Int_t& NbOfSamples)
{
//Constructor with arguments: call to Init(), declare fEcal according to SubDet value ("EB" or "EE")
// and set NbOfSamples according to the user's willing

 // cout << "[Info Management] CLASS: TEcnaRun.           CREATE OBJECT: this = " << this << endl;

  Init();
  SetEcalSubDetector(SubDet.Data());
  if( NbOfSamples>0 && NbOfSamples<=fEcal->MaxSampADC() )
    {
      fNbSampForFic = NbOfSamples;
    }
  else
    {
      cout << "TEcnaRun/CONSTRUCTOR> Number of required samples = " << NbOfSamples
	   << ": OUT OF RANGE. Set to the default value (= " << fEcal->MaxSampADC() << ")."
	   << fTTBELL << endl;
      fNbSampForFic = fEcal->MaxSampADC(); // DEFAULT Number of samples for file reading
    }
}

void TEcnaRun::Init()
{
//Initialisation

  fCnew       = 0;
  fCdelete    = 0;
  fCnaCommand = 0;
  fCnaError   = 0;

  fTTBELL = '\007';

  //........................... TString file names init
  fgMaxCar  = (Int_t)512;

  //................ MiscDiag counters .................
  fMaxMsgIndexForMiscDiag = (Int_t)10;
  fNbOfMiscDiagCounters   = (Int_t)50;
  fMiscDiag = 0;

  fNumberOfEvents = 0;
  //............................. init pointers  ( Init() )
  fT3d_distribs  = 0;
  fT3d2_distribs = 0;
  fT3d1_distribs = 0;

  fT1d_StexStinFromIndex = 0;

  fT2d_NbOfEvts = 0;
  fT1d_NbOfEvts = 0; 

  fT2d_ev        = 0;
  fT1d_ev        = 0;
  fT2d_sig       = 0;
  fT1d_sig       = 0;

  fT3d_cov_ss    = 0;
  fT3d2_cov_ss   = 0;
  fT3d1_cov_ss   = 0;

  fT3d_cor_ss    = 0;
  fT3d2_cor_ss   = 0;
  fT3d1_cor_ss   = 0;

  fT2d_lf_cov  = 0;
  fT2d1_lf_cov = 0;

  fT2d_lf_cor  = 0;
  fT2d1_lf_cor = 0;

  fT2d_hf_cov  = 0;
  fT2d1_hf_cov = 0;

  fT2d_hf_cor  = 0;
  fT2d1_hf_cor = 0;

  fT2d_lfcc_mostins  = 0;
  fT2d1_lfcc_mostins = 0;

  fT2d_hfcc_mostins  = 0;
  fT2d1_hfcc_mostins = 0;

  fT1d_ev_ev            = 0;
  fT1d_evsamp_of_sigevt = 0;
  fT1d_ev_cor_ss        = 0;
  fT1d_av_mped          = 0;
  fT1d_av_totn          = 0;
  fT1d_av_lofn          = 0;
  fT1d_av_hifn          = 0;
  fT1d_av_ev_corss      = 0;
  fT1d_av_sig_corss     = 0;

  fT1d_sigevt_of_evsamp = 0;
  fT1d_evevt_of_sigsamp = 0;
  fT1d_sig_cor_ss       = 0;

  fT2dCrysNumbersTable = 0;
  fT1dCrysNumbersTable = 0;

  //................................ tags   ( Init() )
  fTagStinNumbers = 0;

  fTagNbOfEvts = 0;

  fTagAdcEvt = 0;

  fTagMSp       = 0;
  fTagSSp      = 0;

  fTagCovCss   = 0;
  fTagCorCss   = 0;

  fTagHfCov    = 0;
  fTagHfCor    = 0;
  fTagLfCov    = 0;
  fTagLfCor    = 0;

  fTagLFccMoStins = 0;
  fTagHFccMoStins = 0;

  fTagPed     = 0;
  fTagTno    = 0;
  fTagMeanCorss  = 0;

  fTagLfn    = 0;
  fTagHfn   = 0;
  fTagSigCorss = 0;

  fTagAvPed = 0;
  fTagAvTno = 0;
  fTagAvLfn = 0;
  fTagAvHfn = 0;

  fTagAvMeanCorss  = 0;
  fTagAvSigCorss = 0;

  //................................................... Code Print  ( Init() )
  fFlagPrint = fCodePrintWarnings;
  fCnaParCout = 0; fCnaParCout = new TEcnaParCout();    //fCnew++;
  fCodePrintNoComment   = fCnaParCout->GetCodePrint("NoComment");
  fCodePrintWarnings    = fCnaParCout->GetCodePrint("Warnings ");      // => default value
  fCodePrintComments    = fCnaParCout->GetCodePrint("Comments");
  fCodePrintAllComments = fCnaParCout->GetCodePrint("AllComments");

  //................................................... Paths
  fCnaParPaths = 0; fCnaParPaths = new TEcnaParPaths();  //fCnew++;
  fOpenRootFile = kFALSE;
  
  fReadyToReadData = 0;

  //.............................................. Miscellaneous
  fFileHeader = 0;
  const Text_t *h_name  = "CnaHeader";   //==> voir cette question avec FXG
  const Text_t *h_title = "CnaHeader";   //==> voir cette question avec FXG
  fFileHeader = new TEcnaHeader(h_name, h_title);     //fCnew++;

  fSpecialStexStinNotIndexed = -1;

  fStinIndexBuilt     = 0;
  fBuildEvtNotSkipped = 0;
}// end of Init()

//========================================================================
void  TEcnaRun::SetEcalSubDetector(const TString SubDet)
{
 // Set Subdetector (EB or EE)

  Int_t MaxCar = fgMaxCar;
  fFlagSubDet.Resize(MaxCar);
  fFlagSubDet = SubDet.Data();

  fEcal          = 0; fEcal          = new TEcnaParEcal(fFlagSubDet.Data());           //fCnew++;
  fEcalNumbering = 0; fEcalNumbering = new TEcnaNumbering(fFlagSubDet.Data(), fEcal);  //fCnew++;
  fCnaWrite      = 0;
  fCnaWrite =
    new TEcnaWrite(fFlagSubDet.Data(), fCnaParPaths, fCnaParCout, fEcal, fEcalNumbering); //fCnew++;

  if( fFlagSubDet == "EB" ){fStexName = "SM "; fStinName = "tower";}
  if( fFlagSubDet == "EE" ){fStexName = "Dee"; fStinName = " SC  ";}
}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
//                    copy constructor
//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

TEcnaRun::TEcnaRun(const TEcnaRun& dcop)
{
  cout << "*TEcnaRun::TEcnaRun(const TEcnaRun& dcop)> "
       << " Now is the time to write a copy constructor" 
       << endl;
  
  //{ Int_t cintoto;  cin >> cintoto; }
  
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
//                    overloading of the operator=
//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//TEcnaRun& TEcnaRun::operator=(const TEcnaRun& dcop)
//{
//Overloading of the operator=
//
//  fCopy(dcop);
//  return *this;
//}
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
//                            destructor
//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

TEcnaRun::~TEcnaRun()
{
  //Destructor
  
  if(fFlagPrint == fCodePrintAllComments)
    {
      cout << "*TEcnaRun::~TEcnaRun()> Entering destructor." << endl;
    }

  if(fFlagPrint != fCodePrintNoComment || fFlagPrint == fCodePrintWarnings )
    {
      if( fBuildEvtNotSkipped > 0 )
	{
	  cout << "************************************************************************************* "
	       << endl;
	  cout << "*TEcnaRun::~TEcnaRun()> Nb of calls to BuildEventsDistributions by cmsRun: "
	       << fBuildEvtNotSkipped << endl;
	  cout << "************************************************************************************* "
	       << endl;
	}
    }

  if(fFlagPrint == fCodePrintAllComments)
    {
      Int_t misc_czero = 0;
      for(Int_t i = 0; i < fNbOfMiscDiagCounters; i++)
	{
	  if( fMiscDiag[i] != 0 )
	    {
	      cout << "                          fMiscDiag Counter "
		   << setw(3) << i << " = " << setw(9) << fMiscDiag[i]
		   << " (INFO: alloc on non zero freed zone) " << endl;
	    }
	  else
	    {
	      misc_czero++;
	    }
	}
      cout << "                          Nb of fMiscDiag counters at zero: "
	   << misc_czero << " (total nb of counters: "
	   << fNbOfMiscDiagCounters << ")" << endl;
    }
  
  if (fMiscDiag                != 0){delete [] fMiscDiag;                 fCdelete++;}
  
  //if (fFileHeader              != 0){delete fFileHeader;                  fCdelete++;}
  //if (fEcal                    != 0){delete fEcal;                        fCdelete++;}
  //if (fEcalNumbering           != 0){delete fEcalNumbering;               fCdelete++;}
  //if (fCnaParCout              != 0){delete fCnaParCout;                  fCdelete++;}
  //if (fCnaParPaths             != 0){delete fCnaParPaths;                 fCdelete++;}
  //if (fCnaWrite                != 0){delete fCnaWrite;                    fCdelete++;}
  
  if (fT1d_StexStinFromIndex   != 0){delete [] fT1d_StexStinFromIndex;    fCdelete++;}

  if (fT2d_NbOfEvts        != 0){delete [] fT2d_NbOfEvts;         fCdelete++;}
  if (fT1d_NbOfEvts        != 0){delete [] fT1d_NbOfEvts;         fCdelete++;}

  if (fT3d_distribs        != 0){delete [] fT3d_distribs;         fCdelete++;}
  if (fT3d2_distribs       != 0){delete [] fT3d2_distribs;        fCdelete++;}
  if (fT3d1_distribs       != 0){delete [] fT3d1_distribs;        fCdelete++;}

  if (fT2d_ev              != 0){delete [] fT2d_ev;               fCdelete++;}
  if (fT1d_ev              != 0){delete [] fT1d_ev;               fCdelete++;}

  if (fT2d_sig             != 0){delete [] fT2d_sig;              fCdelete++;}
  if (fT1d_sig             != 0){delete [] fT1d_sig;              fCdelete++;}

  if (fT3d_cov_ss          != 0){delete [] fT3d_cov_ss;           fCdelete++;}
  if (fT3d2_cov_ss         != 0){delete [] fT3d2_cov_ss;          fCdelete++;}
  if (fT3d1_cov_ss         != 0){delete [] fT3d1_cov_ss;          fCdelete++;}

  if (fT3d_cor_ss          != 0){delete [] fT3d_cor_ss;           fCdelete++;}
  if (fT3d2_cor_ss         != 0){delete [] fT3d2_cor_ss;          fCdelete++;}
  if (fT3d1_cor_ss         != 0){delete [] fT3d1_cor_ss;          fCdelete++;}

  if (fT2d_lf_cov          != 0){delete [] fT2d_lf_cov;           fCdelete++;}
  if (fT2d1_lf_cov         != 0){delete [] fT2d1_lf_cov;          fCdelete++;}

  if (fT2d_lf_cor          != 0){delete [] fT2d_lf_cor;           fCdelete++;}
  if (fT2d1_lf_cor         != 0){delete [] fT2d1_lf_cor;          fCdelete++;}

  if (fT2d_hf_cov          != 0){delete [] fT2d_hf_cov;           fCdelete++;}
  if (fT2d1_hf_cov         != 0){delete [] fT2d1_hf_cov;          fCdelete++;}

  if (fT2d_hf_cor          != 0){delete [] fT2d_hf_cor;           fCdelete++;}
  if (fT2d1_hf_cor         != 0){delete [] fT2d1_hf_cor;          fCdelete++;}

  if (fT2d_lfcc_mostins    != 0){delete [] fT2d_lfcc_mostins;     fCdelete++;}
  if (fT2d1_lfcc_mostins   != 0){delete [] fT2d1_lfcc_mostins ;   fCdelete++;}

  if (fT2d_hfcc_mostins    != 0){delete [] fT2d_hfcc_mostins ;    fCdelete++;}
  if (fT2d1_hfcc_mostins   != 0){delete [] fT2d1_hfcc_mostins;    fCdelete++;}

  if (fT1d_ev_ev             != 0){delete [] fT1d_ev_ev;              fCdelete++;}
  if (fT1d_evsamp_of_sigevt  != 0){delete [] fT1d_evsamp_of_sigevt;   fCdelete++;}
  if (fT1d_ev_cor_ss         != 0){delete [] fT1d_ev_cor_ss;          fCdelete++;}
  if (fT1d_av_mped           != 0){delete [] fT1d_av_mped;            fCdelete++;}
  if (fT1d_av_totn           != 0){delete [] fT1d_av_totn;            fCdelete++;}
  if (fT1d_av_lofn           != 0){delete [] fT1d_av_lofn;            fCdelete++;}
  if (fT1d_av_hifn           != 0){delete [] fT1d_av_hifn;            fCdelete++;}
  if (fT1d_av_ev_corss       != 0){delete [] fT1d_av_ev_corss;        fCdelete++;}
  if (fT1d_av_sig_corss      != 0){delete [] fT1d_av_sig_corss;       fCdelete++;}

  if (fT1d_sigevt_of_evsamp    != 0){delete [] fT1d_sigevt_of_evsamp;     fCdelete++;}
  if (fT1d_evevt_of_sigsamp    != 0){delete [] fT1d_evevt_of_sigsamp;     fCdelete++;}
  if (fT1d_sig_cor_ss          != 0){delete [] fT1d_sig_cor_ss;           fCdelete++;}

  if (fT2dCrysNumbersTable     != 0){delete [] fT2dCrysNumbersTable;      fCdelete++;}
  if (fT1dCrysNumbersTable     != 0){delete [] fT1dCrysNumbersTable;      fCdelete++;}

  if (fTagStinNumbers          != 0){delete [] fTagStinNumbers;           fCdelete++;}
  if (fTagNbOfEvts             != 0){delete [] fTagNbOfEvts;              fCdelete++;}
  if (fTagAdcEvt               != 0){delete [] fTagAdcEvt;                fCdelete++;}
  if (fTagMSp                  != 0){delete [] fTagMSp;                   fCdelete++;}
  if (fTagSSp                  != 0){delete [] fTagSSp;                   fCdelete++;}

  if (fTagCovCss               != 0){delete [] fTagCovCss;                fCdelete++;}
  if (fTagCorCss               != 0){delete [] fTagCorCss;                fCdelete++;}

  if (fTagHfCov                != 0){delete [] fTagHfCov;                 fCdelete++;}
  if (fTagHfCor                != 0){delete [] fTagHfCor;                 fCdelete++;}
  if (fTagLfCov                != 0){delete [] fTagLfCov;                 fCdelete++;}
  if (fTagLfCor                != 0){delete [] fTagLfCor;                 fCdelete++;}

  if (fTagLFccMoStins          != 0){delete [] fTagLFccMoStins;           fCdelete++;}
  if (fTagHFccMoStins          != 0){delete [] fTagHFccMoStins;           fCdelete++;}

  if (fTagPed                 != 0){delete [] fTagPed;                  fCdelete++;}
  if (fTagTno                 != 0){delete [] fTagTno;                  fCdelete++;}
  if (fTagMeanCorss           != 0){delete [] fTagMeanCorss;            fCdelete++;}

  if (fTagLfn                != 0){delete [] fTagLfn;                 fCdelete++;}
  if (fTagHfn                != 0){delete [] fTagHfn;                 fCdelete++;}
  if (fTagSigCorss           != 0){delete [] fTagSigCorss;            fCdelete++;}

  if (fTagAvPed          != 0){delete [] fTagAvPed;           fCdelete++;}
  if (fTagAvTno          != 0){delete [] fTagAvTno;           fCdelete++;}
  if (fTagAvLfn          != 0){delete [] fTagAvLfn;           fCdelete++;}
  if (fTagAvHfn          != 0){delete [] fTagAvHfn;           fCdelete++;}
  if (fTagAvMeanCorss    != 0){delete [] fTagAvMeanCorss;     fCdelete++;}
  if (fTagAvSigCorss     != 0){delete [] fTagAvSigCorss;      fCdelete++;}

  if ( fCnew != fCdelete )
    {
      cout << "!TEcnaRun::~TEcnaRun()> WRONG MANAGEMENT OF MEMORY ALLOCATIONS: fCnew = "
	   << fCnew << ", fCdelete = " << fCdelete << fTTBELL << endl;
    }
  else
    {
      // cout << "*TEcnaRun::~TEcnaRun()> Management of memory allocations: OK. fCnew = "
      //   << fCnew << ", fCdelete = " << fCdelete << endl;
    }

  if(fFlagPrint == fCodePrintAllComments)
    {
      cout << "*TEcnaRun::~TEcnaRun()> Exiting destructor (this = " << this << ")." << endl
	   << "~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#~#"
	   << endl;
    }

 // cout << "[Info Management] CLASS: TEcnaRun.           DESTROY OBJECT: this = " << this << endl;
}

//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
//
//                             M  E  T  H  O  D  S
//
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//============================================================================
//
//                           GetReadyToReadData(...)
//                  
//  Preparation of the result file name + tags allocations
//  + ADC event distribution array allocation + nb of events array allocation
//
//============================================================================

void TEcnaRun::GetReadyToReadData(TString      typ_ana, const Int_t& run_number,
				 const Int_t& nfirst,  const Int_t& nlast, const Int_t& nbevts,
				 const Int_t& Stex)
{
//Preparation of the data reading. Set part of the header. No RunType as argument.
//Use default value = 99999999 and call method with all the arguments (see below)

  Int_t RunType = 99999999;
  GetReadyToReadData(typ_ana, run_number, nfirst, nlast, nbevts, Stex, RunType);
}
//--------------------------------------------------------------------------------
void TEcnaRun::GetReadyToReadData( TString       typ_ana, const Int_t& run_number,
				   const Int_t&  nfirst,  const Int_t& nlast, const Int_t& nbevts,
				   const Int_t&  Stex,    const Int_t& run_type)
{
//Preparation of the data reading. Set part of the header
//
//   [nfirst, nlast] = [1,50] (50 events) or [151,300] (150 events) or etc...

  Int_t nrangeevts = nlast - nfirst + 1;  // number of events in range

  if( nrangeevts < nbevts )
    {
      if( nlast >= nfirst )
	{
	  cout << "*TEcnaRun::GetReadyToReadData(...)> --- WARNING ---> number of events = " << nbevts
	       << ", out of range (range = " << nfirst << "," << nlast << ")" << endl
	       << "                                    The number of found events will be less " << endl
	       << "                                    than the number of requested events." << endl;
	}
      if( nlast < nfirst )
	{
	  cout << "*TEcnaRun::GetReadyToReadData(...)> --- INFO ---> last requested event number = " << nlast
	       << ", less than first requested event number (= " << nfirst << ")" << endl
	       << "                                    File will be read until EOF if the number of found events" << endl
	       << "                                    remains less than the number of requested events." << endl;
	}

    }
  
  Int_t ifirst  = nfirst - 1;

  //............. allocation for counters
  fMiscDiag = new Int_t[fNbOfMiscDiagCounters];       fCnew++;
  for (Int_t iz=0; iz<fNbOfMiscDiagCounters; iz++){fMiscDiag[iz] = (Int_t)0;}

  //************** CHECK OF ARGUMENTS: nfirst_arg and nbevts_arg
  Int_t nentries = 99999999;   // => to be reintroduced as argument (like run_type) (?)
  if ( nfirst <= nentries )
    {
      //--------------------- test positivity of nfirst_arg        (GetReadyToReadData)
      if ( nfirst > 0 )
	{
	  //-------- test compatibility between the last requested event number
	  //         and the number of entries
	  if( nlast <= nentries )
	    { 
	      const Text_t *h_name  = "CnaHeader";   //==> voir cette question avec FXG
	      const Text_t *h_title = "CnaHeader";   //==> voir cette question avec FXG
	      //fFileHeader->HeaderParameters(h_name,     h_title ,
	      //	         	      typ_ana,    fNbSampForFic,
	      //			      run_number, nfirst,  nlast,  nbevts,
	      //			      Stex,       nentries);                 fCnew++;

	      if ( fEcal->MaxStinEcnaInStex() > 0  &&  fEcal->MaxCrysInStin() > 0  &&  fNbSampForFic > 0 ) 
		{
		  if( fFileHeader == 0 ){fFileHeader = new TEcnaHeader(h_name, h_title);}     // fCnew++;
		  
		  fFileHeader->HeaderParameters(typ_ana,    fNbSampForFic,
						run_number, nfirst, nlast, nbevts,
						Stex,       run_type);

		  // After this call to TEcnaHeader, we have:               (GetReadyToReadData)
		  //     fFileHeader->fTypAna            = typ_ana
		  //     fFileHeader->fNbOfSamples       = fNbSampForFic
		  //     fFileHeader->fRunNumber         = run_number
		  //     fFileHeader->fFirstReqEvtNumber = nfirst
		  //     fFileHeader->fLastReqEvtNumber  = nlast
		  //     fFileHeader->fReqNbOfEvts       = nbevts
		  //     fFileHeader->fStex              = Stex number
		  //     fFileHeader->fRunType           = run_type
		  
		  // fFileHeader->Print();
		  
		  // {Int_t cintoto; cout << "taper 0 pour continuer" << endl; cin >> cintoto;}
		  
		  //  fFileHeader->SetName("CnaHeader");              *======> voir FXG
		  //  fFileHeader->SetTitle("CnaHeader");
		  
		  //......................................... allocation tags + init of them (GetReadyToReadData)
		  
		  fTagStinNumbers = new Int_t[1]; fCnew++; fTagStinNumbers[0] = (Int_t)0;
		  fTagNbOfEvts    = new Int_t[1]; fCnew++; fTagNbOfEvts[0]    = (Int_t)0;
		  
		  fTagAdcEvt      = new Int_t[fEcal->MaxCrysEcnaInStex()]; fCnew++;
		  for (Int_t iz=0; iz<fEcal->MaxCrysEcnaInStex(); iz++){fTagAdcEvt[iz] = (Int_t)0;}
		  
		  fTagMSp         = new Int_t[1]; fCnew++; fTagMSp[0] = (Int_t)0;
		  fTagSSp         = new Int_t[1]; fCnew++; fTagSSp[0] = (Int_t)0;
		  
		  fTagCovCss      = new Int_t[fEcal->MaxCrysEcnaInStex()]; fCnew++;
		  for (Int_t iz=0; iz<fEcal->MaxCrysEcnaInStex(); iz++){fTagCovCss[iz] = (Int_t)0;}
		  
		  fTagCorCss      = new Int_t[fEcal->MaxCrysEcnaInStex()]; fCnew++;
		  for (Int_t iz=0; iz<fEcal->MaxCrysEcnaInStex(); iz++){fTagCorCss[iz] = (Int_t)0;}
		  
		  fTagLfCov       = new Int_t[1]; fCnew++; fTagLfCov[0] = (Int_t)0;
		  fTagLfCor       = new Int_t[1]; fCnew++; fTagLfCor[0] = (Int_t)0;
		  
		  fTagHfCov       = new Int_t[1]; fCnew++; fTagHfCov[0] = (Int_t)0;
		  fTagHfCor       = new Int_t[1]; fCnew++; fTagHfCor[0] = (Int_t)0;
		  
		  fTagLFccMoStins = new Int_t[1]; fCnew++; fTagLFccMoStins[0] = (Int_t)0;
		  fTagHFccMoStins = new Int_t[1]; fCnew++; fTagHFccMoStins[0] = (Int_t)0;
		  
		  fTagPed         = new Int_t[1]; fCnew++; fTagPed[0]         = (Int_t)0;
		  fTagTno         = new Int_t[1]; fCnew++; fTagTno[0]         = (Int_t)0;
		  fTagMeanCorss   = new Int_t[1]; fCnew++; fTagMeanCorss[0]   = (Int_t)0; 
		  
		  fTagLfn         = new Int_t[1]; fCnew++; fTagLfn[0]         = (Int_t)0; 
		  fTagHfn         = new Int_t[1]; fCnew++; fTagHfn[0]         = (Int_t)0; 
		  fTagSigCorss    = new Int_t[1]; fCnew++; fTagSigCorss[0]    = (Int_t)0; 
		  
		  fTagAvPed       = new Int_t[1]; fCnew++; fTagAvPed[0]       = (Int_t)0; 
		  fTagAvTno       = new Int_t[1]; fCnew++; fTagAvTno[0]       = (Int_t)0; 
		  fTagAvLfn       = new Int_t[1]; fCnew++; fTagAvLfn[0]       = (Int_t)0; 
		  fTagAvHfn       = new Int_t[1]; fCnew++; fTagAvHfn[0]       = (Int_t)0; 
		  fTagAvMeanCorss = new Int_t[1]; fCnew++; fTagAvMeanCorss[0] = (Int_t)0; 
		  fTagAvSigCorss  = new Int_t[1]; fCnew++; fTagAvSigCorss[0]  = (Int_t)0; 
		  
		  //====================================================================================
		  //
		  //   allocation for fT1d_StexStinFromIndex[] and init to fSpecialStexStinNotIndexed
		  //
		  //====================================================================================
		  
		  if(fT1d_StexStinFromIndex == 0)
		    {
		      fT1d_StexStinFromIndex = new Int_t[fEcal->MaxStinEcnaInStex()];          fCnew++;
		    }
		  for ( Int_t i0_Stin = 0; i0_Stin < fEcal->MaxStinEcnaInStex(); i0_Stin++ )
		    {
		      fT1d_StexStinFromIndex[i0_Stin] = fSpecialStexStinNotIndexed;
		    }
		  
		  //-------------------------------------------------------------  (GetReadyToReadData)
	  
		  //====================================================================================
		  //
		  //   allocation of the 3D array fT3d_distribs[channel][sample][events] (ADC values)
		  //
		  //   This array is filled in the BuildEventDistributions(...) method
		  //
		  //====================================================================================
		  
		  if(fT3d_distribs == 0)
		    {
		      //............ Allocation for the 3d array 
		      cout << "*TEcnaRun::GetReadyToReadData(...)> Allocation of 3D array for ADC distributions."
			   << " Nb of requested evts = " << fFileHeader->fReqNbOfEvts << endl
			   << "                                    This number must not be too large"
			   << " (no failure after this message means alloc OK)." << endl;

		      fT3d_distribs = new Double_t**[fEcal->MaxCrysEcnaInStex()];        fCnew++;  

		      fT3d2_distribs  =
			new  Double_t*[fEcal->MaxCrysEcnaInStex()*
				       fNbSampForFic];                          fCnew++;  

		      fT3d1_distribs  =
			new   Double_t[fEcal->MaxCrysEcnaInStex()*
				       fNbSampForFic*
				       fFileHeader->fReqNbOfEvts];                       fCnew++;

		      for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++){
			fT3d_distribs[i0StexEcha] = &fT3d2_distribs[0] + i0StexEcha*fNbSampForFic;
			for(Int_t j0Sample=0; j0Sample<fNbSampForFic; j0Sample++){
			  fT3d2_distribs[fNbSampForFic*i0StexEcha + j0Sample] = &fT3d1_distribs[0]+
			    fFileHeader->fReqNbOfEvts*(fNbSampForFic*i0StexEcha+j0Sample);}}
		    }

		  //................................. Init to zero
		  for (Int_t iza=0; iza<fEcal->MaxCrysEcnaInStex(); iza++)
		    {
		      for (Int_t izb=0; izb<fNbSampForFic; izb++)
			{
			  for (Int_t izc=0; izc<fFileHeader->fReqNbOfEvts; izc++)
			    {
			      if( fT3d_distribs[iza][izb][izc] != (Double_t)0 )
				{
				  fMiscDiag[0]++;
				  fT3d_distribs[iza][izb][izc] = (Double_t)0;
				}
			    }
			}
		    }

		  //--------------------------------------------------------- (GetReadyToReadData)	  
		  //====================================================================================
		  //
		  //   allocation of the 2D array fT2d_NbOfEvts[channel][sample] (Max nb of evts)
		  //
		  //====================================================================================
		  
		  if (fT2d_NbOfEvts == 0)
		    {
		      fT2d_NbOfEvts  = new Int_t*[fEcal->MaxCrysEcnaInStex()];           fCnew++;
		      fT1d_NbOfEvts  = new  Int_t[fEcal->MaxCrysEcnaInStex()*
						      fNbSampForFic];            fCnew++;
		      
		      for(Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
			{
			  fT2d_NbOfEvts[i0StexEcha] =
			    &fT1d_NbOfEvts[0] + i0StexEcha*fNbSampForFic;
			}
		      
		      //................ Init the array to 0
		      for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
			{
			  for(Int_t i0Sample=0; i0Sample<fNbSampForFic; i0Sample++)
			    {
			      fT2d_NbOfEvts[i0StexEcha][i0Sample] = 0;
			    }
			}
		    }
		  else
		    {
		      cerr << "!TEcnaRun::GetReadyToReadData(...)> *** ERROR *** No allocation for fT2d_NbOfEvts!"
			   << " Pointer already not NULL " << fTTBELL << endl;
		      // {Int_t cintoto; cout << "Enter: 0 and RETURN to continue or: CTRL C to exit"
		      //		   << endl; cin >> cintoto;}
		    }
		}
	      else
		{
		  cerr << endl
		       << "!TEcnaRun::GetReadyToReadData(...)> "
		       << " *** ERROR *** " << endl
		       << " --------------------------------------------------"
		       << endl
		       << " NULL or NEGATIVE values for arguments" << endl
		       << " with expected positive values:"        << endl
		       << " Number of Stins in Stex = " << fEcal->MaxStinEcnaInStex()  << endl
		       << " Number of crystals in Stin     = " << fEcal->MaxCrysInStin() << endl
		       << " Number of samples by channel    = " << fNbSampForFic << endl
		       << endl
		       << endl
		       << " hence, no memory allocation for array member has been performed." << endl;
		  
		  cout << "Enter: 0 and RETURN to continue or: CTRL C to exit";
		  Int_t toto;
		  cin >> toto;
		}
	      //----------------------------------------------------------- (GetReadyToReadData)
	      if(fFlagPrint == fCodePrintAllComments ){
		cout << endl;
		cout << "*TEcnaRun::GetReadyToReadData(...)>" << endl
		     << "          The method has been called with the following argument values:"  << endl
		     << "          Analysis name                = "
		     << fFileHeader->fTypAna << endl
		     << "          Run number                   = "
		     << fFileHeader->fRunNumber << endl
		     << "          Run type                     = "
		     << fFileHeader->fRunType << endl
		     << "          First requested event number = "
		     << fFileHeader->fFirstReqEvtNumber << endl
		     << "          Last requested event number  = "
		     << fFileHeader->fLastReqEvtNumber << endl
		     << "          " << fStexName.Data() << " number             = "
		     << fFileHeader->fStex << endl
		     << "          Number of " << fStinName.Data()
		     << " in " << fStexName.Data() << "       = "
		     << fEcal->MaxStinEcnaInStex()  << endl
		     << "          Number of crystals in " << fStinName.Data() << "  = "
		     << fEcal->MaxCrysInStin()  << endl
		     << "          Number of samples by channel = "
		     << fNbSampForFic  << endl
		     << endl;}
	      
	      fReadyToReadData = 1;                        // set flag
	    }
	  else
	    {
	      if (fFlagPrint != fCodePrintNoComment){
		cout << "!TEcnaRun::GetReadyToReadData(...) > WARNING/CORRECTION:" << endl
		     << "! The fisrt requested event number is not positive (nfirst = " << nfirst << ") "
		     << fTTBELL << endl;}
	      ifirst = 0;
	    }
	}
      else
	{
	  if (fFlagPrint != fCodePrintNoComment){
	    cout << endl << "!TEcnaRun::GetReadyToReadData(...)> WARNING/CORRECTION:" << endl
		 << "! The number of requested events (nbevts = " << nbevts << ") is too large." << endl
		 << "! Last event number = " << nlast << " > number of entries = " << nentries << ". "
		 << fTTBELL << endl << endl;}
	}
    }
  else
    {
      cout << "!TEcnaRun::GetReadyToReadData(...) *** ERROR ***> "
	   << " The first requested event number is greater than the number of entries."
	   << fTTBELL << endl;
    }
  if(fFlagPrint == fCodePrintAllComments){
    cout << "*TEcnaRun::GetReadyToReadData(...)> Leaving the method. fReadyToReadData = "
	 << fReadyToReadData << endl; }

} // end of GetReadyToReadData

//====================================================================================================
//
//     BuildEventDistributions: method called by the CMSSW analyzer (cmsRun)
//
//     At each event, put the Sample ADC value in the 3D array: fT3d_distribs[i0StexEcha][i0Sample][i0EventIndex]
//
//         |============================================================|
//         |                                                            |
//         | (Stex,Stin) means: (SM,tower) for EB and: (Dee,SC) for EE  |
//         |                                                            |
//         |============================================================|
//
//    (Stin number <-> Stin index correspondance, ADC sample values)
//
//        THIS METHOD IS CALLED INSIDE THE LOOPS OVER:
//          ( EVENTS (tower or SC (CRYSTAL in tower or SC (SAMPLES)))) 
//
//  Arguments: Event    = event number.              Range = [ 1, fFileHeader->fReqNbOfEvts ]
//             n1StexStin = Stin number in Stex.     Range = [ 1, fEcal->MaxStinEcnaInStex() ]
//             i0StinEcha = channel number in Stin.  Range = [ 0, fEcal->MaxCrysInStin() - 1 ]     
//             sample   = ADC sample number.         Range = [ 0, fNbSampForFic - 1 ]
//             adcvalue = ADC sample value.
//
//====================================================================================================
Bool_t TEcnaRun::BuildEventDistributions(const Int_t&    n1EventNumber, const Int_t& n1StexStin,
					const Int_t&    i0StinEcha,    const Int_t& i0Sample,
					const Double_t& adcvalue)
{
  //Building of the arrays fT1d_StexStinFromIndex[] and fT3d_distribs[][][]
  
  fBuildEvtNotSkipped++;      // event not skipped by cmsRun

  Bool_t ret_code = kFALSE;

  Int_t i0EventIndex   = n1EventNumber - 1;  // INDEX FOR Event number  
  Int_t i0StexStinEcna = n1StexStin - 1;     // INDEX FOR StexStin = Number_of_the_Stin_in_Stex - 1

  Int_t i_trouve = 0;
  //.................................................................. (BuildEventDistributions)
  if(fReadyToReadData == 1)  
    {
      if( n1StexStin>= 1 && n1StexStin <= fEcal->MaxStinEcnaInStex() )
	{      
	  if( i0StinEcha >= 0 && i0StinEcha < fEcal->MaxCrysInStin() )
	    {
	      if( i0Sample >= 0 && i0Sample < fNbSampForFic )
		{
		  //..... Put the StexStin number in 1D array fT1d_StexStinFromIndex[] = Stin index + 1
		  if( fT1d_StexStinFromIndex != 0 )  // table fT1d_StexStinFromIndex[index] already allocated
		    {
		      ret_code = kTRUE;

		      // StexStin already indexed
		      if( n1StexStin == fT1d_StexStinFromIndex[i0StexStinEcna] ){i_trouve = 1;}
		  
		      // StexStin index not found: new StexStin
		      if (i_trouve != 1 )
			{
			  if( fT1d_StexStinFromIndex[i0StexStinEcna] == fSpecialStexStinNotIndexed )
			    {
			      fT1d_StexStinFromIndex[i0StexStinEcna] = n1StexStin;
			      fFileHeader->fStinNumbersCalc = 1;
			      fTagStinNumbers[0] = 1;
			      fStinIndexBuilt++;                      //  number of found Stins
			      
			      if(fFlagPrint == fCodePrintAllComments)
				{
				  if( fStinIndexBuilt == 1 )
				    {
				      cout << endl << "*TEcnaRun::BuildEventDistributions(...)> event " << n1EventNumber
					   << " : first event for " << fStexName.Data() << " " << fFileHeader->fStex
					   << "; " << fStinName.Data() << "s : ";
				    }
				  if( fFlagSubDet == "EB" )
				    {cout << fT1d_StexStinFromIndex[i0StexStinEcna] << ", ";}
				  if( fFlagSubDet == "EE" )
				    {cout << fEcalNumbering->
				       GetDeeSCConsFrom1DeeSCEcna(fFileHeader->fStex, fT1d_StexStinFromIndex[i0StexStinEcna])
					  << ", ";}
				}
			      //.................................................... (BuildEventDistributions)
			      if(fFlagPrint == fCodePrintAllComments)
				{
				  cout << " (" << fStinIndexBuilt << " " << fStinName.Data()
				       << " found), channel " << i0StinEcha << ", i0Sample " << i0Sample << endl;
				}
			      ret_code = kTRUE;
			    } // if ( fT1d_StexStinFromIndex[i0StexStinEcna] == fSpecialStexStinNotIndexed )
			  else
			    {
			      cout << "!TEcnaRun::BuildEventDistributions(...)> *** ERROR ***> NOT ALLOWED if RESULT. " 
				   << " n1StexStin = " << n1StexStin << ", fT1d_StexStinFromIndex["
				   << i0StexStinEcna << "] = "
				   << fT1d_StexStinFromIndex[i0StexStinEcna]
				   << ", fStinIndexBuilt = " << fStinIndexBuilt
				   << fTTBELL << endl;
			      ret_code = kFALSE;
			    }
			}  //  if (i_trouve != 1 )
		    } //  if( fT1d_StexStinFromIndex != 0 ) 
		  else
		    {
		      cout << "!TEcnaRun, BuildEventDistributions *** ERROR ***> "
			   << " fT1d_StexStinFromIndex = " << fT1d_StexStinFromIndex
			   << " fT1d_StexStinFromIndex[] ALLOCATION NOT DONE" << fTTBELL << endl;
		      ret_code = kFALSE;
		    } //.................................................................. (BuildEventDistributions)
		} // if( i0Sample >= 0 && i0Sample < fNbSampForFic )
	      else
		{
		  //.......Reading data => Message and error only if sample >= fEcal->MaxSampADC()
		  //       (not fNbSampForFic, the later is used only for calculations)
		  if( i0Sample >= fEcal->MaxSampADC() )
		    {
		      cout << "!TEcnaRun::BuildEventDistributions(...) *** ERROR ***> "
			   << " sample number = " << i0Sample << ". OUT OF BOUNDS"
			   << " (max = " << fEcal->MaxSampADC() << ")"
			   << fTTBELL << endl;
		      ret_code = kFALSE;
		    }
		  else
		    {
		      ret_code = kTRUE;
		    }
		}
	    }
	  else
	    {
	      cout << "!TEcnaRun::BuildEventDistributions(...) *** ERROR ***> "
		   << " channel number in " << fStinName.Data() << " = " << i0StinEcha << ". OUT OF BOUNDS"
		   << " (max = " << fEcal->MaxCrysInStin() << ")" 
		   << fTTBELL << endl;
	      ret_code = kFALSE;
	    }
	}
      else
	{
	  cout << "!TEcnaRun::BuildEventDistributions(...) *** ERROR ***> "
	       << fStinName.Data() << " number in " << fStexName.Data() << " = " << n1StexStin << ". OUT OF BOUNDS"
	       << " (max = " << fEcal->MaxStinEcnaInStex() << ")"
	       << fTTBELL << endl;
	  ret_code = kFALSE;
	}
      //.................................................................. (BuildEventDistributions)
      //........ Filling of the 2D array of the event numbers in the data reading loop and 
      //         filling of the 3D array of the ADC sample values

      if( ret_code == kTRUE )
	{
	  if( i0Sample < fNbSampForFic )
	    {
	      //............ 1) Conversion (Stin,i0StinEcha) -> i0StexEcha  (same numbering for EB and EE)
	      //=========================================================================================
	      //   n1StexStin (Tower or SC):     1            2            3
	      //   iStexStin                     0            1            2 
	      //
	      //   i0StinEcha:                 0......24    0......24    0......24
	      //
	      //   i0StexEcha         :        0......24   25......49   50......74   grouped by StexStin's
	      //   i0StexEcha+1 (Xtal):        1......25   26......50   51......75       
	      //
	      //=========================================================================================

	      Int_t i0StexEcha = i0StexStinEcna*fEcal->MaxCrysInStin() + i0StinEcha;
	      
	      //--------------------------------------------------------- (BuildEventDistributions)
	      if( i0StexEcha >= 0 && i0StexEcha < fEcal->MaxCrysEcnaInStex() )
		{
		  //............ 2) Increase of the nb of evts for (StexEcha,sample) (events found in the data)
		  (fT2d_NbOfEvts[i0StexEcha][i0Sample])++;     // value after first incrementation = 1
		  fTagNbOfEvts[0] = 1;
		  fFileHeader->fNbOfEvtsCalc = 1;
		  
		  //............ 3) Filling of the 3D array of the ADC values
		  if ( i0EventIndex >= 0 && i0EventIndex < fFileHeader->fReqNbOfEvts )
		    {  
		      fT3d_distribs[i0StexEcha][i0Sample][i0EventIndex] = adcvalue;
		    }
		  else
		    {
		      cout << "!TEcnaRun::BuildEventDistributions(...) *** ERROR ***> "
			   << " event number = " << n1EventNumber << ". OUT OF BOUNDS"
			   << " (max = " << fFileHeader->fReqNbOfEvts << ")"
			   << fTTBELL << endl;
		      ret_code = kFALSE;
		    }
		}
	      else
		{
		  cout << "!TEcnaRun::BuildEventDistributions(...) *** ERROR ***> "
		       << " CHANNEL NUMBER OUT OF BOUNDS" << endl
		       << " i0StexEcha number = " << i0StexEcha
		       << " , n1StexStin = " << n1StexStin
		       << " , i0StinEcha = " << i0StinEcha
		       << " , fEcal->MaxCrysEcnaInStex() = " << fEcal->MaxCrysEcnaInStex() 
		       << fTTBELL << endl; 
		  ret_code = kFALSE;
		  // {Int_t cintoto; cout << "TAPER 0 POUR CONTINUER" << endl; cin >> cintoto;}
		}
	    }
	  else
	    {
	      cout << "!TEcnaRun::BuildEventDistributions(...) *** ERROR ***> Nb of required samples = "
		   << i0Sample << " (Max = " << fNbSampForFic << ")" << fTTBELL << endl; 
	    }
	}
      else
	{
	  cout << "!TEcnaRun::BuildEventDistributions(...) *** ERROR ***> ret_code = kFALSE "
	       << fTTBELL << endl;
	}
    }
  else
    {
      cout << "!TEcnaRun::BuildEventDistributions(...) *** ERROR ***> GetReadyToReadData(...) not called."
	   << fTTBELL << endl;
      ret_code = kFALSE;
    }
  //.................................................................. (BuildEventDistributions)
  if (ret_code == kFALSE)
    {
      cout << "!TEcnaRun::BuildEventDistributions(...) > ret_code = " << ret_code << ". event: " << n1EventNumber
	   << ", n1StexStin: " << n1StexStin
	   << ", i0StinEcha: " << i0StinEcha
	   << ", i0Sample: "   << i0Sample
	   << ", adcvalue: " << adcvalue << endl;
    } 
  return ret_code;
}
//------------- ( end of BuildEventDistributions ) -----------------------
//====================================================================================================
//
//  ReadEventDistributions: called by external program. Get the distributions of the Sample ADC values
//                          from file by using TEcnaRead.
//
//====================================================================================================
Bool_t TEcnaRun::ReadEventDistributions()
{
  return ReadEventDistributions(fEcal->MaxSampADC());
}

Bool_t TEcnaRun::ReadEventDistributions(const Int_t& nb_samp_for_calc)
{
  // read the Sample ADC values from "ADC" result root files                     (ReadEventDistributions)

  // put the number of sample for calculations in attribute fNbSampForCalc
  // and call the method without arguments
  // We must have: nb_samp_for_calc <= fFileHeader->fNbOfSamples (= nb of samples in ROOT file)

  fNbSampForCalc = nb_samp_for_calc;

  TEcnaRead* MyRootFile = new TEcnaRead(fFlagSubDet.Data(), fCnaParPaths, fCnaParCout,
				      fFileHeader, fEcalNumbering, fCnaWrite);          //  fCnew++;
  
  MyRootFile->PrintNoComment();
  
  MyRootFile->GetReadyToReadRootFile(fFileHeader->fTypAna, fFileHeader->fNbOfSamples, fFileHeader->fRunNumber,
				     fFileHeader->fFirstReqEvtNumber, fFileHeader->fLastReqEvtNumber,
				     fFileHeader->fReqNbOfEvts,       fFileHeader->fStex,
				     fCnaParPaths->ResultsRootFilePath().Data());
  
  Bool_t ok_read = MyRootFile->LookAtRootFile();
  
  fFileHeader->fStartTime = MyRootFile->GetStartTime();
  fFileHeader->fStopTime  = MyRootFile->GetStopTime();
  fFileHeader->fStartDate = MyRootFile->GetStartDate();
  fFileHeader->fStopDate  = MyRootFile->GetStopDate();
  
  if ( ok_read == kTRUE )
    {
      fRootFileName      = MyRootFile->GetRootFileName();
      fRootFileNameShort = MyRootFile->GetRootFileNameShort();
      cout << "*TEcnaRun::ReadEventDistributions> Reading sample ADC values from file: " << endl
	   << "           " << fRootFileName << endl;
      
      Int_t i_no_data = 0;

      //.......... Read the StinNumbers in the old file                     (ReadEventDistributions)
      TVectorD vec(fEcal->MaxStinEcnaInStex());
      for(Int_t i=0; i<fEcal->MaxStinEcnaInStex(); i++){vec(i)=(Double_t)0.;}
      vec = MyRootFile->ReadStinNumbers(fEcal->MaxStinEcnaInStex());
      if( MyRootFile->DataExist() == kTRUE )
	{
	  fTagStinNumbers[0] = 1;
	  fFileHeader->fStinNumbersCalc = 1;
	  for(Int_t i0StexStinEcna=0; i0StexStinEcna<fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
	    {fT1d_StexStinFromIndex[i0StexStinEcna] = (Int_t)vec(i0StexStinEcna);}
	}
      else
	{
	  i_no_data++;
	}
      //.......... Read the Numbers of Events in the old file                      (ReadEventDistributions)
      TMatrixD partial_matrix(fEcal->MaxCrysInStin(), fFileHeader->fNbOfSamples);
      for(Int_t i=0; i<fEcal->MaxCrysInStin(); i++)
	{for(Int_t j=0; j<fFileHeader->fNbOfSamples; j++){partial_matrix(i,j)=(Double_t)0.;}}
      
      for(Int_t i0StexStinEcna=0; i0StexStinEcna<fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
	{
	  Int_t n1StexStin = MyRootFile->GetStexStinFromIndex(i0StexStinEcna);
	  if(n1StexStin != -1)
	    {
	      partial_matrix =
		MyRootFile->ReadNumberOfEventsForSamples
		(n1StexStin, fEcal->MaxCrysInStin(), fFileHeader->fNbOfSamples);

	      if( MyRootFile->DataExist() == kTRUE )
		{
		  fTagNbOfEvts[0] = 1;
		  fFileHeader->fNbOfEvtsCalc = 1;
		  for(Int_t i0StinCrys=0; i0StinCrys<fEcal->MaxCrysInStin(); i0StinCrys++)
		    {
		      Int_t i0StexEcha = (n1StexStin-1)*fEcal->MaxCrysInStin() + i0StinCrys;
		      for(Int_t i0Sample=0; i0Sample<fFileHeader->fNbOfSamples; i0Sample++)
			{fT2d_NbOfEvts[i0StexEcha][i0Sample] = (Int_t)partial_matrix(i0StinCrys,i0Sample);}
		    }
		}
	      else
		{
		  i_no_data++;
		}
	    }
	}

      //.......... Read the Sample ADC values in the old file                     (ReadEventDistributions)
      Double_t*** fT3d_read_distribs =
	MyRootFile->ReadSampleValuesSameFile
	(fEcal->MaxCrysEcnaInStex(), fFileHeader->fNbOfSamples, fFileHeader->fReqNbOfEvts);
      
      if( MyRootFile->DataExist() == kTRUE )
	{             
	  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
	    {
	      for(Int_t i0Sample=0; i0Sample<fFileHeader->fNbOfSamples;i0Sample++)
		{      
		  for(Int_t i_event=0; i_event<fFileHeader->fReqNbOfEvts; i_event++)
		    {fT3d_distribs[i0StexEcha][i0Sample][i_event] = 
		       fT3d_read_distribs[i0StexEcha][i0Sample][i_event];}
		}
	    }
	}
      else
	{
	  i_no_data++;
	}
      if( i_no_data != 0 )
	{
	  cout << "!TEcnaRun::ReadEventDistributions(...)> *ERROR* =====> "
	       << " Read failure. i_no_data = " << i_no_data << fTTBELL << endl;  
	}
    }
  else
    {
      cout << "!TEcnaRun::ReadEventDistributions(...)> *ERROR* =====> "
	   << " ROOT file not found" << fTTBELL << endl;     
    }
  delete MyRootFile;        //  fCdelete++;
  return ok_read;
}
//------------- ( end of ReadEventDistributions ) -----------------------
//-------------------------------------------------------------------------
//
//    Get the ROOT file name (long and short)
//
//-------------------------------------------------------------------------
TString TEcnaRun::GetRootFileName(){return fRootFileName;}
TString TEcnaRun::GetRootFileNameShort(){return fRootFileNameShort;}

//###################################################################################################
//
// THE FOLLOWING METHODS ARE CALLED AFTER THE LOOPS OVER EVENTS, STINS, CRYSTALS AND SAMPLES
//
//###################################################################################################


//=========================================================================
//
//     Set start time, stop time, StartDate, StopDate
//
//=========================================================================
void TEcnaRun::StartStopTime(time_t t_startime, time_t t_stoptime)
{
// Put the start an stop time (if they exist) in fFileHeader class attributes.
  
  fFileHeader->fStartTime = t_startime;
  fFileHeader->fStopTime  = t_stoptime;
}

void TEcnaRun::StartStopDate(TString c_startdate, TString c_stopdate)
{
// Put the start an stop date (if they exist) in fFileHeader class attributes.

  fFileHeader->fStartDate = c_startdate;
  fFileHeader->fStopDate  = c_stopdate;
}
//=========================================================================
//      
//       S A M P L E S    A D C   V A L U E S 
//
//       Written in .root file corresponding to analysis name
//       beginning with: "Adc" (see EcanAnalyzer.cc in package "Modules")
//
//=========================================================================

//-------------------------------------------------------------------
//
//                      SampleValues
//
//-------------------------------------------------------------------
void TEcnaRun::SampleValues()
{
//3D histo of the sample ADC values for all the triples (StexEcha, sample, event)

  if(fFlagPrint == fCodePrintAllComments){
    cout << "*TEcnaRun::SampleValues()>"
         << " Sample ADC values 3D histo"
	 << " (channel, sample, event number):" << endl
	 << "                          registration for writing in results .root file."
	 << endl;}

  // The histo is already in fT3d_distribs[][][]
  // this method sets the "Tag", increment the "Calc" (and must be kept for that)
  
  for (Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
    {fTagAdcEvt[i0StexEcha] = 1;        fFileHeader->fAdcEvtCalc++;}
}

//=========================================================================
//
//                         GetReadyToCompute()
//
//=========================================================================
void TEcnaRun::GetReadyToCompute()
{
//
  //  CHECK OF THE NUMBER OF FOUND EVENTS AND init of fNumberOfEvents
  //       (number used to compute the average values over the events)
  // The number of events fNumberOfEvents is extracted from the array fT2d_NbOfEvts[] 
  // which has been built by the BuildEventDistribution(...) method

  if( fT2d_NbOfEvts != 0 )
    {
      fNumberOfEvents = fCnaWrite->NumberOfEvents(fT2d_NbOfEvts, fEcal->MaxCrysEcnaInStex(),
						  fNbSampForFic, fFileHeader->fReqNbOfEvts);
    }
  else
    {
      cout << "*TEcnaRun::GetReadyToCompute()> no data? fT2d_NbOfEvts = " << fT2d_NbOfEvts << endl;
    }

}  
//  end of GetReadyToCompute()

//=========================================================================
//
//               C A L C U L A T I O N    M E T H O D S
//
//     fTag... => Calculation done. OK for writing on result file
//     ...Calc => Incrementation for result file size. 
//
//=========================================================================

//====================================================================
//
//       E X P E C T A T I O N   V A L U E S  ,  V A R I A N C E S
// 
//====================================================================

//----------------------------------------------------------------
//
//     Calculation of the expectation values of the samples
//                 for all the StexEchas
//
//----------------------------------------------------------------
void TEcnaRun::SampleMeans()
{
// Calculation of the expectation values over events
// for the samples 0 to fNbSampForCalc and for all the StexEchas

  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::SampleMeans() " << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout  << "           Calculation: sample expectation values over the events"
	  << " for each channel." << endl;}
  
  //................... Allocation fT2d_ev
  if ( fT2d_ev == 0 ){
    Int_t n_samp = fNbSampForCalc;
    Int_t n_StexEcha = fEcal->MaxCrysEcnaInStex();
    fT2d_ev = new Double_t*[n_StexEcha];             fCnew++;  
    fT1d_ev = new  Double_t[n_StexEcha*n_samp];      fCnew++;   
    for(Int_t i = 0 ; i < n_StexEcha ; i++){
      fT2d_ev[i] = &fT1d_ev[0] + i*n_samp;}
  }
  //................... init fT2d_ev to zero
  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      for(Int_t i0Sample=0; i0Sample<fNbSampForCalc; i0Sample++)
 	{
 	  if( fT2d_ev[i0StexEcha][i0Sample] != (Double_t)0 )
	    {fMiscDiag[1]++; fT2d_ev[i0StexEcha][i0Sample] = (Double_t)0;}
 	} 
    }
  
  //................... Calculation
  for (Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
    {
      for (Int_t i0Sample = 0 ; i0Sample < fNbSampForCalc ; i0Sample++)
	{
	  for( Int_t i_event = 0; i_event < fNumberOfEvents; i_event++ )
	    {
	      fT2d_ev[i0StexEcha][i0Sample] += fT3d_distribs[i0StexEcha][i0Sample][i_event];
	    }
	  fT2d_ev[i0StexEcha][i0Sample] /= fNumberOfEvents;
	}
    }
  fTagMSp[0] = 1;        fFileHeader->fMSpCalc++;
}

//--------------------------------------------------------
//
//      Calculation of the sigmas of the samples
//                 for all the StexEchas
//
//--------------------------------------------------------
void TEcnaRun::SampleSigmas() 
{
//Calculation of the sigmas of the samples for all the StexEchas
  
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::SampleSigmas()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation: sample ADC sigmas over the events "
	 << " for each channel." << endl;}
  
  //... preliminary calculation of the expectation values if not done yet.
  //    The tag is set to 1 after call to the method. It is reset to 0
  //    because the expectation values must not be written in the result ROOT file
  //    (since the tag was equal to 0)
  if(fTagMSp[0] != 1){SampleMeans(); fTagMSp[0] = 0;}

  //................... Allocation fT2d_sig
  if( fT2d_sig == 0){
    Int_t n_StexEcha = fEcal->MaxCrysEcnaInStex();
    Int_t n_samp = fNbSampForCalc;
    fT2d_sig = new Double_t*[n_StexEcha];                fCnew++;        
    fT1d_sig = new  Double_t[n_StexEcha*n_samp];         fCnew++;  
    for(Int_t i0StexEcha = 0 ; i0StexEcha < n_StexEcha ; i0StexEcha++){
      fT2d_sig[i0StexEcha] = &fT1d_sig[0] + i0StexEcha*n_samp;}
  }
  // ................... init fT2d_sig to zero
  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      for(Int_t i0Sample=0; i0Sample<fNbSampForCalc; i0Sample++)
 	{
	  if( fT2d_sig[i0StexEcha][i0Sample] != (Double_t)0 )
	    {fMiscDiag[2]++; fT2d_sig[i0StexEcha][i0Sample] = (Double_t)0;}
 	} 
    }
  
  //................... Calculation
  for (Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
    {
      for (Int_t i0Sample = 0 ; i0Sample < fNbSampForCalc ; i0Sample++)
	{
	  Double_t variance = (Double_t)0.;
	  for( Int_t i_event = 0; i_event < fNumberOfEvents; i_event++ )
	    {
	      Double_t ecart = fT3d_distribs[i0StexEcha][i0Sample][i_event] - fT2d_ev[i0StexEcha][i0Sample];
	      variance += ecart*ecart;
	    }
	  variance /= fNumberOfEvents;
	  fT2d_sig[i0StexEcha][i0Sample] = sqrt(variance);
	}
    }
  fTagSSp[0] = 1;        fFileHeader->fSSpCalc++;
}

//====================================================================
//
//       C O V A R I A N C E S   &   C O R R E L A T I O N S
//
//                 B E T W E E N   S A M P L E S
//
//====================================================================
//-----------------------------------------------------------
//
//      Calculation of the covariances between samples
//      for all the StexEchas
//
//-----------------------------------------------------------
void TEcnaRun::CovariancesBetweenSamples()
{
  //Calculation of the covariances between samples for all the StexEchas
  
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::CovariancesBetweenSamples()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation: covariances between samples"
         << " for each channel." << endl;}
  
  //................... Allocations cov_ss
  if( fT3d_cov_ss == 0 ){
    const Int_t n_samp = fNbSampForCalc;
    const Int_t n_StexEcha = fEcal->MaxCrysEcnaInStex();
    fT3d_cov_ss  = new Double_t**[n_StexEcha];                fCnew++;  
    fT3d2_cov_ss = new  Double_t*[n_StexEcha*n_samp];         fCnew++;  
    fT3d1_cov_ss = new   Double_t[n_StexEcha*n_samp*n_samp];  fCnew++;  
    for(Int_t i = 0 ; i < n_StexEcha ; i++){
      fT3d_cov_ss[i] = &fT3d2_cov_ss[0] + i*n_samp;
      for(Int_t j = 0 ; j < n_samp ; j++){
	fT3d2_cov_ss[n_samp*i+j] = &fT3d1_cov_ss[0]+n_samp*(n_samp*i+j);}}
  }
  
  //.................. Calculation (= init)
  //.................. computation of half of the matrix, diagonal included)

  //... preliminary calculation of the expectation values if not done yet.
  //    The tag is set to 1 after call to the method. It is reset to 0
  //    because the expectation values must not be written in the result ROOT file
  //    (since the tag was equal to 0)
  //    Results in array  fT2d_ev[j0StexEcha][i0Sample]
  if(fTagMSp[0] != 1){SampleMeans(); fTagMSp[0] = 0;}


  for (Int_t j0StexEcha = 0 ; j0StexEcha < fEcal->MaxCrysEcnaInStex() ; j0StexEcha++)
    {
      for (Int_t i0Sample = 0 ; i0Sample < fNbSampForCalc ; i0Sample++)
	{
	  for (Int_t j0Sample = 0 ; j0Sample <= i0Sample; j0Sample++)
	    {
	      fT3d_cov_ss[j0StexEcha][i0Sample][j0Sample] = (Double_t)0;
	      for( Int_t i_event = 0; i_event < fNumberOfEvents; i_event++ )
		{
		  fT3d_cov_ss[j0StexEcha][i0Sample][j0Sample] +=
		    (fT3d_distribs[j0StexEcha][i0Sample][i_event] - fT2d_ev[j0StexEcha][i0Sample])
		    *(fT3d_distribs[j0StexEcha][j0Sample][i_event] - fT2d_ev[j0StexEcha][j0Sample]);
		}
	      fT3d_cov_ss[j0StexEcha][i0Sample][j0Sample] /= (Double_t)fNumberOfEvents;
	      fT3d_cov_ss[j0StexEcha][j0Sample][i0Sample] = fT3d_cov_ss[j0StexEcha][i0Sample][j0Sample];
	    }
	}
      fTagCovCss[j0StexEcha] = 1;     fFileHeader->fCovCssCalc++;
    }
}

//-----------------------------------------------------------
//
//      Calculation of the correlations between samples
//      for all the StexEchas
//
//-----------------------------------------------------------
void TEcnaRun::CorrelationsBetweenSamples()
{
//Calculation of the correlations between samples for all the StexEchas

  //... preliminary calculation of the covariances if not done yet.
  //    Test only the first tag since the cov are computed globaly
  //    but set all the tags to 0 because we don't want to write
  //    the covariances in the result ROOT file    
  if ( fTagCovCss[0] != 1 ){CovariancesBetweenSamples();
  for (Int_t j0StexEcha = 0 ; j0StexEcha < fEcal->MaxCrysEcnaInStex() ; j0StexEcha++)
    {fTagCovCss[j0StexEcha] = 0;}}
  
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::CorrelationsBetweenSamples()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation: correlations between samples"
	 << " for each channel." << endl;}

  //................... Allocations cor_ss
  if( fT3d_cor_ss == 0){
    const Int_t n_samp = fNbSampForCalc;
    const Int_t n_StexEcha = fEcal->MaxCrysEcnaInStex();
    fT3d_cor_ss  = new Double_t**[n_StexEcha];                fCnew++;  
    fT3d2_cor_ss = new  Double_t*[n_StexEcha*n_samp];         fCnew++;  
    fT3d1_cor_ss = new   Double_t[n_StexEcha*n_samp*n_samp];  fCnew++;  
    for(Int_t i = 0 ; i < n_StexEcha ; i++){
      fT3d_cor_ss[i] = &fT3d2_cor_ss[0] + i*n_samp;
      for(Int_t j = 0 ; j < n_samp ; j++){
	fT3d2_cor_ss[n_samp*i+j] = &fT3d1_cor_ss[0]+n_samp*(n_samp*i+j);}}
  }
  
  //..................... calculation of the correlations (=init)
  //......................computation of half of the matrix, diagonal included (verif = 1)
    
 for (Int_t j0StexEcha = 0 ; j0StexEcha < fEcal->MaxCrysEcnaInStex() ; j0StexEcha++)
   {
     for (Int_t i0Sample = 0 ; i0Sample < fNbSampForCalc ; i0Sample++)
       {
	 for (Int_t j0Sample = 0 ; j0Sample <= i0Sample ; j0Sample++)
	   {
	     if( (fT3d_cov_ss[j0StexEcha][i0Sample][i0Sample] > 0) &&
		 (fT3d_cov_ss[j0StexEcha][j0Sample][j0Sample] > 0) )
	       {
		 fT3d_cor_ss[j0StexEcha][i0Sample][j0Sample] = fT3d_cov_ss[j0StexEcha][i0Sample][j0Sample]/
		   ( sqrt(fT3d_cov_ss[j0StexEcha][i0Sample][i0Sample])*sqrt(fT3d_cov_ss[j0StexEcha][j0Sample][j0Sample]) );
	       }
	     else
	       {
		 (fT3d_cor_ss)[j0StexEcha][i0Sample][j0Sample] = (Double_t)0; // prevoir compteur + fTTBELL
	       }
	     fT3d_cor_ss[j0StexEcha][j0Sample][i0Sample] = fT3d_cor_ss[j0StexEcha][i0Sample][j0Sample];
	   }
       }
     fTagCorCss[j0StexEcha] = 1;          fFileHeader->fCorCssCalc++;
   }
}

//===========================================================================
//
//     M E A N   P E D E S T A L S ,   T O T A L    N O I S E ,
//     L O W     F R E Q U E N C Y    N O I S E , 
//     H I G H   F R E Q U E N C Y    N O I S E 
//     M E A N   O F  C O R ( S , S ),  S I G M A   O F  C O R ( S , S )
//
//===========================================================================
//-------------------------------------------------------------------------
//
//         Calculation of the Pedestals for each channel in Stex
//         tag: Ped
//      
//-------------------------------------------------------------------------
void TEcnaRun::Pedestals()
{
// Calculation, for each channel, of the expectation values
// (over the samples 0 to fNbSampForCalc-1) of the ADC expectation values 
// (over the events)

  //... preliminary calculation of the expectation values if not done yet
  if ( fTagMSp[0] != 1 ){SampleMeans(); fTagMSp[0]=0;}

  //................... Allocation ev_ev + init to zero (mandatory)
  if( fT1d_ev_ev == 0 ){fT1d_ev_ev = new Double_t[fEcal->MaxCrysEcnaInStex()]; fCnew++;}
  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {if( fT1d_ev_ev[i0StexEcha] != (Double_t)0 )
      {fMiscDiag[11]++; fT1d_ev_ev[i0StexEcha] = (Double_t)0;}}
  
  //..................... Calculation
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::Pedestals()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for all the channels, of the expectation values (over the samples 1 to "
	 << fNbSampForCalc << ")" << endl
	 << "          of the ADC expectation values (over the events)." << endl;}
  
  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {     
      for (Int_t i0Sample = 0; i0Sample < fNbSampForCalc; i0Sample++)
	{
	  fT1d_ev_ev[i0StexEcha] += fT2d_ev[i0StexEcha][i0Sample];
	}
      fT1d_ev_ev[i0StexEcha] /= fNbSampForCalc;
    }
  fTagPed[0] = 1;                      fFileHeader->fPedCalc++;
}
//------------------------ (end of Pedestals) ----------------------------

//-------------------------------------------------------------------------
//
//         Calculation of the TN (Total Noise)
//         tag: Tno
//      
//-------------------------------------------------------------------------
void TEcnaRun::TotalNoise()
{
// Calculation, for each channel, of the expectation values
// (over the samples 0 to fNbSampForCalc-1) of the sigmas 
// (over the events)

  //... preliminary calculation of the sigmas if not done yet
  if ( fTagSSp[0] != 1 ){SampleSigmas(); fTagSSp[0]=0;}

  //................... Allocation ev_ev + init to zero (mandatory)
  if( fT1d_evsamp_of_sigevt == 0 ){fT1d_evsamp_of_sigevt = new Double_t[fEcal->MaxCrysEcnaInStex()]; fCnew++;}
  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {if( fT1d_evsamp_of_sigevt[i0StexEcha] != (Double_t)0 )
      {fMiscDiag[12]++; fT1d_evsamp_of_sigevt[i0StexEcha] = (Double_t)0;}}
  
  //..................... Calculation
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::TotalNoise()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for all the channels, of the expectation values (over the samples 1 to "
	 << fNbSampForCalc << ")" << endl
	 << "          of the ADC expectation values (over the events)." << endl;}

  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {     
      for (Int_t i0Sample = 0; i0Sample < fNbSampForCalc; i0Sample++)
	{
	  if( fT2d_sig[i0StexEcha][i0Sample] < 0)
	    {
	      cout << "!TEcnaRun::TotalNoise() *** ERROR ***> Negative sigma!"
		   << fTTBELL << endl;
	    }
	  else
	    {
	      fT1d_evsamp_of_sigevt[i0StexEcha] += fT2d_sig[i0StexEcha][i0Sample];
	    }
	}
      fT1d_evsamp_of_sigevt[i0StexEcha] /= fNbSampForCalc;
    }
  fTagTno[0] = 1;                     fFileHeader->fTnoCalc++;
}
//------------------------ (end of TotalNoise) ----------------------------

//-------------------------------------------------------------------------
//
//      Calculation of the LFN  (Low Frequency Noise)
//      tag: Lfn
//
//-------------------------------------------------------------------------
void TEcnaRun::LowFrequencyNoise()
{
// Calculation, for each channel, of the sigma (over the events)
// of the ADC expectation values (over the samples 0 to fNbSampForCalc-1)

  //................... Allocation fT1d_sigevt_of_evsamp + init to zero (mandatory)
  if( fT1d_sigevt_of_evsamp == 0 ){
    fT1d_sigevt_of_evsamp = new Double_t[fEcal->MaxCrysEcnaInStex()];              fCnew++;  
  }
  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      if( fT1d_sigevt_of_evsamp[i0StexEcha] != (Double_t)0 )
	{fMiscDiag[13]++; fT1d_sigevt_of_evsamp[i0StexEcha] = (Double_t)0;}
    }

  //................... Allocation mean_over_samples
  TVectorD  mean_over_samples(fNumberOfEvents);
  for(Int_t i=0; i<fNumberOfEvents; i++){mean_over_samples(i)=(Double_t)0.;}

  //..................... Calculation
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::LowFrequencyNoise()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for each channel, of the sigma (over the events)" << endl
	 << "           of the ADC expectation values (over the samples 1 to "
	 <<  fNbSampForCalc << ")." << endl;}

  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      // Calculation of the mean over the events of the mean over the samples
      Double_t mean_over_events = (Double_t)0;
      for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
	{
	  // Calculation, for each event, of the mean over the samples  
	  mean_over_samples(n_event) = (Double_t)0.;
	  for(Int_t i0Sample=0; i0Sample<fNbSampForCalc; i0Sample++)
	    {
	      mean_over_samples(n_event) += fT3d_distribs[i0StexEcha][i0Sample][n_event];
	    }
	  mean_over_samples(n_event) /= (Double_t)fNbSampForCalc;

	  mean_over_events += mean_over_samples(n_event);
	}
       mean_over_events /= (Double_t)fNumberOfEvents;
       
       // Calculation of the sigma over the events of the mean over the samples
       Double_t var = (Double_t)0;
       for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
	 {
	  Double_t ecart =  mean_over_samples(n_event) - mean_over_events;
	  var += ecart*ecart;
	 }
       var /= (Double_t)fNumberOfEvents;
       
       fT1d_sigevt_of_evsamp[i0StexEcha] = sqrt(var);
    }
  fTagLfn[0] = 1;                      fFileHeader->fLfnCalc++;
}
//------------------------ (end of LowFrequencyNoise) ----------------------------

//-------------------------------------------------------------------------
//
//      Calculation of the HFN  (High Frequency Noise)
//      tag: Hfn
//
//-------------------------------------------------------------------------
void TEcnaRun::HighFrequencyNoise()
{
// Calculation, for each channel, of the mean (over the events)
// of the ADC sigmas (over the samples 0 to fNbSampForCalc-1)

  //................... Allocation fT1d_evevt_of_sigsamp + init to zero (mandatory)
  if( fT1d_evevt_of_sigsamp == 0 ){
    fT1d_evevt_of_sigsamp = new Double_t[fEcal->MaxCrysEcnaInStex()];              fCnew++;  
  }
  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      if( fT1d_evevt_of_sigsamp[i0StexEcha] != (Double_t)0 )
	{fMiscDiag[14]++; fT1d_evevt_of_sigsamp[i0StexEcha] = (Double_t)0;}
    }

  //................... Allocations mean_over_samples, sigma_over_sample
  TVectorD  mean_over_samples(fNumberOfEvents);
  for(Int_t i=0; i<fNumberOfEvents; i++){mean_over_samples(i)=(Double_t)0.;} 
  TVectorD  sigma_over_samples(fNumberOfEvents);
  for(Int_t i=0; i<fNumberOfEvents; i++){sigma_over_samples(i)=(Double_t)0.;}

  //..................... Calculation
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::HighFrequencyNoise()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for each channel, of the sigma (over the events)" << endl
	 << "           of the ADC expectation values (over the samples 1 to "
	 <<  fNbSampForCalc << ")." << endl;}

  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      //..................... Calculation of the sigma over samples
      for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
	{
	  // Calculation, for each event, of the mean over the samples  
	  mean_over_samples(n_event) = (Double_t)0.;
	  for(Int_t i0Sample=0; i0Sample<fNbSampForCalc; i0Sample++)
	    {mean_over_samples(n_event) += fT3d_distribs[i0StexEcha][i0Sample][n_event];}
	  mean_over_samples(n_event) /= (Double_t)fNbSampForCalc;
	  
	  // Calculation, for each event, of the sigma over the samples
	  Double_t var_over_samples =  (Double_t)0;
	  for(Int_t i0Sample=0; i0Sample<fNbSampForCalc; i0Sample++)
	    {Double_t deviation = fT3d_distribs[i0StexEcha][i0Sample][n_event] - mean_over_samples(n_event);
	    var_over_samples += deviation*deviation;}
	  var_over_samples /= (Double_t)fNbSampForCalc;
	  
	  if( var_over_samples < 0)
	    {cout << "!TEcnaRun::HighFrequencyNoise() *** ERROR ***> Negative variance! " << fTTBELL << endl;}
	  else
	    {sigma_over_samples(n_event) = sqrt(var_over_samples);}
	}

      //....... Calculation of the mean over the events of the sigma over samples
      for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
	{fT1d_evevt_of_sigsamp[i0StexEcha] += sigma_over_samples(n_event);}

      fT1d_evevt_of_sigsamp[i0StexEcha] /= (Double_t)fNumberOfEvents;
    }
  fTagHfn[0] = 1;                      fFileHeader->fHfnCalc++;
}
//------------------------ (end of HighFrequencyNoise) ----------------------------

//-------------------------------------------------------------------------
//
//      Calculation of the expectation values of (sample,sample)
//      correlations for all the channels (mean of cor(s,s))
//      tag: MeanCorss
//
//-------------------------------------------------------------------------
void TEcnaRun::MeanOfCorrelationsBetweenSamples()
{
  // Calculation, for all the channels, of the expectation values
  // of the correlations between the first fNbSampForCalc samples
  
  //... preliminary calculation of the correlations if not done yet
  //    (test only the first element since the cor are computed globaly)
  if ( fTagCorCss[0] != 1 ){CorrelationsBetweenSamples(); fTagCorCss[0]=0;}

  //................... Allocations ev_cor_ss + init to zero (mandatory)
  if( fT1d_ev_cor_ss == 0 ){
    Int_t n_StexEcha =  fEcal->MaxCrysEcnaInStex();
    fT1d_ev_cor_ss = new Double_t[n_StexEcha];               fCnew++;  
  }
  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      if( fT1d_ev_cor_ss[i0StexEcha] != (Double_t)0 )
	{fMiscDiag[15]++; fT1d_ev_cor_ss[i0StexEcha] = (Double_t)0;}
    }
  
  //.......... 1D array half_cor_ss[N(N-1)/2] to put the N (sample,sample) correlations
  //           ( half of (them minus the diagonal) ) 
  Int_t ndim = (Int_t)(fNbSampForCalc*(fNbSampForCalc - 1)/2);

  TVectorD  half_cor_ss(ndim); for(Int_t i=0; i<ndim; i++){half_cor_ss(i)=(Double_t)0.;}

  //..................... Calculation
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::MeanOfCorrelationsBetweenSamples()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for all the channels, of the expectation values of the" << endl
	 << "           correlations between the first " << fNbSampForCalc << " samples." << endl;}

  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      //..................... half_cor_ss() array filling
      Int_t i_count = 0;
      for (Int_t i0Sample = 0; i0Sample < fNbSampForCalc; i0Sample++)
	{
	  for (Int_t j0Sample = 0; j0Sample < i0Sample; j0Sample++)
	    {
	      half_cor_ss(i_count) = fT3d_cor_ss[i0StexEcha][i0Sample][j0Sample];
	      i_count++;
	    }
	}
      //...................... mean of cor(s,s') calculation
      fT1d_ev_cor_ss[i0StexEcha] = (Double_t)0;
      for(Int_t i_rcor = 0; i_rcor < ndim; i_rcor++)
	{
	  fT1d_ev_cor_ss[i0StexEcha] += half_cor_ss(i_rcor);
	}
      fT1d_ev_cor_ss[i0StexEcha] /= (Double_t)ndim;
    }
  fTagMeanCorss[0] = 1;               fFileHeader->fMeanCorssCalc++;
}
//--------------- (end of MeanOfCorrelationsBetweenSamples) -----------

//-------------------------------------------------------------------------
//
//      Calculation of the sigmas of the (sample,sample) correlations
//      for all the channels (sigma of cor(s,s))
//      tag: SigCorss
//
//--------------------------------------------------------------------------
void  TEcnaRun::SigmaOfCorrelationsBetweenSamples()
{
  //Calculation of the sigmas of the (sample,sample) correlations for all the StexEchas
 
  //... preliminary calculation of the mean of cor(s,s') if not done yet
  //    (test only the first element since the cor are computed globaly)
  //    Results available in array fT1d_ev_cor_ss[i0StexEcha]
  if ( fTagMeanCorss[0] != 1 ){MeanOfCorrelationsBetweenSamples(); fTagMeanCorss[0]=0;}

  //................... Allocations sig_cor_ss + init to zero
  if( fT1d_sig_cor_ss == 0 ){
    Int_t n_StexEcha =  fEcal->MaxCrysEcnaInStex();
    fT1d_sig_cor_ss = new Double_t[n_StexEcha];                fCnew++;  
  }
  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      if( fT1d_sig_cor_ss[i0StexEcha] != (Double_t)0 )
	{fMiscDiag[16]++; fT1d_sig_cor_ss[i0StexEcha] = (Double_t)0;}
    }

  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::SigmasOfCorrelationsBetweenSamples()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation of the sigmas of the (sample,sample)" << endl
	 << "           correlations for all the channels." << endl;}

  //.......... 1D array half_cor_ss[N(N-1)/2] to put the N (sample,sample) correlations
  //           (half of them minus the diagonal)
  Int_t ndim = (Int_t)(fNbSampForCalc*(fNbSampForCalc - 1)/2);

  TVectorD  half_cor_ss(ndim); for(Int_t i=0; i<ndim; i++){half_cor_ss(i)=(Double_t)0.;}

  //.................. Calculation
  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      //..................... half_cor_ss() array filling
      Int_t i_count = 0;
      for (Int_t i0Sample = 0; i0Sample < fNbSampForCalc; i0Sample++)
	{
	  for (Int_t j0Sample = 0; j0Sample < i0Sample; j0Sample++)
	    {
	      half_cor_ss(i_count) = fT3d_cor_ss[i0StexEcha][i0Sample][j0Sample];
	      i_count++;
	    }
	}

      //...................... sigma of cor(s,s') calculation
      Double_t var = (Double_t)0;
      for(Int_t i_rcor = 0; i_rcor < ndim; i_rcor++)
	{
	  Double_t ecart = half_cor_ss(i_rcor) - fT1d_ev_cor_ss[i0StexEcha];
	  var += ecart*ecart;
	}
      var /= (Double_t)ndim;
      fT1d_sig_cor_ss[i0StexEcha] = sqrt(var);
    }
  fTagSigCorss[0] = 1;              fFileHeader->fSigCorssCalc++;
}
//--------------- (end of SigmaOfCorrelationsBetweenSamples) -----------

//-----------------------------------------------------------------------------
//
//  Calculation of the average of the Pedestals for each Stin in Stex
//  tag: AvPed
//      
//-----------------------------------------------------------------------------
void TEcnaRun::AveragedPedestals()
{
// Calculation of the average 
// (over the Stin's 0 to fEcal->MaxStinInStex()) of the Pedestals

  //... preliminary calculation of the Pedestals if not done yet
  if ( fTagPed[0] != 1 ){Pedestals(); fTagPed[0]=0;}
  //................... Allocation av_mped + init to zero (mandatory)
  if( fT1d_av_mped == 0 ){fT1d_av_mped = new Double_t[fEcal->MaxStinEcnaInStex()]; fCnew++;}
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {if( fT1d_av_mped[i0StexStinEcna] != (Double_t)0 )
      {fMiscDiag[41]++; fT1d_av_mped[i0StexStinEcna] = (Double_t)0;}}
 
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::AveragedPedestals()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for all the "
	 << fStinName.Data() << "s, of the average of the Pedestals" << endl;}

  //................... Calculation
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {
      Int_t n1StexStinEcna = i0StexStinEcna+1;
      fT1d_av_mped[i0StexStinEcna] = (Double_t)0;
      for(Int_t i0StinEcha = 0; i0StinEcha < fEcal->MaxCrysInStin(); i0StinEcha++)
	{
	  Int_t i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(n1StexStinEcna, i0StinEcha);

	  if( fStexName == "SM " )
	    {fT1d_av_mped[i0StexStinEcna] += fT1d_ev_ev[i0StexEcha];}

	  if( fStexName == "Dee" )
	    {
	      //---------------- Special translation for mixed SCEcna (29 and 32)
	      //                 Xtal 11 of SCEcna 29 -> Xtal 11 of SCEcna 10
	      //                 Xtal 11 of SCEcna 32 -> Xtal 11 of SCEcna 11
	      Int_t n1StinEcha = i0StinEcha+1;
	      if( n1StexStinEcna == 10 && n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(29, i0StinEcha);}
	      if( n1StexStinEcna == 11 && n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(32, i0StinEcha);}
	      if( !( (n1StexStinEcna == 29 || n1StexStinEcna == 32) && n1StinEcha == 11 )  )
		{fT1d_av_mped[i0StexStinEcna] += fT1d_ev_ev[i0StexEcha];}
	    }
	}
      Double_t xdivis = (Double_t)0.;
      if( fStexName == "SM "  )
	{xdivis = (Double_t)fEcal->MaxCrysInStin();}
      if( fStexName == "Dee" )
	{xdivis = (Double_t)fEcalNumbering->MaxCrysInStinEcna(fFileHeader->fStex, n1StexStinEcna, "TEcnaRun");}
      
      fT1d_av_mped[i0StexStinEcna] = fT1d_av_mped[i0StexStinEcna]/xdivis;
    }
  
  fTagAvPed[0] = 1;                      fFileHeader->fAvPedCalc++;
}
//-----------------------------------------------------------------------------
//
// Calculation of the average of the Total noise for each Stin in Stex
// tag: AvTno
//      
//-----------------------------------------------------------------------------
void TEcnaRun::AveragedTotalNoise()
{
// Calculation of the average 
// (over the Stin's 0 to fEcal->MaxStinInStex()) of the Total Noise
  
  //... preliminary calculation of the averaged Total Noise if not done yet
  if ( fTagTno[0] != 1 ){TotalNoise(); fTagTno[0]=0;}
  //................... Allocation av_totn + init to zero (mandatory)
  if( fT1d_av_totn == 0 ){fT1d_av_totn = new Double_t[fEcal->MaxStinEcnaInStex()]; fCnew++;}
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {if( fT1d_av_totn[i0StexStinEcna] != (Double_t)0 )
      {fMiscDiag[42]++; fT1d_av_totn[i0StexStinEcna] = (Double_t)0;}}

  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::AveragedTotalNoise()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for all the "
	 << fStinName.Data() << "s, of the average of the Total Noise" << endl;}

  //................... Calculation
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {
      Int_t n1StexStinEcna = i0StexStinEcna+1;
      fT1d_av_totn[i0StexStinEcna] = (Double_t)0;
      for(Int_t i0StinEcha = 0; i0StinEcha < fEcal->MaxCrysInStin(); i0StinEcha++)
	{
	  Int_t i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(n1StexStinEcna, i0StinEcha);
	  
	  if( fStexName == "SM " )
	    {fT1d_av_totn[i0StexStinEcna] += fT1d_evsamp_of_sigevt[i0StexEcha];}
	  
	  if( fStexName == "Dee" )
	    {
	      //---------------- Special translation for mixed SCEcna (29 and 32)
	      //                 Xtal 11 of SCEcna 29 -> Xtal 11 of SCEcna 10
	      //                 Xtal 11 of SCEcna 32 -> Xtal 11 of SCEcna 11
	      Int_t n1StinEcha = i0StinEcha+1;
	      if( n1StexStinEcna == 10 && n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(29, i0StinEcha);}
	      if( n1StexStinEcna == 11 && n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(32, i0StinEcha);}
	      if( !( (n1StexStinEcna == 29 || n1StexStinEcna == 32) && n1StinEcha == 11 )  )
		{fT1d_av_totn[i0StexStinEcna] += fT1d_evsamp_of_sigevt[i0StexEcha];}
	    }
	}
      Double_t xdivis = (Double_t)0.;
      if( fStexName == "SM "  )
	{xdivis = (Double_t)fEcal->MaxCrysInStin();}
      if( fStexName == "Dee" )
	{xdivis = (Double_t)fEcalNumbering->MaxCrysInStinEcna(fFileHeader->fStex, n1StexStinEcna, "TEcnaRun");}
      
      fT1d_av_totn[i0StexStinEcna] = fT1d_av_totn[i0StexStinEcna]/xdivis;
    }
  fTagAvTno[0] = 1;                      fFileHeader->fAvTnoCalc++;
}
//-----------------------------------------------------------------------------
//
// Calculation of the average of the Low Frequency noise for each Stin in Stex
// tag: AvLfn
//      
//-----------------------------------------------------------------------------
void TEcnaRun::AveragedLowFrequencyNoise()
{
// Calculation of the average 
// (over the Stin's 0 to fEcal->MaxStinInStex()) of the Low Frequency Noise
  
  //... preliminary calculation of the Low Frequency Noise if not done yet
  if ( fTagLfn[0] != 1 ){LowFrequencyNoise(); fTagLfn[0]=0;}
  //................... Allocation av_lofn + init to zero (mandatory)
  if( fT1d_av_lofn == 0 ){fT1d_av_lofn = new Double_t[fEcal->MaxStinEcnaInStex()]; fCnew++;}
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {if( fT1d_av_lofn[i0StexStinEcna] != (Double_t)0 )
      {fMiscDiag[43]++; fT1d_av_lofn[i0StexStinEcna] = (Double_t)0;}}

  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::AveragedLowFrequencyNoise()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for all the "
	 << fStinName.Data() << "s, of the average of the Low Frequency Noise" << endl;}

  //................... Calculation
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {
      Int_t n1StexStinEcna = i0StexStinEcna+1;
      fT1d_av_lofn[i0StexStinEcna] = (Double_t)0;
      for(Int_t i0StinEcha = 0; i0StinEcha < fEcal->MaxCrysInStin(); i0StinEcha++)
	{
	  Int_t i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(n1StexStinEcna, i0StinEcha);

	  if( fStexName == "SM " )
	    {fT1d_av_lofn[i0StexStinEcna] += fT1d_sigevt_of_evsamp[i0StexEcha];}
	  
	  if( fStexName == "Dee" )
	    {
	      //---------------- Special translation for mixed SCEcna (29 and 32)
	      //                 Xtal 11 of SCEcna 29 -> Xtal 11 of SCEcna 10
	      //                 Xtal 11 of SCEcna 32 -> Xtal 11 of SCEcna 11
	      Int_t n1StinEcha = i0StinEcha+1;
	      if( n1StexStinEcna == 10 &&  n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(29, i0StinEcha);}
	      if( n1StexStinEcna == 11 &&  n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(32, i0StinEcha);}
	      if( !( (n1StexStinEcna == 29 || n1StexStinEcna == 32) && n1StinEcha == 11 )  )
		{fT1d_av_lofn[i0StexStinEcna] += fT1d_sigevt_of_evsamp[i0StexEcha];}
	    }
	}
      Double_t xdivis = (Double_t)0.;
      if( fStexName == "SM "  )
	{xdivis = (Double_t)fEcal->MaxCrysInStin();}
      if( fStexName == "Dee" )
	{xdivis = (Double_t)fEcalNumbering->MaxCrysInStinEcna(fFileHeader->fStex, n1StexStinEcna, "TEcnaRun");}
      
      fT1d_av_lofn[i0StexStinEcna] = fT1d_av_lofn[i0StexStinEcna]/xdivis;
    }
  fTagAvLfn[0] = 1;                      fFileHeader->fAvLfnCalc++; 
}
//-----------------------------------------------------------------------------
//
// Calculation of the average of the high frequency noise for each Stin in Stex
// tag: AvHfn
//      
//-----------------------------------------------------------------------------
void TEcnaRun::AveragedHighFrequencyNoise()
{
// Calculation of the average 
// (over the Stin's 0 to fEcal->MaxStinInStex()) of the High Frequency Noise
  
  //... preliminary calculation of the High Frequency Noise if not done yet
  if ( fTagHfn[0] != 1 ){HighFrequencyNoise(); fTagHfn[0]=0;}
  //................... Allocation av_hifn + init to zero (mandatory)
  if( fT1d_av_hifn == 0 ){fT1d_av_hifn = new Double_t[fEcal->MaxStinEcnaInStex()]; fCnew++;}
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {if( fT1d_av_hifn[i0StexStinEcna] != (Double_t)0 )
      {fMiscDiag[44]++; fT1d_av_hifn[i0StexStinEcna] = (Double_t)0;}}

  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::AveragedHighFrequencyNoise()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for all the "
	 << fStinName.Data() << "s, of the average of the High Frequency Noise" << endl;}

  //................... Calculation
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {   
      Int_t n1StexStinEcna = i0StexStinEcna+1;
      fT1d_av_hifn[i0StexStinEcna] = (Double_t)0;
      for(Int_t i0StinEcha = 0; i0StinEcha < fEcal->MaxCrysInStin(); i0StinEcha++)
	{
	  Int_t i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(n1StexStinEcna, i0StinEcha);

	  if( fStexName == "SM " )
	    {fT1d_av_hifn[i0StexStinEcna] += fT1d_evevt_of_sigsamp[i0StexEcha];}
	  
	  if( fStexName == "Dee" )
	    {
	      //---------------- Special translation for mixed SCEcna (29 and 32)
	      //                 Xtal 11 of SCEcna 29 -> Xtal 11 of SCEcna 10
	      //                 Xtal 11 of SCEcna 32 -> Xtal 11 of SCEcna 11
	      Int_t n1StinEcha = i0StinEcha+1;
	      if( n1StexStinEcna == 10 &&  n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(29, i0StinEcha);}
	      if( n1StexStinEcna == 11 &&  n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(32, i0StinEcha);}
	      if( !( (n1StexStinEcna == 29 || n1StexStinEcna == 32) && n1StinEcha == 11 )  )
		{fT1d_av_hifn[i0StexStinEcna] += fT1d_evevt_of_sigsamp[i0StexEcha];}
	    }
	}
      Double_t xdivis = (Double_t)0.;
      if( fStexName == "SM "  )
	{xdivis = (Double_t)fEcal->MaxCrysInStin();}
      if( fStexName == "Dee" )
	{xdivis = (Double_t)fEcalNumbering->MaxCrysInStinEcna(fFileHeader->fStex, n1StexStinEcna, "TEcnaRun");}

      fT1d_av_hifn[i0StexStinEcna] = fT1d_av_hifn[i0StexStinEcna]/xdivis;
    }
  fTagAvHfn[0] = 1;                      fFileHeader->fAvHfnCalc++;  
}
//-----------------------------------------------------------------------------
//
// Calculation of the average of the mean of cor(s,s) for each Stin in Stex
// tag: AvMeanCorss
//      
//-----------------------------------------------------------------------------
void TEcnaRun::AveragedMeanOfCorrelationsBetweenSamples()
{
// Calculation of the average 
// (over the Stin's 0 to fEcal->MaxStinInStex()) of the mean of cor(s,s)
  
  //... preliminary calculation of the mean of cor(s,s) if not done yet
  if ( fTagMeanCorss[0] != 1 ){MeanOfCorrelationsBetweenSamples(); fTagMeanCorss[0]=0;}
  //................... Allocation av_ev_corss + init to zero (mandatory)
  if( fT1d_av_ev_corss == 0 ){fT1d_av_ev_corss = new Double_t[fEcal->MaxStinEcnaInStex()]; fCnew++;}
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {if( fT1d_av_ev_corss[i0StexStinEcna] != (Double_t)0 )
      {fMiscDiag[45]++; fT1d_av_ev_corss[i0StexStinEcna] = (Double_t)0;}}

  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::AveragedMeanOfCorrelationsBetweenSamples()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for all the "
	 << fStinName.Data() << "s, of the average of the mean of cor(s,s)" << endl;}

  //................... Calculation
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {
      Int_t n1StexStinEcna = i0StexStinEcna+1;
      fT1d_av_ev_corss[i0StexStinEcna] = (Double_t)0;
      for(Int_t i0StinEcha = 0; i0StinEcha < fEcal->MaxCrysInStin(); i0StinEcha++)
	{
	  Int_t i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(n1StexStinEcna, i0StinEcha);

	  if( fStexName == "SM " )
	    {fT1d_av_ev_corss[i0StexStinEcna] += fT1d_ev_cor_ss[i0StexEcha];}
	  
	  if( fStexName == "Dee" )
	    {
	      //---------------- Special translation for mixed SCEcna (29 and 32)
	      //                 Xtal 11 of SCEcna 29 -> Xtal 11 of SCEcna 10
	      //                 Xtal 11 of SCEcna 32 -> Xtal 11 of SCEcna 11
	      Int_t n1StinEcha = i0StinEcha+1;
	      if( n1StexStinEcna == 10 &&  n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(29, i0StinEcha);}
	      if( n1StexStinEcna == 11 &&  n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(32, i0StinEcha);}
	      if( !( (n1StexStinEcna == 29 || n1StexStinEcna == 32) && n1StinEcha == 11 )  )
		{fT1d_av_ev_corss[i0StexStinEcna] += fT1d_ev_cor_ss[i0StexEcha];}
	    }
	}
      Double_t xdivis = (Double_t)0.;
      if( fStexName == "SM "  )
	{xdivis = (Double_t)fEcal->MaxCrysInStin();}
      if( fStexName == "Dee" )
	{xdivis = (Double_t)fEcalNumbering->MaxCrysInStinEcna(fFileHeader->fStex, n1StexStinEcna, "TEcnaRun");}

      fT1d_av_ev_corss[i0StexStinEcna] = fT1d_av_ev_corss[i0StexStinEcna]/xdivis;
    }
  fTagAvMeanCorss[0] = 1;                      fFileHeader->fAvMeanCorssCalc++;    
}
//-----------------------------------------------------------------------------
//
// Calculation of the average of the sigma of cor(s,s) for each Stin in Stex
// tag: AvSigCorss
//      
//-----------------------------------------------------------------------------
void TEcnaRun::AveragedSigmaOfCorrelationsBetweenSamples()
{
// Calculation of the average 
// (over the Stin's 0 to fEcal->MaxStinInStex()) of the sigma of cor(s,s)
  
  //... preliminary calculation of the sigma of cor(s,s) if not done yet
  if ( fTagSigCorss[0] != 1 ){SigmaOfCorrelationsBetweenSamples(); fTagSigCorss[0]=0;}
  //................... Allocation av_sig_corss + init to zero (mandatory)
  if( fT1d_av_sig_corss == 0 ){fT1d_av_sig_corss = new Double_t[fEcal->MaxStinEcnaInStex()]; fCnew++;}
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {if( fT1d_av_sig_corss[i0StexStinEcna] != (Double_t)0 )
      {fMiscDiag[46]++; fT1d_av_sig_corss[i0StexStinEcna] = (Double_t)0;}}

  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::AveragedSigmaOfCorrelationsBetweenSamples()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for all the "
	 << fStinName.Data() << "s, of the average of the sigma of cor(s,s)" << endl;}

  //................... Calculation
  for(Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {
      Int_t n1StexStinEcna = i0StexStinEcna+1;
      fT1d_av_sig_corss[i0StexStinEcna] = (Double_t)0;
      for(Int_t i0StinEcha = 0; i0StinEcha < fEcal->MaxCrysInStin(); i0StinEcha++)
	{
	  Int_t i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(n1StexStinEcna, i0StinEcha);

	  if( fStexName == "SM " )
	    {fT1d_av_sig_corss[i0StexStinEcna] += fT1d_sig_cor_ss[i0StexEcha];}
	  
	  if( fStexName == "Dee" )
	    {
	      //---------------- Special translation for mixed SCEcna (29 and 32)
	      //                 Xtal 11 of SCEcna 29 -> Xtal 11 of SCEcna 10
	      //                 Xtal 11 of SCEcna 32 -> Xtal 11 of SCEcna 11
	      Int_t n1StinEcha = i0StinEcha+1;
	      if( n1StexStinEcna == 10 &&  n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(29, i0StinEcha);}
	      if( n1StexStinEcna == 11 &&  n1StinEcha == 11 )
		{i0StexEcha = fEcalNumbering->Get0StexEchaFrom1StexStinAnd0StinEcha(32, i0StinEcha);}
	      if( !( (n1StexStinEcna == 29 || n1StexStinEcna == 32) && n1StinEcha == 11 )  )
		{fT1d_av_sig_corss[i0StexStinEcna] += fT1d_sig_cor_ss[i0StexEcha];}
	    }
	}
      Double_t xdivis = (Double_t)0.;
      if( fStexName == "SM "  )
	{xdivis = (Double_t)fEcal->MaxCrysInStin();}
      if( fStexName == "Dee" )
	{xdivis = (Double_t)fEcalNumbering->MaxCrysInStinEcna(fFileHeader->fStex, n1StexStinEcna, "TEcnaRun");}

      fT1d_av_sig_corss[i0StexStinEcna] = fT1d_av_sig_corss[i0StexStinEcna]/xdivis;
    }
  fTagAvSigCorss[0] = 1;                      fFileHeader->fAvSigCorssCalc++;       
}

//====================================================================
//
//       C O V A R I A N C E S   &   C O R R E L A T I O N S
//
//                 B E T W E E N   C H A N N E L S
//
//====================================================================
//------------------------------------------------------------------
//
//  Calculation of the Low Frequency Covariances between channels
//
//------------------------------------------------------------------
void TEcnaRun::LowFrequencyCovariancesBetweenChannels()
{
//Calculation of the Low Frequency Covariances between channels
  
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::LowFrequencyCovariancesBetweenChannels()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation of the Low Frequency Covariances between channels" << endl;}

  //................. allocation fT2d_lf_cov + init to zero (mandatory)
  if( fT2d_lf_cov == 0 ){
    const Int_t n_StexEcha = fEcal->MaxCrysEcnaInStex();
    fT2d_lf_cov  = new Double_t*[n_StexEcha];                   fCnew++;
    fT2d1_lf_cov = new  Double_t[n_StexEcha*n_StexEcha];          fCnew++;
    for(Int_t i0StexEcha = 0 ; i0StexEcha < n_StexEcha ; i0StexEcha++){
	fT2d_lf_cov[i0StexEcha] = &fT2d1_lf_cov[0] + i0StexEcha*n_StexEcha;}}

  for (Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
    {
      for (Int_t j0StexEcha = 0 ; j0StexEcha < fEcal->MaxCrysEcnaInStex() ; j0StexEcha++)
  	{
	  if( fT2d_lf_cov[i0StexEcha][j0StexEcha] != (Double_t)0 )
	    {fMiscDiag[21]++; fT2d_lf_cov[i0StexEcha][j0StexEcha] = (Double_t)0;}
  	}
    }
  //........................................... Calculation  (LowFrequencyCovariancesBetweenChannels)
  //................... Allocation mean_over_samples(i0StexEcha, n_event)
  TMatrixD mean_over_samples(fEcal->MaxCrysEcnaInStex(), fNumberOfEvents);
  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {for(Int_t n_event=0; n_event<fNumberOfEvents; n_event++)
	{mean_over_samples(i0StexEcha, n_event)=(Double_t)0.;}}
  //................... Allocation MoeOfMos(i0StexEcha)
  TVectorD MoeOfMos(fEcal->MaxCrysEcnaInStex());
  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    { MoeOfMos(i0StexEcha)=(Double_t)0.;}

  //................... Calculation  
  if(fFlagPrint != fCodePrintNoComment){
    cout << "          Calculation, for each pair of channels, of the covariance (over the events)" << endl
	 << "          between the ADC expectation values (over the samples 1 to "
	 <<  fNbSampForCalc << ")." << endl;}

  cout << " Please, wait (end at i= " << fEcal->MaxCrysEcnaInStex() << "): " << endl;

  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      MoeOfMos(i0StexEcha) = (Double_t)0;

      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, i0StexEcha) > 0)
	  || ( fFlagSubDet == "EB") )
	{
	  for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
	    {
	      // Calculation, for each event, of the mean over the samples  ( = E_s[A(c_i,s*,e_n] )
	      mean_over_samples(i0StexEcha, n_event) = (Double_t)0.;
	      for(Int_t i0Sample=0; i0Sample<fNbSampForCalc; i0Sample++)
		{
		  mean_over_samples(i0StexEcha, n_event) += fT3d_distribs[i0StexEcha][i0Sample][n_event];
		}
	      mean_over_samples(i0StexEcha, n_event) /= (Double_t)fNbSampForCalc;
	    }
	  //Calculation of the mean over the events of E_s[A(c_i,s*,e_n] ( = E_e[E_s[A(c_i,s*,e*]] )
	  for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
	    {MoeOfMos(i0StexEcha) += mean_over_samples(i0StexEcha, n_event);}
	  MoeOfMos(i0StexEcha) /= (Double_t)fNumberOfEvents;
	}
    }

  //... Calculation of half of the matrix, diagonal included        (LowFrequencyCovariancesBetweenChannels)
  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, i0StexEcha) > 0)
	  || ( fFlagSubDet == "EB") )
	{
	  for(Int_t j0StexEcha = 0; j0StexEcha <= i0StexEcha; j0StexEcha++)
	    {
	      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, j0StexEcha) > 0)
		  || ( fFlagSubDet == "EB") )
		{
		  fT2d_lf_cov[i0StexEcha][j0StexEcha] = (Double_t)0;
		  for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
		    {
		      fT2d_lf_cov[i0StexEcha][j0StexEcha] +=
			( mean_over_samples(i0StexEcha, n_event) - MoeOfMos(i0StexEcha) )
			*( mean_over_samples(j0StexEcha, n_event) - MoeOfMos(j0StexEcha) );    
		    }
		  fT2d_lf_cov[i0StexEcha][j0StexEcha] /= (Double_t)fNumberOfEvents;
		  
		  fT2d_lf_cov[j0StexEcha][i0StexEcha] = fT2d_lf_cov[i0StexEcha][j0StexEcha];
		}
	    }
	  if( i0StexEcha%100 == 0 ){cout << i0StexEcha << "[LFN Cov], ";}
	}
    }
  cout << endl;
  fTagLfCov[0] = 1;    fFileHeader->fLfCovCalc++;
}
//---------- (end of LowFrequencyCovariancesBetweenChannels ) --------------------

//------------------------------------------------------------------
//
//  Calculation of the Low Frequency Correlations between channels
//
//------------------------------------------------------------------
void TEcnaRun::LowFrequencyCorrelationsBetweenChannels()
{
//Calculation of the Low Frequency Correlations between channels

  //... preliminary calculation of the covariances if not done yet.
  if ( fTagLfCov[0] != 1 )
    {LowFrequencyCovariancesBetweenChannels(); fTagLfCov[0] = 0;}
  
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::LowFrequencyCorrelationsBetweenChannels()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "          Calculation of the Low Frequency Correlations between channels" << endl
	 << "          Starting allocation. "
	 << endl;}

  //................. allocation fT2d_lf_cor + init to zero (mandatory)
  if( fT2d_lf_cor == 0 ){
    const Int_t n_StexEcha = fEcal->MaxCrysEcnaInStex();
    fT2d_lf_cor  = new Double_t*[n_StexEcha];                   fCnew++;
    fT2d1_lf_cor = new  Double_t[n_StexEcha*n_StexEcha];        fCnew++;
    for(Int_t i0StexEcha = 0 ; i0StexEcha < n_StexEcha ; i0StexEcha++){
	fT2d_lf_cor[i0StexEcha] = &fT2d1_lf_cor[0] + i0StexEcha*n_StexEcha;} 
  }
  
  for (Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
    {
      for (Int_t j0StexEcha = 0 ; j0StexEcha < fEcal->MaxCrysEcnaInStex() ; j0StexEcha++)
	{
	  if( fT2d_lf_cor[i0StexEcha][j0StexEcha] != (Double_t)0 )
	    {fMiscDiag[22]++; fT2d_lf_cor[i0StexEcha][j0StexEcha] = (Double_t)0;}
	}
    }
  
  //................. calculation  
  //........................... computation of half of the matrix, diagonal included
  for (Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
    {
      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, i0StexEcha) > 0)
	  || ( fFlagSubDet == "EB") )
	{
	  for (Int_t j0StexEcha = 0 ; j0StexEcha <= i0StexEcha ; j0StexEcha++)
	    {
	      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, j0StexEcha) > 0)
		  || ( fFlagSubDet == "EB") )
		{
		  if(  fT2d_lf_cov[i0StexEcha][i0StexEcha] > 0
		       && fT2d_lf_cov[j0StexEcha][j0StexEcha] > 0 )
		    {
		      fT2d_lf_cor[i0StexEcha][j0StexEcha] =
			fT2d_lf_cov[i0StexEcha][j0StexEcha]/
			( (Double_t)sqrt( fT2d_lf_cov[i0StexEcha][i0StexEcha]*
					  fT2d_lf_cov[j0StexEcha][j0StexEcha] ) );
		    }
		  else
		    {
		      fT2d_lf_cor[i0StexEcha][j0StexEcha] = (Double_t)0.;
		    }
		  fT2d_lf_cor[j0StexEcha][i0StexEcha] = fT2d_lf_cor[i0StexEcha][j0StexEcha];
		}
	    }
	}
      if( i0StexEcha%100 == 0 ){cout << i0StexEcha << "[LFN Cor], ";}
    }
  cout << endl;
  
  fTagLfCor[0] = 1;    fFileHeader->fLfCorCalc++;
}
//--------------- (end of LowFrequencyCorrelationsBetweenChannels) --------------------

//------------------------------------------------------------------
//
//  Calculation of the High Frequency Covariances between channels
//
//------------------------------------------------------------------
void TEcnaRun::HighFrequencyCovariancesBetweenChannels()
{
//Calculation of the High Frequency Covariances between channels
  
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::HighFrequencyCovariancesBetweenChannels()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation of the High Frequency Covariances between channels" << endl;}

  //................. allocation fT2d_hf_cov + init to zero (mandatory)
  if( fT2d_hf_cov == 0 ){
    const Int_t n_StexEcha = fEcal->MaxCrysEcnaInStex();
    fT2d_hf_cov   = new Double_t*[n_StexEcha];                   fCnew++;
    fT2d1_hf_cov  = new  Double_t[n_StexEcha*n_StexEcha];        fCnew++;
    for(Int_t i0StexEcha = 0 ; i0StexEcha < n_StexEcha ; i0StexEcha++){
	fT2d_hf_cov[i0StexEcha] = &fT2d1_hf_cov[0] + i0StexEcha*n_StexEcha;} 
  }

  for (Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
    {
      for (Int_t j0StexEcha = 0 ; j0StexEcha < fEcal->MaxCrysEcnaInStex() ; j0StexEcha++)
  	{
	  if( fT2d_hf_cov[i0StexEcha][j0StexEcha] != (Double_t)0 )
	    {fMiscDiag[23]++; fT2d_hf_cov[i0StexEcha][j0StexEcha] = (Double_t)0;}
  	}
    }

  //................... Allocation mean_over_samples(i0StexEcha, n_event) 
  TMatrixD mean_over_samples(fEcal->MaxCrysEcnaInStex(), fNumberOfEvents);
  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {for(Int_t n_event=0; n_event<fNumberOfEvents; n_event++)
	{mean_over_samples(i0StexEcha, n_event)=(Double_t)0.;}}
  //................... Allocation cov_over_samp(i0StexEcha,j0StexEcha)
  TMatrixD cov_over_samp(fEcal->MaxCrysEcnaInStex(), fEcal->MaxCrysEcnaInStex());
  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {for(Int_t j0StexEcha=0; j0StexEcha<fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{cov_over_samp(i0StexEcha, j0StexEcha)=(Double_t)0.;}}

  //........................................... Calculation    (HighFrequencyCovariancesBetweenChannels)
  if(fFlagPrint != fCodePrintNoComment){
    cout << "          Calculation of the mean (over the events)" << endl
	 << "          of the covariances between the channels (over the samples 1 to "
	 <<  fNbSampForCalc << ")." << endl;}

  cout << " Please, wait (end at i= " << fEcal->MaxCrysEcnaInStex() << "): " << endl;

  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, i0StexEcha) > 0)
	  || ( fFlagSubDet == "EB") )
	{
	  for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
	    {
	      // Calculation, for each event, of the mean over the samples  ( = E_s[A(c_i,s*,e_n] )
	      mean_over_samples(i0StexEcha, n_event) = (Double_t)0.;
	      for(Int_t i0Sample=0; i0Sample<fNbSampForCalc; i0Sample++)
		{mean_over_samples(i0StexEcha, n_event) += fT3d_distribs[i0StexEcha][i0Sample][n_event];}
	      mean_over_samples(i0StexEcha, n_event) /= (Double_t)fNbSampForCalc;
	    }
	}
      if( i0StexEcha%100 == 0 ){cout << i0StexEcha << "[HFNa Cov], ";}
    }
  cout << endl;

  cout << " Please, wait (end at i= " << fEcal->MaxCrysEcnaInStex() << "): " << endl;

  //... Calculation of half of the matrix, diagonal included    (HighFrequencyCovariancesBetweenChannels)
  for(Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {
      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, i0StexEcha) > 0)
	  || ( fFlagSubDet == "EB") )
	{
	  for(Int_t j0StexEcha = 0; j0StexEcha <= i0StexEcha; j0StexEcha++)
	    {
	      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, j0StexEcha) > 0)
		  || ( fFlagSubDet == "EB") )
		{
		  for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
		    {
		      // Calculation, for each event, of the covariance over the samples
		      cov_over_samp(i0StexEcha,j0StexEcha) = (Double_t)0;
		      for(Int_t i0Sample=0; i0Sample<fNbSampForCalc; i0Sample++)
			{
			  cov_over_samp(i0StexEcha,j0StexEcha) +=
			    ( fT3d_distribs[i0StexEcha][i0Sample][n_event] - mean_over_samples(i0StexEcha, n_event) )*
			    ( fT3d_distribs[j0StexEcha][i0Sample][n_event] - mean_over_samples(j0StexEcha, n_event) );
			}
		      cov_over_samp(i0StexEcha,j0StexEcha) /= (Double_t)fNbSampForCalc;

		    }
		  //....... Calculation of the mean over the events of Cov_s[A(c_i,s*,e*),A(c_j,s*,e*)]
		  //......... Calculation of half of the matrix, diagonal included
		  fT2d_hf_cov[i0StexEcha][j0StexEcha] = (Double_t)0;
		  for (Int_t n_event = 0; n_event < fNumberOfEvents; n_event++)
		    {
		      fT2d_hf_cov[i0StexEcha][j0StexEcha] += cov_over_samp(i0StexEcha,j0StexEcha);
		    }
		    fT2d_hf_cov[i0StexEcha][j0StexEcha] /= (Double_t)fNumberOfEvents;
		  
		  fT2d_hf_cov[j0StexEcha][i0StexEcha] = fT2d_hf_cov[i0StexEcha][j0StexEcha];
		}
	    }
	}
      if( i0StexEcha%100 == 0 ){cout << i0StexEcha << "[HFNb Cov], ";}
    }
  cout << endl;

  fTagHfCov[0] = 1;    fFileHeader->fHfCovCalc++;
}
//---------- (end of HighFrequencyCovariancesBetweenChannels ) --------------------

//------------------------------------------------------------------
//
//  Calculation of the High Frequency Correlations between channels
//
//------------------------------------------------------------------
void TEcnaRun::HighFrequencyCorrelationsBetweenChannels()
{
//Calculation of the High Frequency Correlations between channels

  //... preliminary calculation of the covariances if not done yet.
  if ( fTagHfCov[0] != 1 )
    {HighFrequencyCovariancesBetweenChannels(); fTagHfCov[0] = 0;}
  
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::HighFrequencyCorrelationsBetweenChannels()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation of the High Frequency Correlations between channels" << endl
	 << "          Starting allocation. "
	 << endl;}

  //................. allocation fT2d_hf_cor + init to zero (mandatory)
  if( fT2d_hf_cor == 0 ){
    const Int_t n_StexEcha = fEcal->MaxCrysEcnaInStex();
    fT2d_hf_cor  = new Double_t*[n_StexEcha];                   fCnew++;
    fT2d1_hf_cor = new  Double_t[n_StexEcha*n_StexEcha];        fCnew++;
    for(Int_t i0StexEcha = 0 ; i0StexEcha < n_StexEcha ; i0StexEcha++){
	fT2d_hf_cor[i0StexEcha] = &fT2d1_hf_cor[0] + i0StexEcha*n_StexEcha;} 
  }

  for (Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
    {
      for (Int_t j0StexEcha = 0 ; j0StexEcha < fEcal->MaxCrysEcnaInStex() ; j0StexEcha++)
  	{
	  if( fT2d_hf_cor[i0StexEcha][j0StexEcha] != (Double_t)0 )
	    {fMiscDiag[24]++; fT2d_hf_cor[i0StexEcha][j0StexEcha] = (Double_t)0;}
  	}
    }
  
  //................. calculation  
  //........................... computation of half of the matrix, diagonal included
  
  for (Int_t i0StexEcha = 0 ; i0StexEcha < fEcal->MaxCrysEcnaInStex() ; i0StexEcha++)
    {
      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, i0StexEcha) > 0)
	  || ( fFlagSubDet == "EB") )
	{
	  for (Int_t j0StexEcha = 0 ; j0StexEcha <= i0StexEcha ; j0StexEcha++)
	    {
	      if( (fFlagSubDet == "EE" && fEcalNumbering->StexEchaForCons(fFileHeader->fStex, j0StexEcha) > 0)
		  || ( fFlagSubDet == "EB") )
		{
		  if(  fT2d_hf_cov[i0StexEcha][i0StexEcha] > 0
		       && fT2d_hf_cov[j0StexEcha][j0StexEcha] > 0 )
		    {
		      fT2d_hf_cor[i0StexEcha][j0StexEcha] =
			fT2d_hf_cov[i0StexEcha][j0StexEcha]/
			(  (Double_t)sqrt(fT2d_hf_cov[i0StexEcha][i0StexEcha]) *
			   (Double_t)sqrt(fT2d_hf_cov[j0StexEcha][j0StexEcha])  );
		    }
		  else
		    {
		      fT2d_hf_cor[i0StexEcha][j0StexEcha] = (Double_t)0.;
		    }
		  
		  fT2d_hf_cor[j0StexEcha][i0StexEcha] = fT2d_hf_cor[i0StexEcha][j0StexEcha];
		}
	    }
	}
      if( i0StexEcha%100 == 0 ){cout << i0StexEcha << "[HFN Cor], ";}
    }
  cout << endl;  
  
  fTagHfCor[0] = 1;    fFileHeader->fHfCorCalc++;
}
//------- (end of HighFrequencyCorrelationsBetweenChannels) ----------

//=================================================================================
//
//          L O W  &  H I G H    F R E Q U E N C Y    C O R R E L A T I O N S
//
//       B E T W E E N   T O W E R S  ( E B )  O R   S C s   ( E E )
//
//=================================================================================
//-----------------------------------------------------------------------------
//      Calculation of the mean of the Low Frequency Correlations
//      between channels for each Stin
//-----------------------------------------------------------------------------
void TEcnaRun::LowFrequencyMeanCorrelationsBetweenTowers()
{LowFrequencyMeanCorrelationsBetweenStins();}
void TEcnaRun::LowFrequencyMeanCorrelationsBetweenSCs()
{LowFrequencyMeanCorrelationsBetweenStins();}

void TEcnaRun::LowFrequencyMeanCorrelationsBetweenStins()
{
//Calculation of the mean of the Low Frequency Correlations
//between channels for each Stin

  //... preliminary calculation of the Low Frequency Cor(c,c) if not done yet
  //    Only one tag (dim=1) to set to 0 (no write in the result ROOT file)
  if(fTagLfCor[0] != 1){LowFrequencyCorrelationsBetweenChannels(); fTagLfCor[0]=0;}

  //..... mean of the fT2d_lfcc_mostins for each pair (Stin_X,Stin_Y)
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::LowFrequencyMeanCorrelationsBetweenStins()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation of the mean, for each "
	 << fStinName.Data() << ", of the" << endl
	 << "           Low Frequency Correlations between channels." << endl;}
  
  //................. allocation fT2d_lfcc_mostins + init to zero (mandatory)
  if( fT2d_lfcc_mostins == 0 ){
    const Int_t n_Stin = fEcal->MaxStinEcnaInStex();
    fT2d_lfcc_mostins  = new Double_t*[n_Stin];                 fCnew++;
    fT2d1_lfcc_mostins = new  Double_t[n_Stin*n_Stin];          fCnew++;  
    for(Int_t i0StexStinEcna = 0 ; i0StexStinEcna < n_Stin ; i0StexStinEcna++){
	fT2d_lfcc_mostins[i0StexStinEcna] = &fT2d1_lfcc_mostins[0] + i0StexStinEcna*n_Stin;} 
  }
  
  for(Int_t i0StexStinEcna=0; i0StexStinEcna<fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {
      for(Int_t j0StexStinEcna=0; j0StexStinEcna<fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
  	{
  	  if( fT2d_lfcc_mostins[i0StexStinEcna][j0StexStinEcna] != (Double_t)0 )
	    {fMiscDiag[31]++; fT2d_lfcc_mostins[i0StexStinEcna][j0StexStinEcna] = (Double_t)0;}
  	}
    }

  //..... Calculation of the mean of the LF Cor(c,c) for each pair (Stin_X,Stin_Y)
  //
  //           ! => Warning: this matrix is NOT symmetric => take N*N elements
  //                Only (Stin,Stin) matrix is symmetric.
  //                (StinEcha,StinEcha) matrix inside a (Stin,Stin) element is NOT symmetric
  //                (except for the (Stin,Stin) DIAGONAL elements)
  //      Then:
  //            1D array half_LFccMos[N*N] to put the (channel,channel) correlations

  Int_t ndim = (Int_t)(fEcal->MaxCrysInStin()*fEcal->MaxCrysInStin());

  TVectorD  half_LFccMos(ndim); for(Int_t i=0; i<ndim; i++){half_LFccMos(i)=(Double_t)0.;}

  //..................... Calculation
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::LowFrequencyMeanCorrelationsBetweenStins()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for each "
	 << fStinName.Data() << ", of the mean of the" << endl
	 << "           Low Frequency cor(c,c)." << endl;}

  for(Int_t i0StexStinEcna=0; i0StexStinEcna<fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {
      for(Int_t j0StexStinEcna=0; j0StexStinEcna<fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	{
	  //................... .Copy the Mean Correlations(c,c') in 1D vector half_LFccMos() 
	  Int_t i_count = 0;
	  for(Int_t i0StinCrys=0; i0StinCrys<fEcal->MaxCrysInStin(); i0StinCrys++)
	    {
	      Int_t i0StexEcha = i0StexStinEcna*fEcal->MaxCrysInStin() + i0StinCrys;
	      for(Int_t j0StinCrys=0; j0StinCrys<fEcal->MaxCrysInStin(); j0StinCrys++)
		{
		  Int_t j0StexEcha = j0StexStinEcna*fEcal->MaxCrysInStin() + j0StinCrys;
		  if(  (i0StexEcha>= 0 && i0StexEcha < fEcal->MaxCrysEcnaInStex()) &&
		       (j0StexEcha>= 0 && j0StexEcha < fEcal->MaxCrysEcnaInStex())  )
		  {half_LFccMos(i_count) = fT2d_lf_cor[i0StexEcha][j0StexEcha]; i_count++;}
		  else
		    {cout << "!TEcnaRun::LowFrequencyMeanCorrelationsBetweenStins()> Channel number out of range."
			  << "i0StexEcha = " << i0StexEcha <<", j0StexEcha = " << j0StexEcha << fTTBELL << endl; }
		}
	    }
	  //...... Calculation of the mean of the absolute values of the LF mean Correlations(c,c')
	  fT2d_lfcc_mostins[i0StexStinEcna][j0StexStinEcna] = (Double_t)0;
	  for(Int_t i_rcor = 0; i_rcor < ndim; i_rcor++)
	    {
	      fT2d_lfcc_mostins[i0StexStinEcna][j0StexStinEcna] += fabs(half_LFccMos(i_rcor));
	    }
	  fT2d_lfcc_mostins[i0StexStinEcna][j0StexStinEcna] /= (Double_t)ndim;
	}
      if( i0StexStinEcna%10 == 0 ){cout << i0StexStinEcna << "[LFN MCtt], ";}
    }
  cout << endl;

  fTagLFccMoStins[0] = 1;           fFileHeader->fLFccMoStinsCalc++;
} // ------- end of LowFrequencyMeanCorrelationsBetweenStins() -------

//-----------------------------------------------------------------------------
//      Calculation of the mean of the High Frequency Correlations
//      between channels for each Stin
//-----------------------------------------------------------------------------
void TEcnaRun::HighFrequencyMeanCorrelationsBetweenTowers()
{HighFrequencyMeanCorrelationsBetweenStins();}
void TEcnaRun::HighFrequencyMeanCorrelationsBetweenSCs()
{HighFrequencyMeanCorrelationsBetweenStins();}

void TEcnaRun::HighFrequencyMeanCorrelationsBetweenStins()
{
//Calculation of the mean of the High Frequency Correlations
//between channels for each Stin

  //... preliminary calculation of the High Frequency Cor(c,c) if not done yet
  //    Only one tag (dim=1) to set to 0 (no write in the result ROOT file)
  if(fTagHfCor[0] != 1){HighFrequencyCorrelationsBetweenChannels();fTagHfCor[0]=0;}

  //..... mean of the fT2d_hfcc_mostins for each pair (Stin_X,Stin_Y)
  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::HighFrequencyMeanCorrelationsBetweenStins()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation of the mean, for each "
	 << fFlagSubDet.Data() << ", of the" << endl
         << "           High Frequency Correlations between channels." << endl;}

  //................. allocation fT2d_hfcc_mostins + init to zero (mandatory)
  if( fT2d_hfcc_mostins == 0 ){
    const Int_t n_Stin = fEcal->MaxStinEcnaInStex();
    fT2d_hfcc_mostins  = new Double_t*[n_Stin];                 fCnew++;
    fT2d1_hfcc_mostins = new  Double_t[n_Stin*n_Stin];           fCnew++;  
    for(Int_t i0StexStinEcna = 0 ; i0StexStinEcna < n_Stin ; i0StexStinEcna++){
	fT2d_hfcc_mostins[i0StexStinEcna] = &fT2d1_hfcc_mostins[0] + i0StexStinEcna*n_Stin;} 
  }
  
  for(Int_t i0StexStinEcna=0; i0StexStinEcna<fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {
      for(Int_t j0StexStinEcna=0; j0StexStinEcna<fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
  	{
  	  if( fT2d_hfcc_mostins[i0StexStinEcna][j0StexStinEcna] != (Double_t)0 )
	    {fMiscDiag[32]++; fT2d_hfcc_mostins[i0StexStinEcna][j0StexStinEcna] = (Double_t)0;}
  	}
    }

  //..... Calculation of the mean of the HF Cor(c,c) for each pair (Stin_X,Stin_Y)
  //
  //           ! => Warning: this matrix is NOT symmetric => take N*N elements
  //                Only (Stin,Stin) matrix is symmetric.
  //                (StinEcha,StinEcha) matrix inside a (Stin,Stin) element is NOT symmetric
  //                (except for the (Stin,Stin) DIAGONAL elements)
  //      Then:
  //            1D array half_LFccMos[N*N] to put the (channel,channel) correlations

  Int_t ndim = (Int_t)(fEcal->MaxCrysInStin()*fEcal->MaxCrysInStin());

  TVectorD half_HFccMos(ndim); for(Int_t i=0; i<ndim; i++){half_HFccMos(i)=(Double_t)0.;}

  if(fFlagPrint != fCodePrintNoComment){cout << "*TEcnaRun::HighFrequencyMeanCorrelationsBetweenStins()" << endl;}
  if(fFlagPrint == fCodePrintAllComments){
    cout << "           Calculation, for each "
	 << fFlagSubDet.Data()  << ", of the mean of the" << endl
	 << "           High Frequency cor(c,c)." << endl;}

  //..................... Calculation
  for(Int_t i0StexStinEcna=0; i0StexStinEcna<fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
    {
      for(Int_t j0StexStinEcna=0; j0StexStinEcna<fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	{
	  //.................... Copy the relevant Mean Correlations(c,c') in 1D vector half_HFccMos()
	  Int_t i_count = 0;
	  for(Int_t i0StinCrys=0; i0StinCrys<fEcal->MaxCrysInStin(); i0StinCrys++)
	    {
	      Int_t i0StexEcha = i0StexStinEcna*fEcal->MaxCrysInStin() + i0StinCrys;
	      for(Int_t j0StinCrys=0; j0StinCrys<fEcal->MaxCrysInStin(); j0StinCrys++)
		{
		  Int_t j0StexEcha = j0StexStinEcna*fEcal->MaxCrysInStin() + j0StinCrys;
		  if(  (i0StexEcha>= 0 && i0StexEcha < fEcal->MaxCrysEcnaInStex()) &&
		       (j0StexEcha>= 0 && j0StexEcha < fEcal->MaxCrysEcnaInStex())  )
		    {half_HFccMos(i_count) = fT2d_hf_cor[i0StexEcha][j0StexEcha]; i_count++;}
		  else
		    {cout << "!TEcnaRun::HighFrequencyMeanCorrelationsBetweenStins()> Channel number out of range."
			  << "i0StexEcha = " << i0StexEcha <<", j0StexEcha = " << j0StexEcha << fTTBELL << endl; }
		}
	    }
	  //..... Calculation of the mean of the absolute values of the HF mean Correlations(c,c')
	  fT2d_hfcc_mostins[i0StexStinEcna][j0StexStinEcna] = (Double_t)0;
	  for(Int_t i_rcor = 0; i_rcor < ndim; i_rcor++)
	    {
	      fT2d_hfcc_mostins[i0StexStinEcna][j0StexStinEcna] += fabs(half_HFccMos(i_rcor));
	    }
	  fT2d_hfcc_mostins[i0StexStinEcna][j0StexStinEcna] /= (Double_t)ndim;
	}
      if( i0StexStinEcna%10 == 0 ){cout << i0StexStinEcna << "[HFN MCtt], ";}
    }
  cout << endl;

  fTagHFccMoStins[0] = 1;                   fFileHeader->fHFccMoStinsCalc++;
} // ------- end of HighFrequencyMeanCorrelationsBetweenStins() -------

//=========================================================================
//
//                  W R I T I N G     M E T H O D S
//
//=========================================================================

//=========================================================================
//
//         W R I T I N G   M E T H O D S :    R O O T    F I L E S
//
//=========================================================================
//-------------------------------------------------------------
//
//                      OpenRootFile
//
//-------------------------------------------------------------
Bool_t TEcnaRun::OpenRootFile(const Text_t *name, TString status) {
//Open the Root file

  TString s_path;
  s_path = fCnaParPaths->ResultsRootFilePath();
  s_path.Append('/');
  s_path.Append(name);
  
  gCnaRootFile   = new TEcnaRootFile(s_path.Data(), status);     fCnew++;
  Bool_t ok_open = kFALSE;

  if ( gCnaRootFile->fRootFileStatus == "RECREATE" ){ok_open = gCnaRootFile->OpenW();}
  if ( gCnaRootFile->fRootFileStatus == "READ"     ){ok_open = gCnaRootFile->OpenR();}

  if (!ok_open) // unable to open file
    {
      cout << "TEcnaRun::OpenRootFile> Cannot open file " << s_path.Data() << endl;
    }
  else
    {
      if(fFlagPrint == fCodePrintAllComments)
	{cout << "*TEcnaRun::OpenRootFile> Open ROOT file OK for file " << s_path.Data() << endl;}  
      fOpenRootFile  = kTRUE;
    }
  return ok_open;
}
//-------------------------------------------------------------
//
//                      CloseRootFile
//
//-------------------------------------------------------------
Bool_t TEcnaRun::CloseRootFile(const Text_t *name) {
//Close the Root file
 
  Bool_t ok_close = kFALSE;

  if (fOpenRootFile == kTRUE ) 
    {
      gCnaRootFile->CloseFile();

      if(fFlagPrint == fCodePrintAllComments){
	cout << "*TEcnaRun::CloseRootFile> Close ROOT file." << endl;}

      delete gCnaRootFile;                                     fCdelete++;
      fOpenRootFile = kFALSE;
      ok_close      = kTRUE;
    }
  else
    {
      cout << "*TEcnaRun::CloseRootFile(...)> No close since no file is open."
	   << fTTBELL << endl;
    }
  return ok_close;
}
//-------------------------------------------------------------
//
//   WriteRootFile without arguments.
//   Call WriteRootFile WITH argument (file name)
//   after an automatic generation of the file name.
//
//   Codification for the file name:
//            see comment at the beginning of this file
//
//-------------------------------------------------------------

//=================================================================================
//
//         WriteRootFile()   ====>  D O N ' T    S U P P R E S S  ! ! !
//                                  Called by the analyzer in package: "Modules"
//
//=================================================================================
Bool_t TEcnaRun::WriteRootFile(){
//Write the Root file. File name automatically generated in fMakeResultsFileName.
  Bool_t ok_write = kFALSE;

  fCnaWrite->RegisterFileParameters(fFileHeader->fTypAna.Data(), fFileHeader->fNbOfSamples,
				    fFileHeader->fRunNumber,
				    fFileHeader->fFirstReqEvtNumber, fFileHeader->fLastReqEvtNumber,
				    fFileHeader->fReqNbOfEvts,       fFileHeader->fStex);
  
  fCnaWrite->fMakeResultsFileName();  // set fRootFileName, fRootFileNameShort
  
  const Text_t *FileShortName = (const Text_t *)fCnaWrite->fRootFileNameShort.Data();

  //============================= check number of found events
  Int_t nCountEvts = 0;
  
  for(Int_t i0StexEcha=0; i0StexEcha<fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
    {for(Int_t i0Sample=0; i0Sample<fFileHeader->fNbOfSamples; i0Sample++)
      {nCountEvts += fT2d_NbOfEvts[i0StexEcha][i0Sample];}}
  
  if ( nCountEvts <= 0 )
    {
      //============== no write if no event found
      cout << "!TEcnaRun::WriteRootFile()> No event found for file " << fCnaWrite->fRootFileNameShort.Data()
	   << ". File will not be written." << endl;
      ok_write = kTRUE;
    }
  else
    {    
      if(fFlagPrint == fCodePrintAllComments){
	cout << "*TEcnaRun::WriteRootFile()> Results are going to be written in the ROOT file: " << endl
	     << "                           " << fCnaWrite->fRootFileName.Data() << endl;}
      
      ok_write = WriteRootFile(FileShortName, fFileHeader->fNbOfSamples);

      if( ok_write == kTRUE )
	{
	  if(fFlagPrint != fCodePrintNoComment)
	    {cout << "*TEcnaRun::WriteRootFile()> Writing OK for file " << fCnaWrite->fRootFileNameShort.Data()
		  << endl;}
	}
      else
	{
	  cout << "!TEcnaRun::WriteRootFile()> Writing FAILLED for file " << fCnaWrite->fRootFileNameShort.Data()
	       << fTTBELL << endl;
	}
    }
  return ok_write;
} // end of WriteRootFile()

//--------------------------------------------------------------------
//
//               WriteNewRootFile with argument
//    Called by TEcnaGui for results file of the Calculations method
//    analysis type and nb of samples changed, other arguments kept
//
//--------------------------------------------------------------------
Bool_t TEcnaRun::WriteNewRootFile(const TString TypAna){
//Write a new Root file. File name automatically generated in fMakeResultsFileName.
  Bool_t ok_write = kFALSE;

  fCnaWrite->RegisterFileParameters(TypAna.Data(),   fNbSampForCalc, fFileHeader->fRunNumber,
				    fFileHeader->fFirstReqEvtNumber, fFileHeader->fLastReqEvtNumber,
				    fFileHeader->fReqNbOfEvts,       fFileHeader->fStex,
				    fFileHeader->fStartDate, fFileHeader->fStopDate,
				    fFileHeader->fStartTime, fFileHeader->fStopTime);

  fCnaWrite->fMakeResultsFileName();  // set fRootFileName, fRootFileNameShort
  fNewRootFileName      = fCnaWrite->GetRootFileName();
  fNewRootFileNameShort = fCnaWrite->GetRootFileNameShort();

  //  const Text_t *FileShortName = (const Text_t *)fCnaWrite->fRootFileNameShort.Data();
  const Text_t *FileShortName = (const Text_t *)fNewRootFileNameShort.Data();

  if(fFlagPrint == fCodePrintAllComments){
    cout << "*TEcnaRun::WriteNewRootFile()> Results are going to be written in the ROOT file: " << endl
	 << "                              " << fNewRootFileNameShort.Data() << endl;}

  ok_write = WriteRootFile(FileShortName, fNbSampForCalc);
  
  return ok_write;
}

//-------------------------------------------------------------------------
//
//    Get the new ROOT file name (long and short)
//   (called by TEcnaGui in Calculations method)
//
//-------------------------------------------------------------------------
TString TEcnaRun::GetNewRootFileName(){return fNewRootFileName;}
TString TEcnaRun::GetNewRootFileNameShort(){return fNewRootFileNameShort;}

//--------------------------------------------------------------------
//
//               WriteRootFile with argument
//
//--------------------------------------------------------------------
Bool_t TEcnaRun::WriteRootFile(const Text_t* name, Int_t& argNbSampWrite) 
{
//Write the Root file

  const Text_t* file_name = name;

  Bool_t ok_open  = kFALSE;
  Bool_t ok_write = kFALSE;

  if ( fOpenRootFile )
    {
      cout << "!TEcnaRun::WriteRootFile(...) *** ERROR ***> Writing on file already open."
	   << fTTBELL << endl;
    }
  else
    {
      //..... List of the different element types and associated parameters as ordered in the ROOT file
      //                                                                    ==========
      //
      //              WARNING  *** SIZES ARE THESE FOR THE BARREL (1700 Xtals) and for 10 samples ***
      //
      //   Nb of   Type of element            Type      Type                                    Size    Comment
      // elements                             Number    Name
      //
      //        1  fMatHis(1,StexStin)         ( 0)  cTypNumbers             1*(   1,  68) =         68

      //        1  fMatHis(1,StexStin)         (12)  cTypAvPed               1*(   1,  68) =         68
      //        1  fMatHis(1, StexStin)        ( 3)  cTypAvTno               1*(   1,  68) =         68
      //        1  fMatHis(1, StexStin)        ( 4)  cTypAvLfn               1*(   1,  68) =         68
      //        1  fMatHis(1, StexStin)        ( 5)  cTypAvHfn               1*(   1,  68) =         68
      //        1  fMatHis(1,StexStin)         (13)  cTypAvMeanCorss         1*(   1,  68) =         68
      //        1  fMatHis(1,StexStin)         (14)  cTypAvSigCorss          1*(   1,  68) =         68

      //        1  fMatHis(1,StexEcha)         (16)  cTypPed                 1*(   1,1700) =      1 700
      //        1  fMatHis(1,StexEcha)         (17)  cTypTno                 1*(   1,1700) =      1 700
      //        1  fMatHis(1,StexEcha)         (10)  cTypMeanCorss           1*(   1,1700) =      1 700
      //        1  fMatHis(1,StexEcha)         (18)  cTypLfn                 1*(   1,1700) =      1 700
      //        1  fMatHis(1,StexEcha)         (19)  cTypHfn                 1*(   1,1700) =      1 700
      //        1  fMatHis(1,StexEcha)         (11)  cTypSigCorss            1*(   1,1700) =      1 700

      //        1  fMatMat(Stin,Stin)          (23)  cTypLFccMoStins         1*(  68,  68) =      4 624
      //        1  fMatMat(Stin,Stin)          (24)  cTypHFccMoStins         1*(  68,  68) =      4 624

      //        1  fMatHis(StexEcha, sample)   (15)  cTypNbOfEvts            1*(1700,  10) =     17 000
      //        1  fMatHis(StexEcha, sample)   ( 1)  cTypMSp                 1*(1700,  10) =     17 000
      //        1  fMatHis(StexEcha, sample)   ( 2)  cTypSSp                 1*(1700,  10) =     17 000
 
      //   StexEcha  fMatMat(sample, sample)   ( 8)  cTypCovCss           1700*(  10,  10) =    170 000
      //   StexEcha  fMatMat(sample, sample    ( 9)  cTypCorCss           1700*(  10,  10) =    170 000

      //   StexEcha  fMatHis(sample, bin_evt)  (20)  cTypAdcEvt,          1700*(  10, 150) =  2 550 000

      //        1  fMatMat(StexEcha, StexEcha) (21)  cTypLfCov               1*(1700,1700) =  2 890 000
      //        1  fMatMat(StexEcha, StexEcha) (22)  cTypLfCor               1*(1700,1700) =  2 890 000

      //        1  fMatMat(StexEcha, StexEcha) ( 6)  cTypHfCov               1*(1700,1700) =  2 890 000 // (06/05/08)
      //        1  fMatMat(StexEcha, StexEcha) ( 7)  cTypHfCor               1*(1700,1700) =  2 890 000 // (06/05/08)


      //......................................................................................................

      ok_open = OpenRootFile(file_name, "RECREATE");

      TString typ_name = "?";
      Int_t v_nb_times = 0;
      Int_t v_dim_one  = 0;
      Int_t v_dim_two  = 0;
      Int_t v_size     = 0;
      Int_t v_tot      = 0;
      Int_t v_tot_writ = 0;

      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      //       
      //  ===> no general method and no translation to TEcnaWrite
      //       because the fT1d.. and fT2d... arrays
      //       are attributes of TEcnaRun (calls to the "TRootXXXX" methods)
      //
      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

      //-------------------------- Stin numbers 
      //       1   fMatHis(1,Stin)           ( 0)  cTypNumbers        1*(   1,  68) =         68

      Int_t MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "StinNumbers";
      v_nb_times = fFileHeader->fStinNumbersCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxStinEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagStinNumbers[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypNumbers;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootStinNumbers();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //-------------------------- Averaged Pedestals (1 value per Stin)
      //       1   fMatHis(1, StexStin)   (12)  cTypAvPed      1*(1,  68) =     68

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "AvPed";
      v_nb_times = fFileHeader->fAvPedCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxStinEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagAvPed[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypAvPed;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootAvPed();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //-------------------------- Averaged Total noise
      // StexEcha   fMatHis(1, StexStin)     ( 3)  cTypAvTno      1*(1,  68) =     68

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "AvTno";
      v_nb_times = fFileHeader->fAvTnoCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxStinEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagAvTno[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypAvTno;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootAvTno();
	      gCnaRootFile->fCnaResultsTree->Fill(); 
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //-------------------------- Averaged Low frequency noise
      //       1   fMatHis(1, StexStin)   ( 4)  cTypAvLfn      1*(1,  68) =     68

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "AvLfn";
      v_nb_times = fFileHeader->fAvLfnCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxStinEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}
      
      for (Int_t i = 0; i < v_nb_times; i++)
      	{
      	  if ( fTagAvLfn[0] == 1 )
      	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypAvLfn;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootAvLfn();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
      	    }
      	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
      
      //-------------------------- Averaged High frequency noise
      //       1   fMatHis(1, StexStin)   ( 5)  cTypAvHfn      1*(1,  68) =     68

      MaxCar = fgMaxCar;      
      typ_name.Resize(MaxCar);
      typ_name   = "AvHfn";
      v_nb_times = fFileHeader->fAvHfnCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxStinEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}
      
      for (Int_t i = 0; i < v_nb_times; i++)
     	{
      	  if ( fTagAvHfn[0] == 1 )
      	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypAvHfn;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootAvHfn();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
      	    }
      	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //-------------------------- Averaged mean of cor(s,s)
      //       1   fMatHis(1, StexStin)   (13)  cTypAvMeanCorss      1*(1,  68) =     68

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "AvMeanCorss";
      v_nb_times = fFileHeader->fAvMeanCorssCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxStinEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagAvMeanCorss[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypAvMeanCorss;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootAvEvCorss();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
      
      //--------------------------  Averaged sigma of cor(s,s)
      //       1   fMatHis(1, StexStin)    (14)  cTypAvSigCorss      1*(1,  68) =     68

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "AvSigCorss";
      v_nb_times = fFileHeader->fAvSigCorssCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxStinEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagAvSigCorss[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypAvSigCorss;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootAvSigCorss();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //-------------------------- Expectation values of the expectation values of the samples (pedestals)
      //       1   fMatHis(1,StexEcha)         (16)  cTypPed                1*(   1,1700) =      1 700

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "Ped";
      v_nb_times = fFileHeader->fPedCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagPed[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypPed;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootPed();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
                  
      //-------------------------- Expectation values of the sigmas the samples
      //       1   fMatHis(1,StexEcha)         (17)  cTypTno               1*(   1,1700) =      1 700

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "Tno";
      v_nb_times = fFileHeader->fTnoCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagTno[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypTno;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootTno();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
                                     
      //-------------------------- Expectation values of the correlations between the samples
      //       1   fMatHis(1,StexEcha)         (10)  cTypMeanCorss            1*(   1,1700) =      1 700

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "MeanCorss";
      v_nb_times = fFileHeader->fMeanCorssCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagMeanCorss[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypMeanCorss;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootMeanCorss();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	} 
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
      
      //-------------------------- Sigmas of the expectation values of the samples  
      //       1   fMatHis(1,StexEcha)         (18)  cTypLfn               1*(   1,1700) =      1 700

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "Lfn";
      v_nb_times = fFileHeader->fLfnCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagLfn[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypLfn;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootLfn();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	} 
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
      
      //-------------------------- Sigmas of the sigmas of the samples  
      //       1   fMatHis(1,StexEcha)         (19)  cTypHfn              1*(   1,1700) =      1 700

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "Hfn";
      v_nb_times = fFileHeader->fHfnCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}
 
      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagHfn[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypHfn;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootHfn();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
                    
      //-------------------------- Sigmas of the correlations between the samples  
      //       1   fMatHis(1,StexEcha)         (11)  cTypSigCorss           1*(   1,1700) =      1 700

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "SigCorss";
      v_nb_times = fFileHeader->fSigCorssCalc;
      v_dim_one  = 1;
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}
   
      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagSigCorss[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypSigCorss;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootSigCorss();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //----- Mean Covariances between StexEchas (averaged over samples) for all (Stin_X,Stin_Y)
      //       1   fMatMat(Stin,Stin)       (23)  cTypLFccMoStins         1*(  68,  68) =      4 624

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "LFccMoStins";
      v_nb_times = fFileHeader->fLFccMoStinsCalc;
      v_dim_one  = fEcal->MaxStinEcnaInStex();
      v_dim_two  = fEcal->MaxStinEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagLFccMoStins[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypLFccMoStins;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeMat(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatHis.ReSet(1,1);
	      TRootLFccMoStins();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	} 
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
            
      //----- Mean Correlations between StexEchas (averaged over samples) for all (Stin_X,Stin_Y)
      //       1   fMatMat(Stin,Stin)       (24)  cTypHFccMoStins         1*(  68,  68) =      4 624

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "HFccMoStins";
      v_nb_times = fFileHeader->fHFccMoStinsCalc;
      v_dim_one  = fEcal->MaxStinEcnaInStex();
      v_dim_two  = fEcal->MaxStinEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagHFccMoStins[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypHFccMoStins;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeMat(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatHis.ReSet(1,1);
	      TRootHFccMoStins();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
      
      //-------------------------- Numbers of found events (NbOfEvts)
      //       1   fMatHis(StexEcha, sample)   (15)  cTypNbOfEvts       1*(1700,  10) =     17 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "NbOfEvts";
      v_nb_times = fFileHeader->fNbOfEvtsCalc;
      v_dim_one  = fEcal->MaxCrysEcnaInStex();
      v_dim_two  = argNbSampWrite;
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagNbOfEvts[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypNbOfEvts;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootNbOfEvts(argNbSampWrite);
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
                  
      //-------------------------- Expectation values of the samples
      //       1   fMatHis(StexEcha, sample)   ( 1)  cTypMSp                  1*(1700,  10) =     17 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "MSp";
      v_nb_times = fFileHeader->fMSpCalc;
      v_dim_one  = fEcal->MaxCrysEcnaInStex();
      v_dim_two  = argNbSampWrite;
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagMSp[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypMSp;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootMSp(argNbSampWrite);
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
      
      //-------------------------- Sigmas of the samples     
      //       1   fMatHis(StexEcha, sample)   ( 2)  cTypSSp                 1*(1700,  10) =     17 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "SSp";
      v_nb_times = fFileHeader->fSSpCalc;
      v_dim_one  = fEcal->MaxCrysEcnaInStex();
      v_dim_two  = argNbSampWrite;
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagSSp[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypSSp;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootSSp(argNbSampWrite);
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //-------------------------- Covariances between samples

      // StexEcha   fMatMat(sample,  sample)   ( 8)  cTypCovCss           1700*(  10,  10) =    170 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "CovCss";
      v_nb_times = fFileHeader->fCovCssCalc;
      v_dim_one  = argNbSampWrite;
      v_dim_two  = argNbSampWrite;
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i0StexEcha = 0; i0StexEcha < v_nb_times; i0StexEcha++)
	{
	  if ( fTagCovCss[i0StexEcha] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypCovCss;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i0StexEcha;
	      gCnaRootFile->fCnaIndivResult->SetSizeMat(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatHis.ReSet(1,1);
	      TRootCovCss(i0StexEcha, argNbSampWrite);
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i0StexEcha == 0  && fFlagPrint == fCodePrintAllComments)
		{cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
      
      //-------------------------- Correlations between samples   
      // StexEcha   fMatMat(sample,  sample)   ( 9)  cTypCorCss           1700*(  10,  10) =    170 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "CorCss";
      v_nb_times = fFileHeader->fCorCssCalc;
      v_dim_one  = argNbSampWrite;
      v_dim_two  = argNbSampWrite;
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}
 
      for (Int_t i0StexEcha = 0; i0StexEcha < v_nb_times; i0StexEcha++)
	{
	  if ( fTagCorCss[i0StexEcha] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypCorCss;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i0StexEcha;
	      gCnaRootFile->fCnaIndivResult->SetSizeMat(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatHis.ReSet(1,1);
	      TRootCorCss(i0StexEcha, argNbSampWrite);
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i0StexEcha == 0  && fFlagPrint == fCodePrintAllComments)
		{cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }	  
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //-------------------------- Samples as a function of event = events distributions
      // StexEcha   fMatHis(sample,  bins)     (20)  cTypAdcEvt,        1700*(  10, 150) =  2 550 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "AdcEvt";
      v_nb_times = fFileHeader->fAdcEvtCalc;
      v_dim_one  = argNbSampWrite;
      v_dim_two  = fFileHeader->fReqNbOfEvts;
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i0StexEcha = 0; i0StexEcha < v_nb_times; i0StexEcha++)
	{
	  if ( fTagAdcEvt[i0StexEcha] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypAdcEvt;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i0StexEcha;
	      gCnaRootFile->fCnaIndivResult->SetSizeHis(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatMat.ReSet(1,1);
	      TRootAdcEvt(i0StexEcha, argNbSampWrite);
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i0StexEcha == 0  && fFlagPrint == fCodePrintAllComments)
		{cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //-------------------------- Low Frequency Covariances between StexEchas
      //  sample   fMatMat(StexEcha, StexEcha)  (21)  cTypLfCov           1*(1700,1700) =  2 890 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "LfCov";
      v_nb_times = fFileHeader->fLfCovCalc;
      v_dim_one  = fEcal->MaxCrysEcnaInStex();
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{      //=================================== Record type EB
	  if ( fTagLfCov[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypLfCov;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeMat(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatHis.ReSet(1,1);
	      TRootLfCov();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
      
      //-------------------------- Low Frequency Correlations between StexEchas
      //  sample   fMatMat(StexEcha, StexEcha)  (22)  cTypLfCor           1*(1700,1700) =  2 890 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "LfCor";
      v_nb_times = fFileHeader->fLfCorCalc;
      v_dim_one  = fEcal->MaxCrysEcnaInStex();
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagLfCor[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypLfCor;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeMat(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatHis.ReSet(1,1);
	      TRootLfCor();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //-------------------------- High Frequency Covariances between StexEchas
      //  sample   fMatMat(StexEcha, StexEcha)  (6)  cTypHfCov           1*(1700,1700) =  2 890 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "HfCov";
      v_nb_times = fFileHeader->fHfCovCalc;
      v_dim_one  = fEcal->MaxCrysEcnaInStex();
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;
 
      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagHfCov[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypHfCov;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeMat(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatHis.ReSet(1,1);
	      TRootHfCov();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}
      
      //-------------------------- High Frequency Correlations between StexEchas
      //  sample   fMatMat(StexEcha, StexEcha)  (7)  cTypHfCor           1*(1700,1700) =  2 890 000

      MaxCar = fgMaxCar;
      typ_name.Resize(MaxCar);
      typ_name   = "HfCor";
      v_nb_times = fFileHeader->fHfCorCalc;
      v_dim_one  = fEcal->MaxCrysEcnaInStex();
      v_dim_two  = fEcal->MaxCrysEcnaInStex();
      v_size     = v_nb_times*v_dim_one*v_dim_two;
      v_tot     += v_size;

      if(fFlagPrint == fCodePrintAllComments){
      cout << "*TEcnaRun::WriteRootFile(...)> " << setw(18) << typ_name << ": " << setw(4) << v_nb_times
	   << " * ("  << setw(4) << v_dim_one << ","  << setw(4) << v_dim_two  << ") = "
	   << setw(9) << v_size;}

      for (Int_t i = 0; i < v_nb_times; i++)
	{
	  if ( fTagHfCor[0] == 1 )
	    {
	      gCnaRootFile->fCnaIndivResult->fTypOfCnaResult = cTypHfCor;
	      gCnaRootFile->fCnaIndivResult->fIthElement     = i;
	      gCnaRootFile->fCnaIndivResult->SetSizeMat(v_dim_one,v_dim_two);
	      gCnaRootFile->fCnaIndivResult->fMatHis.ReSet(1,1);
	      TRootHfCor();
	      gCnaRootFile->fCnaResultsTree->Fill();
	      if( i == 0 && fFlagPrint == fCodePrintAllComments ){cout << " => WRITTEN ON FILE "; v_tot_writ += v_size;}
	    }
	}
      if(fFlagPrint == fCodePrintAllComments){cout << endl;}

      //---------------------------------------------- WRITING 
      //...................................... file 
      gCnaRootFile->fRootFile->Write();
      //...................................... header
      fFileHeader->Write();

      //...................................... status message
      if(fFlagPrint == fCodePrintAllComments){
	cout << "*TEcnaRun::WriteRootFile(...)> " << setw(20) << "TOTAL: "
	     << setw(21) << "CALCULATED = " << setw(9) <<  v_tot
	     << " => WRITTEN ON FILE = "    << setw(9) << v_tot_writ << endl;}

      if(fFlagPrint == fCodePrintAllComments){
	cout << "*TEcnaRun::WriteRootFile(...)> Write OK in file " << file_name << " in directory:" << endl
	     << "                           " << fCnaParPaths->ResultsRootFilePath().Data()
	     << endl;}

      ok_write = kTRUE;

      //...................................... close
      CloseRootFile(file_name);
    }
  return ok_write;
}  //-------------- End of WriteRootFile(...) -----------------------

//======================== "PREPA FILL" METHODS ===========================

//-------------------------------------------------------------------------
//
//  Prepa Fill Stin numbers as a function of the Stin index 
//                       (for writing in the ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootStinNumbers()
{
  if (fTagStinNumbers[0] == 1 )
    {
      for (Int_t j0StexStinEcna = 0; j0StexStinEcna < fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexStinEcna) =
	    fT1d_StexStinFromIndex[j0StexStinEcna];
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill last evt numbers for all the (StexEcha,sample)
//                       (for writing in the ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootNbOfEvts(const Int_t& argNbSampWrite)
{
  if (fTagNbOfEvts[0] == 1 )
    {
      for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{
	  for (Int_t i0Sample = 0; i0Sample < argNbSampWrite; i0Sample++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatHis(j0StexEcha, i0Sample) =
	      	fT2d_NbOfEvts[j0StexEcha][i0Sample];
	    }
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill histogram of samples as a function of event
//                       (for writing in the ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootAdcEvt(const Int_t& user_StexEcha, const Int_t& argNbSampWrite)
{
  if (fTagAdcEvt[user_StexEcha] == 1 )
    {
      for (Int_t i0Sample = 0; i0Sample < argNbSampWrite; i0Sample++)
	{
	  //...................... all the bins set to zero
	  for (Int_t j_bin = 0; j_bin < fFileHeader->fReqNbOfEvts; j_bin++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatHis(i0Sample, j_bin) = (Double_t)0.;
	    }
	  //...................... fill the non-zero bins 
	  for (Int_t j_bin = 0; j_bin < fFileHeader->fReqNbOfEvts; j_bin++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatHis(i0Sample, j_bin) =
		fT3d_distribs[user_StexEcha][i0Sample][j_bin]; 
	    }
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill expectation values of the samples for all the StexEchas
//                       (for writing in the ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootMSp(const Int_t& argNbSampWrite)
{
  if (fTagMSp[0] == 1 )
    {
      for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{
	  for (Int_t i0Sample = 0; i0Sample < argNbSampWrite; i0Sample++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatHis( j0StexEcha, i0Sample) =
	      	fT2d_ev[j0StexEcha][i0Sample];
	    }
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill sigmas of the samples for all the StexEchas
//                       (for writing in the ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootSSp(const Int_t& argNbSampWrite)
{
  if (fTagSSp[0] == 1 )
    {
      for (Int_t j0StexEcha=0; j0StexEcha<fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{
	  for (Int_t i0Sample=0; i0Sample<argNbSampWrite; i0Sample++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatHis(j0StexEcha, i0Sample) =
	      	fT2d_sig[j0StexEcha][i0Sample];
	    }
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill mean covariances between StexEchas, mean over samples
//  for all (Stin_X, Stin_Y)
//                           (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootLFccMoStins()
{
  if (fTagLFccMoStins[0] == 1 )
    {
      for (Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
	{
	  for (Int_t j0StexStinEcna = 0; j0StexStinEcna < fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatMat(i0StexStinEcna, j0StexStinEcna) =
	      	fT2d_lfcc_mostins[i0StexStinEcna][j0StexStinEcna];
	    }
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill mean correlations between StexEchas, mean over samples
//  for all (Stin_X, Stin_Y)
//                           (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootHFccMoStins()
{
  if (fTagHFccMoStins[0] == 1 )
    {
      for (Int_t i0StexStinEcna = 0; i0StexStinEcna < fEcal->MaxStinEcnaInStex(); i0StexStinEcna++)
	{
	  for (Int_t j0StexStinEcna = 0; j0StexStinEcna < fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatMat(i0StexStinEcna, j0StexStinEcna) =
	      	fT2d_hfcc_mostins[i0StexStinEcna][j0StexStinEcna];
	    }
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill ADC distributions of the samples for all the StexEchas
//                       (for writing in the ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootAvTno()
{
  if (fTagAvTno[0] == 1 )
    {
      for (Int_t j0StexStinEcna = 0; j0StexStinEcna < fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexStinEcna) =
	    fT1d_av_totn[j0StexStinEcna];
	}
    }
}
//-------------------------------------------------------------------------
//
//  Prepa Fill ADC distributions xmin of the samples for all the StexEchas
//                       (for writing in the ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootAvLfn()
{
  if (fTagAvLfn[0] == 1 )
    {
      for (Int_t j0StexStinEcna = 0; j0StexStinEcna < fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexStinEcna) =
	    fT1d_av_lofn[j0StexStinEcna];  
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill ADC distributions xmax of the samples for all the StexEchas
//                       (for writing in the ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootAvHfn()
{
  if (fTagAvHfn[0] == 1 )
    {
      for (Int_t j0StexStinEcna = 0; j0StexStinEcna < fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexStinEcna) =
	    fT1d_av_hifn[j0StexStinEcna];  
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill Low Frequency covariances between StexEchas
//                           (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootLfCov()
{
  if (fTagLfCov[0] == 1 )
    {
      for (Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
	{
	  for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatMat(i0StexEcha, j0StexEcha) =
	      	fT2d_lf_cov[i0StexEcha][j0StexEcha];
	    }
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill Low Frequency correlations between StexEchas
//                         (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootLfCor()
{
  if (fTagLfCor[0] == 1 )
    {
      for (Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
	{
	  for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatMat(i0StexEcha, j0StexEcha) =
	      	fT2d_lf_cor[i0StexEcha][j0StexEcha];
	    }
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill High Frequency covariances between StexEchas
//                           (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootHfCov()
{
  if (fTagHfCov[0] == 1 )
    {
      for (Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
	{
	  for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatMat(i0StexEcha, j0StexEcha) =
	      	fT2d_hf_cov[i0StexEcha][j0StexEcha];
	    }
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill High Frequency correlations between StexEchas
//                         (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootHfCor()
{
  if (fTagHfCor[0] == 1 )
    {
      for (Int_t i0StexEcha = 0; i0StexEcha < fEcal->MaxCrysEcnaInStex(); i0StexEcha++)
	{
	  for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	    {
	      gCnaRootFile->fCnaIndivResult->fMatMat(i0StexEcha, j0StexEcha) =
	      	fT2d_hf_cor[i0StexEcha][j0StexEcha];
	    }
	}
    }
}


//-------------------------------------------------------------------------
//
//  Prepa Fill covariances between samples for a given StexEcha
//                      (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootCovCss(const Int_t& user_StexEcha, const Int_t& argNbSampWrite)
{
   if (fTagCovCss[user_StexEcha] == 1 )
     {
       for (Int_t i0Sample = 0; i0Sample < argNbSampWrite; i0Sample++)
	 {
	   for (Int_t j0Sample = 0; j0Sample < argNbSampWrite; j0Sample++)
	     {
	       gCnaRootFile->fCnaIndivResult->fMatMat(i0Sample, j0Sample) =
	       	 fT3d_cov_ss[user_StexEcha][i0Sample][j0Sample];
	     }
	 }
     }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill correlations between samples for a given StexEcha
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootCorCss(const Int_t& user_StexEcha, const Int_t& argNbSampWrite)
{
   if (fTagCorCss[user_StexEcha] == 1 )
     {
       for (Int_t i0Sample = 0; i0Sample < argNbSampWrite; i0Sample++)
	 {
	   for (Int_t j0Sample = 0; j0Sample < argNbSampWrite; j0Sample++)
	     {
	       gCnaRootFile->fCnaIndivResult->fMatMat(i0Sample, j0Sample) =
	       	 fT3d_cor_ss[user_StexEcha][i0Sample][j0Sample];
	     }
	 }
     }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill expectation values of the expectation values of the samples
//  for all the StexEchas
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootPed()
{
  if (fTagPed[0] == 1 )
    {
      for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexEcha) =
	    fT1d_ev_ev[j0StexEcha];
	}      
    }
}
//-------------------------------------------------------------------------
//
//  Prepa Fill expectation values of the sigmas of the samples
//  for all the StexEchas
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootTno()
{
  if (fTagTno[0] == 1 )
    {
      for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexEcha) =
	    fT1d_evsamp_of_sigevt[j0StexEcha];
	}      
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill expectation values of the (sample,sample) correlations
//  for all the StexEchas
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootMeanCorss()
{
  if (fTagMeanCorss[0] == 1 )
    {
      for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexEcha) =
	    fT1d_ev_cor_ss[j0StexEcha];
	}      
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill sigmas of the expectation values of the samples
//  for all the StexEchas
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootLfn()
{
  if (fTagLfn[0] == 1 )
    {
      for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexEcha) =
	    fT1d_sigevt_of_evsamp[j0StexEcha];
	}      
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill sigmas of the expectation values of the sigmas
//  for all the StexEchas
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootHfn()
{
  if (fTagHfn[0] == 1 )
    {
      for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexEcha) =
	    fT1d_evevt_of_sigsamp[j0StexEcha];
	}      
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill sigmas of the (sample,sample) correlations
//  for all the StexEchas
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootSigCorss()
{
  if (fTagSigCorss[0] == 1 )
    {
      for (Int_t j0StexEcha = 0; j0StexEcha < fEcal->MaxCrysEcnaInStex(); j0StexEcha++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexEcha) =
	    fT1d_sig_cor_ss[j0StexEcha];
	}      
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill Averaged Pedestals
//  for all the StexStins
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootAvPed()
{
  if (fTagAvPed[0] == 1 )
    {
      for (Int_t j0StexStinEcna = 0; j0StexStinEcna < fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexStinEcna) =
	    fT1d_av_mped[j0StexStinEcna];
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill 
//  
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootAvEvCorss()
{
  if (fTagAvMeanCorss[0] == 1 )   // test 1st elt only since global calc
    {
      for (Int_t j0StexStinEcna = 0; j0StexStinEcna < fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexStinEcna) =
	    fT1d_av_ev_corss[j0StexStinEcna];
	}
    }
}

//-------------------------------------------------------------------------
//
//  Prepa Fill 
//  
//                        (for writing in ROOT file)
//
//-------------------------------------------------------------------------
void TEcnaRun::TRootAvSigCorss()
{
  if (fTagAvSigCorss[0] == 1 )   // test 1st elt only since global calc
    {
      for (Int_t j0StexStinEcna = 0; j0StexStinEcna < fEcal->MaxStinEcnaInStex(); j0StexStinEcna++)
	{
	  gCnaRootFile->fCnaIndivResult->fMatHis(0, j0StexStinEcna) =
	    fT1d_av_sig_corss[j0StexStinEcna];
	} 
    }
}

//=========================================================================
//
//         METHODS TO SET FLAGS TO PRINT (OR NOT) COMMENTS (DEBUG)
//
//=========================================================================

void TEcnaRun::PrintComments()
{
// Set flags to authorize printing of some comments concerning initialisations (default)

  fFlagPrint = fCodePrintComments;
  cout << "*TEcnaRun::PrintComments()> Warnings and some comments on init will be printed" << endl;
}

void TEcnaRun::PrintWarnings()
{
// Set flags to authorize printing of warnings

  fFlagPrint = fCodePrintWarnings;
  cout << "*TEcnaRun::PrintWarnings()> Warnings will be printed" << endl;
}

void TEcnaRun::PrintAllComments()
{
// Set flags to authorize printing of the comments of all the methods

  fFlagPrint = fCodePrintAllComments;
  cout << "*TEcnaRun::PrintAllComments()> All the comments will be printed" << endl;
}

void  TEcnaRun::PrintNoComment()
{
// Set flags to forbid the printing of all the comments

  fFlagPrint = fCodePrintNoComment;
}
//=========================== E N D ======================================
