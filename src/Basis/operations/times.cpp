#include <vector>
#include "times.h"
#include "../utils/vectorUtilities.h"

namespace spline {

    Basis timesSubBasis(const BSplineBasis& b1, const BSplineBasis& b2) {
        int newDegree = b1.degree() + b2.degree();
        std::vector<AnyScalar> v = unionKnots(b1.knots(), b2.knots(), newDegree,
          b1.degree(), b2.degree());
        return BSplineBasis(v, newDegree) ;
    }

    Basis timesSubBasis(const BSplineBasis& b1, const MonomialBasis& b2) {
        //    TODO experimantal implementation
        int newDegree = b1.degree() + b2.degree();
        std::vector<AnyScalar> v = unionKnots(b1.knots(), b1.knots(), newDegree,
          b1.degree(), b2.degree());
        return BSplineBasis(v, newDegree) ;
    }

    Basis timesSubBasis(const MonomialBasis& b1 , const BSplineBasis& b2) {
        return timesSubBasis(b2, b1);
    }

    Basis timesSubBasis(const MonomialBasis& b1 , const MonomialBasis& b2) {
        int newDegree = b1.degree() + b2.degree();
        return MonomialBasis(newDegree);
    }

} // namespace spline
