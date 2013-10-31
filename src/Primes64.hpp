//---------------------------------------------------------------------------//
//! 
//! \file    Primes64.hpp
//! \author  ?
//! \brief   Declaration of struct to handle 64bit primes.
//! \details This file originally appeared in the sprng4 library. It has been
//!          modified by Alex Robinson (aprobinson@wisc.edu) to better support
//!          the needs of FACEMC.
//!
//---------------------------------------------------------------------------//

#ifndef PRIMES_64_HPP
#define PRIMES_64_HPP

// Std Lib Includes
#include <vector>

namespace sprng{

//! Struct for handling 64bit primes
struct Primes64
{

public:

  //! Initialize the primes static array
  static int init_prime_64();
  
  //! Get the number of primes requested
  static int getprime_64( int need, 
			  std::vector<unsigned int> &prime_array,
			  int offset );

  //! Get a single prime
  static int getprime_64( unsigned int &prime, int offset );

  //! Reset the prime list
  static void reset();

private:

  // Largest odd # < sqrt(2)*2^31+2 
  static const unsigned int max_prime = 3037000501U; 
  // sqrt(max_prime)
  static const unsigned int min_prime = 55108U;
  // Total number of available primes
  static const unsigned int max_prime_offset = 146138719U;

  // Full prime array
  static const int full_prime_list_size = 15613;
  static const unsigned int prime_list_64[15613];

  // Has the primes array been initialized?
  static bool prime_list_initialized;

  // Prime array (once initialized, prime_list_initialized will be set to true)
  static const int n_primes = 10000;
  static const int sub_prime_list_size = 1000;
  static const int step = 10000;
  static int primes[10000];
};

} // end namespace sprng

#endif // end PRIMES_64_HPP

//---------------------------------------------------------------------------//
// end Primes64.hpp
//---------------------------------------------------------------------------//
