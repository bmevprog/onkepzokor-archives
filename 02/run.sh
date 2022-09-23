#!/bin/bash

set -e
g++ -o a -DLOCAL a.cpp
# g++ -o a a.cpp
./a < a.in
