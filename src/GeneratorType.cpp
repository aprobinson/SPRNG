//---------------------------------------------------------------------------//
//!
//! \file   GeneratorType.cpp
//! \author Alex Robinson
//! \brief  Enumeration for the different types of generators
//!
//---------------------------------------------------------------------------//

// Std Lib Includes
#include <assert.h>

// SPRNG Includes
#include "GeneratorType.hpp"

namespace sprng{

// Convert a GeneratorType to an int
int generatorTypeToInt( const GeneratorType gtype )
{
  switch( gtype )
  {
  case LFG_TYPE: return 0;
  case LCG_TYPE: return 1;
  case LCG64_TYPE: return 2;
  // case CMRG_TYPE: return 3;
  // case MLFG_TYPE: return 4;
  // case PMLCG_TYPE: return 5;
  }
}

// Convert an int to a GeneratorType
GeneratorType intToGeneratorType( const int gtype )
{
  assert( gtype >= 0 && gtype <= 5 );

  switch( gtype )
  {
  case 0: return LFG_TYPE;
  case 1: return LCG_TYPE;
  case 2: return LCG64_TYPE;
  // case 3: return CMRG_TYPE;
  // case 4: return MLFG_TYPE;
  // case 5: return PMLCG_TYPE;
  }
}

} // end namespace sprng

//---------------------------------------------------------------------------//
// end GeneratorType.cpp
//---------------------------------------------------------------------------//
