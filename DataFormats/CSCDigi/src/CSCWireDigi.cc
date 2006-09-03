/**\class CSCWireDigi
 *
 * Digi for CSC anode wires.
 * Based on modified DTDigi.
 *
 * $Date: 2006/04/06 11:18:37 $
 * $Revision: 1.3 $
 *
 * \author N. Terentiev, CMU
 */

#include <DataFormats/CSCDigi/interface/CSCWireDigi.h>

#include <iostream>

using namespace std;

  /// Constructors

CSCWireDigi::CSCWireDigi (int wire, int tbinb){
  wire_  = wire;
  tbinb_ = tbinb;
}

   /// Default
CSCWireDigi::CSCWireDigi (){
  wire_ = 0;
  tbinb_ = 0;
}

  /// return tbin number (obsolete, use getTimeBin() instead)
int CSCWireDigi::getBeamCrossingTag() const {
  uint16_t tbit=1;
  int tbin=-1;
  for(int i=0;i<16;i++) {
    if(tbit & tbinb_) tbin=i;
    if(tbin>-1) break;
    tbit=tbit<<1;
  }
  return tbin;
}
  /// return first tbin ON number
int CSCWireDigi::getTimeBin()         const {
  uint16_t tbit=1;
  int tbin=-1;
  for(int i=0;i<16;i++) {
    if(tbit & tbinb_) tbin=i;
    if(tbin>-1) break;
    tbit=tbit<<1;
  }
  return tbin;
}
  /// return vector of time bins ON
std::vector<int> CSCWireDigi::getTimeBinsOn() const {
  std::vector<int> tbins;
  uint16_t tbit=1;
  for(int i=0;i<16;i++) {
    if(tbit & tbinb_) tbins.push_back(i);
    tbit=tbit<<1;
  }
  return tbins;
}

  /// Debug

void CSCWireDigi::print() const {
  std::cout << " CSC Wire " << getWireGroup() 
            << " CSC Wire First Time Bin On" << getTimeBin() 
            << std::endl;
  std::cout << " CSC Time Bins On ";
  std::vector<int> tbins=getTimeBinsOn();
  for(unsigned int i=0; i<tbins.size();i++) std::cout<<tbins[i]<<" ";
  std::cout<<std::endl; 
}
