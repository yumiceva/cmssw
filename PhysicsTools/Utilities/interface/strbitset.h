#ifndef strbitset_h
#define strbitset_h

/**
  \class    strbitset strbitset.h "CommonTools/Utils/interface/strbitset.h"
  \brief    Implements a string-indexed bit_vector

   The strbitset implements a string-indexed bit vector that will allow users
   to access the underlying bits by a string name instead of via an index.

  \author Salvatore Rappoccio
  \version  $Id: strbitset.h,v 1.1 2009/09/24 12:54:36 srappocc Exp $
*/


#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <fstream>

namespace std {

class strbitset {
 public:

  // Add typedefs
  typedef unsigned int                    size_t;
  typedef std::map<std::string, size_t>   str_index_map;
  typedef std::vector<bool>               bit_vector;

  //! constructor: just clears the bitset and map
  strbitset() {
    clear();
  }

  //! clear the bitset and map
  void clear() {
    map_.clear();
    bits_.clear();
  }

  ///! cast to bool
  operator bool() const {
    bool b = true;
    for ( bit_vector::const_iterator bitsBegin = bits_.begin(),
	    bitsEnd = bits_.end(), ibit = bitsBegin;
	  ibit != bitsEnd; ++ibit ) {
      if ( *ibit == false ) b = false;
    }
    return b;
  }

  ///! Logical negation of bool()
  bool operator!() const {
    return ! ( operator bool() );
  }
  
  /// adds an item that is indexed by the string. this
  /// can then be sorted, cut, whatever, and the
  /// index mapping is kept
  void push_back( std::string s ) {
    if ( map_.find(s) == map_.end() ) {
      map_[s] = bits_.size();
      bits_.resize( bits_.size() + 1 );
      *(bits_.rbegin()) = false;
    } else {
      std::cout << "Duplicate entry " << s << ", not added to registry" << std::endl;
    }
  }



  //! print method
  void print(std::ostream & out) const {
    for( str_index_map::const_iterator mbegin = map_.begin(),
	   mend = map_.end(),
	   mit = mbegin;
	 mit != mend; ++mit ) {
      char buff[100];
      sprintf(buff, "%10s = %6d", mit->first.c_str(), bits_.at(mit->second));
      out << buff << std::endl;
    }
  }

  //! access method const
  bit_vector::const_reference operator[] ( std::string s) const {
    size_t index = this->index(s);
    return bits_.operator[](index);
  }

  //! access method non-const
  bit_vector::reference operator[] ( std::string s) {
    size_t index = this->index(s);
    return bits_.operator[](index);
  }


  //! set method of all bits
  strbitset & set( bool val = true) {
    for ( bit_vector::iterator ibegin = bits_.begin(),
	    iend = bits_.end(), i = ibegin;
	  i != iend; ++i ) {
      *i = val;
    }
    return *this;
  }


  //! flip method of all bits
  strbitset & flip() {
    for ( bit_vector::iterator ibegin = bits_.begin(),
	    iend = bits_.end(), i = ibegin;
	  i != iend; ++i ) {
      *i = ! (*i);
    }
    return *this;
  }

  //! set method of one bit
  strbitset & set( std::string s, bool val = true) {
    (*this)[s] = val;
    return *this;
  }


  //! flip method of one bit
  strbitset & flip( std::string s) {
    (*this)[s] = !( (*this)[s] );
    return *this;
  }


  //! logical negation
  strbitset operator~() {
    strbitset ret(*this);
    for ( bit_vector::iterator ibegin = ret.bits_.begin(),
	    iend = ret.bits_.end(), i = ibegin;
	  i != iend; ++i ) {
      *i = !(*i);
    }
    return ret;
  }


  //! bitwise and
  strbitset & operator&=( const strbitset & r) {
    if ( map_.size() != r.map_.size() ) { 
      std::cout << "strbitset operator&= : bitsets not the same size" << std::endl;
    } else {
      str_index_map::iterator ibegin = map_.begin(), iend = map_.end(), i = ibegin;
      for ( ; i != iend; ++i ) {
	std::string key = i->first;
	str_index_map::const_iterator j = r.map_.find( key );
	if ( j == r.map_.end() ) {
	  std::cout << "strbitset operator&= : cannot find key " << key << std::endl;
	} else {
	  (*this)[key] = (*this)[key] && r[key];
	}
      }
    }
    return *this;
  }


  //! bitwise or
  strbitset & operator|=( const strbitset & r) {
    if ( map_.size() != r.map_.size() ) { 
      std::cout << "strbitset operator&= : bitsets not the same size" << std::endl;
    } else {
      str_index_map::iterator ibegin = map_.begin(), iend = map_.end(), i = ibegin;
      for ( ; i != iend; ++i ) {
	std::string key = i->first;
	str_index_map::const_iterator j = r.map_.find( key );
	if ( j == r.map_.end() ) {
	  std::cout << "strbitset operator&= : cannot find key " << key << std::endl;
	} else {
	  (*this)[key] = (*this)[key] || r[key];
	}
      }
    }
    return *this;
  }



  //! bitwise xor
  strbitset & operator^=( const strbitset & r) {
    if ( map_.size() != r.map_.size() ) { 
      std::cout << "strbitset operator&= : bitsets not the same size" << std::endl;
    } else {
      str_index_map::iterator ibegin = map_.begin(), iend = map_.end(), i = ibegin;
      for ( ; i != iend; ++i ) {
	std::string key = i->first;
	str_index_map::const_iterator j = r.map_.find( key );
	if ( j == r.map_.end() ) {
	  std::cout << "strbitset operator&= : cannot find key " << key << std::endl;
	} else {
	  (*this)[key] = ( (*this)[key] || r[key]) && ! ((*this)[key] && r[key]);
	}
      }
    }
    return *this;
  }





  //! equality operator
  bool operator==( const strbitset & r) const {
    if ( map_.size() != r.map_.size() ) { 
      std::cout << "strbitset operator&= : bitsets not the same size" << std::endl;
    } else {
      str_index_map::const_iterator ibegin = map_.begin(), iend = map_.end(), i = ibegin;
      for ( ; i != iend; ++i ) {
	std::string key = i->first;
	str_index_map::const_iterator j = r.map_.find( key );
	if ( j == r.map_.end() ) {
	  std::cout << "strbitset operator&= : cannot find key " << key << std::endl;
	} else {
	  if ( (*this)[key] != r[key] ) return false;
	}
      }
    }
    return true;
  }

  //! equality operator to bool
  bool operator==( bool b ) const {
    bool result = true;
    for ( bit_vector::const_iterator iBegin = bits_.begin(),
	    iEnd = bits_.end(), ibit = iBegin;
	  ibit != iEnd; ++ibit ) {
      result &= ( *ibit == b );
    }
    return result;
  }


  //! inequality operator
  bool operator!=( const strbitset & r) const {
    return ! (operator==(r));
  }

  //! inequality operator to bool
  bool operator!=( bool b ) const {
    return ! ( operator!=(b));
  }

  //! returns number of bits set
  size_t count() const {
    size_t ret = 0;
    for ( bit_vector::const_iterator ibegin = bits_.begin(),
	    iend = bits_.end(), i = ibegin;
	  i != iend; ++i ) {
      if ( *i ) ++ret;
    }    
    return ret;
  }



  //! returns true if any are set
  size_t any() const {
    for ( bit_vector::const_iterator ibegin = bits_.begin(),
	    iend = bits_.end(), i = ibegin;
	  i != iend; ++i ) {
      if ( ! (*i) ) return false;
    }
    return true;
  }

  //! returns true if none are set
  size_t none () const {
    return !any();
  }

  //! test
  bool test(std::string s) const {
    return (*this)[s] == true;
  }

  //! give access to the ordered bits
  const bit_vector& bits() const {
    return bits_;
  }


  //! give access to the ordered strings
  const std::vector<std::string> strings() const {
    std::vector<std::string> strings;
    strings.resize(bits_.size());
    for (str_index_map::const_iterator it = map_.begin(), 
         end = map_.end(); it != end; ++it){
      strings[it->second] = it->first;
    }
    return strings;
  }



  friend strbitset operator&(const strbitset& l, const strbitset& r);
  friend strbitset operator|(const strbitset& l, const strbitset& r);
  friend strbitset operator^(const strbitset& l, const strbitset& r);
  

 private:


  /// workhorse: this gets the index of "bits" that is pointed to by
  /// the string "s"
  size_t  index(std::string s) const {
    str_index_map::const_iterator f = map_.find(s);
    if ( f == map_.end() ) {
      std::cout << "Cannot find " << s << ", returning size()" << std::endl;
      return map_.size();
    } else {
      return f->second;
    }
  }

  
  str_index_map     map_;   //!< map that holds the string-->index map 
  bit_vector        bits_;  //!< the actual bits, indexed by the index in "map_"
};



}

#endif
