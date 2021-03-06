#include "FEDMonitorClient.h"

#include "DataFormats/FEDRawData/interface/FEDNumbering.h"
#include "DQMServices/Core/interface/DQMStore.h"
#include "DQMServices/Core/interface/MonitorElement.h"

#include <iostream>
#include <typeinfo>
#include <map>
#include <sstream>

#include "xgi/Method.h"
#include "xgi/Utils.h"

#include "cgicc/Cgicc.h"
#include "cgicc/FormEntry.h"
#include "cgicc/HTMLClasses.h"

using namespace std;
namespace dqm{
  namespace daq{


    FEDMonitorClient::FEDMonitorClient( const edm::ParameterSet& pset) : 
      ModuleWeb("FEDMonitorClient")
     , qtHandle_(0)
     , nUpdates_(0)
     , qtesttodo_(pset.getUntrackedParameter<bool>("doQTests",false))
     , autorefresh_(false)
    {
      
      // get hold of back-end interface
      dbe = edm::Service<DQMStore>().operator->();
      
      // instantiate Monitor UI without connecting to any monitoring server
      // (i.e. "standalone mode")
      mui = new DQMOldReceiver();
      
      dbe->showDirStructure();

      if(qtesttodo_) bookQTests(pset); 

    }
    void FEDMonitorClient::analyze(const edm::Event & e, const edm::EventSetup& c)
    {
      eventId_ = e.id().event();
      runId_ = e.id().run();
      nUpdates_++;
    }
    
    void FEDMonitorClient::endLuminosityBlock(edm::LuminosityBlock const &lb, edm::EventSetup const &es)
    {
      std::cout << "here I will do something intelligent later " << std::endl;
    }
    void FEDMonitorClient::bookQTests(const edm::ParameterSet &pset)
    {
      qtHandle_ = new QTestHandle;
      qtHandle_->configureTests(pset.getUntrackedParameter<std::string>("qtList", "QualityTests.xml"),dbe);
      if(qtHandle_) qtHandle_->attachTests(dbe);      
    }
    
    void FEDMonitorClient::defaultWebPage(xgi::Input *in, xgi::Output *out)
    {

      std::string path;
      std::string mname;
      try 
	{
	  cgicc::Cgicc cgi(in);
	  if ( xgi::Utils::hasFormElement(cgi,"autorefresh") )
	    autorefresh_ = xgi::Utils::getFormElement(cgi, "autorefresh")->getIntegerValue()  != 0;
	  if ( xgi::Utils::hasFormElement(cgi,"module") )
	    mname = xgi::Utils::getFormElement(cgi, "module")->getValue();
	  cgicc::CgiEnvironment cgie(in);
	  path = cgie.getPathInfo() + "?" + cgie.getQueryString();
	}
      catch (const std::exception & e) 
    {
      // don't care if it did not work
    }


        using std::endl;
	*out << "<html>"                                                   << endl;
	*out << "<head>"                                                   << endl;
	
	*out << "<title>" << typeid(FEDMonitorClient).name()
	     << " MAIN</title>"                                            << endl;
	if (autorefresh_) {
	  *out << "<meta http-equiv=\"refresh\" content=\"3\">"            << endl;
	}
	
	*out << "</head>"                                                  << endl;
	*out << "<body>"                                                   << endl;

	*out << cgicc::form().set("method","GET").set("action", path ) 
	     << std::endl;
	*out << cgicc::input().set("type","hidden").set("name","module").set("value", mname) 
	     << std::endl;
	*out << cgicc::input().set("type","hidden").set("name","autorefresh").set("value", autorefresh_?"0":"1") 
	     << std::endl;
	*out << cgicc::input().set("type","submit").set("value",autorefresh_?"Toggle AutoRefresh OFF":"Toggle AutoRefresh ON")  
	     << std::endl;
	*out << cgicc::form()						   << std::endl;  


	*out << "<p>Run: " << runId_ << " Total updates: " << nUpdates_ << " Last Event analyzed: "
	     << eventId_;


	*out << " FED Monitor status "		                           << endl;


	int status = dbe->getStatus();
	switch(status)
	  {
	  case dqm::qstatus::ERROR:
	    *out << " Error(s)";
	    break;
	  case dqm::qstatus::WARNING:
	    *out << " Warning(s)";
	    break;
	  case dqm::qstatus::OTHER:
	    *out << " Some tests did not run;";
	    break; 
	  default:
	    *out << (nUpdates_ > 2 ? " No problems" : "Starting");
	  }

	*out << "<table>"						<< endl;
	*out << "<tr><th>SubSystem</th><th>Present/Nominal</th><th>Errors</th><th>Warnings</th><th>Other</th></tr>" << endl;
	
	/*
	std::vector<std::string> contents; 
	mui->getContents(contents);
	for(unsigned int i = 0; i < contents.size() ; i++)
	  *out << contents[i] << std::endl;
	  */
	//	std::vector<MonitorElement*> plots = dbe->getContents("FEDs/Collated");
	int sstn = 0;
	int sste = 0;
	int sstw = 0;
	int ssto = 0;
	int sstt = FEDNumbering::MAXSiStripFEDID - FEDNumbering::MINSiStripFEDID + 1;
	for (int i = FEDNumbering::MINSiStripFEDID; i <= FEDNumbering::MAXSiStripFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">SiStrip </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXSiPixelFEDID - FEDNumbering::MINSiPixelFEDID + 1;
	for (int i = FEDNumbering::MINSiPixelFEDID; i <= FEDNumbering::MAXSiPixelFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">SiPixel </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXDTFEDID - FEDNumbering::MINDTFEDID + 1;
	for (int i = FEDNumbering::MINDTFEDID; i <= FEDNumbering::MAXDTFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">DT </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;


	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXCSCFEDID - FEDNumbering::MINCSCFEDID + 1;
	for (int i = FEDNumbering::MINCSCFEDID; i <= FEDNumbering::MAXCSCFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">CSC </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXDTTFFEDID - FEDNumbering::MINDTTFFEDID + 1;
	for (int i = FEDNumbering::MINDTTFFEDID; i <= FEDNumbering::MAXDTTFFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">DTTF </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;


	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXRPCFEDID - FEDNumbering::MINRPCFEDID+ 1;
	for (int i = FEDNumbering::MINRPCFEDID; i <= FEDNumbering::MAXRPCFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">RPC </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXPreShowerFEDID - FEDNumbering::MINPreShowerFEDID + 1;
	for (int i = FEDNumbering::MINPreShowerFEDID; i <= FEDNumbering::MAXPreShowerFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">PreShower </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXECALFEDID - FEDNumbering::MINECALFEDID + 1;
	for (int i = FEDNumbering::MINECALFEDID; i <= FEDNumbering::MAXECALFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">Ecal </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXHCALFEDID - FEDNumbering::MINHCALFEDID + 1;
	for (int i = FEDNumbering::MINHCALFEDID; i <= FEDNumbering::MAXHCALFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
	*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">Hcal </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXTriggerGTPFEDID - FEDNumbering::MINTriggerGTPFEDID + 1;
	for (int i = FEDNumbering::MINTriggerGTPFEDID; i <= FEDNumbering::MAXTriggerGTPFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
		*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">GTP </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXTriggerEGTPFEDID - FEDNumbering::MINTriggerEGTPFEDID + 1;
	for (int i = FEDNumbering::MINTriggerEGTPFEDID; i <= FEDNumbering::MAXTriggerEGTPFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
		*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">EGTP </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;


	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXCSCTFFEDID - FEDNumbering::MINCSCFEDID + 1;
	for (int i = FEDNumbering::MINCSCFEDID; i <= FEDNumbering::MAXCSCTFFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
		*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">CSCTF </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	sstn = 0; sste=0; sstw=0; ssto=0;
	sstt = FEDNumbering::MAXDAQeFEDFEDID - FEDNumbering::MINDAQeFEDFEDID + 1;
	for (int i = FEDNumbering::MINDAQeFEDFEDID; i <= FEDNumbering::MAXDAQeFEDFEDID; i++) 
	  {
	    ostringstream oss;
	    oss << "FEDs/Collated/fed" << i;
	    MonitorElement *me = dbe->get(oss.str());
	    if(me != 0)
	      {
		sstn++;
		if(me->hasError()) sste++;
		if(me->hasWarning()) sstw++;
		if(me->hasOtherReport()) ssto++;
	      }
	  }
		*out << "<tr><td bgcolor=";
	if(sstn==0)
	  *out << "\"grey\"";
	else if(sste!=0)
	  *out << "\"red\"";
	else if(sstw!=0)
	  *out << "\"orange\"";
	else if(ssto!=0)
	  *out << "\"yellow\"";
	else
	  *out << "\"green\"";

	*out << ">eFED </td><td> " <<  sstn << "/" << sstt 
	     << " </td><td> " << sste << " </td><td> " << sstw
	     << " </td><td> " << ssto << " </td></tr> " << endl;

	*out << "</table>"                                                 << endl;
	*out << "</body>"                                                  << endl;
	*out << "</html>"                                                  << endl;
    }
    std::string FEDMonitorClient::findSubsystemPath(std::string &subs)
    {
      return subs;
    }

  }
}
