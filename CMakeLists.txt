cmake_minimum_required(VERSION 3.10)
include(CheckSymbolExists)

project( mrproc LANGUAGES CXX )
set (CMAKE_CXX_STANDARD 11)
add_executable( mrproc
    src/mrproc.cpp
)
