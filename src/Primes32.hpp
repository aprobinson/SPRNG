//---------------------------------------------------------------------------//
//!
//! \file    Primes32.hpp
//! \author  ?
//! \brief   Declaration of struct to handle 32bit primes.
//! \details This file originally appeared in the sprng4 library. It has been
//!          modified by Alex Robinson (aprobinson@wisc.edu) to better support
//!          the needs of FACEMC.
//!
//---------------------------------------------------------------------------//

#ifndef PRIMES_32_HPP
#define PRIMES_32_HPP

// Std Lib Includes
#include <vector>

namespace sprng{

//! Struct for handling 32bit primes
struct Primes32
{

public:

  //! Initialize the primes static array
  static int init_prime_32();

  //! Get the number of primes requested
  static int getprime_32( int need,
			  std::vector<int> &prime_array,
			  int offset );

  //! Get a single prime
  static int getprime_32( int &prime, int offset );
  
  //! Reset the primes list
  static void reset();

  // sqrt(2)*2^23 + 2
  static const int max_prime = 11863285; 
  
  // sqrt(max_prime)
  static const int min_prime = 3444;
  
  // Total number of available primes
  static const int max_prime_offset = 779156;

private:

  // Full prime array
  static const int full_prime_list_size = 1778;
  static const int prime_list_32[1778];

  // Has the primes array been initialized?
  static bool prime_list_initialized;

  // Prime array (once initialized, prime_list_initialized will be set to true)
  static const int n_primes = 1000;
  static const int sub_prime_list_size = 1000;
  static const int step = 1000;
  static int primes[1000];
};

} // end namespace sprng

#endif // end PRIMES_32_HPP

//---------------------------------------------------------------------------//
// end Primes32.hpp
//---------------------------------------------------------------------------//

