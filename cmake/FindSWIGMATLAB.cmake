# Define SWIG_EXECUTABLE variable before searching for SWIG
# which points to SWIG with support for MATLAB

message(STATUS "Looking for SWIG for MATLAB")
# Set hints to look for SWIG, end with 0 to look once without hints also
set(SWIGMATLABHINTS "$ENV{SWIGMATLAB}" "0")
foreach(SWIGMATLABHINT ${SWIGMATLABHINTS})
  find_program(SWIG_EXECUTABLE
    swig
    HINTS ${SWIGMATLABHINT}
  )
  # Check if the found SWIG binary supports MATLAB
  execute_process(COMMAND ${SWIG_EXECUTABLE} -matlab ERROR_VARIABLE SWIGOUT)
  if("${SWIGOUT}" STREQUAL "Must specify an input file. Use -help for available options.")
    # Found a version of SWIG supporting MATLAB
    break()
  elseif()
    unset(SWIG_EXECUTABLE CACHE)
  endif()
endforeach()
unset(SWIGMATLABHINTS)

if(SWIG_EXECUTABLE)
  set(SWIGMATLAB_FOUND TRUE)
  message(STATUS "Found SWIG for MATLAB: " ${SWIG_EXECUTABLE})
else()
  set(SWIGMATLAB_FOUND FALSE)
  set(WITH_MATLAB OFF)
  message(STATUS "Cound not find SWIG for MATLAB. Try setting SWIGMATLAB env var.")
endif()
