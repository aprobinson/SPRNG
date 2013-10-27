//---------------------------------------------------------------------------//
//!
//! \file   tstSeedHelpers.cpp
//! \author Alex Robinson
//! \brief  Unit tests for seed helper functions.
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// Boost Includes
#define BOOST_TEST_MODULE Seed Helpers
#include <boost/test/included/unit_test.hpp>

// SPRNG Includes
#include "SeedHelpers.hpp"

//---------------------------------------------------------------------------//
// Testing Structs.
//---------------------------------------------------------------------------//
struct MPITestSuite{
  MPITestSuite() { /* ... */ }
  
  ~MPITestSuite()
  {
#ifdef HAVE_SPRNG_MPI
    BOOST_TEST_MESSAGE( "Finalizing MPI\n" );
    MPI_Finalize();
#endif
  }
  
  static bool mpi_initialized;  
};

bool MPITestSuite::mpi_initialized = false;

struct MPITest{
  MPITest() 
  {  
#ifdef HAVE_SPRNG_MPI
    if( !MPITestSuite::mpi_initialized )
    {
      MPI_Init(&boost::unit_test::framework::master_test_suite().argc,
	       &boost::unit_test::framework::master_test_suite().argv);
      MPITestSuite::mpi_initialized = true;
    }
#endif
  }
  ~MPITest() { /* ... */ }
};

//---------------------------------------------------------------------------//
// Global Testing Fixture.
//---------------------------------------------------------------------------//
BOOST_GLOBAL_FIXTURE( MPITestSuite );

//---------------------------------------------------------------------------//
// Test Suite.
//---------------------------------------------------------------------------//
BOOST_FIXTURE_TEST_SUITE( MPIHelpers, MPITest );

//---------------------------------------------------------------------------//
// Tests.
//---------------------------------------------------------------------------//
// Check that a valid seed can be created
BOOST_AUTO_TEST_CASE( make_new_seed )
{
  // make_new_seed_mpi defaults to make_new_seed when mpi isn't used
  int seed = sprng::make_new_seed_mpi();
  
  // Check that the seed is greater that zero
  BOOST_CHECK_GE( seed, 0 );

  // Check that the seed is valid (not NaN)
  BOOST_CHECK_EQUAL( seed, seed );
}

//---------------------------------------------------------------------------//
// End the boost auto test suite.
//---------------------------------------------------------------------------//
BOOST_AUTO_TEST_SUITE_END()

//---------------------------------------------------------------------------//
// end tstSeedHelpers.cpp
//---------------------------------------------------------------------------//
