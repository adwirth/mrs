cmake_minimum_required(VERSION 3.10)
include(CheckSymbolExists)

project( mrproc LANGUAGES CXX )
set (CMAKE_CXX_STANDARD 17)
add_executable( mrproc
    src/mrproc.cpp
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
