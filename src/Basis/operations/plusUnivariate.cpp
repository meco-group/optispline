#include <vector>
#include "plusUnivariate.h"
#include "../utils/vectorUtilities.h"

namespace spline{

    SubBasis plusSubBasis (const SubBSplineBasis &b1, const SubBSplineBasis &b2) {
        int newDegree = std::max(b1.getDegree(), b2.getDegree());
        const std::vector<double> v = unionKnots(b1.getKnots(), b2.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
        return SubBSplineBasis(v, newDegree);
    }

    SubBasis plusSubBasis (const SubBSplineBasis &b1, const SubMonomialBasis &b2) {
        //    TODO experimantal implementation
        int newDegree = std::max(b1.getDegree(), b2.getDegree());
        const std::vector<double> v = increaseMultiplicityKnots(b1.getKnots(), newDegree - b1.getDegree());
        return SubBSplineBasis(v, newDegree);
    }

    SubBasis plusSubBasis (const SubMonomialBasis &b1, const SubBSplineBasis &b2) {
        return plusSubBasis(b2, b1);
    }

    SubBasis plusSubBasis (const SubMonomialBasis &b1, const SubMonomialBasis &b2) {
        int newDegree = std::max(b1.getDegree(), b2.getDegree());
        return SubMonomialBasis(newDegree);
    }

}  // namespace spline
