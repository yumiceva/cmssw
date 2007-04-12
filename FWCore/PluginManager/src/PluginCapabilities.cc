// -*- C++ -*-
//
// Package:     PluginManager
// Class  :     PluginCapabilities
// 
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Fri Apr  6 12:36:24 EDT 2007
// $Id: PluginCapabilities.cc,v 1.1.2.4 2007/04/12 00:00:25 chrjones Exp $
//

// system include files

// user include files
#include "FWCore/PluginManager/interface/PluginCapabilities.h"
#include "FWCore/PluginManager/interface/SharedLibrary.h"
#include "FWCore/PluginManager/interface/PluginManager.h"
#include "FWCore/Utilities/interface/Exception.h"

namespace edmplugin {
//
// constants, enums and typedefs
//

//
// static data member definitions
//

//
// constructors and destructor
//
PluginCapabilities::PluginCapabilities()
{
  finishedConstruction();
}

// PluginCapabilities::PluginCapabilities(const PluginCapabilities& rhs)
// {
//    // do actual copying here;
// }

PluginCapabilities::~PluginCapabilities()
{
}

//
// assignment operators
//
// const PluginCapabilities& PluginCapabilities::operator=(const PluginCapabilities& rhs)
// {
//   //An exception safe implementation is
//   PluginCapabilities temp(rhs);
//   swap(rhs);
//
//   return *this;
// }

//
// member functions
//
bool 
PluginCapabilities::tryToFind(const SharedLibrary& iLoadable)
{
  void* sym;
  if( not iLoadable.symbol("SEAL_CAPABILITIES",sym) ) {
    return false;
  }
  
  const char** names;
  int size;
  //reinterpret_cast<void (*)(const char**&,int&)>(sym)(names,size);
  ((void (*)(const char**&,int&))(sym))(names,size);

  for(int i=0; i < size; ++i) {
    std::string name(names[i]);
    classToLoadable_[name]=iLoadable.path();
    newPlugin(name);
  }
  return true;
}

void 
PluginCapabilities::load(const std::string& iName)
{
  if(classToLoadable_.end() == classToLoadable_.find(iName) ) {
    const SharedLibrary& lib = PluginManager::get()->load(category(),
                                                          iName);
    //read the items from the 'capabilities' symbol
    if(not tryToFind(lib) ) {
      throw cms::Exception("PluginNotFound")<<"The dictionary for class '"<<iName <<"' is supposed to be in file\n '"
      <<lib.path().native_file_string()<<"'\n but no dictionaries are in that file.\n"
      "It appears like the cache is wrong.  Please do 'EdmPluginRefresh "<<lib.path().native_file_string()<<"'.";
    }
    
    if(classToLoadable_.end() == classToLoadable_.find(iName)) {
      throw cms::Exception("PluginNotFound")<<"The dictionary for class '"<<iName<<"' is supposed to be in file\n '"
      <<lib.path().native_file_string()<<"'\n but was not found.\n"
      "It appears like the cache is wrong.  Please do 'EdmPluginRefresh "<<lib.path().native_file_string()<<"'.";
    }
  }
}

//
// const member functions
//
std::vector<PluginInfo> 
PluginCapabilities::available() const
{
  PluginInfo info;
  std::vector<PluginInfo> infos;
  infos.reserve(classToLoadable_.size());
  
  for(std::map<std::string, boost::filesystem::path>::const_iterator it = classToLoadable_.begin();
      it != classToLoadable_.end();
      ++it) {
    info.name_ = it->first;
    info.loadable_ = it->second;
    infos.push_back(info);
  }
  return infos;
}

const std::string& 
PluginCapabilities::category() const
{
  static const std::string s_cat("Capability");
  return s_cat;
}

//
// static member functions
//
PluginCapabilities*
PluginCapabilities::get() {
  static PluginCapabilities s_instance;
  return &s_instance;
}

}
