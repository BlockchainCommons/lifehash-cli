#!/bin/bash

PROJ_ROOT=${PWD}
SRC_ROOT=${PROJ_ROOT}/src
DEPS_ROOT=${PROJ_ROOT}/deps
BUILD_ROOT=${PROJ_ROOT}/sysroot

TARGET_OPT=
# TARGET_OPT="-target apple-darwin-x86_64"
# TARGET_OPT="-target apple-darwin-arm64"

# DEBUG_OPT=--debug
# OPTIMIZE_OPT=-O0

DEBUG_OPT=
OPTIMIZE_OPT=-O3

export CFLAGS="${TARGET_OPT} ${DEBUG_OPT} ${OPTIMIZE_OPT} -fdiagnostics-absolute-paths"
export CXXFLAGS="${TARGET_OPT} ${DEBUG_OPT} ${OPTIMIZE_OPT} -std=c++17"
export CPPFLAGS="-I${BUILD_ROOT}/include"
export LDFLAGS="${TARGET_OPT} -L${BUILD_ROOT}/lib"

build_lifehash()
(
    cd deps/bc-lifehash
    
    ./build.sh
    make install
)

build_zlib()
(
    cd deps/zlib

    ./configure --static --prefix=${BUILD_ROOT}

    make clean
    make install
)

build_libpng()
(
    cd deps/libpng

    export ZLIBLIB=${BUILD_ROOT}/lib
    export ZLIBINC=${BUILD_ROOT}/include
    export CPPFLAGS="${CPPFLAGS} -I$ZLIBINC"
    export LDFLAGS="${LDFLAGS} -L$ZLIBLIB"
    export LD_LIBRARY_PATH="$ZLIBLIB:$LD_LIBRARY_PATH"
    ./autogen.sh
    ./configure --enable-shared=no --prefix=${BUILD_ROOT}

    make clean
    make install

    echo ${PWD}
)

build_deps()
(
    git submodule update --init --recursive

    export PREFIX=${BUILD_ROOT}
    
    build_lifehash
    build_zlib
    build_libpng
)

clean()
(
    make clean
)

configure()
(
    ./configure
    clean
)

build()
(
    export CXXFLAGS="${CXXFLAGS}"
    export CPPFLAGS="${CPPFLAGS} -I${BUILD_ROOT}/include"
    export LDFLAGS="${LDFLAGS} -L${BUILD_ROOT}/lib -lstdc++"
    make lifehash
    #dsymutil src/lifehash
)

install()
(
    make install
)

uninstall()
(
    make uninstall
)
