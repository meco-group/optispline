cmake_minimum_required(VERSION 2.8.6)
project( tensor C CXX )

# CMake module(s) path
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR} )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake )
# Find CasADi
find_package(CASADI REQUIRED)
# Include directories
include_directories(
  ${PROJECT_SOURCE_DIR}
)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -g -fPIC" )

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -g -fPIC" )


# Simple library that shows how CasADi can be extended
include_directories(${CASADI_INCLUDE_DIR})
add_definitions(-DSHARED_LIBRARY)


add_library(tensortools
            any_tensor.cpp any_tensor.hpp tensor.hpp
          )


add_executable(testme
            test.cpp
          )

target_link_libraries(testme ${CASADI_LIBRARIES} tensortools)

target_include_directories(testme PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
