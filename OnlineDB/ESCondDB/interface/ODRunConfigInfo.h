#ifndef ODRUNCONFIGINFO_H
#define ODRUNCONFIGINFO_H

#include <stdexcept>
#include <iostream>

#include "OnlineDB/EcalCondDB/interface/IODConfig.h"
#include "OnlineDB/EcalCondDB/interface/Tm.h"
#include "OnlineDB/ESCondDB/interface/RunModeDef.h"
#include "OnlineDB/EcalCondDB/interface/RunTypeDef.h"

class ODRunConfigInfo : public IODConfig {
 public:
  friend class EcalCondDBInterface;

  ODRunConfigInfo();
  ~ODRunConfigInfo();
  inline std::string getTable() { return "ES_RUN_CONFIGURATION_DAT"; }

  inline void setId(int id) { m_ID = id; }
  inline int getId() const { return m_ID; }
 
  

inline Tm getDBTime() const{  return m_db_time;}
//
inline void setTag(std::string x) { m_tag = x; }
std::string getTag() const{  return m_tag;}
//
void setDescription(std::string x) { m_description = x;}
std::string getDescription() const{  return m_description;}
//
void setVersion(int x){ m_version = x;  }
int getVersion()const {return m_version;  }
//
void setNumberOfSequences(int n){ m_num_seq = n;  }
int getNumberOfSequences()const {return m_num_seq;  }
//
void setDefaults(int x) { m_defaults = x;}
int getDefaults() const{  return m_defaults;}
//
void setTriggerMode(std::string x) { m_trigger_mode = x;}
std::string getTriggerMode() const{  return m_trigger_mode;}
//
void setNumberOfEvents(int x) { m_num_events = x ;}
int getNumberOfEvents() const{  return m_num_events ;}
//


  RunTypeDef getRunTypeDef() const;
  void setRunTypeDef(const RunTypeDef runTypeDef);
  RunModeDef getRunModeDef() const;
  void setRunModeDef(const RunModeDef runModeDef);


  // operators
  inline bool operator==(const ODRunConfigInfo &r) const {  return (m_ID   == r.m_ID ); }
  inline bool operator!=(const ODRunConfigInfo &r) const { return !(*this == r); }

 private:
  // User data for this IOV
  int m_ID;
  Tm m_db_time;
  std::string m_tag;
  int m_version;
  RunModeDef m_runModeDef;
  RunTypeDef m_runTypeDef;
  int m_num_seq;
  std::string m_description;
  int m_defaults;
  std::string m_trigger_mode;
  int m_num_events;

  // Methods from IUniqueDBObject
  int fetchNextId()  throw(std::runtime_error);
  int fetchID() throw(std::runtime_error);
  int fetchIDFromTagAndVersion() throw(std::runtime_error);
  int fetchIDLast() throw(std::runtime_error);
  void setByID(int id) throw(std::runtime_error);

  void prepareWrite()  throw(std::runtime_error);
  void writeDB()       throw(std::runtime_error);
  void fetchData(ODRunConfigInfo * result)     throw(std::runtime_error);
  int updateDefaultCycle()   throw(runtime_error);

};



#endif
