SET(SRC ${CMAKE_SOURCE_DIR}/src)

ADD_EXECUTABLE(tstStore	
  tstStore.cpp)
ADD_TEST(Store_test tstStore)

ADD_EXECUTABLE(tstMPIHelpers
  tstMPIHelpers.cpp
  ${SRC}/MPIHelpers.cpp)
IF(${${PROJECT_NAME}_ENABLE_MPI})
  TARGET_LINK_LIBRARIES(tstMPIHelpers ${MPI_CXX_LIBRARIES})
  ADD_TEST(MPIHelpers_test_1_proc
    mpiexec -n 1 tstMPIHelpers)
  ADD_TEST(MPIHelpers_test_2_proc
    mpiexec -n 2 tstMPIHelpers)
  ADD_TEST(MPIHelpers_test_3_proc
    mpiexec -n 3 tstMPIHelpers)
  ADD_TEST(MPIHelpers_test_4_proc
    mpiexec -n 4 tstMPIHelpers)
ELSE()
  ADD_TEST(MPIHelpers_test tstMPIHelpers)
ENDIF()

ADD_EXECUTABLE(tstSeedHelpers
  tstSeedHelpers.cpp
  ${SRC}/SeedHelpers.cpp)
ADD_TEST(SeedHelpers_test tstSeedHelpers)
IF(${${PROJECT_NAME}_ENABLE_MPI})
  TARGET_LINK_LIBRARIES(tstSeedHelpers ${MPI_CXX_LIBRARIES})
  ADD_TEST(SeedHelpers_test_2_proc
    mpiexec -n 2 tstSeedHelpers)
ENDIF()

ADD_EXECUTABLE(tstPrimes32
  tstPrimes32.cpp
  ${SRC}/Primes32.cpp)
ADD_TEST(Primes32_test tstPrimes32)

ADD_EXECUTABLE(tstPrimes64
  tstPrimes64.cpp
  ${SRC}/Primes64.cpp)
ADD_TEST(Primes64_test tstPrimes64)

ADD_EXECUTABLE(tstGeneratorType
  tstGeneratorType.cpp
  ${SRC}/GeneratorType.cpp)
ADD_TEST(GeneratorType_test tstGeneratorType)

ADD_EXECUTABLE(tstLFG
  tstLFG.cpp
  ${SRC}/LFG.cpp
  ${SRC}/GeneratorType.cpp)
ADD_TEST(LFG_test tstLFG)