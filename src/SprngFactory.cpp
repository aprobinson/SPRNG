//---------------------------------------------------------------------------//
//!
//! \file    SprngFactory.cpp
//! \author  Alex Robinson
//! \brief   Definition of a Simple struct to aid the Sprng class 
//!
//---------------------------------------------------------------------------//

// SPRNG Includes
#include "SprngFactory.hpp"
#include "LFG.hpp"
#include "LCG.hpp"
#include "LCG64.hpp"
#include "CMRG.hpp"
#include "MLFG.hpp"
#include "PMLCG.hpp"

namespace sprng{

// Constructor
SprngFactory::SprngFactory( const GeneratorType gtype )
  : d_gen_type( gtype )
{ /* ... */ }

// Generator creation method
SprngFactory::SprngPtr SprngFactory::create() const
{
  switch( d_gen_type )
  {
  case LFG_TYPE: return new LFG;
  case LCG_TYPE: return new LCG;
  case LCG64_TYPE: return new LCG64;
  case CMRG_TYPE: return new CMRG;
  case MLFG_TYPE: return new MLFG;
  case PMLCG_TYPE: return new PMLCG;
  }
}

} // end namespace sprng

//---------------------------------------------------------------------------//
// end SprngFactory.cpp
//---------------------------------------------------------------------------//
