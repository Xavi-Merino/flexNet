cmake_minimum_required(VERSION 3.10)
project(flexible-networks-simulator)

set(CMAKE_CXX_STANDARD 11)
FILE(GLOB fns "src/*.cpp")
add_library(fnsim STATIC ${fns})

# Prepare "Catch" library for other executables
set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
set(CATCH_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/test)
add_library(Catch INTERFACE)
target_include_directories(Catch INTERFACE ${CATCH_INCLUDE_DIR})

# Make test executable
add_custom_target(ALL_TESTS ALL)
include(Catch)
file( GLOB TEST_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp )
foreach( testsourcefile ${TEST_SOURCES} )
    get_filename_component(filename ${testsourcefile} NAME)
    string( REPLACE ".cpp" "" testname ${filename} )
    add_executable( ${testname} ${testsourcefile} )
    target_link_libraries( ${testname} Catch fnsim )
    catch_discover_tests(${testname})
    add_dependencies(ALL_TESTS ${testname})
endforeach( testsourcefile ${TEST_SOURCES} )
