cmake_minimum_required(VERSION 2.8.6)
project( cpp_splines CXX C )
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

######################################################
################## compiler options ##################
######################################################
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -O2 -fPIC" )
set( CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -fPIC" )

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
