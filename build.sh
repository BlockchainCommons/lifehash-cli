#!/bin/zsh

set +e

source tasks.sh

build_deps
configure
build
