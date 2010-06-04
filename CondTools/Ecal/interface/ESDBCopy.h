#ifndef ESDBCOPY_H
#define ESDBCOPY_H

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "CondCore/DBCommon/interface/DBSession.h"
#include "CondCore/DBCommon/interface/Exception.h"
//#include "CondCore/DBCommon/interface/ConnectMode.h"
#include "CondCore/DBCommon/interface/MessageLevel.h"

#include "FWCore/Framework/interface/IOVSyncValue.h"

#include <string>
#include <map>

namespace edm {
  class ParameterSet;
  class Event;
  class EventSetup;
}

using namespace std;

class  ESDBCopy : public edm::EDAnalyzer {
 public:
  explicit  ESDBCopy(const edm::ParameterSet& iConfig );
  ~ESDBCopy();

  virtual void analyze( const edm::Event& evt, const edm::EventSetup& evtSetup);

 private:
  bool shouldCopy(const edm::EventSetup& evtSetup, std::string container);
  void copyToDB(const edm::EventSetup& evtSetup, std::string container);

  std::string m_timetype;
  std::map<std::string, unsigned long long> m_cacheIDs;
  std::map<std::string, std::string> m_records;
};

#endif
