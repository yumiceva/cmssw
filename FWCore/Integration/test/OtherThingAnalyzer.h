#ifndef EDMREFTEST_OTHERTHINGANALYZER_H
#define EDMREFTEST_OTHERTHINGANALYZER_H

#include "FWCore/CoreFramework/interface/EDAnalyzer.h"

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

namespace edmreftest {

  class OtherThingAnalyzer : public edm::EDAnalyzer {
  public:

    // The following is not yet used, but will be the primary
    // constructor when the parameter set system is available.
    //
    explicit OtherThingAnalyzer(edm::ParameterSet const&) {}

    virtual ~OtherThingAnalyzer() {}

    virtual void analyze(edm::Event const& e, edm::EventSetup const& c);

  private:
  };

}

#endif
