//---------------------------------------------------------------------------//
//!
//! \file   tstPrimes64.cpp
//! \author Alex Robinson
//! \brief  Unit tests for Primes64 struct.
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// Boost Includes
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

// SPRNG Includes
#include "Primes64.hpp"

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that a prime value can be retrieved.
BOOST_AUTO_TEST_CASE( getprime_64 )
{
  unsigned int prime1, prime5, prime10; 
  int obtained1, obtained5, obtained10;

  obtained1 = sprng::Primes64::getprime_64( prime1, 0 );
  obtained5 = sprng::Primes64::getprime_64( prime5, 4 );
  obtained10 = sprng::Primes64::getprime_64( prime10, 9 );

  BOOST_REQUIRE_EQUAL( obtained1, 1 );
  BOOST_REQUIRE_EQUAL( obtained5, 1 );
  BOOST_REQUIRE_EQUAL( obtained10, 1 );

  BOOST_CHECK_EQUAL( prime1, 3037000493U );
  BOOST_CHECK_EQUAL( prime5, 3037000399U );
  BOOST_CHECK_EQUAL( prime10, 3037000289U );
}

//---------------------------------------------------------------------------//
// end tstPrimes64.cpp
//---------------------------------------------------------------------------//
