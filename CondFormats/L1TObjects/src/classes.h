#include "CondFormats/L1TObjects/interface/L1MuScale.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerScales.h"
#include "CondFormats/L1TObjects/interface/L1MuTriggerPtScale.h"
#include "CondFormats/L1TObjects/interface/L1MuGMTParameters.h"
#include "CondFormats/L1TObjects/interface/L1MuGMTChannelMask.h"
#include "CondFormats/L1TObjects/interface/L1MuGMTScales.h"
#include "CondFormats/L1TObjects/interface/L1CaloEtScale.h"
#include "CondFormats/L1TObjects/interface/L1GctChannelMask.h"
#include "CondFormats/L1TObjects/interface/L1GctJetCounterSetup.h"
#include "CondFormats/L1TObjects/interface/L1GctJetEtCalibrationFunction.h"
#include "CondFormats/L1TObjects/interface/L1GctJetFinderParams.h"
#include "CondFormats/L1TObjects/interface/L1GctHfLutSetup.h"
#include "CondFormats/L1TObjects/interface/L1MuDTExtLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTPhiLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTPtaLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTEtaPatternLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTQualPatternLut.h"
#include "CondFormats/L1TObjects/interface/L1MuDTTFParameters.h"
#include "CondFormats/L1TObjects/interface/L1MuCSCTFConfiguration.h"
#include "CondFormats/L1TObjects/interface/L1MuCSCPtLut.h"
#include "CondFormats/L1TObjects/interface/L1MuCSCTFAlignment.h"
#include "CondFormats/L1TObjects/interface/L1RPCConfig.h"
#include "CondFormats/L1TObjects/interface/L1RCTParameters.h"
#include "CondFormats/L1TObjects/interface/L1RCTChannelMask.h"
#include "CondFormats/L1TObjects/interface/L1CaloEcalScale.h"
#include "CondFormats/L1TObjects/interface/L1CaloHcalScale.h"
#include "CondFormats/L1TObjects/interface/L1TriggerKey.h"
#include "CondFormats/L1TObjects/interface/L1TriggerKeyList.h"

#include "CondFormats/L1TObjects/interface/L1GtStableParameters.h"
#include "CondFormats/L1TObjects/interface/L1GtParameters.h"
#include "CondFormats/L1TObjects/interface/L1GtPrescaleFactors.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMask.h"
#include "CondFormats/L1TObjects/interface/L1GtBoard.h"
#include "CondFormats/L1TObjects/interface/L1GtBoardMaps.h"
#include "CondFormats/L1TObjects/interface/L1GtPsbConfig.h"
#include "CondFormats/L1TObjects/interface/L1GtPsbSetup.h"
#include "CondFormats/L1TObjects/interface/L1GtCondition.h"
#include "CondFormats/L1TObjects/interface/L1GtAlgorithm.h"
#include "CondFormats/L1TObjects/interface/L1GtTriggerMenu.h"

#include "CondFormats/L1TObjects/interface/L1CaloGeometry.h"

namespace {
  namespace {
    std::map< std::string, std::map< std::string, std::string > > dummy0 ;
    std::pair< std::string, std::map< std::string, std::string > > dummy0a ;
    std::pair< std::string, std::string > dummy0b ;
    std::vector<L1MuDTExtLut::LUT> dummy3 ;
    std::vector<L1GtMuonTemplate> dummy4 ;
    std::vector<L1GtCaloTemplate> dummy5 ;
    std::vector<L1GtEnergySumTemplate> dummy6 ;
    std::vector<L1GtJetCountsTemplate> dummy7 ;
    std::vector<L1GtCorrelationTemplate> dummy8 ;
    std::vector<L1GtCastorTemplate> dummy8a ;
    std::vector<L1GtHfBitCountsTemplate> dummy8b ;
    std::vector<L1GtHfRingEtSumsTemplate> dummy8c ;
    std::map< std::string, L1GtAlgorithm > dummy9 ;
    std::pair< std::string, L1GtAlgorithm > dummy9a ;
    std::pair<L1GctHfLutSetup::hfLutType,std::vector<uint16_t> > dummy10 ;
    std::pair<short,L1MuDTEtaPattern> dummy11 ;
    std::pair<std::pair<short,short>,std::pair<short,std::vector<short> > > dummy12 ;
    std::pair<int, std::vector<L1GtObject> > dummy13 ;
  }
}
