//---------------------------------------------------------------------------//
//!
//! \file    LCG64.cpp
//! \author  ?
//! \brief   Declaration of the 64bit linear congruential generator class
//! \details This file originally appeared in the sprng4 library. It has been
//!          modified by Alex Robinson (aprobinson@wisc.edu) to better support
//!          the needs of FACEMC.
//!
//---------------------------------------------------------------------------//

#ifndef LCG64_HPP
#define LCG64_HPP

// Std Lib Includes
#include <vector>

// Boost Includes
#include <boost/shared_ptr.hpp>

// SPRNG Includes
#include "Sprng.hpp"
#include "GeneratorType.hpp"

namespace sprng{

//! A 64bit linear congruential pseudo-random number generator
class LCG64 : public Sprng
{

public:

  //! Default constructor
  LCG64();

  //! Copy constructor
  LCG64( const LCG64 &c );

  //! Assignment operator
  LCG64& operator=( const LCG64 &c );
  
  //! Destructor
  virtual ~LCG64();

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

  //! Advance the seed state
  inline void advance_state();

  //! Increment the number of open streams
  static void increment_number_of_streams( int num = 1 );

  //! Decrement the number of open streams
  static void decrement_number_of_streams( int num = 1 );

  //! Max number of LCG64 streams possible
  static const int max_streams = 146138719;

  //! Number of streams currently open
  static int num_generators;

  //! Available mutlipliers
  static const unsigned int parameter_list[3][2];

  // Generator type
  GeneratorType d_rng_type;

  // A string that describes the generator type
  std::string d_gentype;

  // Stream number of this generator
  int d_stream_number;

  // Initial Seed
  int d_init_seed;

  // A parameter for the generator
  int d_parameter;

  // Total number of generators spawned
  int d_spawn_offset;

  // Prime that will be used by generator
  unsigned int d_prime;

  // Generator state
  unsigned long long d_state;
  
  // A multiplier for the generator
  unsigned long long d_multiplier;
};

} // end namespace sprng

#endif // end LCG64_HPP

//---------------------------------------------------------------------------//
// end LCG64.hpp
//---------------------------------------------------------------------------//
