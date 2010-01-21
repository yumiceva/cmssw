#ifndef _SiStripQualityChecker_h_
#define _SiStripQualityChecker_h_

#include "DQMServices/Core/interface/MonitorElement.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <vector>
#include <string>

class DQMStore;
class MonitorElement;
class TkDetMap;
class SiStripDetCabling;

class SiStripQualityChecker {

 public:


  SiStripQualityChecker(edm::ParameterSet const& ps);
  virtual ~SiStripQualityChecker();


 void bookStatus(DQMStore* dqm_store);     
  void resetStatus();
  void fillDummyStatus();
  void fillStatus(DQMStore* dqm_store, const edm::ESHandle< SiStripDetCabling >& cabling);
  void printStatusReport();
  void fillFaultyModuleStatus(DQMStore* dqm_store);
  
 private:

  struct SubDetMEs{
    MonitorElement* DetFraction;
    MonitorElement* SToNFlag;
    MonitorElement* SummaryFlag;
    std::string     detectorTag;
  };

  void fillDetectorStatus(DQMStore* dqm_store, const edm::ESHandle< SiStripDetCabling >& cabling);
  void fillTrackingStatus(DQMStore* dqm_store);
  void fillSubDetStatus(DQMStore* dqm_store,const edm::ESHandle< SiStripDetCabling >& cabling, SubDetMEs& mes, unsigned int xbin,float& gflag);
  void getModuleStatus(DQMStore* dqm_store, int& errdet);
  void getModuleStatus(std::vector<MonitorElement*>& layer_mes, int& errdet);

  void fillStatusHistogram(MonitorElement*, int xbin, int ybin, float val);
  void initialiseBadModuleList();  
  
  std::map<std::string, SubDetMEs> SubDetMEsMap;
  std::map<std::string, std::string> SubDetFolderMap;

  
  MonitorElement* DetFractionReportMap;
  MonitorElement* SToNReportMap;
  MonitorElement* SummaryReportMap;

  MonitorElement* SummaryReportGlobal;

  MonitorElement* ReportTrackRate;
  MonitorElement* ReportTrackChi2overDoF;
  MonitorElement* ReportTrackRecHits;
  MonitorElement* TrackSummaryReportMap;

  MonitorElement* TrackSummaryReportGlobal;

  std::map<uint32_t,uint16_t> badModuleList;
 
  edm::ParameterSet pSet_;

  bool bookedStripStatus_;
  bool bookedTrackingStatus_;
  int globalStatusFilling_;

  TkDetMap* tkDetMap_;

};
#endif
