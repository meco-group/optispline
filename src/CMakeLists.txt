# Simple library that shows how CasADi can be extended
add_library(Basis SHARED 
            Basis/Basis.h Basis/Basis.cpp
            Basis/vectorUtilities.h Basis/vectorUtilities.cpp
            Basis/plus.h Basis/plus.cpp
            Basis/BSplineBasis.h Basis/BSplineBasis.cpp
            Basis/UnivariateBasis.h Basis/UnivariateBasis.cpp
            Basis/MonomialeBasis.h Basis/MonomialeBasis.cpp
          )
target_link_libraries(Basis ${CASADI_LIBRARIES})
add_definitions(-DSHARED_LIBRARY)
