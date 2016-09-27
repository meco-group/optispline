# Simple library that shows how CasADi can be extended
include_directories(${CASADI_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/../tensor-tools)
add_library(Basis SHARED
            common.h

            SharedObject/SharedObject.cpp SharedObject/SharedObjectNode.cpp
            SharedObject/SharedObject.h SharedObject/SharedObjectNode.h
 	    
	    Basis/SubBasis.h Basis/SubBasis.cpp
	    Basis/SubBSplineBasis.h Basis/SubBSplineBasis.cpp
	    Basis/SubUnivariateBasis.h Basis/SubUnivariateBasis.cpp
	    Basis/SubMonomialBasis.h Basis/SubMonomialBasis.cpp
            Basis/Basis.h Basis/Basis.cpp
            Basis/BSplineBasis.h Basis/BSplineBasis.cpp
            Basis/UnivariateBasis.h Basis/UnivariateBasis.cpp
            Basis/MonomialBasis.h Basis/MonomialBasis.cpp
	   
            Basis/operations/operationsBasis.h
            Basis/operations/plus.h 
            Basis/operations/plusUnivariate.h Basis/operations/plusUnivariate.cpp
            Basis/operations/times.h
	    Basis/operations/timesUnivariate.h Basis/operations/timesUnivariate.cpp
            
            Basis/utils/vectorUtilities.h Basis/utils/vectorUtilities.cpp
            Basis/utils/CommonBasis.h

            Function/Function.h Function/Function.cpp
            Function/Polynomial.h Function/Polynomial.cpp
            Function/Argument.h Function/Argument.cpp

            Coefficients/Coefficient.h Coefficients/Coefficient.cpp

	    ../tensor-tools/any_tensor.cpp
	    ../tensor-tools/any_tensor.hpp
	    ../tensor-tools/tensor.hpp
          )
target_link_libraries(Basis ${CASADI_LIBRARIES})
add_definitions(-DSHARED_LIBRARY)
