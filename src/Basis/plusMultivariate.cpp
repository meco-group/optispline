#include "plusMultivariate.h"

namespace spline{


        Basis plusMultivariateMultivariate2 (const Basis& b1, const Basis& b2){
            Basis returnBasis = b1;

            for (Basis & subBasis : b2.getSubBasis()) {
                returnBasis = returnBasis + subBasis;
            }

            return returnBasis;
        }
}  // namespace spline
