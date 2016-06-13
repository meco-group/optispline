# Simple library that shows how CasADi can be extended
include_directories(${CASADI_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../tensor-tools)
add_library(Basis SHARED
            Basis/Basis.h Basis/Basis.cpp
            Basis/vectorUtilities.h Basis/vectorUtilities.cpp
            Basis/plus.h Basis/plus.cpp
            # Basis/times.h Basis/times.cpp
            Basis/BSplineBasis.h Basis/BSplineBasis.cpp
            Basis/UnivariateBasis.h Basis/UnivariateBasis.cpp
            Basis/MonomialeBasis.h Basis/MonomialeBasis.cpp
            Function/Argument.h Function/Argument.cpp
            SharedObject/SharedObject.cpp SharedObject/SharedObjectNode.cpp
            SharedObject/SharedObject.h SharedObject/SharedObjectNode.h
          )
target_link_libraries(Basis ${CASADI_LIBRARIES})
#target_link_libraries(Function ${CASADI_LIBRARIES})
add_definitions(-DSHARED_LIBRARY)
