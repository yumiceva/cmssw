#include "CondFormats/SiPixelObjects/interface/SiPixelGainCalibrationOffline.h"
#include "FWCore/Utilities/interface/Exception.h"

//
// Constructors
//
SiPixelGainCalibrationOffline::SiPixelGainCalibrationOffline() :
  minPed_(0.),
  maxPed_(255.),
  minGain_(0.),
  maxGain_(255.)
{
}
//
SiPixelGainCalibrationOffline::SiPixelGainCalibrationOffline(float minPed, float maxPed, float minGain, float maxGain) :
  minPed_(minPed),
  maxPed_(maxPed),
  minGain_(minGain),
  maxGain_(maxGain)
{
}

bool SiPixelGainCalibrationOffline::put(const uint32_t& DetId, Range input, const int& nCols) {
  // put in SiPixelGainCalibrationOffline of DetId

  Registry::iterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,SiPixelGainCalibrationOffline::StrictWeakOrdering());
  if (p!=indexes.end() && p->detid==DetId)
    return false;
  
  size_t sd= input.second-input.first;
  DetRegistry detregistry;
  detregistry.detid=DetId;
  detregistry.ncols=nCols;
  detregistry.ibegin=v_pedestals.size();
  detregistry.iend=v_pedestals.size()+sd;
  indexes.insert(p,detregistry);

  v_pedestals.insert(v_pedestals.end(),input.first,input.second);
  return true;
}

const int SiPixelGainCalibrationOffline::getNCols(const uint32_t& DetId) const {
  // get number of columns of DetId
  RegistryIterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,SiPixelGainCalibrationOffline::StrictWeakOrdering());
  if (p==indexes.end()|| p->detid!=DetId) 
    return 0;
  else
    return p->ncols; 
}

const SiPixelGainCalibrationOffline::Range SiPixelGainCalibrationOffline::getRange(const uint32_t& DetId) const {
  // get SiPixelGainCalibrationOffline Range of DetId
  
  RegistryIterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,SiPixelGainCalibrationOffline::StrictWeakOrdering());
  if (p==indexes.end()|| p->detid!=DetId) 
    return SiPixelGainCalibrationOffline::Range(v_pedestals.end(),v_pedestals.end()); 
  else 
    return SiPixelGainCalibrationOffline::Range(v_pedestals.begin()+p->ibegin,v_pedestals.begin()+p->iend);
}

const std::pair<const SiPixelGainCalibrationOffline::Range, const int>
SiPixelGainCalibrationOffline::getRangeAndNCols(const uint32_t& DetId) const {
  RegistryIterator p = std::lower_bound(indexes.begin(),indexes.end(),DetId,SiPixelGainCalibrationOffline::StrictWeakOrdering());
  if (p==indexes.end()|| p->detid!=DetId) 
    return std::make_pair(SiPixelGainCalibrationOffline::Range(v_pedestals.end(),v_pedestals.end()), 0); 
  else 
    return std::make_pair(SiPixelGainCalibrationOffline::Range(v_pedestals.begin()+p->ibegin,v_pedestals.begin()+p->iend), p->ncols);
}
  

void SiPixelGainCalibrationOffline::getDetIds(std::vector<uint32_t>& DetIds_) const {
  // returns vector of DetIds in map
  SiPixelGainCalibrationOffline::RegistryIterator begin = indexes.begin();
  SiPixelGainCalibrationOffline::RegistryIterator end   = indexes.end();
  for (SiPixelGainCalibrationOffline::RegistryIterator p=begin; p != end; ++p) {
    DetIds_.push_back(p->detid);
  }
}

void SiPixelGainCalibrationOffline::setDataGain(float gain, const int& nRows, std::vector<char>& vped){
   
  
  float theEncodedGain  = encodeGain(gain);

  char gain_  = (static_cast<char>(theEncodedGain)) & 0xFF;

  vped.resize(vped.size()+1);
  //check to make sure the column is being placed in the right place in the blob
  if (vped.size() % (nRows + 1) != 0) 
  {
    throw cms::Exception("FillError")
      << "[SiPixelGainCalibrationOffline::setDataGain] Column gain average must be filled after the pedestal for each row has been added";
  }  
  // insert in vector of char
  ::memcpy((void*)(&vped[vped.size()-1]),(void*)(&gain_),1);
}

void SiPixelGainCalibrationOffline::setDataPedestal(float pedestal,  std::vector<char>& vped){

  float theEncodedPedestal  = encodePed(pedestal);

  char ped_  = (static_cast<char>(theEncodedPedestal)) & 0xFF;

  vped.resize(vped.size()+1);
  // insert in vector of char
  ::memcpy((void*)(&vped[vped.size()-1]),(void*)(&ped_),1);
}

float SiPixelGainCalibrationOffline::getPed(const int& col, const int& row, const Range& range, const int& nCols) const {

  int nRows = (range.second-range.first)/nCols - 1;
  //int startOfColumn = *(range.first) + col*(nRows+1);
  const DecodingStructure & s = (const DecodingStructure & ) *(range.first + col*(nRows+1)+row);
  if (col >= nCols || row >= nRows){
    throw cms::Exception("CorruptedData")
      << "[SiPixelGainCalibrationOffline::getPed] Pixel out of range: col " << col << " row " << row;
  }  
  return decodePed(s.datum & 0xFF);  
}

float SiPixelGainCalibrationOffline::getGain(const int& col, const Range& range, const int& nCols) const {

  int lengthOfColumnData = (range.second-range.first)/nCols;
  const DecodingStructure & s = (const DecodingStructure & ) *(range.first+(col+1)*(lengthOfColumnData));

  if (col >= nCols){
    throw cms::Exception("CorruptedData")
      << "[SiPixelGainCalibrationOffline::getPed] Pixel out of range: col " << col;
  }  
  return decodeGain(s.datum & 0xFF);
}

float SiPixelGainCalibrationOffline::encodeGain( const float& gain ) {
  
  if(gain < minGain_ || gain > maxGain_ ) {
    throw cms::Exception("InsertFailure")
      << "[SiPixelGainCalibrationOffline::encodeGain] Trying to encode gain out of range\n";
  } else {
    double precision   = (maxGain_-minGain_)/255.;
    float  encodedGain = (float)((gain-minGain_)/precision);
    return encodedGain;
  }

}

float SiPixelGainCalibrationOffline::encodePed( const float& ped ) {

  if(ped < minPed_ || ped > maxPed_ ) {
    throw cms::Exception("InsertFailure")
      << "[SiPixelGainCalibrationOffline::encodePed] Trying to encode pedestal out of range\n";
  } else {
    double precision   = (maxPed_-minPed_)/255.;
    float  encodedPed = (float)((ped-minPed_)/precision);
    return encodedPed;
  }

}

float SiPixelGainCalibrationOffline::decodePed( unsigned int ped ) const {

  double precision = (maxPed_-minPed_)/255.;
  float decodedPed = (float)(ped*precision + minPed_);
  return decodedPed;

}

float SiPixelGainCalibrationOffline::decodeGain( unsigned int gain ) const {

  double precision = (maxGain_-minGain_)/255.;
  float decodedGain = (float)(gain*precision + minGain_);
  return decodedGain;

}


// functions for template compatibility with other payloads. should never run.
float SiPixelGainCalibrationOffline::getPed(const int& col, const Range& range, const int& nCols) const {
   throw cms::Exception("ConfigurationError")
      << "[SiPixelGainCalibrationOffline::getPed(col, range)] Data is stored at pixel granularity in this payload.  Please use getPed(col, row, range, ncols)";
   return -1.;
}

float SiPixelGainCalibrationOffline::getGain(const int& col, const int& row, const Range& range, const int& nCols) const {
   throw cms::Exception("ConfigurationError")
      << "[SiPixelGainCalibrationOffline::getGain(col, row, range)] Gain is stored at column granularity in this payload.  Please use getGain(col, range, ncols)";
   return -1.;
}
