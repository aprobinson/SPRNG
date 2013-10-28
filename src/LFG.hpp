// Copyright 1996 September 3, United States Government as Represented   
// by the Director, National Security Agency. All rights reserved.       
//                                                                       
// Disclaimer 1: NCSA expressly disclaims any and all warranties, expressed
// or implied, concerning the enclosed software.  The intent in sharing  
// this software is to promote the productive interchange of ideas       
// throughout the research community. All software is furnished on an    
// "as is" basis. No further updates to this software should be          
// expected. Although this may occur, no commitment exists. The authors  
// certainly invite your comments as well as the reporting of any bugs.  
// NCSA cannot commit that any or all bugs will be fixed.                
//                                                                       
// Disclaimer 2: CCS expressly disclaims any and all warranties, expressed 
// or implied, concerning the enclosed software. This software was       
// developed at CCS for use in internal research. The intent in sharing  
// this software is to promote the productive interchange of ideas       
// throughout the research community. All software is furnished on an    
// "as is" basis. No further updates to this software should be          
// expected. Although this may occur, no commitment exists. The authors  
// certainly invite your comments as well as the reporting of any bugs.  
// CCS cannot commit that any or all bugs will be fixed.                 
//---------------------------------------------------------------------------//
//!
//! \file    LFG.hpp
//! \author  Steven A. Cuccaro and Daniel V. Pryor, Askov Srinivasan 
//!          (IDA/Center for Computing Sciences (CCS), cuccaro@super.org, 
//!           pryor@super.org, ?)
//! \brief   Parallel modified additive lagged fibonacci generator declaration
//! \details Modified by J. Ren (Florida State University, ren@csit.fsu.edu ).
//!          This version has been modified to use two integer-based additive
//!          lagged-Fibonacci generators to produce integer, float and double
//!          values. The lagged-Fibonacci generators each have 31 bits of 
//!          precision (after the bit fixed by the canonical form of the  
//!          generator is removed), 31-bit values are generated by XORing     
//!          the values after one has been shifted left one bit. The floating 
//!          point value is formed by dividing the integer by 1.e+32 (the     
//!          lsb's will be dropped from the mantissa to make room for the     
//!          exponent), and two of these integer values in sequence are used  
//!          to get the necessary precision for the double value. This method 
//!          has the advantage that the generators pass fairly strict 
//!          randomness tests, including the Birthday Spacings test that 
//!          additive lagged-Fibonacci generators are well known to fail. The 
//!          disadvantage is the additional time needed to do the division 
//!          explicitly, which was avoided in previous versions. (As the 
//!          division is by powers of 2, the user might well consider making 
//!          machine-specific versions of this code to insert the bits into 
//!          the appropriate places and avoid the problem entirely.) 
//! \note    This file has been rewritten by Alex Robinson (University of
//!          Wisconsin-Madison, aprobinson@wisc.edu). Only minor interface
//!          changes have been made. All of the underlying algorithms are still
//!          the same.
//!
//---------------------------------------------------------------------------//

#ifndef LFG_HPP
#define LFG_HPP

// Std Lib Includes
#include <string>

// Boost Includes
#include <boost/shared_array.hpp>

// SPRNG Includes
#include "Sprng.hpp"
#include "GeneratorType.hpp"

namespace sprng{

//! A parallel modified additive lagged fibonacci generator
class LFG : public Sprng
{

public:

  //! Default constructor
  LFG();

  //! Copy constructor
  LFG( const LFG &c );

  //! Assignment operator
  LFG& operator=( const LFG &c );

  //! Destructor
  virtual ~LFG();

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
  int pack_rng( std::string &buffer );

  //! Print this generators info
  int print_rng();

  //! Unpack this generator from a character buffer
  int unpack_rng( char *packed );
  int unpack_rng( const std::string &packed );
  //@}

private:

  //! Simple struct for defining valid seeds
  struct vstruct{
    int L;
    int K;
    int LSBS; // Number of least significant bits that are 1
    int first; // The first seed whose LSB is 1
  };

  //! Get the bit count
  static int bitcnt( int x );

  //! Advance the registers
  static int advance_reg( boost::shared_array<int> &reg_fill );

  //! Get the register fill
  static int get_fill( boost::shared_array<unsigned> &n,
		       boost::shared_array<unsigned> &r,
		       int param_local,
		       unsigned seed_local );

  //! Update index for next spawning
  static void si_double( boost::shared_array<unsigned> &a, 
			 boost::shared_array<unsigned> &b, 
			 int length );
  
  //! Initialize the streams
  static LFG** initialize( GeneratorType rng_type_local, 
			   int ngen_local,
			   int param_local,
			   int seed_local,
			   boost::shared_array<unsigned> &nstart_local,
			   unsigned initseed_local );

  //! Increment the number of open streams
  static void increment_number_of_streams( int num = 1 );

  //! Decrement the number of open streams
  static void decrement_number_of_streams( int num = 1 );

  // Max number of LFG streams possible
  static const int max_streams = 0x7fffffff;

  // Number of streams currently open
  static int num_generators;

  // Global generator seed
  static int global_seed;

  // Global L value
  static int global_lval;
  
  // Global K value
  static int global_kval;

  // Array of valid seeds
  static const vstruct valid[11];

  // Generator type
  GeneratorType d_rng_type;

  // A string that describes the generator type
  std::string d_gentype;
  
  // Sets the next branch seed
  boost::shared_array<unsigned> d_si;

  // Even generator
  boost::shared_array<unsigned> d_r0;

  // Odd generator
  boost::shared_array<unsigned> d_r1;

  // Stream number of this generator
  int d_stream_number;

  // Integer pointer into fill
  int d_hptr;

  // Generator seed
  int d_seed;

  // Initial seed
  int d_init_seed;

  // L value
  int d_lval;

  // K value
  int d_kval;

  // A parameter for the generator
  int d_param;

};

} // end namespace sprng

#endif // end LFG_HPP

//---------------------------------------------------------------------------//
// end LFG.hpp
//---------------------------------------------------------------------------//
