// $Id: StreamQueue.h,v 1.3.14.1 2011/01/24 12:18:39 mommsen Exp $
/// @file: StreamQueue.h 

#ifndef EventFilter_StorageManager_StreamQueue_h
#define EventFilter_StorageManager_StreamQueue_h

#include "boost/shared_ptr.hpp"
#include "EventFilter/StorageManager/interface/ConcurrentQueue.h"
#include "EventFilter/StorageManager/interface/I2OChain.h"

namespace stor {

  /**
   * Queue holding I2OChains of events to be written to disk
   *
   * $Author: mommsen $
   * $Revision: 1.3.14.1 $
   * $Date: 2011/01/24 12:18:39 $
   */

  typedef ConcurrentQueue<I2OChain> StreamQueue;  
  typedef boost::shared_ptr<StreamQueue> StreamQueuePtr;

} // namespace stor

#endif // EventFilter_StorageManager_StreamQueue_h 


/// emacs configuration
/// Local Variables: -
/// mode: c++ -
/// c-basic-offset: 2 -
/// indent-tabs-mode: nil -
/// End: -
