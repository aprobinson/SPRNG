//---------------------------------------------------------------------------//
//!
//! \file    LCG.hpp
//! \author  ?
//! \brief   Declaration of the 48bit linear congruential generator class
//! \details This file originally appeared in the sprng4 library. It has been
//!          modified by Alex Robinson (aprobinson@wisc.edu) to better support
//!          the needs of FACEMC.
//!
//---------------------------------------------------------------------------//

#ifndef LCG_HPP
#define LCG_HPP

// Std Lib Includes
#include <string>

// SPRNG Includes
#include "Sprng.hpp"
#include "GeneratorType.hpp"

namespace sprng{

//! A 48bit linear congruential pseudo-random number generator
class LCG : public Sprng
{
  
public:

  //! Default constructor
  LCG();

  //! Copy constructor
  LCG( const LCG &c );

  //! Assignment operator
  LCG& operator=( const LCG &c );
  
  //! Destructor
  virtual ~LCG();

  //@{
  //! Generator interface
  //! Initialize the generator
  int init_rng( int gn, int tg, int s, int m );
  
  //! Return a random int
  int get_rn_int();

  //! Return a random float in interval [0,1)
  float get_rn_flt();

  //! Return a random double in interval [0,1)
  double get_rn_dbl();

  //! Spawn new generators
  int spawn_rng( int nspawned, Sprng ***newgens );
  int spawn_rng( int nspawned, 
		  std::vector<boost::shared_ptr<Sprng> > &newgens );

  //! Return the generator seed
  int get_seed_rng();

  //! Free the memory allocated to this generator
  int free_rng();

  //! Pack this generator into a character buffer
  int pack_rng( char **buffer );
  int pack_rng( std::string &buffer ) const;

  //! Print this generators info
  int print_rng();
  void print( std::ostream &os ) const;

  //! Unpack this generator from a character buffer
  int unpack_rng( char *packed );
  int unpack_rng( const std::string &packed );

  //! Get the number of open streams
  static int get_number_of_streams();
  //@}

private:

  //! Multiply the seed by the multiplier
  inline void multiply();

  //! Advance the seed
  void advance_seed();

  //! Increment the number of open streams
  static void increment_number_of_streams( int num = 1 );

  //! Decrement the number of open streams
  static void decrement_number_of_streams( int num = 1 );

  //! Max number of LCG streams possible 
  static const int max_streams;

  //! Number of streams currently open
  static int num_generators;

  //! Available multipliers
  static const unsigned long long mults[7];

  // Generator type
  GeneratorType d_rng_type;

  // Generator seed
  unsigned long long d_seed;

  // Initial seed
  int d_init_seed;

  // Prime that will be used by generator
  int d_prime;

  // Index of prime position
  int d_prime_position;

  // The next prime
  int d_prime_next;

  // A string that describes the generator type
  std::string d_gentype;

  // A parameter for the generator
  int d_parameter;

  // A multiplier for the generator
  unsigned long long d_multiplier; 
};

} // end namespace sprng

#endif // LCG_HPP

//---------------------------------------------------------------------------//
// end LCG.hpp
//---------------------------------------------------------------------------//
