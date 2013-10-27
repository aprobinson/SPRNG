//---------------------------------------------------------------------------//
//!
//! \file   tstGeneratorType.cpp
//! \author Alex Robinson
//! \brief  GeneratorType conversion unit tests
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// Boost Includes
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

// SPRNG Includes
#include "GeneratorType.hpp"

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that a generator type can be converted to an int
BOOST_AUTO_TEST_CASE( generatorTypeToInt )
{
  sprng::GeneratorType gen_type0 = sprng::LFG;
  sprng::GeneratorType gen_type1 = sprng::LCG;
  sprng::GeneratorType gen_type2 = sprng::LCG64;
  // sprng::GeneratorType gen_type3 = sprng::CMRG;
  // sprng::GeneratorType gen_type4 = sprng::MLFG;
  // sprng::GeneratorType gen_type5 = sprng::PMLCG;

  int gen_type0_int = sprng::generatorTypeToInt( gen_type0 );
  int gen_type1_int = sprng::generatorTypeToInt( gen_type1 );
  int gen_type2_int = sprng::generatorTypeToInt( gen_type2 );
  // int gen_type3_int = sprng::generatorTypeToInt( gen_type3 );
  // int gen_type4_int = sprng::generatorTypeToInt( gen_type4 );
  // int gen_type5_int = sprng::generatorTypeToInt( gen_type5 );
  
  BOOST_CHECK_EQUAL( gen_type0_int, 0 );
  BOOST_CHECK_EQUAL( gen_type1_int, 1 );
  BOOST_CHECK_EQUAL( gen_type2_int, 2 );
  // BOOST_CHECK_EQUAL( gen_type3_int, 3 );
  // BOOST_CHECK_EQUAL( gen_type4_int, 4 );
  // BOOST_CHECK_EQUAL( gen_type5_int, 5 );
}

//---------------------------------------------------------------------------//
// Check that an int can be converted to a generator type
BOOST_AUTO_TEST_CASE( intToGeneratorType )
{
  int gen_type0_int = 0;
  int gen_type1_int = 1;
  int gen_type2_int = 2;
  // int gen_type3_int = 3;
  // int gen_type4_int = 4;
  // int gen_type5_int = 5;

  sprng::GeneratorType gen_type0 = sprng::intToGeneratorType( gen_type0_int );
  sprng::GeneratorType gen_type1 = sprng::intToGeneratorType( gen_type1_int );
  sprng::GeneratorType gen_type2 = sprng::intToGeneratorType( gen_type2_int );
  // sprng::GeneratorType gen_type3 = sprng::intToGeneratorType( gen_type3_int );
  // sprng::GeneratorType gen_type4 = sprng::intToGeneratorType( gen_type4_int );
  // sprng::GeneratorType gen_type5 = sprng::intToGeneratorType( gen_type5_int );
  
  BOOST_CHECK_EQUAL( gen_type0, sprng::LFG );
  BOOST_CHECK_EQUAL( gen_type1, sprng::LCG );
  BOOST_CHECK_EQUAL( gen_type2, sprng::LCG64 );
  // BOOST_CHECK_EQUAL( gen_type3, sprng::CMRG );
  // BOOST_CHECK_EQUAL( gen_type4, sprng::MLFG );
  // BOOST_CHECK_EQUAL( gen_type5, sprng::PMLCG );
}

//---------------------------------------------------------------------------//
// end tstGeneratorType.cpp
//---------------------------------------------------------------------------//
