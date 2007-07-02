#ifndef DataFormats_Common_RefToBase_h
#define DataFormats_Common_RefToBase_h
// -*- C++ -*-
//
// Package:     Common
// Class  :     RefToBase
// 
/**\class RefToBase RefToBase.h DataFormats/Common/interface/RefToBase.h

Description: Interface to a reference to an item based on the base class of the item

Usage:
Using an edm:RefToBase<T> allows one to hold references to items in different containers
within the edm::Event where those objects are only related by a base class, T.

\code
edm::Ref<Foo> foo(...);
std::vector<edm::RefToBase<Bar> > bars;
bars.push_back(edm::RefToBase<Bar>(foo));
\endcode

Cast to concrete type can be done via the castTo<REF> 
function template. This function throws an exception
if the type passed as REF does not match the concrete
reference type.

*/
//
// Original Author:  Chris Jones
//         Created:  Mon Apr  3 16:37:59 EDT 2006
// $Id: RefToBase.h,v 1.19 2007/06/14 04:56:29 wmtan Exp $
//

// system include files

// user include files

#include "boost/shared_ptr.hpp"

#include "DataFormats/Common/interface/EDProductfwd.h"
#include "FWCore/Utilities/interface/EDMException.h"
#include "DataFormats/Common/interface/BaseHolder.h"

#include "DataFormats/Common/interface/Holder.h"
#include "DataFormats/Common/interface/IndirectHolder.h"
#include "DataFormats/Common/interface/RefHolder.h"

namespace edm {
  //--------------------------------------------------------------------
  // Class template RefToBase<T>
  //--------------------------------------------------------------------

  /// RefToBase<T> provides a mechanism to refer to an object of type
  /// T (or which has T as a public base), held in a collection (of
  /// type not known to RefToBase<T>) which itself it in an Event.

  template <class T>
  class RefToBase
  {
  public:
    typedef T   value_type;

    RefToBase();
    RefToBase(RefToBase const& other);
    template <class REF> explicit RefToBase(REF const& r);
    RefToBase(boost::shared_ptr<reftobase::RefHolderBase> p);

    ~RefToBase();

    RefToBase const& operator= (RefToBase const& rhs);

    value_type const& operator*() const;
    value_type const* operator->() const;
    value_type const* get() const;

    ProductID id() const;

    template <class REF> REF castTo() const;

    bool isNull() const;
    bool isNonnull() const;
    bool operator!() const;

    bool operator==(RefToBase const& rhs) const;
    bool operator!=(RefToBase const& rhs) const;

    void swap(RefToBase& other);

  private:
    value_type const* getPtrImpl() const;
    reftobase::BaseHolder<value_type>* holder_;
  };

  //--------------------------------------------------------------------
  // Implementation of RefToBase<T>
  //--------------------------------------------------------------------

  template <class T>
  inline
  RefToBase<T>::RefToBase() :
    holder_(0)
  { }

  template <class T>
  inline
  RefToBase<T>::RefToBase(RefToBase const& other) : 
    holder_(other.holder_  ? other.holder_->clone() : 0)
  { }

  template <class T>
  template <class REF>
  inline
  RefToBase<T>::RefToBase(REF const& iRef) : 
    holder_(new reftobase::Holder<T,REF>(iRef)) 
  { }

  template <class T>
  inline
  RefToBase<T>::RefToBase(boost::shared_ptr<reftobase::RefHolderBase> p) : 
    holder_(new reftobase::IndirectHolder<T>(p))
  { }

  template <class T>
  inline
  RefToBase<T>::~RefToBase() 
  {
    delete holder_; 
  }
     
  template <class T>
  inline
  RefToBase<T> const&
  RefToBase<T>:: operator= (RefToBase<T> const& iRHS) 
  {
    RefToBase<T> temp(iRHS);
    this->swap(temp);
    return *this;
  }

  template <class T>
  inline
  T const& 
  RefToBase<T>::operator*() const 
  {
    return *getPtrImpl(); 
  }

  template <class T>
  inline
  T const* 
  RefToBase<T>::operator->() const 
  {
    return getPtrImpl();
  }


  template <class T>
  inline
  T const* 
  RefToBase<T>::get() const 
  {
    return getPtrImpl();
  }

  template <class T>
  inline
  ProductID 
  RefToBase<T>::id() const 
  { 
    return  holder_ ? holder_->id() : ProductID();
  }
    
  /// cast to a concrete type
  template <class T>
  template <class REF>
  REF
  RefToBase<T>::castTo() const
  {
    if (!holder_)
      {
	throw edm::Exception(errors::InvalidReference)
	  << "attempting to cast a null RefToBase;\n"
	  << "You should check for nullity before casting.";
      }

    reftobase::RefHolder<REF> concrete_holder;
    std::string hidden_ref_type;
    if (!holder_->fillRefIfMyTypeMatches(concrete_holder,
					 hidden_ref_type))
      {
	throw edm::Exception(errors::InvalidReference)
	  << "cast to type: " << typeid(REF).name()
	  << "\nfrom type: " << hidden_ref_type
	  << " failed. Catch this exception in case you need to check"
	  << " the concrete reference type.";
      }
    return concrete_holder.getRef();
  }
    
  /// Checks for null
  template <class T>
  inline
  bool 
  RefToBase<T>::isNull() const 
  { 
    return !id().isValid();
  }
    
  /// Checks for non-null
  template <class T>
  inline
  bool 
  RefToBase<T>::isNonnull() const 
  {
    return !isNull(); 
  }
    
  /// Checks for null
  template <class T>
  inline
  bool 
  RefToBase<T>::operator!() const 
  {
    return isNull(); 
  }
  
  template <class T>
  inline
  bool 
  RefToBase<T>::operator==(RefToBase<T> const& rhs) const
  {
    return holder_
      ? holder_->isEqualTo(*rhs.holder_)
      : holder_ == rhs.holder_;
  }
  
  template <class T>
  inline
  bool
  RefToBase<T>::operator!=(RefToBase<T> const& rhs) const 
  {
    return !(*this == rhs);
  }

  template <class T>
  inline
  void 
  RefToBase<T>::swap(RefToBase<T> & other) 
  {
    std::swap(holder_, other.holder_);
  }
    

  template <class T>
  inline
  T const*
  RefToBase<T>::getPtrImpl() const 
  {
    return holder_ ? holder_->getPtr() : 0;
  }
  
  // Free swap function
  template <class T>
  inline
  void
  swap(RefToBase<T>& a, RefToBase<T>& b) 
  {
    a.swap(b);
  }

}

#endif
