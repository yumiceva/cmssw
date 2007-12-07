#ifndef DQM_HCALMONITORTASKS_HCALHOTCELLMONITOR_H
#define DQM_HCALMONITORTASKS_HCALHOTCELLMONITOR_H

#include "DQM/HcalMonitorTasks/interface/HcalBaseMonitor.h"
#include "DataFormats/HcalRecHit/interface/HcalRecHitCollections.h"
#include <map>

/** \class HcalHotCellMonitor
  *  
  * $Date: 2007/11/21 15:48:15 $
  * $Revision: 1.10 $
  * \author W. Fisher - FNAL
  * \ updated by J. Temple - Univ. of Maryland
  */


// Structure holds all hot cell data for a subdetector
struct HotCellHists{
  //Miscellaneous hot cell plots
  MonitorElement* maxCellOccMap;
  MonitorElement* maxCellEnergyMap;
  MonitorElement* maxCellEnergy;
  MonitorElement* maxCellTime;
  MonitorElement* maxCellID; 
  
  // Threshold plots
  std::vector<double> thresholds;
  std::vector<MonitorElement*> threshOccMap;
  std::vector<MonitorElement*> threshEnergyMap;
  // Are these histograms overkill?
  std::vector <std::vector<MonitorElement*> > threshOccMapDepth;
  std::vector <std::vector<MonitorElement*> > threshEnergyMapDepth;
  
  // NADA hot cell info
  MonitorElement* nadaOccMap;
  MonitorElement* nadaEnergyMap;
  MonitorElement* nadaNumHotCells;
  MonitorElement* nadaTestPlot;
  MonitorElement* nadaEnergy;
  MonitorElement* nadaNumNegCells;
  MonitorElement* nadaNegOccMap;
  MonitorElement* nadaNegEnergyMap;
  std::vector<MonitorElement*> nadaOccMapDepth;
  std::vector<MonitorElement*> nadaEnergyMapDepth;
  std::vector<MonitorElement*> nadaNegOccMapDepth;
  std::vector<MonitorElement*> nadaNegEnergyMapDepth;

  // Parameters used in setting NADA cube sizes, thresholds
  double nadaEnergyCandCut0,nadaEnergyCandCut1, nadaEnergyCandCut2;
  double nadaEnergyCubeCut,nadaEnergyCellCut,nadaNegCandCut;
  double nadaEnergyCubeFrac, nadaEnergyCellFrac;
  int nadaMaxDeltaDepth, nadaMaxDeltaEta, nadaMaxDeltaPhi;

  // subdetector info
  int type;
  std::string name;
  bool subdetOn;

  double etaMax, etaMin, phiMax, phiMin;
  int etaBins, phiBins;
  // store max cell
  int etaS, phiS, depthS;
  int idS;
  double enS, tS;
  std::vector<std::string> vetoCells;
  int numhotcells;
  int numnegcells;
  bool fVerbosity;
};


class HcalHotCellMonitor: public HcalBaseMonitor {
public:
  HcalHotCellMonitor(); 
  ~HcalHotCellMonitor(); 

  void setup(const edm::ParameterSet& ps, DaqMonitorBEInterface* dbe);
  void processEvent(const HBHERecHitCollection& hbHits, const HORecHitCollection& hoHits, const HFRecHitCollection& hfHits);
  void reset();
  void setupVals(HotCellHists& h, int type, HotCellHists& base, const edm::ParameterSet& ps);
  void setupHists(HotCellHists& h, DaqMonitorBEInterface* dbe);

private:  ///Monitoring elements

  bool debug_;
  int ievt_;

  float HF_offsets[13][36][2];


  HotCellHists hbHists,heHists,hfHists,hoHists;
  HotCellHists hcalHists;
 
  MonitorElement* meEVT_;

};

#endif
