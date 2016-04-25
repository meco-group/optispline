# Simple library that shows how CasADi can be extended
include_directories(${CASADI_INCLUDE_DIR})
add_library(Basis SHARED
            Basis/Basis.h Basis/Basis.cpp
            Basis/vectorUtilities.h Basis/vectorUtilities.cpp
            Basis/plus.h Basis/plus.cpp
            Basis/times.h Basis/times.cpp
            Basis/BSplineBasis.h Basis/BSplineBasis.cpp
            Basis/UnivariateBasis.h Basis/UnivariateBasis.cpp
            Basis/MonomialeBasis.h Basis/MonomialeBasis.cpp
            Basis/TensorBasis.h Basis/TensorBasis.cpp
          )
target_link_libraries(Basis ${CASADI_LIBRARIES})
add_definitions(-DSHARED_LIBRARY)
