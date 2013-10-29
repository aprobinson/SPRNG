//---------------------------------------------------------------------------//
//!
//! \file   timing.cpp
//! \author Chris S. (June 10, 1999)
//! \brief  Main function for timing generators
//! \details 
//!  <ol>
//!   <li> Modified by Alex Robinson (Oct 2013)
//!   <li> Modified by J. Ren (Sept 2006)
//!  </ol>
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <time.h>

// Boost Includes
#include <boost/shared_ptr.hpp>

// SPRNG Includes
#include "Sprng.hpp"
#include "LFG.hpp"
#include "LCG.hpp"
#include "LCG64.hpp"

// Time macro
#define TIME() (clock()/((double)CLOCKS_PER_SEC))

// Generator timing function
template<typename Generator>
void timeGenerator( const int trial_size )
{
  // Timing info
  double time1, time2, time3, time4;
  double mints_per_sec, mflts_per_sec, mdbls_per_sec;
  
  // Create the generator
  boost::shared_ptr<sprng::Sprng> generator;
  generator.reset( new Generator );
  generator->init_rng( 0, 1, 0, 0 );

  time1 = TIME();

  // Integer generation timing
  for( int i = 0; i < trial_size; ++i )
    generator->get_rn_int();

  time2 = TIME();

  // Float generation timing
  for( int i = 0; i < trial_size; ++i )
    generator->get_rn_flt();

  time3 = TIME();

  // Double generation timing
  for( int i = 0; i < trial_size; ++i )
    generator->get_rn_dbl();

  time4 = TIME();

  // Check for valid time intervals
  if( time2-time1 < 1.0e-15 || time3-time2 < 1.0e-15 || time4-time3 < 1.0e-15 )
  {
    std::cerr << "Timing Information not accurate enough for this generator."
	      << std::endl;
  }
  else
  {
    // Calculate the generation speed (Millions/sec)
    mints_per_sec = trial_size/(time2-time1)/1e6;
    mflts_per_sec = trial_size/(time3-time2)/1e6;
    mdbls_per_sec = trial_size/(time4-time3)/1e6;
    
    // Print the last double generated
    std::cout << "Last random number generated: " << generator->get_rn_dbl()  
	      << std::endl
	      << "User + System time information (Note: MRS = Million Random " 
	      << "Numbers Per Second)\n" << std::endl
	      << "  Integer generator:\tTime = " << time2-time1 << " seconds "
	      << "=> " << mints_per_sec << std::endl
	      << "  Float generator:\tTime = " << time3-time2 << " seconds "
	      << "=> " << mflts_per_sec << std::endl
	      << "  Double generator:\tTime = " << time4-time3 << " seconds "
	      << "=> " << mdbls_per_sec << std::endl << std::endl;
  } 
}

// Main timing function
int main()
{
  int trial_size = 10000000;

  // Time the LFG
  std::cout << "Timing LFG:" << std::endl;
  timeGenerator<sprng::LFG>( trial_size );
  
  // Time the LCG
  std::cout << "Timing LCG:" << std::endl;
  timeGenerator<sprng::LCG>( trial_size );

  // Time the LCG64
  std::cout << "Timing LCG64:" << std::endl;
  timeGenerator<sprng::LCG64>( trial_size );  
}

//---------------------------------------------------------------------------//
// end timing.cpp
//---------------------------------------------------------------------------//
