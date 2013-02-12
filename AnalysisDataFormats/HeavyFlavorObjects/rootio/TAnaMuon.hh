#ifndef TANAMUON
#define TANAMUON


#include "TObject.h"
#include "TArrayC.h"
#include "TArrayF.h"
#include "TVector3.h"

#include "TAnaTrack.hh"

#include <map>
#include <vector>

class xpTrack {
public: 
  xpTrack(int i = -1) {idx = i;}
  virtual ~xpTrack() {}
  void clear() {idx = -1;}
  int idx; 
  float dist; 
  TVector3 r, p;
  ClassDef(xpTrack,1)
};


class TAnaMuon : public TAnaTrack {

public:

  TAnaMuon(int index = -1);
  ~TAnaMuon() { };
  void clear(); 
  void dump();

  // ----------------------------------------------------------------------
  TVector3  fInnerPlab, fOuterPlab, fGlobalPlab;
  TVector3  fPositionAtM1, fPositionAtM2;
  TVector3  fMuonTrackPosAtM1, fMuonTrackPlabAtM1;

  // see AN2008_098 "Muon identification in CMS"
  double    fMuonChi2, fMuonZ, fMuonR;
  double    fCaloComp, fSegmComp, fIsolation; 
  int       fNhitsCSC, fNhitsDT,fNhitsRPC; 
  int       fNmatchedStations;
  float     fTimeInOut, fTimeInOutE, fTimeOutIn, fTimeOutInE; 
  int       fTimeNdof;

  // -- new variables for MVA muon id
  double fItrkValidFraction; 
  double fGtrkNormChi2;    
  double fChi2LocalPosition, fChi2LocalMomentum; 
  int    fNumberOfLostTrkHits, fNumberOfValidTrkHits; 
  double fSegmentComp;    
  double fGtrkProb;           
 
  // information on tracks near this candidate
  std::map<int,float> fNstTracks; // usage: (trackIx, doca)
  // information on tracks extrapolated to muon station 1 (position and momentum): tkIdx, dist to muon, tk pos, tk mom
  static const unsigned int NXPTRACKS = 10; 
  xpTrack fXpTracks[NXPTRACKS];

  // -- reserve
  int            fInt1,    fInt2,    fInt3; 
  double         fDouble1, fDouble2, fDouble3;

private:

  ClassDef(TAnaMuon,1)

};

#endif
