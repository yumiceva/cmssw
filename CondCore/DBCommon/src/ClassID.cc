#include "CondCore/DBCommon/interface/ClassID.h"
#include "StorageSvc/DbReflex.h"
#include <typeinfo>
#include "cxxabi.h"
#include "RVersion.h"

//#include <iostream>
#if ROOT_VERSION_CODE < ROOT_VERSION(5,19,0)
 using namespace ROOT;
#endif


namespace pool {  void genMD5(const std::string& s, void* code);  }

namespace cond {

  char const * className(const std::type_info& t) {
    int status=0;
    return __cxxabiv1::__cxa_demangle(t.name(), 0, 0, &status); 
  }


  namespace {
    std::string id(const std::type_info& t) {
      const Reflex::Type type=Reflex::Type::ByTypeInfo(t);
      std::string s;
      if (type) {
	Reflex::PropertyList pl = type.Properties();
	if ( pl.HasProperty("ClassID") )  {
	  s = pl.PropertyAsString("ClassID");
	}
	else  {
	  char buff[20];
	  pool::genMD5(type.Name(Reflex::SCOPED),buff);
	  s = ((pool::Guid*)buff)->toString();
	}
//       std::cout << "CondCore::ClassID: CLID for Reflex type " << type.Name(Reflex::SCOPED) 
//                 << " is " << s << std::endl;
      }
      else {
	// only for test
	std::string name = className(t);
//	std::cout << "CondCore::ClassID: Warning no Reflex type for " << name << std::endl;
	char buff[20];
	pool::genMD5(name,buff);
	s = ((pool::Guid*)buff)->toString();
      }
      return s;
    }
  }




  ClassIDRegistry::ClassIDRegistry(std::string const & pfix) : prefix(pfix+"/"){}

  const char * ClassIDRegistry::registerMe(const std::type_info& t) {
    sids.push_back(prefix+id(t)+"\0");
    csids.push_back(sids.back().data());
    return sids.back().c_str();
  }
  
  const char * ClassInfo::registerMe(const std::type_info& t) {
    return registry->registerMe(t);
  }

  std::string ClassInfo::pluginName(std::string const & prefix) const {
    return prefix + "/" +id(tinfo);
  }

  

}

EDM_REGISTER_PLUGINFACTORY(cond::ClassInfoFactory, "CondFormatsID");
