//---------------------------------------------------------------------------//
//!
//! \file   tstLFG.cpp
//! \author Alex Robinson
//! \brief  Unit tests for LFG class
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <limits>

// Boost Includes
#define BOOST_TEST_MAIN
#include <boost/test/included/unit_test.hpp>

// SPRNG Includes
#include "LFG.hpp"

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that a single LFG can be initialized
BOOST_AUTO_TEST_CASE( init_rng )
{
  int return_value;
  sprng::LFG generator;
  
  return_value = generator.init_rng( 0, 1, 1, 0 );

  BOOST_CHECK_EQUAL( return_value, 1 );
  BOOST_CHECK_EQUAL( sprng::LFG::get_number_of_streams(), 1 );
}

//---------------------------------------------------------------------------//
// Check that an random integer can be created
BOOST_AUTO_TEST_CASE( get_rn_int )
{
  int random_int;
  sprng::LFG generator;

  generator.init_rng( 0, 1, 1, 0 );

  random_int = generator.get_rn_int();

  BOOST_CHECK_GE( random_int, 0 );
  BOOST_CHECK_LE( random_int, std::numeric_limits<int>::max() );
}

//---------------------------------------------------------------------------//
// Check that a random float can be created
BOOST_AUTO_TEST_CASE( get_rn_flt )
{
  float random_flt;
  sprng::LFG generator;

  generator.init_rng( 0, 1, 1, 0 );

  random_flt = generator.get_rn_flt();

  BOOST_CHECK_GE( random_flt, 0.0f );
  BOOST_CHECK_LE( random_flt, 1.0f );
}

//---------------------------------------------------------------------------//
// Check that a random double can be created
BOOST_AUTO_TEST_CASE( get_rn_dbl )
{
  float random_dbl;
  sprng::LFG generator;

  generator.init_rng( 0, 1, 1, 0 );

  random_dbl = generator.get_rn_dbl();
  
  BOOST_CHECK_GE( random_dbl, 0.0 );
  BOOST_CHECK_LE( random_dbl, 1.0 );
}

//---------------------------------------------------------------------------//
// Check that the generator seed can be obtained
BOOST_AUTO_TEST_CASE( get_seed_rng )
{
  int generator_seed;
  sprng::LFG generator;

  generator.init_rng( 0, 1, 1, 0 );

  generator_seed = generator.get_seed_rng();
    
  BOOST_CHECK_EQUAL( generator_seed, 1 );
}

//---------------------------------------------------------------------------//
// Check that the number of generator streams can be decremented
BOOST_AUTO_TEST_CASE( free_rng )
{
  boost::shared_ptr<sprng::LFG> generator1( new sprng::LFG ), 
    generator2( new sprng::LFG );

  generator1->init_rng( 0, 2, 1, 0 );
  generator2->init_rng( 1, 2, 1, 0 );

  BOOST_REQUIRE_EQUAL( sprng::LFG::get_number_of_streams(), 2 );

  generator1.reset();
  
  BOOST_REQUIRE_EQUAL( sprng::LFG::get_number_of_streams(), 1 );

  generator2.reset();

  BOOST_REQUIRE_EQUAL( sprng::LFG::get_number_of_streams(), 0 );
}

//---------------------------------------------------------------------------//
// Check that the generator can be printed
BOOST_AUTO_TEST_CASE( print )
{
  sprng::LFG generator;
  generator.init_rng( 0, 1, 1, 0 );
  
  std::stringstream ss;
  
  ss << generator << std::endl;
  
  std::cout << std::endl << "PRINT TEST:" << std::endl;
  generator.print_rng();
  std::cout << "PRINT TEST FINISHED" << std::endl;
}

//---------------------------------------------------------------------------//
// Check that a generator can be packed into a character buffer and unpacked
BOOST_AUTO_TEST_CASE( pack_unpack_rng_char )
{
  boost::shared_ptr<sprng::LFG> generator1( new sprng::LFG ),
    generator2( new sprng::LFG );
  char *buffer1, *buffer2;
  
  generator1->init_rng( 0, 2, 1, 0 );
  generator2->init_rng( 1, 2, 1, 0 );
  
  // Pack the generators
  generator1->pack_rng( &buffer1 );
  generator2->pack_rng( &buffer2 );
  
  // Save the current generator states
  std::stringstream ss;
  std::string generator1_orig_state, generator2_orig_state;

  ss << *generator1;
  generator1_orig_state = ss.str();
  ss.str("");
  
  ss << *generator2;
  generator2_orig_state = ss.str();
  ss.str("");

  // Free the original generator
  generator1.reset( new sprng::LFG );
  generator2.reset( new sprng::LFG );

  // Unpack the original generators
  generator1->unpack_rng( buffer1 );
  generator2->unpack_rng( buffer2 );

  delete[] buffer1;
  delete[] buffer2;

  // Get the unpacked generator states
  std::string generator1_unpacked_state, generator2_unpacked_state;

  ss << *generator1;
  generator1_unpacked_state = ss.str();
  ss.str("");

  ss << *generator2;
  generator2_unpacked_state = ss.str();
  ss.str("");

  // Check that the states match
  BOOST_CHECK_EQUAL( sprng::LFG::get_number_of_streams(), 2 );
  BOOST_CHECK_EQUAL( generator1_orig_state, generator1_unpacked_state );
  BOOST_CHECK_EQUAL( generator2_orig_state, generator2_unpacked_state );
}
//---------------------------------------------------------------------------//
//  Check that a generator can be packed into a string buffer and unpacked
BOOST_AUTO_TEST_CASE( pack_unpack_rng_string )
{
  boost::shared_ptr<sprng::LFG> generator1( new sprng::LFG ),
    generator2( new sprng::LFG );
  std::string buffer1, buffer2;
  
  generator1->init_rng( 0, 2, 1, 0 );
  generator2->init_rng( 1, 2, 1, 0 );
  
  // Pack the generators
  generator1->pack_rng( buffer1 );
  generator2->pack_rng( buffer2 );
  
  // Save the current generator states
  std::stringstream ss;
  std::string generator1_orig_state, generator2_orig_state;

  ss << *generator1;
  generator1_orig_state = ss.str();
  ss.str("");
  
  ss << *generator2;
  generator2_orig_state = ss.str();
  ss.str("");

  // Free the original generator
  generator1.reset( new sprng::LFG );
  generator2.reset( new sprng::LFG );

  // Unpack the original generators
  generator1->unpack_rng( buffer1 );
  generator2->unpack_rng( buffer2 );

  // Get the unpacked generator states
  std::string generator1_unpacked_state, generator2_unpacked_state;

  ss << *generator1;
  generator1_unpacked_state = ss.str();
  ss.str("");

  ss << *generator2;
  generator2_unpacked_state = ss.str();
  ss.str("");

  // Check that the states match
  BOOST_CHECK_EQUAL( sprng::LFG::get_number_of_streams(), 2 );
  BOOST_CHECK_EQUAL( generator1_orig_state, generator1_unpacked_state );
  BOOST_CHECK_EQUAL( generator2_orig_state, generator2_unpacked_state );
}

//---------------------------------------------------------------------------//
// Check that new generators can be spawned
BOOST_AUTO_TEST_CASE( spawn_rng )
{
  int num_generators_spawned;
  sprng::LFG generator;
  std::vector<boost::shared_ptr<sprng::Sprng> > new_generators;
  
  generator.init_rng( 0, 1, 1, 0 );
  num_generators_spawned = generator.spawn_rng( 10, new_generators );

  BOOST_CHECK_EQUAL( num_generators_spawned, 10 );
  BOOST_CHECK_EQUAL( sprng::LFG::get_number_of_streams(), 11 );
}

//---------------------------------------------------------------------------//
// end tstLFG.cpp
//---------------------------------------------------------------------------//
