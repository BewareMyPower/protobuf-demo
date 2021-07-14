#!/bin/bash
set -e
cd $(dirname $0)/..

cmake -B ./build -DVCPKG_TRIPLET=x64-osx
