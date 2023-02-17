#!/bin/bash

set +e

source tasks.sh

build_deps
configure
build
