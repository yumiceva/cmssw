#ifndef GaussianSumUtilities_h_
#define GaussianSumUtilities_h_

#include "TrackingTools/GsfTools/interface/MultiGaussianState1D.h"
#include "TrackingTools/GsfTools/interface/SingleGaussianState.h"
#include "TrackingTools/GsfTools/interface/MultiGaussianState.h"
#include <vector>

#define DRAW_GSND

/** Utility class for the analysis of multi-dimensional Gaussian
 *  mixtures. The input state is assumed to exist for 
 *  the lifetime of this object.
 */

template <unsigned int N>
class GaussianSumUtilities {
public:
  typedef SingleGaussianState<N> SingleState;
  typedef MultiGaussianState<N> MultiState; 
//   typedef ROOT::Math::SVector<double, N> Vector;
  typedef ROOT::Math::SMatrix<double,N,N,ROOT::Math::MatRepStd<double,N> > GenMatrix;

  typedef typename SingleState::Vector Vector;
  typedef typename SingleState::Matrix Matrix;
  typedef typename MultiState::SingleStatePtr SingleStatePtr;
  typedef typename MultiState::SingleStateContainer SingleStateContainer;

private:
  enum ModeStatus { Valid, NotValid, NotComputed };

public:
  GaussianSumUtilities (const MultiState& state) :
    theState(state), 
    theModeStatus(NotComputed) {
#ifdef DRAW_GSND
    v1Draw_ = 1;
    v2Draw_ = 2;
    instance_ = this;
#endif
  } 
  ~GaussianSumUtilities () {
  }

  /// number of components
  inline unsigned int size () const {
    return components().size();
  }
  /// components
  const SingleStateContainer& components () const {
    return theState.components();
  }
  /// multi-state
  const MultiState& state () const {
    return theState;
  }
  /// weight of a component
  inline double weight (unsigned int i) const {
    return components()[i]->weight();
  }
  /// mean value of a component
  inline const Vector& mean (unsigned int i) const {
    return components()[i]->mean();
  }
  /// covariance matrix of a component
  inline const Matrix& covariance (unsigned int i) const {
    return components()[i]->covariance();
  }
  /// mode status
  bool modeIsValid () const;
  /// mode
  Vector mode () const;
  /** Local variance from Hessian matrix.
   *  Only valid if x corresponds to a (local) maximum! */
  Matrix localCovariance (const Vector& x) const;
  /// value of the p.d.f.
  double pdf (const Vector&) const;
  /// gradient
  Vector d1Pdf (const Vector&) const;
  /// Hessian
  Matrix d2Pdf (const Vector&) const;
  /// value of ln(pdf)
  double lnPdf (const Vector&) const;
  /// gradient of ln(pdf)
  Vector d1LnPdf (const Vector&) const;
  /// Hessian of ln(pdf)
  Matrix d2LnPdf (const Vector&) const;

  /// combined weight
  double weight () const {
    return theState.weight();
  }
  /// combined mean
  const Vector& mean () const {
    return theState.mean();
  }
  /// combined covariance
  const Matrix& covariance () const {
    return theState.covariance();
  }


protected:
  /// calculation of mode
  Vector computeModeWithoutTransform () const;

private:
  /// Symmetric Tensor Product (not recognized by standard ROOT Math)
  Matrix tensorProduct (const Vector&) const;
  /// value of gaussian distribution
  double gauss (const double&, const double&, const double&) const;
  /// value of multidimensional gaussian distribution
  double gauss (const Vector&, 
		const Vector&,
		const Matrix&) const;
//   /// mode from starting value in pdf; returns true on success
//   bool findModeAlternative (Vector& mode, double& pdfAtMode,
// 			    const Vector& xStart) const;
  /// mode from starting value in ln(pdf); returns true on success
  bool findMode (Vector& mode, double& pdfAtMode,
		 const Vector& xStart) const;
  /// calculation of mode with transformation of pdf
  Vector computeMode () const;
  /// state constrained to a line x = s*d+x0
  MultiGaussianState1D constrainedState (const Vector& d,
					 const Vector& x0) const;
  /// replacement of CLHEP determinant (which rounds off small values)
  double determinant (const Matrix& matrix) const;

#ifdef DRAW_GSND
public:
  void setDraw (int i1, int i2) {v1Draw_=i1; v2Draw_=i2;}
private:
  static double fcn2 (double* x, double* p);
#endif

private:
  const MultiState& theState;
//   int theDimension;

  mutable ModeStatus theModeStatus;
  mutable Vector theMode;

#ifdef DRAW_GSND
  static int v1Draw_;
  static int v2Draw_;
  static GaussianSumUtilities<N>* instance_;
#endif
};

#include "TrackingTools/GsfTools/interface/GaussianSumUtilities.icc"

#endif
