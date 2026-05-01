#!/bin/bash 

cd ./build
cmake .. -DCMAKE_BUILD_TYPE=Debug -DTESTS=True -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
