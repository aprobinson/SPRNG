//---------------------------------------------------------------------------//
//!
//! \file    Store.hpp
//! \author  ?
//! \brief   Definition of array storage functions.
//! \details Numbers are stored with the most significant bit first (left 
//!          most). This file originally appeared in the sprng4 library. It has
//!          been modified by Alex Robinson (aprobinson@wisc.edu) to better 
//!          support the needs of FACEMC.
//!
//---------------------------------------------------------------------------//

#ifndef STORE_DEF_HPP
#define STORE_DEF_HPP

// Std Lib Includes
#include <sstream>
#include <assert.h>

namespace sprng{

// Store an integer value in a character buffer
template<typename OrdinalType>
void store_value( const OrdinalType l, std::string &c )
{
  typename std::size_t nbytes = sizeof( OrdinalType );

  c.resize( nbytes );

  for( int i = 0; i<nbytes; i++ )
    c[i] = (l>>(8*(nbytes-i-1)))&0xff;
  
  // std::ostringstream oss;
  // oss << l;

  // c = oss.str();
}

// Store an array of integer values in a character buffer
template<typename OrdinalType>
void store_array( const std::vector<OrdinalType> &l, std::string &c )
{
  c.clear();
  std::string sub_string;

  for( int i = 0; i < l.size(); ++i )
  {
    store_value( l[i], sub_string );
    c += sub_string; 
  }
}

template<typename OrdinalType>
void store_array( const boost::shared_array<OrdinalType> &l,
		  const int n,
		  std::string &c )
{
  c.clear();
  std::string sub_string;
  
  for( int i = 0; i < n; ++i )
  {
    store_value( l[i], sub_string );
    c += sub_string;
  }
}

// Load an integer value from a character buffer
template<typename OrdinalType>
void load_value( const std::string &c, OrdinalType &l )
{
  typename std::size_t nbytes = sizeof( OrdinalType );

  l = 0;

  for( int i = 0; i < nbytes; ++i )
    l = (l<<8) + (c[i]&0xff);
  
  // std::istringstream iss( c );
  
  // iss >> l;  
}

// Load an array of integer values from a character buffer
template<typename OrdinalType>
void load_array( const std::string &c, int n, std::vector<OrdinalType> &l )
{
  l.resize( n );
  
  typename std::size_t nbytes = sizeof( OrdinalType );

  assert( c.size() >= n*nbytes );

  for( int i = 0; i < n; ++i )
    load_value( c.substr(i*nbytes, nbytes), l[i] );
}

template<typename OrdinalType>
void load_array( const std::string &c,
		 int n,
		 boost::shared_array<OrdinalType> &l )
{
  l.reset( new OrdinalType[n] );

  typename std::size_t nbytes = sizeof( OrdinalType );

  assert( c.size() >= n*nbytes );

  for( int i = 0; i < n; ++i )
    load_value( c.substr(i*nbytes, nbytes), l[i] );
}

} // end namespace sprng

#endif // end STORE_DEF_HPP

//---------------------------------------------------------------------------//
// end Store_def.hpp
//---------------------------------------------------------------------------//
