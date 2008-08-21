#ifndef Fireworks_Core_FWModelFilter_h
#define Fireworks_Core_FWModelFilter_h
// -*- C++ -*-
//
// Package:     Core
// Class  :     FWModelFilter
// 
/**\class FWModelFilter FWModelFilter.h Fireworks/Core/interface/FWModelFilter.h

 Description: <one line class summary>

 Usage:
    <usage>

*/
//
// Original Author:  Chris Jones
//         Created:  Fri Feb 29 13:39:51 PST 2008
// $Id: FWModelFilter.h,v 1.3 2008/08/01 13:43:15 chrjones Exp $
//

// system include files
#include <string>
#include <Reflex/Type.h>

// user include files
#include "PhysicsTools/Utilities/src/SelectorPtr.h"
#include "PhysicsTools/Utilities/src/SelectorBase.h"


// forward declarations

class FWModelFilter
{

   public:
      FWModelFilter(const std::string& iExpression,
                   const std::string& iClassName);
      virtual ~FWModelFilter();

      // ---------- const member functions ---------------------

      const std::string& expression() const;
   
      bool passesFilter(const void*) const;
   
      const bool trivialFilter() const;
      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------
      /** Throws an FWExpressionException if there is a problem */
      void setExpression(const std::string& );
      void setClassName(const std::string& );
   
   private:
      //FWModelFilter(const FWModelFilter&); // stop default

      //const FWModelFilter& operator=(const FWModelFilter&); // stop default

      // ---------- member data --------------------------------
      std::string m_expression;
      std::string m_className;   
      reco::parser::SelectorPtr m_selector;
      ROOT::Reflex::Type m_type;
};


#endif
