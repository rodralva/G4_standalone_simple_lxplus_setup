#!/bin/bash
(
    if [ ! -d "build" ]; then
    mkdir build
    fi
)&&
(rm -rf build/*) &&
(cmake -DGeant4_DIR=/cvmfs/sft.cern.ch/lcg/releases/Geant4/10.07.p01-daa34/x86_64-centos7-gcc8-opt -B build/) &&
(cmake --build build/ --config Release -- -j 8)
