#include "DQM/SiStripMonitorClient/interface/SiStripConfigParser.h"
#include "DQMServices/ClientConfig/interface/ParserFunctions.h"
#include "FWCore/MessageLogger/interface/MessageLogger.h"
#include <iostream>

using namespace xercesc;
using namespace std;

//
// -- Constructor
// 
SiStripConfigParser::SiStripConfigParser() : DQMParserBase() {
  edm::LogInfo("SiStripConfigParser") << 
    " Creating SiStripConfigParser " << "\n" ;
}
//
// --  Destructor
// 
SiStripConfigParser::~SiStripConfigParser() {
  edm::LogInfo("SiStripConfigParser") << 
    " Deleting SiStripConfigParser " << "\n" ;
}
//
// -- Get List of MEs for the summary plot and the
//
bool SiStripConfigParser::getMENamesForSummary(map<string, string>& me_names) {
  if (!doc) {
    cout << " SiStripConfigParser::Configuration File is not set!!! " << endl;
    return false;
  }

  me_names.clear();
  unsigned int summaryNodes = doc->getElementsByTagName(qtxml::_toDOMS("SummaryPlot"))->getLength();
  if (summaryNodes == 0) return false;
  /// Get Node
  DOMNode* summaryNode = doc->getElementsByTagName(qtxml::_toDOMS("SummaryPlot"))->item(0);
 //Get Summary ME name and type
  if (! summaryNode) return false;
  DOMElement* summaryElement = static_cast<DOMElement *>(summaryNode);          
  if (! summaryElement) return false;		 
		

  DOMNodeList * meList 
		  = summaryElement->getElementsByTagName(qtxml::_toDOMS("MonElement"));
  for (unsigned int k = 0; k < meList->getLength(); k++) {
    DOMNode* meNode = meList->item(k);
    if (!meNode) return false;
    DOMElement* meElement = static_cast<DOMElement *>(meNode);          
    if (!meElement) return false;
    string me_name = qtxml::_toString(meElement->getAttribute (qtxml::_toDOMS ("name"))); 
    string me_type = qtxml::_toString(meElement->getAttribute (qtxml::_toDOMS ("type"))); 
    me_names.insert(pair<string,string>(me_name,me_type));    
  }
  if (me_names.size() == 0) return false;
  else return true;
  
}
//
// -- Get List of MEs for the summary plot and the
//
bool SiStripConfigParser::getFrequencyForSummary(int& u_freq) {
  if (!doc) {
    cout << " SiStripConfigParser::Configuration File is not set!!! " << endl;
    return false;
  }

  unsigned int summaryNodes = doc->getElementsByTagName(qtxml::_toDOMS("SummaryPlot"))->getLength();
  if (summaryNodes != 1 ) return false;
  /// Get Node
  DOMNode* summaryNode = doc->getElementsByTagName(qtxml::_toDOMS("SummaryPlot"))->item(0);
 //Get Node name
  if (! summaryNode) return false;
  DOMElement* summaryElement = static_cast<DOMElement *>(summaryNode);          
  if (! summaryElement) return false;		 
		
  u_freq = atoi(qtxml::_toString(summaryElement->getAttribute(qtxml::_toDOMS("update_frequency"))).c_str());
  return true;
}
