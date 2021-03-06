//---------------------------------------------------------------------------//
//!
//! \file    LCG.cpp
//! \author  ?
//! \brief   Definition of the 48bit linear congruential generator class
//! \details This file originally appeared in the sprng4 library. It has been
//!          modified by Alex Robinson (aprobinson@wisc.edu) to better support
//!          the needs of FACEMC.
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <assert.h>

// SPRNG Includes
#include "LCG.hpp"
#include "Primes32.hpp"
#include "Store.hpp"

#define GENTYPE "48 bit Linear Congruential Generator with Prime Addend"
#define INIT_SEED 0x2bc68cfe166dLL
#define MSB_SET 0x3ff0000000000000LL
#define LCGRUNUP 29
#define LSB48 0xffffffffffffLL

using namespace std;

namespace sprng{

// Initialize static member data
const int LCG::max_streams = 1<<19;
int LCG::num_generators = 0;
const unsigned long long LCG::mults[] = {
  0x2875a2e7b175LL,
  0x5deece66dLL,
  0x3eac44605265LL,
  0x1ee1429cc9f5LL,
  0x275b38eb4bbdLL,
  0x739a9cb08605LL,
  0x3228d7cc25f5LL,
};

// Default constructor
LCG::LCG()
  : d_rng_type( LCG_TYPE ),
    d_seed( 0 ),
    d_init_seed( 0 ),
    d_prime( 0 ),
    d_prime_position( 0 ),
    d_prime_next( 0 ),
    d_gentype( GENTYPE ),
    d_parameter( 0 ),
    d_multiplier( 0 )
{ /* ... */ }

// Copy constructor
LCG::LCG( const LCG &c )
: d_rng_type( c.d_rng_type ),
  d_seed( c.d_seed ),
  d_init_seed( c.d_init_seed ),
  d_prime( c.d_prime ),
  d_prime_position( c.d_prime_position ),
  d_prime_next( c.d_prime_next ),
  d_gentype( c.d_gentype ),
  d_parameter( c.d_parameter ),
  d_multiplier( c.d_multiplier )
{ /* ... */ }

// Assignment operator
LCG& LCG::operator=( const LCG &c )
{
  if( this != &c )
  {
    this->free_rng();

    d_rng_type = c.d_rng_type;
    d_seed = c.d_seed;
    d_init_seed = c.d_init_seed;
    d_prime = c.d_prime;
    d_prime_position = c.d_prime_position;
    d_prime_next = c.d_prime_next;
    d_gentype = c.d_gentype;
    d_parameter = c.d_parameter;
    d_multiplier = c.d_multiplier;
  }

  return *this;
}
  
// Destructor
LCG::~LCG()
{
  free_rng();
}

// Initialize the generator
/*! \details Gives back one generator (node gn) with updated spawning info.
 * Should be called tn times with a different value of gn in [0,tn) each call.
 */
int LCG::init_rng( int gn, int tg, int s, int m )
{
  int i;

  // Check if total_gen (tn) is valid 
  if (tg <= 0) 
  {
    std::cerr << "WARNING - init_rng: total_gen <= 0. "
	      << "Default value of 1 used for total_gen."
	      << std::endl;
    tg = 1;
  }

  if (gn < 0 || gn >= tg) 
  {
    std::cerr << "ERROR - init_rng: gennum " << gn << " out of range "
	      << "[0," << tg << ")" << std::endl;
    return 0;
  }

  // Only 31 LSB of seed are considered
  s &= 0x7fffffff;
  
  // Check if multiplier index (m) is valid
  if (m < 0 || m >= 7) 
  {
    std::cerr << "WARNING - init_rng: multiplier " << m << " not valid. "
	      << "Using default parameter (m=0)" << std::endl;
    m = 0;
  }

  // Set the multiplier
  if(d_multiplier == 0)
    d_multiplier = LCG::mults[m];

  // Set the generator type
  d_rng_type = LCG_TYPE;
  
  // Set the generator type description
  d_gentype = GENTYPE;
  
  // Set the initial seed
  d_init_seed = s;

  // Set the prime that this generator will use
  Primes32::getprime_32( d_prime, gn );

  // Set the prime position
  d_prime_position = gn;
  
  // Set the next prime
  d_prime_next = tg;

  // Set the parameter used by this generator
  d_parameter = m;
  
  // Set the seed
  d_seed = INIT_SEED;
  d_seed ^= ((unsigned long long) s)<<16;
  if (d_prime == 0) 
    d_seed |= 1;

  // Set the multiplier used by this generator
  d_multiplier = LCG::mults[m];

  // Initialize this generator
  for(i=0; i<LCGRUNUP*d_prime_position; i++)
    get_rn_dbl();

  // Increment the number of LCG streams being used
  LCG::increment_number_of_streams();

  return 1;
}
  
// Return a random int
int LCG::get_rn_int()
{
  multiply();
  
  return ((unsigned long long) d_seed) >> 17;
}

// Return a random float in interval [0,1)
float LCG::get_rn_flt()
{
  return (float) get_rn_dbl();
}

// Return a random double in interval [0,1)
double LCG::get_rn_dbl()
{
  double temp[1];
  unsigned long long *ltemp;
    
  temp[0] = 0.0;
  multiply();
  
#if defined(CONVEX) || defined(GENERIC)
  ltemp = (unsigned long long *) temp;
  *ltemp = (d_seed<<4) | MSB_SET;
  
  return temp[0] - (double) 1.0;
#else
  return d_seed*3.5527136788005008e-15;
#endif
}

// Spawn new generators
int LCG::spawn_rng( int nspawned, Sprng ***newgens )
{
  LCG ** genptr;
  int i, j;
  
  // Check if nspawned is valid
  if (nspawned <= 0) 
  {
    std::cerr << "WARNING - spawn_rng: nspawned <=0. "
	      << "Default value of 1 used for nspawned." << std::endl;
    nspawned = 1;
  }

  genptr = new LCG *[nspawned];

  if(genptr == NULL)
  {
    *newgens = NULL;
    return 0;
  }
  
  for(i=0; i<nspawned; i++)
  {
    genptr[i] = new LCG;

    if(genptr[i] == NULL)
    {
      nspawned = i;
      break;
    }
    
    // Set the initial seed
    genptr[i]->d_init_seed = d_init_seed;

    // Set the prime position
    genptr[i]->d_prime_position = d_prime_position + d_prime_next*(i+1);

    if(genptr[i]->d_prime_position > Primes32::max_prime_offset)
    {
      std::cerr << "WARNING - spawn_rng: gennnum: " 
		<< genptr[i]->d_prime_position 
		<< " > maximum number of independent streams: "
		<< LCG::max_streams << std::endl
		<< "Independence of streams cannot be  guarnateed."
		<< std::endl;
    
      genptr[i]->d_prime_position %= Primes32::max_prime_offset;
    }
    
    // Set the next prime
    genptr[i]->d_prime_next = (nspawned+1)*d_prime_next;

    // Set the prime that will be used
    Primes32::getprime_32( genptr[i]->d_prime, genptr[i]->d_prime_position );

    // Set the multiplier
    genptr[i]->d_multiplier = d_multiplier;

    // Set the parameter
    genptr[i]->d_parameter = d_parameter;

    // Set the generator description
    genptr[i]->d_gentype = d_gentype;

    // Set the generator type
    genptr[i]->d_rng_type = d_rng_type;
    
    // Initialize the seed
    genptr[i]->d_seed = INIT_SEED;	
    genptr[i]->d_seed ^= ((unsigned long long)d_init_seed)<<16;	

    if (genptr[i]->d_prime == 0) 
      genptr[i]->d_seed |= 1;

    // Advance lcg 10^6 steps from initial seed
    if(genptr[i]->d_prime_position > Primes32::max_prime_offset)
      genptr[i]->advance_seed(); 

    // Initialize the generator
    for(j=0; j<LCGRUNUP*(genptr[i]->d_prime_position); j++)
      genptr[i]->get_rn_dbl();
  }

  d_prime_next = (nspawned+1)*d_prime_next;

  // Increment the number of generators
  LCG::increment_number_of_streams( nspawned );

  // Set the input array to the array of LCGs that was created
  *newgens = (Sprng **) genptr;
  
  return nspawned;
}

int LCG::spawn_rng( int nspawned, 
		    std::vector<boost::shared_ptr<Sprng> > &newgens )
{
  Sprng **tmp_newgens;

  // The number of generators spawned
  int val = spawn_rng( nspawned, &tmp_newgens );

  newgens.resize( val );

  for( int i = 0; i < val; ++i )
    newgens[i].reset( tmp_newgens[i] );

  delete[] tmp_newgens;
  
  return val;
}

// Return the generator seed
int LCG::get_seed_rng()
{
  return d_init_seed;
}

// Free the memory allocated to this generator
int LCG::free_rng()
{
  assert( this != NULL );
  
  LCG::decrement_number_of_streams();
  
  return LCG::num_generators;
}

// Pack this generator into a character buffer
/*! \details A null character buffer should be passed to this function. The
 * new command will be used to allocate the correct amount of memory. Just
 * remember to call delete[] on the buffer when you are done using it.
 */
int LCG::pack_rng( char **buffer )
{
  std::string tmp_buffer;
  
  int val = pack_rng( tmp_buffer );
  
  *buffer = new char[tmp_buffer.size()];

  tmp_buffer.copy( *buffer, tmp_buffer.size() );

  return val;
}

int LCG::pack_rng( std::string &buffer ) const
{
  // Clear the buffer
  buffer.clear();
  
  // All member data will be stored in a sub buffer and appended to main buffer
  std::string partial_buffer;

  // Store the generator type
  store_value( generatorTypeToInt( d_rng_type ), partial_buffer );
  buffer += partial_buffer;

  // Store the seed
  store_value( d_seed, partial_buffer );
  buffer += partial_buffer;

  // Store the initial seed
  store_value( d_init_seed, partial_buffer );
  buffer += partial_buffer;
  
  // Store this generators prime value
  store_value( d_prime, partial_buffer );
  buffer += partial_buffer;

  // Store the prime index
  store_value( d_prime_position, partial_buffer );
  buffer += partial_buffer;
  
  // Store the next prime
  store_value( d_prime_next, partial_buffer );
  buffer += partial_buffer;

  // Store the parameter
  store_value( d_parameter, partial_buffer );
  buffer += partial_buffer;
  
  // Store the multiplier
  store_value( d_multiplier, partial_buffer );
  buffer += partial_buffer;

  return buffer.size();
}

// Print this generators info
int LCG::print_rng()
{
  print( std::cout );

  return 1;
}

void LCG::print( std::ostream &os ) const
{
  os << d_gentype << ":" << std::endl
     << "  seed = " << d_init_seed << "," << std::endl
     << "  stream_number = " << d_prime_position << "," << std::endl
     << "  parameter = " << d_parameter << std::endl;
}

// Unpack this generator from a character buffer
int LCG::unpack_rng( char *packed )
{
  std::size_t nbytes;
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
  
  // Load the seed
  nbytes = sizeof( d_seed );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_seed );
  packed += nbytes;
  
  // Load the initial seed
  nbytes = sizeof( d_init_seed );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_init_seed );
  packed += nbytes;

  // Load the prime for this generator
  nbytes = sizeof( d_prime );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_prime );
  packed += nbytes;

  // Load the index of the prime position
  nbytes = sizeof( d_prime_position );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_prime_position );
  packed += nbytes;

  // Load the next prime
  nbytes = sizeof( d_prime_next );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_prime_next );
  packed += nbytes;
  
  // Load the parameter
  nbytes = sizeof( d_parameter );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_parameter );
  packed += nbytes;

  // Load the multiplier
  nbytes = sizeof( d_multiplier );
  sub_string.assign( packed, nbytes );
  load_value( sub_string, d_multiplier );
  nbytes += nbytes;

  if(d_parameter < 0 || d_parameter >= 7)
  {
    std::cerr << "ERROR: Unpacked parameters not acceptable." << std::endl;
    
    return 0;
  }
  
  d_multiplier = LCG::mults[d_parameter];
  
  // Increment the number of streams
  LCG::increment_number_of_streams();

  return 1;
}

int LCG::unpack_rng( const std::string &packed )
{
  char* packed_string = const_cast<char*>( packed.c_str() );

  return unpack_rng( packed_string );
}

// Get the number of open streams
int LCG::get_number_of_streams()
{
  return LCG::num_generators;
}

// Multiply the seed by the multiplier
void LCG::multiply()
{
  d_seed *= d_multiplier;
  d_seed += d_prime;
  d_seed &= LSB48;
}

// Advance the seed
void LCG::advance_seed()
{
  int i, found;
  unsigned long long an, pmult;
  
  for(i=0, found=0; i<7; i++)
  {
    if (d_multiplier == LCG::mults[i])
    {
      found = 1;
      break;
    }
  }
  
  if(found == 0)
  {
    std::cerr << "WARNING: advance_seed: multiplier not acceptable."
	      << std::endl;
    
    return ;
  }

  // a^n, n = 10^6 and pmult =  (a^n-1)/(a-1), n = 10^6 
  switch(i)
  {
  case 0 :
    an = 0xdadf0ac00001LL;
    pmult = 0xa42c22700000LL;
    break;
  case 1 :
    an = 0xfefd7a400001LL;
    pmult = 0xfa858cb00000LL;
    break;
  case 2 :
    an = 0x6417b5c00001LL;
    pmult = 0xd0c4ef00000LL;
    break;
  case 3 :
    an = 0xcf9f72c00001LL;
    pmult = 0x11bdbe700000LL;
    break;
  case 4 :
    an = 0xbdf07b400001LL;
    pmult = 0xc3cc8e300000LL;
    break;
  case 5 :
    an = 0xf33747c00001LL;
    pmult = 0xb0f0e9f00000LL;
    break;
  case 6 :
    an = 0xcbe632c00001LL;
    pmult = 0x6407de700000LL;
    break;
  default:
    std::cerr << "WARNING: advance_seed parameters for multiplier " 
	      << i << " not available." << std::endl;
    
    return;
  }
  
  d_seed = d_seed*an + pmult*d_prime;
  d_seed &= LSB48;
}

// Increment the number of open streams
void LCG::increment_number_of_streams( int num )
{
  LCG::num_generators += num;
  
  if( LCG::num_generators >= LCG::max_streams )
    std::cerr << "WARNING: " << LCG::num_generators << " open LCG streams. "
	      << "Independence can only be guaranteed with " 
	      << LCG::max_streams << " LCG streams."
	      << std::endl;
}

// Decrement the number of open streams
void LCG::decrement_number_of_streams( int num )
{
  LCG::num_generators -= num;
}

} // end namespace sprng

//---------------------------------------------------------------------------//
// end LCG.cpp
//---------------------------------------------------------------------------//



