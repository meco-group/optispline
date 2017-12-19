# Simple library that shows how CasADi can be extended
include_directories(${CASADI_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../tensor-tools)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-sign-compare")

if(NOT WIN32)
set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror" )
endif()

add_library(splines SHARED
    common.h
	common.cpp

    SharedObject/SharedObject.cpp SharedObject/SharedObjectNode.cpp
    SharedObject/SharedObject.h SharedObject/SharedObjectNode.h
    SharedObject/PrintableObject.h

    Basis/utils/vectorUtilities.h

    Basis/BasisNode.h Basis/BasisNode.cpp
    Basis/Basis.h Basis/Basis.cpp
    Basis/EmptyBasisNode.h Basis/EmptyBasisNode.cpp
    Basis/EmptyBasis.h Basis/EmptyBasis.cpp
    Basis/UnivariateBasisNode.h Basis/UnivariateBasisNode.cpp
    Basis/UnivariateBasis.h Basis/UnivariateBasis.cpp
    Basis/BSplineBasisNode.h Basis/BSplineBasisNode.cpp
    Basis/BSplineBasis.h Basis/BSplineBasis.cpp
    Basis/MonomialBasisNode.h Basis/MonomialBasisNode.cpp
    Basis/MonomialBasis.h Basis/MonomialBasis.cpp
    Basis/TensorBasisNode.h Basis/TensorBasisNode.cpp
    Basis/TensorBasis.h Basis/TensorBasis.cpp

    Basis/operations/operationsBasis.h Basis/operations/operationsBasis.cpp
    Basis/operations/plus.h Basis/operations/plus.cpp
    Basis/operations/times.h Basis/operations/times.cpp

    Function/FunctionNode.h Function/FunctionNode.cpp
    Function/Function.h Function/Function.cpp
    Function/Polynomial.h Function/Polynomial.cpp
    Function/Parameter.h Function/Parameter.cpp
    Function/Argument.h Function/Argument.cpp
    Function/NumericIndex.h

    Domain/Interval.h Domain/Interval.cpp
    Domain/Domain.h Domain/Domain.cpp
    Domain/TensorDomain.h Domain/TensorDomain.cpp

    Coefficients/Coefficient.h Coefficients/Coefficient.cpp

    Optistack/optistack.h Optistack/optistack.cpp

    ../tensor-tools/any_tensor.cpp
    ../tensor-tools/any_tensor.hpp
    ../tensor-tools/tensor.hpp
    ../tensor-tools/slice.hpp
    ../tensor-tools/slice.cpp
    )
target_link_libraries(splines ${CASADI_LIBRARIES})
add_definitions(-DSHARED_LIBRARY)
