#ifndef RPCDigi_RPCDigi_h
#define RPCDigi_RPCDigi_h

/** \class RPCDigi
 *
 * Digi for Rsisitive Plate Chamber
 *  
 *  $Date: 2006/04/05 15:03:07 $
 *  $Revision: 1.7 $
 *
 * \author I. Segoni -- CERN & M. Maggi -- INFN Bari
 *
 */

#include <boost/cstdint.hpp>

class RPCDigi{

public:
  explicit RPCDigi (int strip, int bx);
  RPCDigi ();

  bool operator==(const RPCDigi& digi) const;
  bool operator<(const RPCDigi& digi) const;

  int strip() const ;
  int bx() const;
  void print() const;

private:
  uint16_t strip_;
  int32_t  bx_; 
};


#include<iostream>
inline std::ostream & operator<<(std::ostream & o, const RPCDigi& digi) {
  return o << " " << digi.strip()
	   << " " << digi.bx();
}
#endif

