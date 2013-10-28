//---------------------------------------------------------------------------//
//!
//! \file    LCG64.cpp
//! \author  ?
//! \brief   Definition of the 64bit linear congruential generator class
//! \details This file originally appeared in the sprng4 library. It has been
//!          modified by Alex Robinson (aprobinson@wisc.edu) to better support
//!          the needs of FACEMC.
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>

// SPRNG Includes
#include "LCG64.hpp"
#include "Primes64.hpp"
#include "Store.hpp"

#define GENTYPE "64 bit Linear Congruential Generator with Prime Addend"
#define INIT_SEED1 0x2bc6ffffU
#define INIT_SEED0 0x8cfe166dU
#define TWO_M64 5.4210108624275222e-20 /* 2^(-64) */
#define EXPO 0x3ff0000000000000ULL


namespace sprng{

// Initialize static member data
int LCG64::num_generators = 0;
const unsigned int LCG64::parameter_list[][2] =
  {{0x87b0b0fdU, 0x27bb2ee6U}, 
   {0xe78b6955U, 0x2c6fe96eU},
   {0x31a53f85U, 0x369dea0fU}};

// Default constructor
LCG64::LCG64()
: d_rng_type( LCG64_TYPE ),
  d_gentype( GENTYPE ),
  d_stream_number( 0 ),
  d_init_seed( 0 ),
  d_parameter( 0 ),
  d_spawn_offset( 0 ),
  d_prime( 0 ),
  d_state( 0 ),
  d_multiplier( 0 )
{ /* ... */ }

// Copy constructor
LCG64::LCG64( const LCG64 &c )
: d_rng_type( c.d_rng_type ),
  d_gentype( c.d_gentype ),
  d_stream_number( c.d_stream_number ),
  d_init_seed( c.d_init_seed ),
  d_parameter( c.d_parameter ),
  d_spawn_offset( c.d_spawn_offset ),
  d_prime( c.d_prime ),
  d_state( c.d_state ),
  d_multiplier( c.d_multiplier )
{ /* ... */ }

// Assignment operator
LCG64& LCG64::operator=( const LCG64 &c )
{
  if( this != &c )
  {
    this->free_rng();
    
    d_rng_type = c.d_rng_type;
    d_gentype = c.d_gentype;
    d_stream_number = c.d_stream_number;
    d_init_seed = c.d_init_seed;
    d_parameter = c.d_parameter;
    d_spawn_offset = c.d_spawn_offset;
    d_prime = c.d_prime;
    d_state = c.d_state;
    d_multiplier = c.d_multiplier;
  }

  return *this;
}
  
// Destructor
LCG64::~LCG64()
{
  free_rng();
}

// Initialize the generator
/*! \details Gives back one stream (node gn) with updated spawning info. It
 * should be called tn times with a different value of gn in [0,tn) each call.
 */
int LCG64::init_rng( int gn, int tg, int s, int m )
{
  int i;
  double tempdbl;

  // Check if tn is valid?
  if (tg <= 0) 
  {
    tg = 1;
    fprintf(stderr,"WARNING - lcg64 init_rng: Total_gen <= 0. Default value of 1 used for total_gen\n");
  }

  // Check if gn is valid
  if (gn >= LCG64::max_streams) 
    fprintf(stderr,"WARNING - lcg64 init_rng: gennum: %d > maximum number of independent streams: %d\n\tIndependence of streams cannot be guranteed.\n",
	    gn, LCG64::max_streams); 
  
  if (gn < 0 || gn >= tg) 
  {
    fprintf(stderr,"ERROR - lcg64 init_rng: gennum %d out of range [%d,%d).\n",
	    gn, 0, tg); 
    return 0;
  }

  // Check if the parameter is valid
  if (m < 0 || m >= 3)   
  {
    fprintf(stderr,"WARNING - lcg64 init_rng: parameter not valid. Using Default parameter.\n");
    m = 0;
  }

  // Set the generator type
  d_rng_type = LCG64_TYPE;

  // Set the generator type description
  d_gentype = GENTYPE;

  // Set the stream number
  d_stream_number = gn;
  
  // Set the initial seed (only 31 LSB of seed considered)
  d_init_seed = s & 0x7fffffff; 

  // Set the parameter
  d_parameter = m;

  // Set the spawn offset
  d_spawn_offset = tg;

  // Set the prime for this generator
  Primes64::getprime_64( d_prime, gn );
  
  // Set the multiplier
  d_multiplier = ((unsigned long long) LCG64::parameter_list[m][1])<<32 | ((unsigned long long) LCG64::parameter_list[m][0]);

  // Set the generator state
  d_state = (((unsigned long long) INIT_SEED1)<<32 | INIT_SEED0) ^(((unsigned long long) s<<33)|gn);

  // Initialize this generator
  for(i=0; i<127*d_stream_number; i++)
    tempdbl = get_rn_dbl();
  
  // Increment the number of LCG64 streams being used
  LCG64::increment_number_of_streams();

  return 1;
}
  
// Return a random int
int LCG64::get_rn_int()
{
  advance_state();
  return d_state>>33;
}

// Return a random float in interval [0,1)
float LCG64::get_rn_flt()
{
  return (float)get_rn_dbl();
}

// Return a random double in interval [0,1)
double LCG64::get_rn_dbl()
{
  static double dtemp[1] = {0.0};

  advance_state();	
#if defined(CONVEX) || defined(O2K) || defined(SGI) || defined(GENERIC)
  *((unsigned long long *) dtemp) = (state>>12) | EXPO;
  return *dtemp - (double) 1.0;
#else
  return d_state*TWO_M64;
#endif
}

// Spawn new generators
int LCG64::spawn_rng( int nspawned, Sprng ***newgens )
{
  LCG64 ** genptr;
  int i, j;
  
  // Check that nspawned is valid
  if (nspawned <= 0) 
  {
    nspawned = 1;
    fprintf(stderr,"WARNING - spawn_rng: nspawned <= 0. Default value of 1 used for nspawned\n");
  }
  
  // Allocate memory for pointers to structures
  genptr = new LCG64 * [nspawned];

  if(genptr == NULL)	   
  {
    *newgens = NULL;
    return 0;
  }
  
  // Create nspawned new streams
  for(i=0; i<nspawned; i++)	
  {
    int s, gn;
    
    gn = d_stream_number + d_spawn_offset*(i+1);
  
    // Change seed to avoid repeating sequence
    if(gn > LCG64::max_streams)   
      s = (d_init_seed)^gn; 
    else
      s = d_init_seed;
    
    // Initialize a stream. The spawning info will be incorrect but it will be
    // corrected below.
    genptr[i] = new LCG64 ;
    genptr[i]->init_rng( gn, gn+1, s, d_parameter );
  
    if(genptr[i] == NULL)	/* Was generator initiallized? */
    {
      nspawned = i;
      break;
    }
    genptr[i]->d_spawn_offset = (nspawned+1)*d_spawn_offset;
  }
  
  d_spawn_offset *= (nspawned+1);
  *newgens = (Sprng **) genptr;
 
  return nspawned;
}

int LCG64::spawn_rng( int nspawned, 
		      std::vector<boost::shared_ptr<Sprng> > &newgens )
{
  Sprng **tmp_newgens;
  int val = spawn_rng( nspawned, &tmp_newgens );

  newgens.resize( val );

  for( int i = 0; i < val; ++i )
    newgens[i].reset( tmp_newgens[i] );

  delete[] tmp_newgens;

  return val;
}

// Return the generator seed
int LCG64::get_seed_rng()
{
  return d_init_seed;
}

// Free the memory allocated to this generator
int LCG64::free_rng()
{
  assert( this != NULL );

  LCG64::decrement_number_of_streams();
  
  return LCG64::num_generators;
}

// Pack this generator into a character buffer
int LCG64::pack_rng( char **buffer )
{
  std::string tmp_buffer;

  int val = pack_rng( tmp_buffer );

  *buffer = new char[tmp_buffer.size()];

  tmp_buffer.copy( *buffer, tmp_buffer.size() );

  return val;
}

int LCG64::pack_rng( std::string &buffer ) const
{
  // Clear the buffer
  buffer.clear();

  // All member data will be stored in a sub buffer and appended to main buffer
  std::string partial_buffer;

  // Store the generator type
  store_value( generatorTypeToInt( d_rng_type ), partial_buffer );
  buffer += partial_buffer;

  // Store the stream number
  store_value( d_stream_number, partial_buffer );
  buffer += partial_buffer;
  
  // Store the initial seed
  store_value( d_init_seed, partial_buffer );
  buffer += partial_buffer;
  
  // Store the parameter
  store_value( d_parameter, partial_buffer );
  buffer += partial_buffer;
  
  // Store the spawn offset
  store_value( d_spawn_offset, partial_buffer );
  buffer += partial_buffer;
  
  // Store the prime
  store_value( d_prime, partial_buffer );
  buffer += partial_buffer;
  
  // Store the state
  store_value( d_state, partial_buffer );
  buffer += partial_buffer;
  
  // Store the multiplier
  store_value( d_multiplier, partial_buffer );
  buffer += partial_buffer;

  return buffer.size();
}

// Print this generators info
int LCG64::print_rng()
{
  print( std::cout );

  return 1;    
}

void LCG64::print( std::ostream &os ) const
{
  os << d_gentype << ":" << std::endl
     << "  seed = " << d_init_seed << "," << std::endl
     << "  stream_number = " << d_stream_number << "," << std::endl
     << "  parameter = " << d_parameter << std::endl;
}

// Unpack this generator from a character buffer
int LCG64::unpack_rng( char* packed )
{
  std::size_t nbytes, offset = 0;
  int generator_type;
  std::string sub_string;

  // Load the generator type
  nbytes = sizeof( generator_type );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, generator_type );
  d_rng_type = intToGeneratorType( generator_type );
  packed += nbytes;

  // Load the generator description (not packed because always the same)
  d_gentype = GENTYPE;

  // Load the stream number
  nbytes = sizeof( d_stream_number );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_stream_number );
  packed += nbytes;

  // Load the initial seed
  nbytes = sizeof( d_init_seed );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_init_seed );
  packed += nbytes;

  // Load the parameter
  nbytes = sizeof( d_parameter );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_parameter );
  packed += nbytes;
  
  // Load the spawn offset
  nbytes = sizeof( d_spawn_offset );
  sub_string.assign( packed, nbytes );
  load_value( sub_string , d_spawn_offset );
  packed += nbytes;
  
  // Load the prime
  nbytes = sizeof( d_prime );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_prime );
  packed += nbytes;
  
  // Load the state
  nbytes = sizeof( d_state );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_state );
  packed += nbytes;
  
  // Load the multiplier
  nbytes = sizeof( d_multiplier );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_multiplier );
  packed += nbytes;

  // Increment the number of streams
  LCG64::increment_number_of_streams();

  return 1;
}

int LCG64::unpack_rng( const std::string &packed )
{
  char* packed_string = const_cast<char*>( packed.c_str() );

  return unpack_rng( packed_string );
}

// Get the number of open streams
int LCG64::get_number_of_streams()
{
  return LCG64::num_generators;
}

// Advance the seed state
void LCG64::advance_state()
{
  d_state = d_state*d_multiplier + d_prime;
}

// Increment the number of open streams
void LCG64::increment_number_of_streams( int num )
{
  LCG64::num_generators += num;
  
  if( LCG64::num_generators >= LCG64::max_streams )
    std::cerr << "WARNING: " << LCG64::num_generators 
	      << " open LCG64 streams. "
	      << "Independence can only be guaranteed with " 
	      << LCG64::max_streams << " LCG64 streams."
	      << std::endl;
}

// Decrement the number of open streams
void LCG64::decrement_number_of_streams( int num )
{
  LCG64::num_generators -= num;
}

} // end namespace sprng

//---------------------------------------------------------------------------//
// end LCG64.cpp
//---------------------------------------------------------------------------//

