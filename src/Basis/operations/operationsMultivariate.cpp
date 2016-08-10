#include "operationsMultivariate.h"

namespace spline{

    Basis plusMultivariate(const Basis& b1, const Basis& b2){
        Basis returnBasis = b1;
        for (Basis & subBasis : b2.getSubBasis()) {
           returnBasis = casted_plus_univariate(returnBasis, subBasis);
        }
        return returnBasis;
    }


    Basis timesMultivariate(const Basis& b1, const Basis& b2){
        Basis returnBasis = b1;
        for (Basis & subBasis : b2.getSubBasis()) {
           returnBasis = casted_times_univariate(returnBasis, subBasis);
        }
        return returnBasis;
    }

}  // namespace spline
