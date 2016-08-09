#include "plusMultivariate.h"

namespace spline{

    Basis plusMultivariate(const Basis& b1, const Basis& b2){
        Basis returnBasis = b1;

        for (Basis & subBasis : b2.getSubBasis()) {
            BasisNode* w = subBasis.get();
            if(dynamic_cast<MonomialeBasisNode*>(w)){
                returnBasis = returnBasis + w->shared_from_this<MonomialeBasis>();
            }

            if(dynamic_cast<BSplineBasisNode*>(w)){
                returnBasis = returnBasis + w->shared_from_this<BSplineBasis>();
            }
        }

        return returnBasis;
    }
}  // namespace spline
