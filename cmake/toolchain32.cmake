# this one is important
SET(CMAKE_SYSTEM_NAME Windows)
#this one not so much
SET(CMAKE_SYSTEM_VERSION 1)
SET(PREFIX i686-w64-mingw32)
# specify the cross compiler
SET(CMAKE_C_COMPILER   i686-w64-mingw32-gcc)
SET(CMAKE_CXX_COMPILER i686-w64-mingw32-g++)
SET(CMAKE_Fortran_COMPILER i686-w64-mingw32-gfortran)
set(CMAKE_RC_COMPILER i686-w64-mingw32-windres)
        
# where is the target environment 
SET(CMAKE_FIND_ROOT_PATH  /usr/i686-w64-mingw32)
SET(CMAKE_CXX_FLAGS "-fno-ipa-cp-clone" CACHE STRING "" FORCE)
          
# search for programs in the build host directories
SET(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
# for libraries and headers in the target directories
SET(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY NEVER)
SET(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE NEVER)
set(CMAKE_SHARED_LIBRARY_SUFFIX "dll")
