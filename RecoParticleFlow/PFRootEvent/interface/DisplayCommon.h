#ifndef PF_DisplayCommon_h
#define PF_DisplayCommon_h

enum View_t { XY = 0, RZ = 1, EPE = 2, EPH = 3, NViews = 4 };

enum ObjType { 
  RECHITECALID=0, 
  RECHITHCALID,
  RECHITHFEMID,
  RECHITHFHADID,
  RECHITPSID,
  CLUSTERECALID,
  CLUSTERHCALID,
  CLUSTERHFEMID,
  CLUSTERHFHADID,
  CLUSTERPSID,
  CLUSTERIBID,
  RECTRACKID,
  BREMID,
  GSFRECTRACKID,
  SIMPARTICLEID,
  GENPARTICLEID
};   

const uint SHIFTID=26;
const int HITTYPES=5;
             
#endif
