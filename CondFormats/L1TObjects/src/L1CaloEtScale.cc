// -*- C++ -*-
//
// Package:     L1TObjects
// Class  :     L1CaloEtScale
// 
// Implementation:
//     <Notes on implementation>
//
// Author:      
// Created:     Wed Sep 27 17:18:27 CEST 2006
// $Id: 

#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"

using std::vector;
using std::ostream;
using std::endl;


// default constructor (testing only!)
L1CaloEtScale::L1CaloEtScale() :
  m_linScaleMax(0x3ff),
  m_rankScaleMax(0x3f),
  m_linearLsb(1.0),
  m_thresholds(0x3f)
{

  for (unsigned i=0; i<m_rankScaleMax; i++) {
    m_thresholds[i] = m_linearLsb * i;
  }

}

// real constructor
L1CaloEtScale::L1CaloEtScale(const double linearLsbInGeV, const vector<double> thresholdsInGeV) :
  m_linScaleMax(0x3ff),
  m_rankScaleMax(0x3f),
  m_linearLsb(linearLsbInGeV),
  m_thresholds(thresholdsInGeV) {

  // protect against too many thresholds!
  //  while ( m_threshold.size() > (L1GctJetScale::maxRank+1) ) {
  //    m_thresholds.pop_back();
  //  }

}

// real constructor
L1CaloEtScale::L1CaloEtScale(unsigned linScaleMax, unsigned rankScaleMax, const double linearLsbInGeV, const vector<double> thresholdsInGeV) :
  m_linScaleMax(linScaleMax),
  m_rankScaleMax(rankScaleMax),
  m_linearLsb(linearLsbInGeV),
  m_thresholds(thresholdsInGeV) {

  // protect against too many thresholds!
  //  while ( m_threshold.size() > (L1GctJetScale::maxRank+1) ) {
  //    m_thresholds.pop_back();
  //  }

}

L1CaloEtScale::~L1CaloEtScale() {

}

// convert from linear Et to rank
uint16_t L1CaloEtScale::rank(const uint16_t linear) const {

  return rank( (linear & m_linScaleMax) * m_linearLsb);

}

/// convert from Et in GeV to rank
uint16_t L1CaloEtScale::rank(const double EtInGeV) const {

  uint16_t out = 0;

  for (unsigned i=0; i<m_thresholds.size() && i<(unsigned)(m_rankScaleMax+1); i++) {
    if ( EtInGeV >= m_thresholds[i] ) { out = i; }
  }

  return out & m_rankScaleMax;
}

// convert from rank to Et/GeV
double L1CaloEtScale::et(const uint16_t rank) const {

  // return bin lower edge
  return m_thresholds[rank];

}

void L1CaloEtScale::print(ostream& s) const {
  s << "L1CaloEtScale" << endl;
  s << "L1CaloEtScale : linear LSB = " << m_linearLsb << " GeV" << endl;
  for (unsigned i=0; i<m_thresholds.size(); i++) {
    s << "L1CaloEtScale : threshold " << i << " = " << m_thresholds[i] << " GeV" << endl;
  }
}


