cmake_minimum_required(VERSION 3.10)
project(flexible-networks-simulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
FILE(GLOB fns "src/*.cpp")
add_library(fnsim STATIC ${fns})

add_subdirectory(test)
