#!/bin/bash

CXX=g++
CXXFLAGS="-std=c++17 -I/usr/local/include/mysql"
LDFLAGS="-L/usr/local/lib -lmysqlclient"
SRC_FILES="main.cpp"
OUTPUT="shamrock_shelf"

echo "Compiling the program..."
$CXX $CXXFLAGS $SRC_FILES -o $OUTPUT $LDFLAGS

if [ $? -eq 0 ]; then
    echo "Compilation successful. Running the program..."
    ./shamrock_shelf
else
    echo "Compilation failed."
    exit 1
fi

