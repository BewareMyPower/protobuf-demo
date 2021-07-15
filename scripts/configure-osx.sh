#!/bin/bash
set -e
cd $(dirname $0)/..

cmake -B ./build -DVCPKG_TRIPLET=x64-osx -DCMAKE_BUILD_TYPE=Debug
