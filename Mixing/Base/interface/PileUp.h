#ifndef Base_PileUp_h
#define Base_PileUp_h

#include <string>
#include <vector>
#include "FWCore/Framework/interface/Frameworkfwd.h"
#include "FWCore/Sources/interface/VectorInputSource.h"

namespace CLHEP {
  class RandPoissonQ;
}

namespace edm {
  class PileUp {
  public:
    typedef VectorInputSource::EventPrincipalVector EventPrincipalVector;
    explicit PileUp(ParameterSet const& pset, int const minb, int const maxb, double averageNumber);
    ~PileUp();

    void readPileUp(std::vector<EventPrincipalVector> & result);

    //    int minBunch() const {return minBunch_;}
    //    int maxBunch() const {return maxBunch_;}
    double averageNumber() const {return averageNumber_;}
    bool poisson() const {return poisson_;}
    bool doPileup() {return none_ ? false :  averageNumber_>0.;}

  private:
    std::string const type_;
    int const minBunch_;
    int const maxBunch_;
    double const averageNumber_;
    int const intAverage_;
    bool const poisson_;
    bool const fixed_;
    bool const none_;
    VectorInputSource * const input_;
    CLHEP::RandPoissonQ *poissonDistribution_;
  };
}

#endif
