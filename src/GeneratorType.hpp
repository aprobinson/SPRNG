//---------------------------------------------------------------------------//
//!
//! \file   GeneratorType.hpp
//! \author Alex Robinson
//! \brief  Enumeration for the different types of generators
//!
//---------------------------------------------------------------------------//

#ifndef GENERATOR_TYPE_HPP
#define GENERATOR_TYPE_HPP

namespace sprng{

//! Generator Type enumeration
enum GeneratorType{
  LFG_TYPE,
  LCG_TYPE,
  LCG64_TYPE// ,
  // CMRG_TYPE,
  // MLFG_TYPE,
  // PMLCG_TYPE
};

//! Convert a GeneratorType to an int
int generatorTypeToInt( const GeneratorType gtype );

//! Convert an int to a GeneratorType
GeneratorType intToGeneratorType( const int gtype );

} // end SPRNG

#endif // end GENERATOR_TYPE_HPP

//---------------------------------------------------------------------------//
// end GeneratorType.hpp
//---------------------------------------------------------------------------//
