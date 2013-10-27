//---------------------------------------------------------------------------//
//!
//! \file   tstMPIHelpers.cpp
//! \author Alex Robinson
//! \brief  Unit tests for MPI helper functions
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>

// Booost Includes
#include <boost/shared_array.hpp>
#define BOOST_TEST_MODULE MPI Helpers
#include <boost/test/included/unit_test.hpp>

// SPRNG Includes
#include "MPIHelpers.hpp"

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
// Check that the process info can be determined
BOOST_AUTO_TEST_CASE( get_proc_info_mpi )
{
  int proc_id, ref_proc_id;
  int number_of_procs, ref_number_of_procs;

#ifdef HAVE_SPRNG_MPI
  MPI_Comm_rank( MPI_COMM_WORLD, &ref_proc_id );
  MPI_Comm_size( MPI_COMM_WORLD, &ref_number_of_procs );
#else
  ref_proc_id = 0;
  ref_number_of_procs = 1;
#endif
  
  sprng::get_proc_info_mpi( proc_id, number_of_procs );

  BOOST_CHECK_EQUAL( proc_id, ref_proc_id );
  BOOST_CHECK_EQUAL( number_of_procs, ref_number_of_procs );
}

//---------------------------------------------------------------------------//
// End the boost auto test suite.
//---------------------------------------------------------------------------//
BOOST_AUTO_TEST_SUITE_END()

//---------------------------------------------------------------------------//
// end tstMPIHelpers.cpp
//---------------------------------------------------------------------------//

