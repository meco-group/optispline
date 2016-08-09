#ifndef PLUSMULTIVARIATE_H
#define PLUSMULTIVARIATE_H

#include "../Function/Argument.h"
#include "Basis.h"
#include "BSplineBasis.h"
#include "MonomialeBasis.h"
// #include "plusUnivariate.h"
namespace spline{

    template< class BasisT >
    Basis plusMultivariate(const Basis& b1, const BasisT b2){
        Basis returnBasis = Basis();
        bool matchingArguments = false;

        for (Basis & subBasis : b1.getSubBasis()) {
            if(subBasis.getArgument() == b2.getArgument()){
                Basis sum = casted_plus_univariate(subBasis, b2);
                sum.setArgument(subBasis.getArgument());
                returnBasis.addBasis(sum);
                matchingArguments = true;
            }else{
                returnBasis.addBasis(subBasis);
            }
        }
        if(! matchingArguments){
            returnBasis.addBasis(b2);
        }
        return returnBasis;
    }

    Basis plusMultivariate(const Basis& b1, const Basis& b2);

    template< class BasisT >
    Basis casted_plus_univariate(const Basis& b1, const BasisT b2){
        BasisNode* w = b1.get();
        Basis sum;
        if(dynamic_cast<MonomialeBasisNode*>(w)){
            sum = b2 + w->shared_from_this<MonomialeBasis>();
        }
        if(dynamic_cast<BSplineBasisNode*>(w)){
            sum = b2 + w->shared_from_this<BSplineBasis>();
        }
        return  sum;
    }

} // namespace spline
#endif //
