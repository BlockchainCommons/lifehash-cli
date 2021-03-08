#!/bin/zsh

set +e

git submodule update --init

DEPS_ROOT=${PWD}/deps
BUILD_ROOT=${DEPS_ROOT}/build
mkdir -p ${BUILD_ROOT}

# TARGET_OPT="-target apple-darwin-x86_64"
# TARGET_OPT="-target apple-darwin-arm64"

export CFLAGS="${TARGET_OPT} -O3"
export LDFLAGS="${TARGET_OPT}"
export PREFIX=${BUILD_ROOT}

build_lifehash()
(
    cd deps/bc-lifehash

    export prefix=${PREFIX}

    ./build.sh
    make install
)

build_zlib()
(
    cd deps/zlib

    export prefix=${PREFIX}
    ./configure --static

    make clean
    make install
)

build_libpng()
(
    cd deps/libpng

    export ZLIBLIB=${PREFIX}/lib
    export ZLIBINC=${PREFIX}/include
    export CPPFLAGS="${CPPFLAGS} -I$ZLIBINC"
    export LDFLAGS="-L$ZLIBLIB"
    export LD_LIBRARY_PATH="$ZLIBLIB:$LD_LIBRARY_PATH"
    ./autogen.sh
    ./configure --enable-shared=no --prefix=${PREFIX}

    make clean
    make install

    echo ${PWD}
)

build_deps()
(
    build_lifehash
    build_zlib
    build_libpng
)
