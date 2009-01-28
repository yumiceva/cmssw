#include "CondFormats/Common/interface/IOVSequence.h"

#include<iostream>

int main() {


  cond::IOVSequence iov;
  
  iov.add(10,"a");
  iov.add(20,"b");
  if (iov.add(30,"c")!=2) std::cerr << "error pos" << std::endl;
  iov.add(40,"d");

  cond::IOVSequence::Container const & v = iov.iovs();

  if (v.size()!=4) std::cerr << "error size" << std::endl;
  
  if (iov.find(0)!=v.end()) std::cerr << "error find 0" << std::endl;
  if (iov.find(5)!=v.end()) std::cerr << "error find 5" << std::endl;
  if (iov.find(10)!=v.begin()) std::cerr << "error find 10" << std::endl;
  if (iov.find(15)!=(v.begin())) std::cerr << "error find 15" << std::endl;
  if (iov.find(25)!=(v.begin()+1)) std::cerr << "error find 25" << std::endl;
  if (iov.find(40)!=v.end()-1) std::cerr << "error find 45" << std::endl;
  if (iov.find(45)!=v.end()-1) std::cerr << "error find 45" << std::endl;


  return 0;

}
