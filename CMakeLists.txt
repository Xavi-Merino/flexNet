cmake_minimum_required(VERSION 3.10)
project(flexible-networks-simulator)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
FILE(GLOB fns "${PROJECT_SOURCE_DIR}/src/*.cpp")
FILE(GLOB headers "${PROJECT_SOURCE_DIR}/src/*.hpp")
add_library(fnsim STATIC ${fns})

INSTALL(TARGETS fnsim DESTINATION lib)
INSTALL(FILES ${headers} DESTINATION include/fnsim)

add_subdirectory(test)
