cmake_minimum_required(VERSION 2.8.6)
project( cpp_splines C CXX )
enable_testing()

# CMake module(s) path
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR} )
set( CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake )
# Find CasADi
find_package(CASADI REQUIRED)
# Include directories
include_directories(
  ${PROJECT_SOURCE_DIR}
	${PROJECT_SOURCE_DIR}/src
  ${PROJECT_SOURCE_DIR}/src/Basis
)

##################################################################
########################## Options ###############################
##################################################################
option(WITH_MATLAB "Compile the MATLAB front-end" ON)
option(WITH_PYTHON "Compile the Python front-end" ON)
option(WITH_COVERAGE "Enable coverage" OFF)


######################################################
################## compiler options ##################
######################################################

if(CMAKE_BUILD_TYPE)
else()
  set(CMAKE_BUILD_TYPE Release)
endif()
string(TOUPPER ${CMAKE_BUILD_TYPE} UPPER_CMAKE_BUILD_TYPE)

set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -fPIC -DWITH_DEPRECATED_FEATURES" )
if(WITH_COVERAGE)
  set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage" )
endif()

set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC" )

#####################################################
#################### cpp_splines ####################
#####################################################
add_subdirectory(src)

#####################################################
######################## swig #######################
#####################################################
if(WITH_MATLAB OR WITH_PYTHON)
  add_subdirectory(swig)
endif()

add_subdirectory(tests)
