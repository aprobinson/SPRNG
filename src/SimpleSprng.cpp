//---------------------------------------------------------------------------//
//!
//! \file    SimpleSprng.hpp
//! \author  ?
//! \brief   Definition of a simple struct to aid the Sprng class 
//! \details This file originally appeared in the sprng4 library. It has been
//!          modified by Alex Robinson (aprobinson@wisc.edu) to better support
//!          the needs of FACEMC.
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <ctime>

// Sprng Includes
#include "SimpleSprng.hpp"
#include "SprngFactory.hpp"
#include "SeedHelpers.hpp"
#include "MPIHelpers.hpp"

using namespace std;

namespace sprng{

// Generator initialization
int SimpleSprng::init_rng_simple( int seed, int mult, int gtype )
{
  return SimpleSprng::init_rng_simple( seed, 
				       mult, 
				       intToGeneratorType( gtype ) );
}

int SimpleSprng::init_rng_simple( int seed, int mult, GeneratorType gtype )
{
  int myid=0, nprocs=1;
  int val;
  Sprng * temp;
  
  SprngFactory generator_factory( gtype );
  temp = generator_factory.create();
  
  val = temp->init_rng(myid,nprocs,seed,mult);

  if( val != 0 )
    SimpleSprng::default_generator.reset( temp );

  return val;
}

// Generator initialization with mpi
int SimpleSprng::init_rng_simple_mpi( int seed, int mult, int gtype )
{
  return SimpleSprng::init_rng_simple_mpi( seed,
					   mult,
					   intToGeneratorType( gtype ) );
}

int SimpleSprng::init_rng_simple_mpi( int seed, int mult, GeneratorType gtype)
{
  int myid=0, nprocs=1;
  int val;
  Sprng *temp;

  get_proc_inf_mpi(&myid,&nprocs);

  SprngFactory generator_factory( gtype );
  temp = generator_factory.create();

  val = temp->init_rng(myid,nprocs,seed,mult);

  if( val != 0 )
    SimpleSprng::default_generator.reset( tmp );

  return val;
}

// Return a random int
int SimpleSprng::get_rn_int_simple()
{
  if(!SimpleSprng::default_genenerator) {
    if(SimpleSprng::init_rng_simple(0,0) == 0) {
      return static_cast<int>(-1.0);
    }
  }

  return SimpleSprng::default_generator->get_rn_int();
}
  
// Return a random int with mpi
int SimpleSprng::get_rn_int_simple_mpi()
{
  if(!SimpleSprng::default_generator)
    if(SimpleSprng::init_rng_simple_mpi(0,0) == 0)
      return static_cast<int>(-1.0);
  
  return SimpleSprng::default_generator->get_rn_int();
}

// Return a random float in interval [0,1)
int SimpleSprng::get_rn_flt_simple()
{
  if(!SimpleSprng::default_generator)
    if(SimpleSprng::init_rng_simple(0,0) == 0)
      return -1.0;
  
  return SimpleSprng::default_generator->get_rn_flt();
}

// Return a random float in interval [0,1) with mpi
int SimpleSprng::get_rn_flt_simple_mpi()
{
  if(!SimpleSprng::default_generator)
    if(SimpleSprng::init_rng_simple_mpi(0,0) == 0)
      return -1.0;
  
  return SimpleSprng::default_generator->get_rn_flt();
}

// Return a random double in interval [0,1)
int SimpleSprng::get_rn_dbl_simple()
{
  if(!SimpleSprng::default_generator)
    if(SimpleSprng::init_rng_simple(0,0) == 0)
      return -1.0;
  
  return SimpleSprng::default_generator->get_rn_dbl();
}

// Return a random double in interval [0,1) with mpi
int SimpleSprng::get_rn_dbl_simple_mpi()
{
  if(!SimpleSprng::default_generator)
    if(SimpleSprng::init_rng_simple_mpi(0,0) == 0)
      return -1.0;

  return SimpleSprng::default_generator->get_rn_dbl();
}

// Pack a generator into a character buffer
int SimpleSprng::pack_rng_simple( char **buffer )
{
  std::string temp_buffer;
  int val;
  
  val = SimpleSprng::default_generator->pack_rng( temp_buffer );

  temp_buffer.copy( *buffer, temp_buffer.size() );

  return val;
}

int SimpleSprng::pack_rng_simple( std::string &buffer )
{
  if(!SimpleSprng::default_generator)
    return 0;
  
  return SimpleSpring::default_generator->pack_rng( buffer );
}

// Unpack a generator from a character buffer
int SimpleSprng::unpack_rng_simple( char *packed,
				    int gtype )
{
  std::string temp_packed( packed );
  
  return SimpleSprng::unpack_rng_simple( packed,
					 intToGeneratorType( gtype ) );
  
}

int SimpleSprng::unpack_rng_simple( const std::string &packed, 
				    const GeneratorType gtype )
{
  Sprng * temp;
  int val;
  
  SprngFactory generator_factory( gtype );
  temp = generator_factory.create();
  
  val = temp->unpack_rng( packed );
  
  if( val != 0 )
    SimpleSprng::default_generator.reset( temp );
      
  return val;
}
  
// Print the generator info
int SimpleSprng::print_rng_simple()
{
  if(!SimpleSprng::default_generator) {
    fprintf(stderr,"WARNING: No generator initialized so far\n");
    return 0;
  }
  
  return SimpleSprng::default_generator->print_rng();
}

// Generator initialization
/*! \ingroup simple_sprng_interface
 */
inline int init_rng_simple( int seed, int mult, int gtype )
{ return SimpleSprng::init_rng_simple( seed, mult, gtype ); }

inline int init_rng_simple( int seed, int mult, GeneratorType gtype )
{ return SimpleSprng::init_rng_simple( seed, mult, gtype ); }

// Generator initialization with mpi
/*! \ingroup simple_sprng_interface
 */
inline int init_rng_simple_mpi( int seed, int mult, int gtype )
{ return SimpleSprng::init_rng_simple_mpi( seed, mult, gtype ); }

inline int init_rng_simple_mpi( int seed, int mult, GeneratorType gtype )
{ return SimpleSprng::init_rng_simple_mpi( seed, mult, gtype ); }

// Return a random int
/*! \ingroup simple_sprng_interface
 */
inline int get_rn_int_simple()
{ return SimpleSprng::get_rn_int_simple(); }
  
// Return a random int with mpi
/*! \ingroup simple_sprng_interface
 */
inline int get_rn_int_simple_mpi()
{ return SimpleSprng::get_rn_int_simple_mpi(); }

// Return a random float in interval [0,1)
/*! \ingroup simple_sprng_interface
 */
inline int get_rn_flt_simple()
{ return SimpleSprng::get_rn_flt_simple(); }

// Return a random float in interval [0,1) with mpi
/*! \ingroup simple_sprng_interface
 */
inline int get_rn_flt_simple_mpi()
{ return SimpleSprng::get_rn_flt_simple_mpi(); }

// Return a random double in interval [0,1)
/*! \ingroup simple_sprng_interface
 */
inline int get_rn_dbl_simple()
{ return SimpleSprng::get_rn_dbl_simple(); }

// Return a random double in interval [0,1) with mpi
/*! \ingroup simple_sprng_interface
 */
inline int get_rn_dbl_simple_mpi()
{ return SimpleSprng::get_rn_dbl_simple_mpi(); }

// Pack a generator into a character buffer
/*! \ingroup simple_sprng_interface
 */
inline int pack_rng_simple( char **buffer )
{ return SimpleSprng::pack_rng_simple( buffer ); }

inline int pack_rng_simple( std::string &buffer )
{ return SimpleSprng::pack_rng_simple( buffer ); }

// Unpack a generator from a character buffer
/*! \ingroup simple_sprng_interface
 */
inline int unpack_rng_simple( char* packed, in gtype )
{ return SimpleSprng::unpack_rng_simple( packed, gtype ); }

inline int unpack_rng_simple( const std::string &packed,  
			       const GeneratorType gtype )
{ return SimpleSprng::unpack_rng_simple( packed, gtype ); }
  
// Print the generator info
/*! \ingroup simple_sprng_interface
 */
inline int print_rng_simple()
{ return SimpleSprng::print_rng_simple(); }

} // end namespace sprng

//---------------------------------------------------------------------------//
// end SimpleSprng.cpp
//---------------------------------------------------------------------------//
