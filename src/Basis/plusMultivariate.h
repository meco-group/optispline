#ifndef CPP_SPLINES_PLUS_H
#define CPP_SPLINES_PLUS_H

#include "../Function/Argument.h"
#include "BSplineBasis.h"
#include "MonomialeBasis.h"

namespace spline{

// #ifndef SWIG
    template< class BasisT >
        Basis plusMultivariateUnivariate(Basis b1, BasisT b2){
            if(b1.getArgument() == b2.getArgument()){
                Basis univariat = plusBasis(b1, b2);
                univariat.setArgument(b1.getArgument());
                return univariat;
            }else{
                Basis tensor = Basis();
                tensor.addBasis(b1);
                tensor.addBasis(b2);
                return tensor;
            }
        }
// #endif // SWIG
} // namespace spline
#endif //CPP_SPLINES_PLUS_H
