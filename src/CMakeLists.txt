SET(HEADERS
  SeedHelpers.hpp
  SeedHelpers_def.hpp
  MPIHelpers.hpp
  MPIHelpers_def.hpp
  GeneratorType.hpp
  GeneratorType_def.hpp
  SprngFactory.hpp
  SprngFactory_def.hpp
  SimpleSprng.hpp
  SimpleSprng_def.hpp
  Sprng.hpp
  LFG.hpp
  LCG.hpp
  LCG64.hpp
  ${HEADERS}
  )

# SET(SPRNG_LIBRARY_FILES
#   SimpleSprng.cpp
#   LFG.cpp
#   LCG.cpp
#   LCG64.cpp 
# )

SET(SPRNG_LIBRARY_NAME sprng)

IF( CMAKE_VERBOSE_CONFIGURE )
  MESSAGE("HEADERS = ${HEADERS}")
  MESSAGE("SPRNG_LIBRARY_FILES = ${SPRNG_LIBRARY_FILES}")
ENDIF()

# ADD_LIBRARY(${SPRNG_LIBRARY_NAME ${SPRNG_LIBRARY_FILES})

# INSTALL(LIBRARIES ${SPRNG_LIBRARY_NAME}
#   RUNTIME DESTINATION ${CMAKE_INSTALL_PREFIX}/lib)
INSTALL(FILES ${HEADERS}
   DESTINATION ${CMAKE_INSTALL_PREFIX}/include
   PERMISSIONS OWNER_READ GROUP_READ WORLD_READ)
