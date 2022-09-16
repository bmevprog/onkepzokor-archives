#!/bin/bash

set -e
# g++ -o c -DLOCAL c.cpp
g++ -o c c.cpp
./c < c.in
