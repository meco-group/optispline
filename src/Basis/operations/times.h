#ifndef CPP_SPLINES_TIMESUNIVARIATE_H
#define CPP_SPLINES_TIMESUNIVARIATE_H

#include "Basis.h"
#include "MonomialBasis.h"
#include "BSplineBasis.h"
#include "BSplineBasisNode.h"

namespace spline{
#ifndef SWIG
    Basis times_basis(const BSplineBasisNode& b1 , const BSplineBasisNode& b2);
    Basis times_basis(const MonomialBasisNode& b1 , const MonomialBasisNode& b2);
    Basis times_basis(const MonomialBasisNode& b1 , const BSplineBasisNode& b2);
    Basis times_basis(const BSplineBasisNode& b1 , const MonomialBasisNode& b2);
#endif // SWIG */
} // namespace spline

#endif //CPP_SPLINES_TIMES_H
