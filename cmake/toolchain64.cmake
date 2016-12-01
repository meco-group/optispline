# this one is important
SET(CMAKE_SYSTEM_NAME Windows)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)
SET(PREFIX x86_64-w64-mingw32)
# specify the cross compiler
SET(CMAKE_C_COMPILER   x86_64-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
SET(CMAKE_Fortran_COMPILER x86_64-w64-mingw32-gfortran)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
        
# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/x86_64-w64-mingw32)
SET(CMAKE_CXX_FLAGS "-fno-ipa-cp-clone" CACHE STRING "" FORCE)
          
# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
set(CMAKE_SHARED_LIBRARY_SUFFIX "dll")


set(MATLAB_LIBRARIES $ENV{MATLAB_ROOT}/bin/win64/libut.dll $ENV{MATLAB_ROOT}/bin/win64/libmex.dll $ENV{MATLAB_ROOT}/bin/win64/libmx.dll $ENV{MATLAB_ROOT}/bin/win64/libeng.dll)
          
set(MATLAB_INCLUDE_DIR "$ENV{MATLAB_ROOT}/extern/include")
set(MATLAB_EXTRA_CXXFLAGS "-D__STDC_UTF_16__" "-DWINMAT64")
set(MATLAB_MEX_EXT "mexw64")

