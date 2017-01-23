#!/bin/bash

cmake -E make_directory build
cmake -E chdir build cmake -E time cmake -G Xcode ../src
cmake -E time cmake --build build --config Debug --clean-first
