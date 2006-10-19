#include "L1Trigger/RegionalCaloTrigger/interface/L1RCTLookupTables.h"

// lookup method for HF
unsigned short L1RCTLookupTables::lookup(unsigned short hfenergy){
  //float energy = (float)hfenergy*0.5;
  float energy = (float) hfenergy*1.0;  // LSB for HCAL is now 1.0!
  return convertTo10Bits(energy);
}

// lookup method for barrel (ecal and hcal)
unsigned long L1RCTLookupTables::lookup(unsigned short ecal,unsigned short hcal,
					 unsigned short fgbit){
  float ecalLinear = convertEcal(ecal);
  float hcalLinear = convertHcal(hcal);
  unsigned long HE_FGBit = (calcHEBit(ecalLinear,hcalLinear) || fgbit);
  unsigned long etIn7Bits = convertTo7Bits(ecalLinear+hcalLinear);
  unsigned long etIn9Bits = convertTo9Bits(ecalLinear+hcalLinear);
  unsigned long activityBit = calcActivityBit(ecalLinear,hcalLinear);

  unsigned long shiftEtIn9Bits = etIn9Bits<<8;
  unsigned long shiftHE_FGBit = HE_FGBit<<7;
  unsigned long shiftActivityBit = activityBit<<17;
  unsigned long output=etIn7Bits+shiftHE_FGBit+shiftEtIn9Bits+shiftActivityBit;
  return output;
}

// converts compressed ecal energy to linear (real) scale
float L1RCTLookupTables::convertEcal(unsigned short ecal){
  return (float)ecal*0.5;
}

// converts compressed hcal energy to linear (real) scale
float L1RCTLookupTables::convertHcal(unsigned short hcal){
  // return (float)hcal*0.5;
  return (float) hcal*1.0;  // LSB for HCAL is now 1.0!
}

// calculates activity bit for each tower
unsigned short L1RCTLookupTables::calcActivityBit(float ecal, float hcal){
  return ((ecal > 2) || (hcal > 4));
}

// calculates h-over-e bit (true if hcal energy > ecal energy)
unsigned short L1RCTLookupTables::calcHEBit(float ecal, float hcal){
  return (hcal > ecal);
}

// sets maximum energy of 2^7 - 1
unsigned long L1RCTLookupTables::convertTo7Bits(float et){
  unsigned long etBits = (unsigned long)(et/0.5);
  unsigned long sevenBits = (unsigned long) pow(2,7)-1;
  if(etBits > sevenBits)
    return sevenBits;
  else
    return etBits;
}

// sets max energy of 2^9 - 1
unsigned long L1RCTLookupTables::convertTo9Bits(float et){
  unsigned long etBits = (unsigned short)(et/0.5);
  unsigned long nineBits = (unsigned long) pow(2,9)-1;
  if(etBits > nineBits)
    return nineBits;
  else
    return etBits;
}

// sets max energy of 2^10 - 1
unsigned long L1RCTLookupTables::convertTo10Bits(float et){
  unsigned long etBits = (unsigned short)(et/0.5);
  unsigned long tenBits = (unsigned long) pow(2,10)-1;
  if(etBits > tenBits)
    return tenBits;
  else
    return etBits;
}
