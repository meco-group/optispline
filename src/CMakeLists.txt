# # Simple library that shows how CasADi can be extended
# include_directories(${CASADI_INCLUDE_DIR})
# add_library(Basis SHARED
#             Basis/Basis.h Basis/Basis.cpp
#             Basis/vectorUtilities.h Basis/vectorUtilities.cpp
#             Basis/plus.h Basis/plus.cpp
#             # Basis/times.h Basis/times.cpp
#             Basis/BSplineBasis.h Basis/BSplineBasis.cpp
#             Basis/UnivariateBasis.h Basis/UnivariateBasis.cpp
#             Basis/MonomialeBasis.h Basis/MonomialeBasis.cpp
#         # Basis/TensorProductBasis.h Basis/TensorProductBasis.cpp
#           )
# # add_library(Coefficients SHARED
# #             Coefficients/Coefficients.h Coefficients/Coefficients.cpp
# #             Coefficients/Coefficient.h Coefficients/Coefficient.cpp
# #           )
# target_link_libraries(Basis ${CASADI_LIBRARIES})
# # target_link_libraries(Coefficients ${CASADI_LIBRARIES})
# add_definitions(-DSHARED_LIBRARY)



# Simple library that shows how CasADi can be extended
include_directories(${CASADI_INCLUDE_DIR})
add_library(Basis SHARED
            Basis/Basis.h Basis/Basis.cpp
            Basis/vectorUtilities.h Basis/vectorUtilities.cpp
            Basis/plus.h Basis/plus.cpp
            # Basis/times.h Basis/times.cpp
            Basis/BSplineBasis.h Basis/BSplineBasis.cpp
            Basis/UnivariateBasis.h Basis/UnivariateBasis.cpp
            Basis/MonomialeBasis.h Basis/MonomialeBasis.cpp
            #          )
            #add_library(Function SHARED
    #            Function/Function.h Function/Function.cpp
            Function/Argument.h Function/Argument.cpp
            SharedObject/SharedObject.cpp SharedObject/SharedObjectNode.cpp
            SharedObject/SharedObject.h SharedObject/SharedObjectNode.h
          )
target_link_libraries(Basis ${CASADI_LIBRARIES})
#target_link_libraries(Function ${CASADI_LIBRARIES})
add_definitions(-DSHARED_LIBRARY)
