# Simple library that shows how CasADi can be extended
add_library(Basis
            Basis/Basis.h Basis/Basis.cpp
            Basis/plus.h Basis/plus.cpp
            Basis/vectorUtilities.h Basis/vectorUtilities.cpp
            Basis/BSplineBasis.h Basis/BSplineBasis.cpp
            Basis/UnivariateBasis.h Basis/UnivariateBasis.cpp
            Basis/MonomialeBasis.h Basis/MonomialeBasis.cpp
          )
target_link_libraries(Basis ${CASADI_LIBRARIES})
