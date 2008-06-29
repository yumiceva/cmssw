#ifndef SiStripUtility_H
#define SiStripUtility_H

/** \class SiStripUtility
 * *
 *  Class that handles the SiStrip Quality Tests
 * 
 *  $Date: 2007/12/20 16:25:43 $
 *  $Revision: 1.5 $
 *  \author Suchandra Dutta
  */

#include <vector>
#include <fstream>
#include <string>
#include <map>


class MonitorElement;
class DQMStore;
class SiStripUtility
{
 public:
 
 static int getMEList(std::string name, std::vector<std::string>& values);
 static bool checkME(std::string element, std::string name, std::string& full_path);
 static int getMEList(std::string name, std::string& dir_path, std::vector<std::string>& me_names);

 static void split(const std::string& str, std::vector<std::string>& tokens, 
             const std::string& delimiters=" ");
 static void getMEStatusColor(int status, int& rval, int&gval, int& bval);
 static void getMEStatusColor(int status, int& icol, std::string& tag);
 static int getMEStatus(MonitorElement* me);
 static void getModuleFolderList(DQMStore* dqm_store, std::vector<std::string>& m_ids);
};

#endif
