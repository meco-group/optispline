#ifndef PLUSMULTIVARIATE_H
#define PLUSMULTIVARIATE_H

#include "../Function/Argument.h"
#include "Basis.h"
#include "BSplineBasis.h"
#include "MonomialBasis.h"
// #include "plusUnivariate.h"
namespace spline{

    template< class BasisT >
        Basis plusMultivariate(const Basis& b1, const BasisT b2){
            Basis returnBasis = Basis();
            bool matchingArguments = false;

            for (Basis & subBasis : b1.getSubBasis()) {
                if(subBasis.getArgument() == b2.getArgument()){
                    Basis sum = casted_plus_univariate(b2, subBasis);
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

 template< class BasisT >
        Basis timesMultivariate(const Basis& b1, const BasisT b2){
            Basis returnBasis = Basis();
            bool matchingArguments = false;

            for (Basis & subBasis : b1.getSubBasis()) {
                if(subBasis.getArgument() == b2.getArgument()){
                    Basis sum = casted_times_univariate(b2, subBasis);
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
    Basis timesMultivariate(const Basis& b1, const Basis& b2);

    template< class BasisT >
        Basis casted_plus_univariate(const BasisT b1, const Basis& b2){
            BasisNode* w = b2.get();
            Basis sum;
            if(dynamic_cast<MonomialBasisNode*>(w)){
                sum = b1 + w->shared_from_this<MonomialBasis>();
            }
            if(dynamic_cast<BSplineBasisNode*>(w)){
                sum = b1 + w->shared_from_this<BSplineBasis>();
            }
            return sum;
        }

    template< class BasisT >
        Basis casted_times_univariate(const BasisT b1, const Basis& b2){
            BasisNode* w = b2.get();
            Basis product;
            if(dynamic_cast<MonomialBasisNode*>(w)){
                product = b1 + w->shared_from_this<MonomialBasis>();
            }
            if(dynamic_cast<BSplineBasisNode*>(w)){
                product = b1 + w->shared_from_this<BSplineBasis>();
            }
            return product;
        }

} // namespace spline
#endif //
