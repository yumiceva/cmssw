//
// This class collects the information
// about the range of DAC settings used
// in scans of the DACs.
//
//
//

#include "CalibFormats/SiPixelObjects/interface/PixelDACScanRange.h"
#include "CalibFormats/SiPixelObjects/interface/PixelDACNames.h"
#include <iostream>
#include <assert.h>

PixelDACScanRange::PixelDACScanRange(std::string name, unsigned int first, 
                                     unsigned int last, unsigned int step,
                                     unsigned int index){
  
  name_=name;
  first_=first;
  last_=last;
  step_=step;
  index_=index;
  
  if (name==pos::k_DACName_Vdd) {
    dacchannel_=pos::k_DACAddress_Vdd;
  } else if (name==pos::k_DACName_Vana) {
    dacchannel_=pos::k_DACAddress_Vana;
  } else if (name==pos::k_DACName_Vsf) {
    dacchannel_=pos::k_DACAddress_Vsf;
  } else if (name==pos::k_DACName_Vcomp) {
    dacchannel_=pos::k_DACAddress_Vcomp;
  } else if (name==pos::k_DACName_Vleak) {
    dacchannel_=pos::k_DACAddress_Vleak;
  } else if (name==pos::k_DACName_VrgPr) {
    dacchannel_=pos::k_DACAddress_VrgPr;
  } else if (name==pos::k_DACName_VwllPr) {
    dacchannel_=pos::k_DACAddress_VwllPr;
  } else if (name==pos::k_DACName_VrgSh) {
    dacchannel_=pos::k_DACAddress_VrgSh;
  } else if (name==pos::k_DACName_VwllSh) {
    dacchannel_=pos::k_DACAddress_VwllSh;
  } else if (name==pos::k_DACName_VHldDel) {
    dacchannel_=pos::k_DACAddress_VHldDel;
  } else if (name==pos::k_DACName_Vtrim) {
    dacchannel_=pos::k_DACAddress_Vtrim;
  } else if (name==pos::k_DACName_VcThr) {
    dacchannel_=pos::k_DACAddress_VcThr;
  } else if (name==pos::k_DACName_VIbias_bus) {
    dacchannel_=pos::k_DACAddress_VIbias_bus;
  } else if (name==pos::k_DACName_VIbias_sf) {
    dacchannel_=pos::k_DACAddress_VIbias_sf;
  } else if (name==pos::k_DACName_VOffsetOp) {
    dacchannel_=pos::k_DACAddress_VOffsetOp;
  } else if (name==pos::k_DACName_VbiasOp) {
    dacchannel_=pos::k_DACAddress_VbiasOp;
  } else if (name==pos::k_DACName_VOffsetRO) {
    dacchannel_=pos::k_DACAddress_VOffsetRO;
  } else if (name==pos::k_DACName_VIon) {
    dacchannel_=pos::k_DACAddress_VIon;
  } else if (name==pos::k_DACName_VIbias_PH) {
    dacchannel_=pos::k_DACAddress_VIbias_PH;
  } else if (name==pos::k_DACName_VIbias_DAC) {
    dacchannel_=pos::k_DACAddress_VIbias_DAC;
  } else if (name==pos::k_DACName_VIbias_roc) {
    dacchannel_=pos::k_DACAddress_VIbias_roc;
  } else if (name==pos::k_DACName_VIColOr) {
    dacchannel_=pos::k_DACAddress_VIColOr;
  } else if (name==pos::k_DACName_Vnpix) {
    dacchannel_=pos::k_DACAddress_Vnpix;
  } else if (name==pos::k_DACName_VsumCol) {
    dacchannel_=pos::k_DACAddress_VsumCol;
  } else if (name==pos::k_DACName_Vcal) {
    dacchannel_=pos::k_DACAddress_Vcal;
  } else if (name==pos::k_DACName_CalDel) {
    dacchannel_=pos::k_DACAddress_CalDel;
  } else if (name==pos::k_DACName_WBC) {
    dacchannel_=pos::k_DACAddress_WBC;
  } else if (name==pos::k_DACName_ChipContReg) {
    dacchannel_=pos::k_DACAddress_ChipContReg;
  } else {
    std::cout << "The dac name:"<<name<<" is not known!"<<std::endl;
    assert(0);
  }
}

