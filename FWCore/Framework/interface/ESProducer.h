#ifndef EVENTSETUPPRODUCER_ESPRODUCER_H
#define EVENTSETUPPRODUCER_ESPRODUCER_H
// -*- C++ -*-
//
// Package:     CoreFramework
// Class  :     ESProducer
// 
/**\class ESProducer ESProducer.h Core/CoreFramework/interface/ESProducer.h

 Description: An EventSetup algorithmic Provider that encapsulates the algorithm as a member method

 Usage:
    Inheriting from this class is the simplest way to create an algorithm which gets called when a new
  data item is needed for the EventSetup.  This class is designed to call a member method of inheriting
  classes each time the algorithm needs to be run.  (A more flexible system in which the algorithms can be
  set at run-time instead of compile time can be obtained by inheriting from ProxyFactoryProducer instead.)

    If only one algorithm is being encapsulated then the user needs to
      1) add a method name 'produce' to the class.  The 'produce' takes as its argument a const reference
         to the record that is to hold the data item being produced.  If only one data item is being produced,
         the 'produce' method must return either an 'std::auto_ptr' or 'boost::shared_ptr' to the object being
         produced.  (The choice depends on if the EventSetup or the ESProducer is managing the lifetime of 
         the object).  If multiple items are being Produced they the 'produce' method must return an
         ESProducts<> object which holds all of the items.
      2) add 'setWhatProduced(this);' to their classes constructor

Example: one algorithm creating only one object
\code
    class FooProd : public edm::eventsetup::ESProducer {
       std::auto_ptr<Foo> produce( const FooRecord& );
       ...
    };
    FooProd::FooProd( const edm::ParameterSet& ) {
       setWhatProduced(this);
       ...
    }
\endcode
Example: one algorithm creating two objects
\code
   class FoosProd : public edm::eventsetup::ESProducer {
      edm::eventsetup::ESProducts<std::auto_ptr<Foo1>, std::auto_ptr<Foo2> > produce( const FooRecord& );
      ...
   };
\endcode

  If multiple algorithms are being encapsulated then
      1) like 1 above except the methods can have any names you want
      2) add 'setWhatProduced(this, &<class name>::<method name>);' for each method in the class' constructor
   NOTE: the algorithms can put data into the same record or into different records

Example: two algorithms each creating only one objects
\code
   class FooBarProd : public edm::eventsetup::ESProducer {
      std::auto_ptr<Foo> produceFoo( const FooRecord& );
      std::auto_ptr<Bar> produceBar( const BarRecord& );
      ...
   };
   FooBarProd::FooBarProd( const edm::ParameterSet& ) {
      setWhatProduced(this,&FooBarProd::produceFoo);
      setWhatProduced(this,&FooBarProd::produceBar);
      ...
   }
\endcode

*/
//
// Author:      Chris Jones
// Created:     Thu Apr  7 17:08:14 CDT 2005
// $Id: ESProducer.h,v 1.1 2005/05/29 02:29:53 wmtan Exp $
//

// system include files
#include <memory>

// user include files
#include "FWCore/CoreFramework/interface/ProxyFactoryProducer.h"
#include "FWCore/CoreFramework/interface/ProxyArgumentFactoryTemplate.h"

#include "FWCore/CoreFramework/interface/CallbackProxy.h"
#include "FWCore/CoreFramework/interface/Callback.h"
#include "FWCore/CoreFramework/interface/produce_helpers.h"
#include "FWCore/CoreFramework/interface/ESProducts.h"

// forward declarations
namespace edm {
   namespace eventsetup {      
class ESProducer : public ProxyFactoryProducer
{

   public:
      ESProducer();
      virtual ~ESProducer();

      // ---------- const member functions ---------------------

      // ---------- static member functions --------------------

      // ---------- member functions ---------------------------
   protected:
         /** \param iThis the 'this' pointer to an inheriting class instance
         The method determines the Record argument and return value of the 'produce'
         method in order to do the registration with the EventSetup
         */
         template<typename T >
         void setWhatProduced( T* iThis) {
            using namespace boost;
            //BOOST_STATIC_ASSERT( (typename boost::is_base_and_derived<ED, T>::type) );
            setWhatProduced( iThis , &T::produce );
         }
      
      /** \param iThis the 'this' pointer to an inheriting class instance
         \param iMethod a member method of then inheriting class
         The method determines the Record argument and return value of the iMethod argument
         method in order to do the registration with the EventSetup
         */
      template<typename T, typename TReturn, typename TRecord>
         void setWhatProduced( T* iThis, TReturn (T ::* iMethod)(const TRecord& ) ) {
            using namespace boost;
            boost::shared_ptr<Callback<T,TReturn,TRecord> > callback( new
                                                             Callback<T,TReturn,TRecord>( iThis, iMethod) );
            registerProducts( callback,
                              static_cast<const typename produce::product_traits<TReturn>::type *>(0),
                              static_cast<const TRecord*>(0) );
            //BOOST_STATIC_ASSERT( (boost::is_base_and_derived<ED, T>::type) );
         }

      /*
      template<typename T, typename TReturn, typename TArg>
         void setWhatProduced( T* iThis, TReturn (T ::* iMethod)(const TArg&) ) {
            using namespace boost;
            registerProducts( iThis, static_cast<const typename produce::product_traits<TReturn>::type *>(0) );
            registerGet(iThis, static_cast<const TArg*>(0) );
            //BOOST_STATIC_ASSERT( (boost::is_base_and_derived<ED, T>::type) );
         }
      */
   private:
      ESProducer( const ESProducer& ); // stop default

      ESProducer const& operator=( const ESProducer& ); // stop default

      /*
      template<typename T, typename TProduct>
         void registerGet(T* i, const TProduct* iProd) {
            using namespace produce;
            std::cout <<"registered 'get' for product type "
            << test::name( iProd ) <<
            std::endl;
         };
      */
      template<typename CallbackT, typename TList, typename TRecord>
         void registerProducts(boost::shared_ptr<CallbackT> iCallback, const TList*, const TRecord* iRecord) {
            registerProduct(iCallback, static_cast<const typename TList::tail_type*>(0), iRecord );
            registerProducts(iCallback, static_cast<const typename TList::head_type*>(0), iRecord );
         };
      template<typename T, typename TRecord>
         void registerProducts(boost::shared_ptr<T>, const produce::Null*, const TRecord*) {
            //do nothing
         };
      
      
      template<typename T, typename TProduct, typename TRecord>
         void registerProduct(boost::shared_ptr<T> iCallback, const TProduct* iProd, const TRecord*) {
            registerFactory( new ProxyArgumentFactoryTemplate<
                             CallbackProxy<T, TRecord, TProduct>, boost::shared_ptr<T> >( iCallback ) );
         };
      
      // ---------- member data --------------------------------
      // NOTE: the factories share ownership of the callback
      //std::vector<boost::shared_ptr<CallbackBase> > callbacks_;
      
};
   }
}
#endif /* EVENTSETUPPRODUCER_ESPRODUCER_H */
