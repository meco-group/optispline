#include <vector>
#include "plus.h"
#include "../utils/vectorUtilities.h"

namespace spline{

    Basis plusSubBasis (const BSplineBasis &b1, const BSplineBasis &b2) {
        int newDegree = std::max(b1.getDegree(), b2.getDegree());
        AnyVector v = unionKnots(b1.getKnots(), b2.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
        return BSplineBasis(v, newDegree);
    }

    Basis plusSubBasis (const BSplineBasis &b1, const MonomialBasis &b2) {
        //    TODO experimantal implementation
        int newDegree = std::max(b1.getDegree(), b2.getDegree());
        AnyVector v = increaseMultiplicityKnots(b1.getKnots(), newDegree - b1.getDegree());
        return BSplineBasis(v, newDegree);
    }

    Basis plusSubBasis (const MonomialBasis &b1, const BSplineBasis &b2) {
        return plusSubBasis(b2, b1);
    }

    Basis plusSubBasis (const MonomialBasis &b1, const MonomialBasis &b2) {
        int newDegree = std::max(b1.getDegree(), b2.getDegree());
        return MonomialBasis(newDegree);
    }

}  // namespace spline
