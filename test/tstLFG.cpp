//---------------------------------------------------------------------------//
//!
//! \file   tstLFG.cpp
//! \author Alex Robinson
//! \brief  Unit tests for LFG class
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// Boost Includes
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

// SPRNG Includes
#include "LFG.hpp"

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that a single LFG can be initialized
BOOST_AUTO_TEST_CASE( init_rng_single )
{
  int return_value;
  sprng::LFG generator;
  
  return_value = generator.init_rng( 0, 1, 1, 0 );

  BOOST_CHECK_EQUAL( return_value, 1 );
}

//---------------------------------------------------------------------------//
// end tstLFG.cpp
//---------------------------------------------------------------------------//
