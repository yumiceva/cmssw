#ifndef CalibMuon_DTCalibration_DTResidualFitter_h
#define CalibMuon_DTCalibration_DTResidualFitter_h

/*
 *  $Date: 2010/11/18 10:35:51 $
 *  $Revision: 1.3 $
 *  \author A. Vilela Pereira
 */

class TH1F;

struct DTResidualFitResult {
public:
   DTResidualFitResult(double mean, double meanErr, double sigma, double sigmaErr): fitMean(mean), 
                                                                                    fitMeanError(meanErr),
                                                                                    fitSigma(sigma),
                                                                                    fitSigmaError(sigmaErr) {} 

   double fitMean;
   double fitMeanError;
   double fitSigma;
   double fitSigmaError;
}

class DTResidualFitter {
public:
   DTResidualFitter();
   ~DTResidualFitter();

   DTResidualFitResult fitResiduals(TH1F const& histo, int nSigmas = 1);
};
#endif
