#include "DataFormats/HcalDetId/interface/HcalOtherDetId.h"

HcalOtherDetId::HcalOtherDetId() {
}
  
HcalOtherDetId::HcalOtherDetId(uint32_t rawid) : DetId(rawid) {
}
  
HcalOtherDetId::HcalOtherDetId(HcalOtherSubdetector other_type)
  : DetId(Hcal,HcalOther) {
  id_|= (int(other_type&0x1F)<<20);
}
  
HcalOtherDetId::HcalOtherDetId(const DetId& gen) {
  if (gen.det()!=Hcal || gen.subdetId()!=HcalOther) {
    throw new std::exception();
  }
  id_=gen.rawId();    
}
  
HcalOtherDetId& HcalOtherDetId::operator=(const DetId& gen) {
  if (gen.det()!=Hcal || gen.subdetId()!=HcalOther) {
    throw new std::exception();
  }
  id_=gen.rawId();
  return *this;
}
  


