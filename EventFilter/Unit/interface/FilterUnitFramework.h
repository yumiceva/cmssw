#ifndef FilterUnitFramework_H
#define FilterUnitFramework_H
//
///
//
//
/// Author: E.Meschi PH/CMD
/// 
//
//  Modification history:
//    $Log: FilterUnitFramework.h,v $
//    Revision 1.9  2005/10/07 15:32:33  meschi
//    flush logic
//
//    Revision 1.8  2005/07/15 07:47:39  meschi
//    correct run initialization and dataset management
//
//    Revision 1.7  2005/06/28 08:49:02  meschi
//    added support for orcarc upload
//
//    Revision 1.6  2005/06/17 12:20:28  meschi
//    added state name and collector config
//
//    Revision 1.5  2005/02/23 09:17:19  meschi
//    XDAQ3 Porting
//
//    Revision 1.4  2005/01/06 11:30:58  meschi
//    switchable debug printout
//
//    Revision 1.3  2004/09/22 09:47:28  meschi
//    mods for evb integration debugging
//
//    Revision 1.2  2004/07/06 08:18:31  meschi
//    more thread handling
//
//    Revision 1.1  2004/06/30 22:16:35  meschi
//    now compatible with cobra task and daqevent
//
//
//
#if defined(linux) || defined(macosx)
using namespace std;
#endif      

#include <semaphore.h>
#include <map>
#include <vector>
#include <deque>
#include <pthread.h>

#include "toolbox/include/Task.h"
#include "toolbox/include/TaskGroup.h"
#include "toolbox/include/toolbox/Chrono.h"
#include "xdata/include/xdata/UnsignedLong.h"
#include "xoap/include/xoap/SOAPEnvelope.h"
#include "xoap/include/xoap/SOAPBody.h"
#include "xoap/include/xoap/domutils.h"
#include "xgi/include/xgi/Input.h"
#include "xgi/include/xgi/Output.h"
#include "xgi/include/xgi/exception/Exception.h"
#include "xdaq/include/xdaq/NamespaceURI.h"

#include "EventFilter/Unit/interface/FUAdapter.h"
#include "EventFilter/Utilities/interface/EPStateMachine.h"
#include "EventFilter/Utilities/interface/Css.h"


//
// forward-declarations 


class FilterUnitFramework : public FUAdapter/*, 
			    public soapMonitorAdapter, 
			    public soapConfigurationListener,
			    public xdaqApplication */
{
public:
  
  XDAQ_INSTANTIATOR();

  // constructor
  
  FilterUnitFramework(xdaq::ApplicationStub *s);

  virtual ~FilterUnitFramework();

  // The main interface for the HLT
  FURawEvent * rqstEvent();
  
  unsigned long myId(){return instance_;}
  double birth(){return birth_.tstamp();}
  


private:

  // config exec
  
  xdata::UnsignedLong buInstance_;
	
  // config Fw

  xdata::UnsignedLong queueSize_;

  // config hlt

  xdata::Boolean runActive_;
  xdata::Integer runNumber_;

  // exported (monitor) variables

  xdata::UnsignedLong nbEvents_;		
  xdata::String stateName_;

  // monitor thread configuration
  xdata::String add_;
  xdata::Integer port_;
  xdata::Integer del_;
  xdata::String nam_;

  // stats
  
  toolbox::Chrono birth_;
  double deltaT;
  vector<int> runs;
  vector<int> events;
  // locks
  BSem *mutex_;
  BSem *ack_;
  sem_t empty_; // a POSIX counting semaphore
  BSem *comm_;
  pthread_cond_t ready_;
  pthread_mutex_t lock_;

  //flags
  bool flush_;

  // components (see inheritance tree for more components
  // pool of filter tasks

  TaskGroup             *taskGroup_;

  // private service methods

  void exportParams();

  /**
   * State Machine and related methods
   */
  EPStateMachine *fsm_;

  /**
   * Non trivial state-change actions
   */

  void configureAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
  void enableAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
  virtual void suspendAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
  virtual void resumeAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);
  virtual void haltAction(toolbox::Event::Reference e) throw (toolbox::fsm::exception::Exception);

  void defaultWebPage(xgi::Input *in, xgi::Output *out) throw (xgi::exception::Exception);
  xoap::MessageReference getStateMsg(xoap::MessageReference msg) throw (xoap::exception::Exception);
  xoap::MessageReference createFSMReplyMsg(const string cmd, const string state);

  void bindFSMSoapCallbacks();

  /**
   * SOAP Callback used to trigger state change.
   */
  xoap::MessageReference fireEvent(xoap::MessageReference msg)
    throw (xoap::exception::Exception)
    {
      xoap::SOAPPart     part      = msg->getSOAPPart();
      xoap::SOAPEnvelope env       = part.getEnvelope();
      xoap::SOAPBody     body      = env.getBody();
      DOMNode            *node     = body.getDOMNode();
      DOMNodeList        *bodyList = node->getChildNodes();
      DOMNode            *command  = 0;
      std::string        commandName;
      
      for (unsigned int i = 0; i < bodyList->getLength(); i++)
	{
	  command = bodyList->item(i);
	  
	  if(command->getNodeType() == DOMNode::ELEMENT_NODE)
            {
	      commandName = xoap::XMLCh2String(command->getLocalName());
	      return fsm_->processFSMCommand(commandName);
            }
        }
      
      XCEPT_RAISE(xoap::exception::Exception, "Command not found");
    }

  /**
   * Processes FSM commands from run-control.
   */
  

  /** default actions for commands not yet implemented */

  virtual void disableAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception) {}
  
  virtual void failAction(toolbox::Event::Reference e)
    throw (toolbox::fsm::exception::Exception) {}


};

#endif

