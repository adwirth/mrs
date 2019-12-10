cmake_minimum_required(VERSION 3.10)
include(CheckSymbolExists)

project( mrproc LANGUAGES CXX )
set (CMAKE_CXX_STANDARD 17)

include_directories("include")

set( SOURCES
    include/fileIO.h
    include/scanData.h
    include/fftScan.h
    include/evalIQ.h
    src/fileIO.cpp
    src/mrproc.cpp  
    src/scanData.cpp
    src/fftScan.cpp
    src/evalIQ.cpp
)

add_executable( mrproc
    ${SOURCES}
)
target_link_libraries(mrproc fftw3f)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall -O3")
