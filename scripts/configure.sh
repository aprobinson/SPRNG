#!/bin/bash
##---------------------------------------------------------------------------##
## CONFIGURE SPRNG with CMAKE
##---------------------------------------------------------------------------##

EXTRA_ARGS=$@
SPRNG_PATH=$HOME/research/rng/SPRNG/
PREFIX_PATH=$HOME/research/rng/

##---------------------------------------------------------------------------##

rm -rf CMakeCache.txt

##---------------------------------------------------------------------------##

cmake \
    -D CMAKE_INSTALL_PREFIX:PATH=$PWD/../ \
    -D CMAKE_BUILD_TYPE:STRING=DEBUG \
    -D CMAKE_VERBOSE_CONFIGURE:BOOL=ON \
    -D CMAKE_VERBOSE_MAKEFILE:BOOL=ON \
    -D SPRNG_ENABLE_MPI:BOOL=ON \
    $EXTRA_ARGS \
    $SPRNG_PATH