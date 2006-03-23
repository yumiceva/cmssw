#ifndef Common_RefVectorIterator_h
#define Common_RefVectorIterator_h

/*----------------------------------------------------------------------
  
RefVectorIterator: An iterator for a RefVector


$Id: RefVectorIterator.h,v 1.1 2006/02/07 07:01:50 wmtan Exp $

----------------------------------------------------------------------*/

#include <memory>
#include "DataFormats/Common/interface/RefItem.h"
#include "DataFormats/Common/interface/RefCore.h"
#include "DataFormats/Common/interface/Ref.h"

namespace edm {

  template <typename C, typename T = typename Ref<C>::value_type, typename F = typename Ref<C>::finder_type>
  class RefVectorIterator : public std::iterator <std::random_access_iterator_tag, T> {
  public:
    typedef T value_type;
    typedef typename Ref<C, T, F>::index_type size_type;

    typedef RefVectorIterator<C, T, F> iterator;
    typedef std::ptrdiff_t difference;
    typedef typename std::vector<RefItem<size_type> >::const_iterator itemIter;
    RefVectorIterator() : product_(), iter_() {}
    explicit RefVectorIterator(RefCore const& product, itemIter const& it) :
      product_(product), iter_(it) {}
    Ref<C, T, F> operator*() const {
      RefItem<size_type> const& item = *iter_;
      getPtr<C, T, F>(product_, item);
      return Ref<C, T, F>(product_, item);
    }
    Ref<C, T, F> operator[](difference n) const {
      RefItem<size_type> const& item = iter_[n];
      getPtr<C, T, F>(product_, item);
      return Ref<C, T, F>(product_, item);
    }
    std::auto_ptr<Ref<C, T, F> > operator->() const {
      RefItem<size_type> const& item = *iter_;
      getPtr<C, T, F>(product_, item);
      return std::auto_ptr<Ref<C, T, F> >(new T(product_, item));
    }
    iterator & operator++() {++iter_; return *this;}
    iterator & operator--() {--iter_; return *this;}
    iterator & operator+=(difference n) {iter_ += n; return *this;}
    iterator & operator-=(difference n) {iter_ -= n; return *this;}

    iterator operator++(int) {iterator it(*this); ++iter_; return it;}
    iterator operator--(int) {iterator it(*this); --iter_; return it;}
    iterator operator+(difference n) const {iterator it(*this); it.iter_+=n; return it;}
    iterator operator-(difference n) const {iterator it(*this); it.iter_-=n; return it;}

    difference operator-(iterator const& rhs) const {return this->iter_ - rhs.iter_;}

    bool operator==(iterator const& rhs) const {return this->iter_ == rhs.iter_;}
    bool operator!=(iterator const& rhs) const {return this->iter_ != rhs.iter_;}
    bool operator<(iterator const& rhs) const {return this->iter_ < rhs.iter_;}
    bool operator>(iterator const& rhs) const {return this->iter_ > rhs.iter_;}
    bool operator<=(iterator const& rhs) const {return this->iter_ <= rhs.iter_;}
    bool operator>=(iterator const& rhs) const {return this->iter_ >= rhs.iter_;}
  private:
    RefCore product_;
    itemIter iter_;
  };

  template <typename C, typename T, typename F>
  inline
  RefVectorIterator<C, T, F> operator+(typename RefVectorIterator<C, T, F>::difference n, RefVectorIterator<C, T, F> const& iter) {
    return iter + n;
  } 
}
#endif
