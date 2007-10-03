#ifndef PixelFEDConfig_h
#define PixelFEDConfig_h
//
// This class specifies which FED boards
// are used and how they are addressed
// 
// 
// 
//
//
//

#include <vector>
#include <string>
#include "CalibFormats/SiPixelObjects/interface/PixelConfigBase.h"
#include "CalibFormats/SiPixelObjects/interface/PixelFEDParameters.h"

namespace pos{
  class PixelFEDConfig: public PixelConfigBase {

  public:

    PixelFEDConfig(std::string filename);  //  <---- Modified for the conversion from parallel vectors to object that contain the configuration
   
    PixelFEDConfig(std::vector<std::vector<std::string> >& tableMat ); 

    unsigned int getNFEDBoards() const;

    unsigned int getFEDNumber(unsigned int i) const;
    unsigned int getCrate(unsigned int i) const;
    unsigned int getVMEBaseAddress(unsigned int i) const;
    unsigned int crateFromFEDNumber(unsigned int fednumber) const;
    unsigned int VMEBaseAddressFromFEDNumber(unsigned int fednumber) const;
    
    unsigned int FEDNumberFromCrateAndVMEBaseAddress(unsigned int crate, unsigned int vmebaseaddress) const;

    //friend std::ostream& operator<<(std::ostream& s, const PixelDetectorconfig& config);

  private:

    //Already fixed from parallel vectors to vector of objects .... the object that contains the FED config is PixelFEDParameters 
    
    //    std::vector<unsigned int> fednumber_; 
    //    std::vector<unsigned int> crate_;
    //    std::vector<unsigned int> vmebaseaddress_;
    
    std::vector< PixelFEDParameters > fedconfig_;
 
  };
}
#endif
