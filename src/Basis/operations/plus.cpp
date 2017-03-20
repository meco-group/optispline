#include <vector>
#include "plus.h"
#include "../utils/vectorUtilities.h"

namespace spline {

    Basis plus_basis(const BSplineBasisNode& b1, const BSplineBasisNode& b2) {
        int newDegree = std::max(b1.degree(), b2.degree());
        std::vector<AnyScalar> v = unionKnots(b1.knots(), b2.knots(), newDegree,
          b1.degree(), b2.degree());
        return BSplineBasis(v, newDegree);
    }

    Basis plus_basis(const BSplineBasisNode& b1, const MonomialBasisNode& b2) {
        // TODO experimantal implementation
        int newDegree = std::max(b1.degree(), b2.degree());
        std::vector<AnyScalar> v = increaseMultiplicityKnots(b1.knots(), newDegree - b1.degree());
        return BSplineBasis(v, newDegree);
    }

    Basis plus_basis(const MonomialBasisNode& b1, const BSplineBasisNode& b2) {
        return plus_basis(b2, b1);
    }

    Basis plus_basis(const MonomialBasisNode& b1, const MonomialBasisNode& b2) {
        int newDegree = std::max(b1.degree(), b2.degree());
        return MonomialBasis(newDegree);
    }

}  // namespace spline
