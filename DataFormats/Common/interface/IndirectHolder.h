#ifndef Common_IndirectHolder_h
#define Common_IndirectHolder_h
#include "DataFormats/Common/interface/BaseHolder.h"
#include "DataFormats/Common/interface/RefHolderBase.h"

namespace edm {
  namespace reftobase {
    class RefHolderBase;

    //------------------------------------------------------------------
    // Class template IndirectHolder<T>
    //------------------------------------------------------------------

    template <class T>
    class IndirectHolder : public BaseHolder<T> {
    public:
      // It may be better to use auto_ptr<RefHolderBase> in
      // this constructor, so that the cloning can be avoided. I'm not
      // sure if use of auto_ptr here causes any troubles elsewhere.
      IndirectHolder() { }
      IndirectHolder(boost::shared_ptr<RefHolderBase> p);
      IndirectHolder(IndirectHolder const& other);
      IndirectHolder& operator= (IndirectHolder const& rhs);
      void swap(IndirectHolder& other);
      virtual ~IndirectHolder();
      
      virtual BaseHolder<T>* clone() const;
      virtual T const* getPtr() const;
      virtual ProductID id() const;
      virtual bool isEqualTo(BaseHolder<T> const& rhs) const;

      virtual bool fillRefIfMyTypeMatches(RefHolderBase& fillme,
					  std::string& msg) const;

    private:
      RefHolderBase* helper_;
    };
    //------------------------------------------------------------------
    // Implementation of IndirectHolder<T>
    //------------------------------------------------------------------

    template <class T>
    inline
    IndirectHolder<T>::IndirectHolder(boost::shared_ptr<RefHolderBase> p) :
      helper_(p->clone()) 
    { }

    template <class T>
    inline
    IndirectHolder<T>::IndirectHolder(IndirectHolder const& other) : 
      helper_(other.helper_->clone()) 
    { }

    template <class T>
    inline
    IndirectHolder<T>& 
    IndirectHolder<T>::operator= (IndirectHolder const& rhs) 
    {
      IndirectHolder temp(rhs);
      swap(temp);
      return *this;
    }

    template <class T>
    inline
    void
    IndirectHolder<T>::swap(IndirectHolder& other) 
    {
      std::swap(helper_, other.helper_);
    }

    template <class T>
    IndirectHolder<T>::~IndirectHolder()
    {
      delete helper_;
    }

    template <class T>
    BaseHolder<T>* 
    IndirectHolder<T>::clone() const
    {
      return new IndirectHolder<T>(*this);
    }

    template <class T>
    T const* 
    IndirectHolder<T>::getPtr() const 
    {
      return helper_-> template getPtr<T>();
    }

    template <class T>
    ProductID
    IndirectHolder<T>::id() const
    {
      return helper_->id();
    }

    template <class T>
    bool
    IndirectHolder<T>::isEqualTo(BaseHolder<T> const& rhs) const 
    {
      IndirectHolder const* h = dynamic_cast<IndirectHolder const*>(&rhs);
      return h && helper_->isEqualTo(*h->helper_);
    }

    template <class T>
    bool
    IndirectHolder<T>::fillRefIfMyTypeMatches(RefHolderBase& fillme,
					      std::string& msg) const
    {
      return helper_->fillRefIfMyTypeMatches(fillme, msg);
    }

  }
}

#endif
