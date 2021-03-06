#ifndef TauAnalysis_Core_EventWeightHistManager_h  
#define TauAnalysis_Core_EventWeightHistManager_h

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"
#include "FWCore/Utilities/interface/InputTag.h"
#include "FWCore/ParameterSet/interface/ParameterSet.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include "TauAnalysis/Core/interface/HistManagerBase.h"

#include <string>

class EventWeightHistManager : public HistManagerBase 
{
 public:
  explicit EventWeightHistManager(const edm::ParameterSet&);
  ~EventWeightHistManager();
  
 private: 
//--- histogram booking and filling functions 
//    inherited from HistManagerBase class
  void bookHistogramsImp();
  void fillHistogramsImp(const edm::Event&, const edm::EventSetup&, double);

//--- histograms
  MonitorElement* hEvtWeightPosLog_;
  MonitorElement* hEvtWeightNegLog_;
  MonitorElement* hEvtWeightZero_;
  MonitorElement* hEvtWeightLinear_;
};

#endif  


