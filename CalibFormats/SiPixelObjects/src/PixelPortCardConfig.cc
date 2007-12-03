//
// This class specifies the settings on the TKPCIFEC
// and the settings on the portcard
//
//
//
//

#include "CalibFormats/SiPixelObjects/interface/PixelPortCardConfig.h"
#include "CalibFormats/SiPixelObjects/interface/PixelPortCardSettingNames.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;
using namespace pos::PortCardSettingNames;
using namespace pos;

//added by Umesh
PixelPortCardConfig::PixelPortCardConfig(vector < vector< string> >  &tableMat):PixelConfigBase(" "," "," ")
{
   fillNameToAddress();
}

//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
PixelPortCardConfig::PixelPortCardConfig(std::string filename):
  PixelConfigBase(" "," "," "){

  //std::cout << "filename:"<<filename<<std::endl;

  unsigned int portcardpos=filename.find(std::string("portcard_"));
  //std::cout << "portcardpos:"<<portcardpos<<std::endl;
  assert(portcardpos!=std::string::npos);
  unsigned int datpos=filename.find(std::string(".dat"));
  //std::cout << "datpos:"<<datpos<<std::endl;
  assert(datpos!=std::string::npos);
  assert(datpos>portcardpos);
  
  portcardname_=filename.substr(portcardpos+9,datpos-portcardpos-9);

  //std::cout << "Portcard name extracted from file name:"<<portcardname_<<std::endl;

  std::ifstream in(filename.c_str());
  
  if(!in.good()){
    std::cout<<"Could not open:"<<filename<<std::endl;
    assert(0);
  }
  else {
    std::cout<<"Opened:"<<filename<<std::endl;
  }
  
  string dummy;

  in >> dummy;
  if ( dummy == "Name:" ) // check that port card name matches the file name
  {
    in >> dummy; assert( dummy==portcardname_ );
    in >> dummy;
  }
  if ( dummy == "Type:" ) // read in the type, defaulting to "fpix" if not specified
  {
    in >> type_;
    assert( type_ == "fpix" || type_ == "bpix" );
    in >> dummy;
  }
  else
  {
    type_ = "fpix";
  }
  fillNameToAddress();
               assert(dummy=="TKFECID:");        in >> TKFECID_;
  in >> dummy; assert(dummy=="ringAddress:");    in >> std::hex >> ringAddress_;
  in >> dummy; assert(dummy=="ccuAddress:");     in >> std::hex >> ccuAddress_;
  in >> dummy; assert(dummy=="channelAddress:"); in >> std::hex >> channelAddress_;
  in >> dummy; assert(dummy=="i2cSpeed:");       in >> std::hex >> i2cSpeed_;
    
  //std::cout<<TKFECAddress_<<", "<<ringAddress_<<", "<<ccuAddress_<<", "<<channelAddress_<<", "<<i2cSpeed_<<std::endl;
  
  assert( nameToAddress_.size() != 0 );
  do {
      
    std::string settingName;
    unsigned int i2c_address;
    unsigned int i2c_values;
    
    in >> settingName >> std::hex >> i2c_values >> std::dec;
    if (in.eof()) break;
    
    if ( settingName[settingName.size()-1] == ':' ) settingName.resize( settingName.size()-1 ); // remove ':' from end of string, if it's there
    
    // Special handling for AOHX_GainY
    if ( settingName.find("AOH") != string::npos && settingName.find("Gain") != string::npos )
    {
    	string::size_type GainPosition = settingName.find("Gain");
    	unsigned int whichAOH;
    	if ( settingName[GainPosition-2] == 'H' ) whichAOH = 0; // fpix
    	else  // bpix
    	{
    		char whichAOHDigit[2]={0,0};
    		whichAOHDigit[0]=settingName[GainPosition-2];
    		whichAOH = atoi( whichAOHDigit );
    	}
    	char digit[2]={0,0};
    	digit[0]=settingName[GainPosition+4];
    	unsigned int channelOnAOH = atoi( digit );
    	assert( (type_=="fpix" && whichAOH==0)||(type_=="bpix" && 1 <= whichAOH&&whichAOH <= 4) );
    	assert( 1 <= channelOnAOH && channelOnAOH <= 6 );
    	
    	if      ( whichAOH == 0 && channelOnAOH <= 3 ) i2c_address = k_fpix_AOH_Gain123_address;
    	else if ( whichAOH == 0 && channelOnAOH >= 4 ) i2c_address = k_fpix_AOH_Gain456_address;
    	else if ( whichAOH == 1 && channelOnAOH <= 3 ) i2c_address = k_bpix_AOH1_Gain123_address;
    	else if ( whichAOH == 1 && channelOnAOH >= 4 ) i2c_address = k_bpix_AOH1_Gain456_address;
    	else if ( whichAOH == 2 && channelOnAOH <= 3 ) i2c_address = k_bpix_AOH2_Gain123_address;
    	else if ( whichAOH == 2 && channelOnAOH >= 4 ) i2c_address = k_bpix_AOH2_Gain456_address;
    	else if ( whichAOH == 3 && channelOnAOH <= 3 ) i2c_address = k_bpix_AOH3_Gain123_address;
    	else if ( whichAOH == 3 && channelOnAOH >= 4 ) i2c_address = k_bpix_AOH3_Gain456_address;
    	else assert(0);
    	
    	// Search for this address in the previously-defined settings.
    	bool foundOne = false;
    	for (unsigned int i=0;i<device_.size();i++)
    	{
    		if ( device_[i].first == i2c_address ) // Change this setting in all previous instances
    		{
    			foundOne = true;
    			unsigned int oldValue = device_[i].second;
    			if      ( channelOnAOH%3 == 1 )
    				device_[i].second = (0x3c & oldValue) + ((i2c_values & 0x3)<<0); // replace bits 0 and 1 with i2c_values
    			else if ( channelOnAOH%3 == 2 )
    				device_[i].second = (0x33 & oldValue) + ((i2c_values & 0x3)<<2); // replace bits 2 and 3 with i2c_values
    			else if ( channelOnAOH%3 == 0 )
    				device_[i].second = (0x0f & oldValue) + ((i2c_values & 0x3)<<4); // replace bits 4 and 5 with i2c_values
    			else assert(0);
    			//std::cout << "Changed setting "<< k_fpix_AOH_Gain123 <<"(address 0x"<<std::hex<<k_fpix_AOH_Gain123_address<<") from 0x"<<oldValue<<" to 0x"<< device_[i].second << std::dec <<"\n";
    		}
    	}
    	if ( foundOne ) continue;
    	else // If this was not set previously, add this setting with the other two gains set to zero.
    	{
    		if      ( channelOnAOH%3 == 1 ) i2c_values  = ((i2c_values & 0x3)<<0);
    		else if ( channelOnAOH%3 == 2 ) i2c_values  = ((i2c_values & 0x3)<<2);
    		else if ( channelOnAOH%3 == 0 ) i2c_values  = ((i2c_values & 0x3)<<4);
    		else assert(0);
    	}
    }
    else // no special handling for this name
    {
    
    	std::map<std::string, unsigned int>::iterator foundName_itr = nameToAddress_.find(settingName);
    
    	if ( foundName_itr != nameToAddress_.end() )
    	{
    		i2c_address = foundName_itr->second;
    	}
    	else
    	{
    		i2c_address = strtoul(settingName.c_str(), 0, 16); // convert string to integer using base 16
    	}
    }
    
    //std::cout << "Setting name = " << settingName << ", i2c address = 0x" << std::hex << i2c_address <<", value = 0x"<< i2c_values << std::dec << std::endl;
      
    if (!in.eof()){
      pair<unsigned int, unsigned int> p(i2c_address, i2c_values);
      device_.push_back(p);
    }

  }
  while (!in.eof());
  
  in.close();

}

void PixelPortCardConfig::fillNameToAddress()
{
	if ( nameToAddress_.size() != 0 ) return;
	
	if ( type_ == "fpix" )
	{
		nameToAddress_[PortCardSettingNames::k_AOH_Bias1] = PortCardSettingNames::k_fpix_AOH_Bias1_address;
		nameToAddress_[PortCardSettingNames::k_AOH_Bias2] = PortCardSettingNames::k_fpix_AOH_Bias2_address;
		nameToAddress_[PortCardSettingNames::k_AOH_Bias3] = PortCardSettingNames::k_fpix_AOH_Bias3_address;
		nameToAddress_[PortCardSettingNames::k_AOH_Bias4] = PortCardSettingNames::k_fpix_AOH_Bias4_address;
		nameToAddress_[PortCardSettingNames::k_AOH_Bias5] = PortCardSettingNames::k_fpix_AOH_Bias5_address;
		nameToAddress_[PortCardSettingNames::k_AOH_Bias6] = PortCardSettingNames::k_fpix_AOH_Bias6_address;
		nameToAddress_[PortCardSettingNames::k_AOH_Gain123] = PortCardSettingNames::k_fpix_AOH_Gain123_address;
		nameToAddress_[PortCardSettingNames::k_AOH_Gain456] = PortCardSettingNames::k_fpix_AOH_Gain456_address;
		
		nameToAddress_[PortCardSettingNames::k_PLL_CTR1] = PortCardSettingNames::k_fpix_PLL_CTR1_address;
		nameToAddress_[PortCardSettingNames::k_PLL_CTR2] = PortCardSettingNames::k_fpix_PLL_CTR2_address;
		nameToAddress_[PortCardSettingNames::k_PLL_CTR3] = PortCardSettingNames::k_fpix_PLL_CTR3_address;
		nameToAddress_[PortCardSettingNames::k_PLL_CTR4] = PortCardSettingNames::k_fpix_PLL_CTR4_address;
		
		nameToAddress_[PortCardSettingNames::k_Delay25_RDA] = PortCardSettingNames::k_fpix_Delay25_RDA_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_RCL] = PortCardSettingNames::k_fpix_Delay25_RCL_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_SDA] = PortCardSettingNames::k_fpix_Delay25_SDA_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_TRG] = PortCardSettingNames::k_fpix_Delay25_TRG_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_SCL] = PortCardSettingNames::k_fpix_Delay25_SCL_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_GCR] = PortCardSettingNames::k_fpix_Delay25_GCR_address;
		
		nameToAddress_[PortCardSettingNames::k_DOH_Ch0Bias_CLK] = PortCardSettingNames::k_fpix_DOH_Ch0Bias_CLK_address;
		nameToAddress_[PortCardSettingNames::k_DOH_Ch1Bias_Data] = PortCardSettingNames::k_fpix_DOH_Ch1Bias_Data_address;
		nameToAddress_[PortCardSettingNames::k_DOH_Gain_SEU] = PortCardSettingNames::k_fpix_DOH_Gain_SEU_address;
	}
	else if ( type_ == "bpix" )
	{
		nameToAddress_[PortCardSettingNames::k_AOH1_Bias1] = PortCardSettingNames::k_bpix_AOH1_Bias1_address;
		nameToAddress_[PortCardSettingNames::k_AOH1_Bias2] = PortCardSettingNames::k_bpix_AOH1_Bias2_address;
		nameToAddress_[PortCardSettingNames::k_AOH1_Bias3] = PortCardSettingNames::k_bpix_AOH1_Bias3_address;
		nameToAddress_[PortCardSettingNames::k_AOH1_Bias4] = PortCardSettingNames::k_bpix_AOH1_Bias4_address;
		nameToAddress_[PortCardSettingNames::k_AOH1_Bias5] = PortCardSettingNames::k_bpix_AOH1_Bias5_address;
		nameToAddress_[PortCardSettingNames::k_AOH1_Bias6] = PortCardSettingNames::k_bpix_AOH1_Bias6_address;
		nameToAddress_[PortCardSettingNames::k_AOH1_Gain123] = PortCardSettingNames::k_bpix_AOH1_Gain123_address;
		nameToAddress_[PortCardSettingNames::k_AOH1_Gain456] = PortCardSettingNames::k_bpix_AOH1_Gain456_address;
		
		nameToAddress_[PortCardSettingNames::k_AOH2_Bias1] = PortCardSettingNames::k_bpix_AOH2_Bias1_address;
		nameToAddress_[PortCardSettingNames::k_AOH2_Bias2] = PortCardSettingNames::k_bpix_AOH2_Bias2_address;
		nameToAddress_[PortCardSettingNames::k_AOH2_Bias3] = PortCardSettingNames::k_bpix_AOH2_Bias3_address;
		nameToAddress_[PortCardSettingNames::k_AOH2_Bias4] = PortCardSettingNames::k_bpix_AOH2_Bias4_address;
		nameToAddress_[PortCardSettingNames::k_AOH2_Bias5] = PortCardSettingNames::k_bpix_AOH2_Bias5_address;
		nameToAddress_[PortCardSettingNames::k_AOH2_Bias6] = PortCardSettingNames::k_bpix_AOH2_Bias6_address;
		nameToAddress_[PortCardSettingNames::k_AOH2_Gain123] = PortCardSettingNames::k_bpix_AOH2_Gain123_address;
		nameToAddress_[PortCardSettingNames::k_AOH2_Gain456] = PortCardSettingNames::k_bpix_AOH2_Gain456_address;
		
		nameToAddress_[PortCardSettingNames::k_AOH3_Bias1] = PortCardSettingNames::k_bpix_AOH3_Bias1_address;
		nameToAddress_[PortCardSettingNames::k_AOH3_Bias2] = PortCardSettingNames::k_bpix_AOH3_Bias2_address;
		nameToAddress_[PortCardSettingNames::k_AOH3_Bias3] = PortCardSettingNames::k_bpix_AOH3_Bias3_address;
		nameToAddress_[PortCardSettingNames::k_AOH3_Bias4] = PortCardSettingNames::k_bpix_AOH3_Bias4_address;
		nameToAddress_[PortCardSettingNames::k_AOH3_Bias5] = PortCardSettingNames::k_bpix_AOH3_Bias5_address;
		nameToAddress_[PortCardSettingNames::k_AOH3_Bias6] = PortCardSettingNames::k_bpix_AOH3_Bias6_address;
		nameToAddress_[PortCardSettingNames::k_AOH3_Gain123] = PortCardSettingNames::k_bpix_AOH3_Gain123_address;
		nameToAddress_[PortCardSettingNames::k_AOH3_Gain456] = PortCardSettingNames::k_bpix_AOH3_Gain456_address;
		
		nameToAddress_[PortCardSettingNames::k_AOH4_Bias1] = PortCardSettingNames::k_bpix_AOH4_Bias1_address;
		nameToAddress_[PortCardSettingNames::k_AOH4_Bias2] = PortCardSettingNames::k_bpix_AOH4_Bias2_address;
		nameToAddress_[PortCardSettingNames::k_AOH4_Bias3] = PortCardSettingNames::k_bpix_AOH4_Bias3_address;
		nameToAddress_[PortCardSettingNames::k_AOH4_Bias4] = PortCardSettingNames::k_bpix_AOH4_Bias4_address;
		nameToAddress_[PortCardSettingNames::k_AOH4_Bias5] = PortCardSettingNames::k_bpix_AOH4_Bias5_address;
		nameToAddress_[PortCardSettingNames::k_AOH4_Bias6] = PortCardSettingNames::k_bpix_AOH4_Bias6_address;
		nameToAddress_[PortCardSettingNames::k_AOH4_Gain123] = PortCardSettingNames::k_bpix_AOH4_Gain123_address;
		nameToAddress_[PortCardSettingNames::k_AOH4_Gain456] = PortCardSettingNames::k_bpix_AOH4_Gain456_address;
		
		nameToAddress_[PortCardSettingNames::k_PLL_CTR1] = PortCardSettingNames::k_bpix_PLL_CTR1_address;
		nameToAddress_[PortCardSettingNames::k_PLL_CTR2] = PortCardSettingNames::k_bpix_PLL_CTR2_address;
		nameToAddress_[PortCardSettingNames::k_PLL_CTR3] = PortCardSettingNames::k_bpix_PLL_CTR3_address;
		nameToAddress_[PortCardSettingNames::k_PLL_CTR4] = PortCardSettingNames::k_bpix_PLL_CTR4_address;
		
		nameToAddress_[PortCardSettingNames::k_Delay25_RDA] = PortCardSettingNames::k_bpix_Delay25_RDA_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_RCL] = PortCardSettingNames::k_bpix_Delay25_RCL_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_SDA] = PortCardSettingNames::k_bpix_Delay25_SDA_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_TRG] = PortCardSettingNames::k_bpix_Delay25_TRG_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_SCL] = PortCardSettingNames::k_bpix_Delay25_SCL_address;
		nameToAddress_[PortCardSettingNames::k_Delay25_GCR] = PortCardSettingNames::k_bpix_Delay25_GCR_address;
		
		nameToAddress_[PortCardSettingNames::k_DOH_Ch0Bias_CLK] = PortCardSettingNames::k_bpix_DOH_Ch0Bias_CLK_address;
		nameToAddress_[PortCardSettingNames::k_DOH_Ch1Bias_Data] = PortCardSettingNames::k_bpix_DOH_Ch1Bias_Data_address;
		nameToAddress_[PortCardSettingNames::k_DOH_Gain_SEU] = PortCardSettingNames::k_bpix_DOH_Gain_SEU_address;
	}
	else assert(0);
	
	return;
}

void PixelPortCardConfig::writeASCII(std::string dir) const {

  if (dir!="") dir+="/";
  std::string filename=dir+"portcard_"+portcardname_+".dat";

  std::ofstream out(filename.c_str());
  if (!out.good()){
    std::cout << "Could not open file:"<<filename.c_str()<<std::endl;
    assert(0);
  }

  out << "Name: " << portcardname_ << std::endl;
  out << "Type: " << type_ << std::endl;
  out << "TKFECID: " << TKFECID_ << std::endl;
  out << "ringAddress: 0x" <<std::hex<< ringAddress_ <<std::dec<< std::endl;
  out << "ccuAddress: 0x" <<std::hex<< ccuAddress_ <<std::dec<< std::endl;
  
  out << "channelAddress: 0x" <<std::hex<< channelAddress_ <<std::dec<< std::endl;

  out << "i2cSpeed: 0x" <<std::hex<< i2cSpeed_ <<std::dec<< std::endl;

  for (unsigned int i=0;i<device_.size();i++)
  {
    unsigned int deviceAddress = device_.at(i).first;
    
    // Special handling for AOH gains
    if (    deviceAddress == k_fpix_AOH_Gain123_address
         || deviceAddress == k_fpix_AOH_Gain456_address
         || deviceAddress == k_bpix_AOH1_Gain123_address
         || deviceAddress == k_bpix_AOH1_Gain456_address
         || deviceAddress == k_bpix_AOH2_Gain123_address
         || deviceAddress == k_bpix_AOH2_Gain456_address
         || deviceAddress == k_bpix_AOH3_Gain123_address
         || deviceAddress == k_bpix_AOH3_Gain456_address
         || deviceAddress == k_bpix_AOH4_Gain123_address
         || deviceAddress == k_bpix_AOH4_Gain456_address
       )
    {
		std::string whichAOHString;
		unsigned int zeroOrThree;
		if      ( deviceAddress == k_fpix_AOH_Gain123_address )  { whichAOHString = "";  zeroOrThree = 0; }
		else if ( deviceAddress == k_fpix_AOH_Gain456_address )  { whichAOHString = "";  zeroOrThree = 3; }
		else if ( deviceAddress == k_bpix_AOH1_Gain123_address ) { whichAOHString = "1"; zeroOrThree = 0; }
		else if ( deviceAddress == k_bpix_AOH1_Gain456_address ) { whichAOHString = "1"; zeroOrThree = 3; }
		else if ( deviceAddress == k_bpix_AOH2_Gain123_address ) { whichAOHString = "2"; zeroOrThree = 0; }
		else if ( deviceAddress == k_bpix_AOH2_Gain456_address ) { whichAOHString = "2"; zeroOrThree = 3; }
		else if ( deviceAddress == k_bpix_AOH3_Gain123_address ) { whichAOHString = "3"; zeroOrThree = 0; }
		else if ( deviceAddress == k_bpix_AOH3_Gain456_address ) { whichAOHString = "3"; zeroOrThree = 3; }
		else if ( deviceAddress == k_bpix_AOH4_Gain123_address ) { whichAOHString = "4"; zeroOrThree = 0; }
		else if ( deviceAddress == k_bpix_AOH4_Gain456_address ) { whichAOHString = "4"; zeroOrThree = 3; }
		else assert(0);
		
		out << "AOH"<<whichAOHString<<"_Gain"<<zeroOrThree+1<<": 0x"<< (((device_[i].second) & 0x03)>>0) << std::endl; // output bits 0 & 1
		out << "AOH"<<whichAOHString<<"_Gain"<<zeroOrThree+2<<": 0x"<< (((device_[i].second) & 0x0c)>>2) << std::endl; // output bits 2 & 3
		out << "AOH"<<whichAOHString<<"_Gain"<<zeroOrThree+3<<": 0x"<< (((device_[i].second) & 0x30)>>4) << std::endl; // output bits 4 & 5
		continue;
    }
    // End of special handling
    
    // Check to see if there's a name corresponding to this address.
    std::string settingName = "";
    for ( std::map<std::string, unsigned int>::const_iterator nameToAddress_itr = nameToAddress_.begin(); nameToAddress_itr != nameToAddress_.end(); ++nameToAddress_itr )
    {
      if ( nameToAddress_itr->second == deviceAddress ) {settingName = nameToAddress_itr->first; break;}
    }
    
    if ( settingName=="" ) out << "0x" <<std::hex<< device_.at(i).first <<std::dec;
    else                   out << settingName << ":";
    
    out << " 0x" <<std::hex<< device_.at(i).second <<std::dec<< std::endl;
  }

  out.close();
}

unsigned int PixelPortCardConfig::getdevicesize() const{
  return device_.size();
}

std::string  PixelPortCardConfig::getTKFECID() const{
  return TKFECID_;
}

unsigned int PixelPortCardConfig::getringAddress() const{
  return ringAddress_;
}

unsigned int PixelPortCardConfig::getccuAddress() const{
  return ccuAddress_;
}

unsigned int PixelPortCardConfig::getchannelAddress() const{
  return channelAddress_;
}

unsigned int PixelPortCardConfig::geti2cSpeed() const{
  return i2cSpeed_;
}

std::string  PixelPortCardConfig::gettype() const{
  return type_;
}

unsigned int PixelPortCardConfig::getdeviceAddress(unsigned int i) const{
  assert(i<device_.size());
  return device_[i].first;
}

unsigned int PixelPortCardConfig::getdeviceValues(unsigned int i) const{
  assert(i<device_.size());
  return device_[i].second;
}

void PixelPortCardConfig::setdeviceValues(unsigned int address, unsigned int value) {
  for (unsigned int i=0; i<device_.size(); i++)
    {
      if( device_.at(i).first==address )
        {
          device_.at(i).second=value;
          break;
        }
    }
  return;
}

void PixelPortCardConfig::setdeviceValues(std::string settingName, unsigned int value)
{
	setdeviceValues( getdeviceAddressForSetting(settingName), value );
	return;
}

unsigned int PixelPortCardConfig::getdeviceAddressForSetting(std::string settingName) const
{
	std::map<std::string, unsigned int>::const_iterator foundName_itr = nameToAddress_.find(settingName);
	assert( foundName_itr != nameToAddress_.end() );
	return foundName_itr->second;
}

unsigned int PixelPortCardConfig::getdeviceValuesForSetting(std::string settingName) const
{
	unsigned int address = getdeviceAddressForSetting(settingName);
	for (unsigned int i=0; i<device_.size(); i++)
    {
      if( device_.at(i).first==address )
        {
          return device_.at(i).second;
        }
    }
  assert(0); // didn't find this device
  return 0;
}

unsigned int PixelPortCardConfig::AOHBiasAddressFromAOHNumber(unsigned int AOHNumber) const
{
	if ( type_ == "fpix" )
	{
		if      (AOHNumber == 1) return PortCardSettingNames::k_fpix_AOH_Bias1_address;
		else if (AOHNumber == 2) return PortCardSettingNames::k_fpix_AOH_Bias2_address;
		else if (AOHNumber == 3) return PortCardSettingNames::k_fpix_AOH_Bias3_address;
		else if (AOHNumber == 4) return PortCardSettingNames::k_fpix_AOH_Bias4_address;
		else if (AOHNumber == 5) return PortCardSettingNames::k_fpix_AOH_Bias5_address;
		else if (AOHNumber == 6) return PortCardSettingNames::k_fpix_AOH_Bias6_address;
		else {std::cout << "ERROR: For fpix, AOH number must be in the range 1-6, but the given AOH number was "<<AOHNumber<<"."<<std::endl; assert(0);}
	}
	else if ( type_ == "bpix" )
	{
		if      (AOHNumber == 1) return PortCardSettingNames::k_bpix_AOH1_Bias1_address;
		else if (AOHNumber == 2) return PortCardSettingNames::k_bpix_AOH1_Bias2_address;
		else if (AOHNumber == 3) return PortCardSettingNames::k_bpix_AOH1_Bias3_address;
		else if (AOHNumber == 4) return PortCardSettingNames::k_bpix_AOH1_Bias4_address;
		else if (AOHNumber == 5) return PortCardSettingNames::k_bpix_AOH1_Bias5_address;
		else if (AOHNumber == 6) return PortCardSettingNames::k_bpix_AOH1_Bias6_address;
		else if (AOHNumber == 7) return PortCardSettingNames::k_bpix_AOH2_Bias1_address;
		else if (AOHNumber == 8) return PortCardSettingNames::k_bpix_AOH2_Bias2_address;
		else if (AOHNumber == 9) return PortCardSettingNames::k_bpix_AOH2_Bias3_address;
		else if (AOHNumber ==10) return PortCardSettingNames::k_bpix_AOH2_Bias4_address;
		else if (AOHNumber ==11) return PortCardSettingNames::k_bpix_AOH2_Bias5_address;
		else if (AOHNumber ==12) return PortCardSettingNames::k_bpix_AOH2_Bias6_address;
		else if (AOHNumber ==13) return PortCardSettingNames::k_bpix_AOH3_Bias1_address;
		else if (AOHNumber ==14) return PortCardSettingNames::k_bpix_AOH3_Bias2_address;
		else if (AOHNumber ==15) return PortCardSettingNames::k_bpix_AOH3_Bias3_address;
		else if (AOHNumber ==16) return PortCardSettingNames::k_bpix_AOH3_Bias4_address;
		else if (AOHNumber ==17) return PortCardSettingNames::k_bpix_AOH3_Bias5_address;
		else if (AOHNumber ==18) return PortCardSettingNames::k_bpix_AOH3_Bias6_address;
		else if (AOHNumber ==19) return PortCardSettingNames::k_bpix_AOH4_Bias1_address;
		else if (AOHNumber ==20) return PortCardSettingNames::k_bpix_AOH4_Bias2_address;
		else if (AOHNumber ==21) return PortCardSettingNames::k_bpix_AOH4_Bias3_address;
		else if (AOHNumber ==22) return PortCardSettingNames::k_bpix_AOH4_Bias4_address;
		else if (AOHNumber ==23) return PortCardSettingNames::k_bpix_AOH4_Bias5_address;
		else if (AOHNumber ==24) return PortCardSettingNames::k_bpix_AOH4_Bias6_address;
		else {std::cout << "ERROR: For bpix, AOH number must be in the range 1-24, but the given AOH number was "<<AOHNumber<<"."<<std::endl; assert(0);}
	}
	else assert(0);
}
