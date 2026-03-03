#!/bin/bash

find -name "CMakeFiles" | xargs rm -rf
find . -name "Makefile" -delete
find . -name "cmake_install.cmake" -delete

rm -rf build/*
