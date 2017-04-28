#include <vector>
#include "times.h"
#include "../utils/vectorUtilities.h"

namespace spline {

    Basis times_basis(const BSplineBasisNode& b1, const BSplineBasisNode& b2) {
        int newDegree = b1.degree() + b2.degree();
        std::vector<AnyScalar> v = unionKnots(b1.knots(), b2.knots(), newDegree,
          b1.degree(), b2.degree());
        return BSplineBasis(v, newDegree) ;
    }

    Basis times_basis(const BSplineBasisNode& b1, const MonomialBasisNode& b2) {
        // TODO experimantal implementation
        int newDegree = b1.degree() + b2.degree();
        std::vector<AnyScalar> v = increaseMultiplicityKnots(b1.knots(), b2.degree());
        return BSplineBasis(v, newDegree) ;
    }

    Basis times_basis(const MonomialBasisNode& b1 , const BSplineBasisNode& b2) {
        return times_basis(b2, b1);
    }

    Basis times_basis(const MonomialBasisNode& b1 , const MonomialBasisNode& b2) {
        int newDegree = b1.degree() + b2.degree();
        return MonomialBasis(newDegree);
    }

} // namespace spline
