#ifndef _StreamerFileWriter_h
#define _StreamerFileWriter_h 

#include "FWCore/ParameterSet/interface/ParameterSet.h"

#include "IOPool/Streamer/interface/StreamerFileIO.h"
#include "IOPool/Streamer/interface/InitMsgBuilder.h"
#include "IOPool/Streamer/interface/EventMsgBuilder.h"

#include <iostream>
#include <vector>
#include <memory>
#include <string>

namespace edm
{
  class StreamerFileWriter 
  {
  public:

    explicit StreamerFileWriter(edm::ParameterSet const& ps);
    ~StreamerFileWriter();

    void doSerializeHeader(std::auto_ptr<InitMsgBuilder> init_message);    
    void doSerializeEvent(std::auto_ptr<EventMsgBuilder> msg);
 
  private:

    std::auto_ptr<StreamerOutputFile> stream_writer_;
    std::auto_ptr<StreamerOutputIndexFile> index_writer_; 
  };
}
#endif

