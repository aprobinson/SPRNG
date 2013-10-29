SET(HEADERS
  SeedHelpers.hpp
  MPIHelpers.hpp
  GeneratorType.hpp
  Store.hpp
  Store_def.hpp
  SprngFactory.hpp
  SimpleSprng.hpp
  Sprng.hpp
  LFG.hpp
  LCG.hpp
  LCG64.hpp
  ${HEADERS}
  )

SET(EXECUTABLES
  timesprng
  ${TARGETS}
  )

ADD_EXECUTABLE(timesprng
  timing.cpp
  GeneratorType.cpp
  Primes32.cpp
  Primes64.cpp
  LFG.cpp
  LCG.cpp
  LCG64.cpp
  )

ADD_LIBRARY(sprng
  timing.cpp
  GeneratorType.cpp
  Primes32.cpp
  Primes64.cpp
  LFG.cpp
  LCG.cpp
  LCG64.cpp
  )

IF( CMAKE_VERBOSE_CONFIGURE )
  MESSAGE("HEADERS = ${HEADERS}")
  MESSAGE("SPRNG_LIBRARY_FILES = ${SPRNG_LIBRARY_FILES}")
ENDIF()

INSTALL(TARGETS ${EXECUTABLES}
  RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
INSTALL(TARGETS sprng
  ARCHIVE DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
INSTALL(FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_PREFIX}/include
   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
