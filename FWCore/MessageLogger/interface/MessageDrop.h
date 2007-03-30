#ifndef MessageLogger_MessageDrop_h
#define MessageLogger_MessageDrop_h

// -*- C++ -*-
//
// Package:     MessageLogger
// Class  :     MessageDrop
//
/**\class MessageDrop MessageDrop.h 

 Description: <one line class summary>

 Usage:
    <usage>

*/

//
// Original Author:  M. Fischler and Jim Kowalkowski
//         Created:  Tues Feb 14 16:38:19 CST 2006
// $Id: MessageDrop.h,v 1.7 2006/10/02 20:31:21 marafino Exp $
//

// Framework include files

#include "FWCore/Utilities/interface/EDMException.h"	// change log 4


// system include files

#include <string>

// Change log
//
//  1  mf 5/12/06	initialize debugEnabled to true, to avoid unitialized
//			data detection in memory checks (and to be safe in
//			getting enabled output independant of timings) 
//
//  4  mf 2/22/07	static ex_p to have a way to convey exceptions to throw
//			(this is needed when configuring could lead to an 
//			exception, for example)

// user include files

namespace edm {

struct MessageDrop {
private:
  MessageDrop() 
  : moduleName ("")
  , runEvent("pre-events")
  , debugEnabled(true) 					// change log 1
  , infoEnabled(true) 					// change log 3
  , warningEnabled(true)				// change log 3
  {  } 
public:
  static MessageDrop * instance ();
  std::string moduleName;
  std::string runEvent;
  bool debugEnabled;
  bool infoEnabled;
  bool warningEnabled;
  static edm::Exception * ex_p;				// change log 4
};

} // end of namespace edm


#endif // MessageLogger_MessageDrop_h
