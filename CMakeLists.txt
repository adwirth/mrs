cmake_minimum_required(VERSION 3.10)
include(CheckSymbolExists)

project( mrproc LANGUAGES CXX )
set (CMAKE_CXX_STANDARD 17)

include_directories("include")

set( SOURCES
    include/fileIO.h
    include/scanData.h
    src/fileIO.cpp
    src/mrproc.cpp  
    src/scanData.cpp
)

add_executable( mrproc
    ${SOURCES}
)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
