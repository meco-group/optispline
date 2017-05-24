find_package(SWIGMATLAB REQUIRED)
include(${CMAKE_CURRENT_SOURCE_DIR}/../cmake/UseSWIGMOD.cmake)

if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
  SET(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 
  SET(CMAKE_INSTALL_RPATH "\$ORIGIN/../src:\$ORIGIN:$ENV{CASADI_PREFIX}:.")
endif()

include_directories(${CASADI_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../tensor-tools)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
if(WITH_PYTHON)
  find_package(PythonLibs 2.7)
  find_package(NUMPY)
  include_directories(${PYTHON_INCLUDE_PATH})
  include_directories(${NUMPY_PATH})
endif()
if(WITH_MATLAB)
  if(NOT MATLAB_LIBRARIES)
  find_package(MATLAB)
  endif()
  include_directories(${MATLAB_INCLUDE_DIR})
endif()

set(CMAKE_SWIG_FLAGS "")

if(WITH_STACKTRACE)
  set(SWIG_EXTRA_FLAGS "-DWITH_STACKTRACE")
else()
  set(SWIG_EXTRA_FLAGS)
endif()

set_source_files_properties(splines.i PROPERTIES CPLUSPLUS ON)
set_property(SOURCE splines.i PROPERTY SWIG_FLAGS "-I${PROJECT_SOURCE_DIR}" "-I${CASADI_INCLUDE_DIR}" "-I${CASADI_INCLUDE_DIR}" ${MATLAB_EXTRA_CXXFLAGS} ${SWIG_EXTRA_FLAGS})

#set(SWIG_MODULE_splines_EXTRA_DEPS ${CMAKE_SOURCE_DIR}/src/libsplines${CMAKE_SHARED_LIBRARY_SUFFIX})
  
if(WITH_PYTHON)
  swig_add_module(splines python splines.i)
  swig_link_libraries(splines ${PYTHON_LIBRARIES} splines ${CASADI_LIBRARIES})
endif()

if(WITH_MATLAB)
   swig_add_module(splinesMEX matlab splines.i)
   swig_link_libraries(splinesMEX ${MATLAB_LIBRARIES} splines ${CASADI_LIBRARIES})
   set_target_properties(splinesMEX PROPERTIES PREFIX "" SUFFIX .${MATLAB_MEX_EXT})
   add_custom_target(splines_matlab DEPENDS splinesMEX splines)
   add_custom_command(TARGET splinesMEX
                   POST_BUILD
                   COMMAND rm -f ${CMAKE_CURRENT_BINARY_DIR}/SwigMem.m)
                   
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/OptiSplineYalmip.m DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
  file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/OptiSplineSolverYalmip.m DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endif()


file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/meco_setup.m "meco_binaries('casadi','$ENV{CASADIVERSION}');casadi.SX.sym('x');" )
file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/meco_setup.py "meco_binaries(casadi='$ENV{CASADIVERSION}');import casadi;casadi.SX.sym('x')")

