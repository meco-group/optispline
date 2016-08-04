#ifndef PLUSUNIVARIATE_H
#define PLUSUNIVARIATE_H

#include "../Function/Argument.h"
#include "Basis.h"
#include "MonomialeBasis.h"
#include "BSplineBasis.h"

namespace spline{

    // class BSplineBasis;
    // class MonomialeBasis;
// #ifndef SWIG
    Basis plusBasis(const BSplineBasis& b1 , const BSplineBasis& b2);
    Basis plusBasis(const MonomialeBasis& b1 , const MonomialeBasis& b2);
    Basis plusBasis(const MonomialeBasis& b1 , const BSplineBasis& b2);
    Basis plusBasis(const BSplineBasis& b1 , const MonomialeBasis& b2);

    template< class Basis1, class Basis2 >
        Basis plusUnivariateUnivariate(Basis1 b1, Basis2 b2){
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

#endif /* PLUSUNIVARIATE_H */
