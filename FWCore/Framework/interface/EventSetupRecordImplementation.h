#ifndef EVENTSETUP_EVENTSETUPRECORDIMPLEMENTATION_H
#define EVENTSETUP_EVENTSETUPRECORDIMPLEMENTATION_H
// -*- C++ -*-
//
// Package:     CoreFramework
// Class  :     EventSetupRecordImplementation
// 
/**\class EventSetupRecordImplementation EventSetupRecordImplementation.h Core/CoreFramework/interface/EventSetupRecordImplementation.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Author:      Chris Jones
// Created:     Fri Apr  1 16:50:49 EST 2005
// $Id: EventSetupRecordImplementation.h,v 1.1 2005/05/29 02:29:53 wmtan Exp $
//

// system include files
#include <string>

// user include files
#include "FWCore/CoreFramework/interface/EventSetupRecord.h"
#include "FWCore/CoreFramework/interface/EventSetupRecordKey.h"
#include "FWCore/CoreFramework/interface/recordGetImplementation.h"

// forward declarations
namespace edm {
   namespace eventsetup {

template<class T>
class EventSetupRecordImplementation : public EventSetupRecord
{

   public:
      //virtual ~EventSetupRecordImplementation();

      // ---------- const member functions ---------------------
      template< typename HolderT>
         void get(HolderT& iHolder, const char* iName = "") const {
            const typename HolderT::value_type* value;
            recordGetImplementation(static_cast<const T&>(*this), value, iName);
            iHolder = HolderT(value);
         }

   template< typename HolderT>
   void get(HolderT& iHolder, const std::string& iName) const {
      const typename HolderT::value_type* value;
      recordGetImplementation(static_cast<const T&>(*this), value, iName.c_str());
      iHolder = HolderT(value);
   }
   
   virtual EventSetupRecordKey key() const {
      return EventSetupRecordKey::makeKey<T>();
   }
      // ---------- static member functions --------------------
   static EventSetupRecordKey keyForClass()  {
      return EventSetupRecordKey::makeKey<T>();
   }
   
      // ---------- member functions ---------------------------
 
   protected:
      EventSetupRecordImplementation() {}

   private:
      EventSetupRecordImplementation(const EventSetupRecordImplementation&); // stop default

      const EventSetupRecordImplementation& operator=(const EventSetupRecordImplementation&); // stop default

      // ---------- member data --------------------------------

};
   }
}
#endif /* EVENTSETUP_EVENTSETUPRECORDIMPLEMENTATION_H */
