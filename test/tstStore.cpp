//---------------------------------------------------------------------------//
//!
//! \file   tstStore.cpp
//! \author Alex Robinson
//! \brief  Unit tests for storage functions
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <string>
#include <vector>

// Boost Includes
#include <boost/shared_array.hpp>
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>
#include <boost/test/test_case_template.hpp>
#include <boost/mpl/list.hpp>

// SPRNG Includes
#include "Store.hpp"

//---------------------------------------------------------------------------//
// Testing Typedefs.
//---------------------------------------------------------------------------//
typedef boost::mpl::list<int,
			 unsigned,
			 long,
			 unsigned long,
			 long long, 
			 unsigned long long> test_types;

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that an OrdinalType can be stored in a character buffer and recovered
BOOST_AUTO_TEST_CASE_TEMPLATE( store_load_value, OrdinalType, test_types )
{
  OrdinalType value = 1234, recovered_value;

  std::string buffer;
  
  sprng::store_value( value, buffer );
  sprng::load_value( buffer, recovered_value );

  BOOST_CHECK_EQUAL( value, recovered_value );
}

//---------------------------------------------------------------------------//
// Check that a std::vector of OrdinalTypes can be stored in a character 
// buffer and recovered
BOOST_AUTO_TEST_CASE_TEMPLATE( store_vector, OrdinalType, test_types )
{
  std::vector<OrdinalType> array( 3 ), recovered_array;
  array[0] = 1234;
  array[1] = 5678;
  array[2] = 9101112;

  std::string buffer;

  sprng::store_array( array, buffer );
  sprng::load_array( buffer, 3, recovered_array );
  
  BOOST_CHECK_EQUAL( array[0], recovered_array[0] );
  BOOST_CHECK_EQUAL( array[1], recovered_array[1] );
  BOOST_CHECK_EQUAL( array[2], recovered_array[2] );
}

//---------------------------------------------------------------------------//
// Check that a boost::shared_array of OrdinalTypes can be stored in a 
// character buffer and recovered
BOOST_AUTO_TEST_CASE_TEMPLATE( store_shared_array, OrdinalType, test_types )
{
  boost::shared_array<OrdinalType> array( new OrdinalType[3] );    
  array[0] = 1234;
  array[1] = 5678;
  array[2] = 9101112;

  boost::shared_array<OrdinalType> recovered_array;

  std::string buffer;

  sprng::store_array( array, 3, buffer );
  sprng::load_array( buffer, 3, recovered_array );

  BOOST_CHECK_EQUAL( array[0], recovered_array[0] );
  BOOST_CHECK_EQUAL( array[1], recovered_array[1] );
  BOOST_CHECK_EQUAL( array[2], recovered_array[2] );
}

//---------------------------------------------------------------------------//
// end tstStore.cpp
//---------------------------------------------------------------------------//
