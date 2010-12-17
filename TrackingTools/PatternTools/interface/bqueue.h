#ifndef CMSUTILS_BEUEUE_H
#define CMSUTILS_BEUEUE_H
#include <boost/intrusive_ptr.hpp>

/**  Backwards linked queue with "head sharing"

     Author: Giovanni Petrucciani
     
     For use in trajectory building, where we want to "fork" a trajectory candidate in two
     without copying around all the hits before the fork.

     Supported operations (mimics a std container):
       - push_back()
       - fork() which returns a new queue that shares the head part with the old one
       - copy constructor, which just does fork()
       - rbegin(), rend(): return iterators which can work only backwards
            for (cmsutils::bqueue<T>::const_iterator it = queue.rbegin(), end = queue.rend();
                        it != end; --it) { ... }
       - front() and back(): returns a reference to the first and last element
       - size(), empty(): as expected. size() does not count the items
     
     Note that boost::intrusive_ptr is used for items, so they are deleted automatically
     while avoiding problems if one deletes a queue which shares the head with another one

     Disclaimer: I'm not sure the const_iterator is really const-correct..
*/
namespace cmsutils {

template<class T> class _bqueue_itr;
template<class T> class bqueue;
template<class T> class _bqueue_item;
template<class T> void intrusive_ptr_add_ref(_bqueue_item<T> *it) ;
template<class T> void intrusive_ptr_release(_bqueue_item<T> *it) ;

template <class T> 
class _bqueue_item {
        friend class bqueue<T>;
        friend class _bqueue_itr<T>;
        friend void intrusive_ptr_add_ref<T>(_bqueue_item<T> *it);
        friend void intrusive_ptr_release<T>(_bqueue_item<T> *it);
        void addRef() { ++refCount; }
        void delRef() { if ((--refCount) == 0) delete this; }
    private:
        _bqueue_item() : back(0), value(), refCount(0) { }
        _bqueue_item(boost::intrusive_ptr< _bqueue_item<T> > tail, const T &val) : back(tail), value(val), refCount(0) { }
#if defined( __GXX_EXPERIMENTAL_CXX0X__)
        _bqueue_item(boost::intrusive_ptr< _bqueue_item<T> > tail, T &&val) : 
	  back(tail), value(std::move(val)), refCount(0) { }
#endif
        boost::intrusive_ptr< _bqueue_item<T> > back;
        T value;
        size_t refCount;
};

template<class T> inline void intrusive_ptr_add_ref(_bqueue_item<T> *it) { it->addRef(); }
template<class T> inline void intrusive_ptr_release(_bqueue_item<T> *it) { it->delRef(); }
//inline void intrusive_ptr_add_ref(const _bqueue_item *it) { it->addRef(); }
//inline void intrusive_ptr_release(const _bqueue_item *it) { it->delRef(); }

template<class T>
class _bqueue_itr {
    public:
        T* operator->() { return &it->value; }
        T& operator*() { return it->value; }
        const T* operator->() const { return &it->value; }
        const T& operator*() const { return it->value; }
        _bqueue_itr<T> & operator--() { it = it->back; return *this; }
        const _bqueue_itr<T> & operator--() const { it = it->back.get(); return *this; }
        bool operator==(const _bqueue_itr<T> &t2) const  { return t2.it == it; }
        bool operator!=(const _bqueue_itr<T> &t2) const { return t2.it != it; }
        // so that I can assign a const_iterator to a const_iterator
        const _bqueue_itr<T> & operator=(const _bqueue_itr<T> &t2) const { it = t2.it; return *this; }
        friend class bqueue<T>;
    private:
        _bqueue_itr(_bqueue_item<T> *t) : it(t) { }
        _bqueue_itr(const _bqueue_item<T> *t) : it(t) { }
        mutable _bqueue_item<T> *it;
};

template<class T>
class bqueue {
    public:
        typedef T value_type;
        typedef unsigned short int size_type;
        typedef _bqueue_item<value_type>       item;
        typedef boost::intrusive_ptr< _bqueue_item<value_type> >  itemptr;
        typedef _bqueue_itr<value_type>       iterator;
        typedef const _bqueue_itr<value_type> const_iterator;
        bqueue() : m_size(0), m_bound(), m_head(m_bound), m_tail(m_bound) { }
        ~bqueue() { }
        bqueue(const bqueue<T> &cp) : m_size(cp.m_size), m_bound(cp.m_bound), m_head(cp.m_head), m_tail(cp.m_tail) { }
#if defined( __GXX_EXPERIMENTAL_CXX0X__)

  bqueue(bqueue<T> &&cp) : 
    m_size(cp.m_size),
    m_bound(std::move(cp.m_bound)), m_head(std::move(cp.m_head)), m_tail(std::move(cp.m_tail)) { }

  bqueue & operator=(bqueue<T> &&cp) {
    using std::swap;
    m_size=cp.m_size;
    swap(m_bound,cp.m_bound);
    swap(m_head,cp.m_head); 
    swap(m_tail,cp.m_tail);
    return *this;
    }
#endif

      void swap(bqueue<T> &cp) {
          using std::swap;
	  swap(m_size,cp.m_size);
	  swap(m_bound,cp.m_bound);
	  swap(m_head,cp.m_head); 
	  swap(m_tail,cp.m_tail);
      }

        bqueue<T> fork() {
            return bqueue<T>(m_size,m_bound,m_head,m_tail);
        }
        void push_back(const T& val) {
            m_tail = itemptr(new item(this->m_tail, val)); 
            if ((++m_size) == 1) { m_head = m_tail; };
        }
        void push_back(T&& val) {
	  m_tail = itemptr(new item(this->m_tail, std::forward<T>(val))); 
	  if ((++m_size) == 1) { m_head = m_tail; };
        }
        void pop_back() {
            assert(m_size > 0);
            --m_size;
            m_tail = m_tail->back;
            if (m_size == 0) m_head = m_bound; 
        }
        T & front() { return m_head->value; }
        const T & front() const { return m_head->value; }
        T & back() { return m_tail->value; }
        const T & back() const { return m_tail->value; }
        iterator rbegin() { return m_tail.get(); }
        const_iterator rbegin() const { return m_tail.get(); }
        const_iterator rend() const { return m_bound.get(); }
        size_type size() const { return m_size; }
        bool empty() const { return m_size == 0; }
        const T & operator[](size_type i) const {
                int idx = m_size - i - 1;
                const_iterator it = rbegin();
                while (idx-- > 0) --it;
                return *it;
        }
        bool shared() { 
            // size = 0: never shared
            // size = 1: shared if head->refCount > 2 (m_head and m_tail)
            // size > 1: shared if head->refCount > 2 (m_head and second_hit->back)
            return (m_size > 0) && (m_head->refCount > 2);
        }
        // connect 'other' at the tail of this. will reset 'other' to an empty sequence
        void join(bqueue<T> &other) {
            using std::swap;
            if (m_size == 0) {
                swap(m_head,other.m_head);
                swap(m_tail,other.m_tail);
                swap(m_size,other.m_size);
            } else {
                other.m_head->back = this->m_tail;
                m_tail = other.m_tail;
                m_size += other.m_size;
                other.m_head = other.m_tail = other.m_bound;
                other.m_size = 0;
            }
        }
        void clear() { 
            m_head = m_bound; 
            m_tail = m_bound;
            m_size = 0;
        }
    private:
        bqueue(size_type size, itemptr bound, itemptr head, itemptr tail) :
            m_size(size), m_bound(bound), m_head(head), m_tail(tail) { }
        size_type m_size;
        itemptr m_bound, m_head, m_tail;
        
};
  template<typename T>
  void swap(bqueue<T> &rh, bqueue<T> &rh) {
    rh.swap(lh);
  }

}

#endif
