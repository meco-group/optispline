# Simple library that shows how CasADi can be extended
include_directories(${CASADI_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../tensor-tools)
add_library(Basis SHARED
            common.h

            SharedObject/SharedObject.cpp SharedObject/SharedObjectNode.cpp
            SharedObject/SharedObject.h SharedObject/SharedObjectNode.h

            Basis/Basis.h Basis/Basis.cpp
            Basis/BSplineBasis.h Basis/BSplineBasis.cpp
            Basis/UnivariateBasis.h Basis/UnivariateBasis.cpp
            Basis/MonomialeBasis.h Basis/MonomialeBasis.cpp
            Basis/plus.h Basis/plus.cpp
            # Basis/times.h Basis/times.cpp
            Basis/vectorUtilities.h Basis/vectorUtilities.cpp
            Basis/CommonBasis.h
            Function/Function.h Function/Function.cpp
            Function/Polynomial.h Function/Polynomial.cpp
            Function/Argument.h Function/Argument.cpp
            Coefficients/Coefficient.h Coefficients/Coefficient.cpp
          )
target_link_libraries(Basis ${CASADI_LIBRARIES})
add_definitions(-DSHARED_LIBRARY)
