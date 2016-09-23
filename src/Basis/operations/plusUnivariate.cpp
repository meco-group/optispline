#include <vector>
#include "plusUnivariate.h"
#include "vectorUtilities.h"

namespace spline{

    Basis plusBasis (const BSplineBasis &b1, const BSplineBasis &b2) {
        int newDegree = std::max(b1.getDegree(), b2.getDegree());
        const std::vector<double> v = unionKnots(b1.getKnots(), b2.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
        return BSplineBasis(v, newDegree);
    }

    Basis plusBasis (const BSplineBasis &b1, const MonomialBasis &b2) {
        //    TODO experimantal implementation
        int newDegree = std::max(b1.getDegree(), b2.getDegree());
        const std::vector<double> v = increaseMultiplicityKnots(b1.getKnots(), newDegree - b1.getDegree());
        return BSplineBasis(v, newDegree);
    }

    Basis plusBasis (const MonomialBasis &b1, const BSplineBasis &b2) {
        return plusBasis(b2, b1);
    }

    Basis plusBasis (const MonomialBasis &b1, const MonomialBasis &b2) {
        int newDegree = std::max(b1.getDegree(), b2.getDegree());
        return MonomialBasis(newDegree);
    }

}  // namespace spline
