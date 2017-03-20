#ifndef PLUSUNIVARIATE_H
#define PLUSUNIVARIATE_H

#include "Basis.h"
#include "MonomialBasis.h"
#include "BSplineBasis.h"

namespace spline{
#ifndef SWIG
    Basis plus_basis(const BSplineBasisNode& b1 , const BSplineBasisNode& b2);
    Basis plus_basis(const MonomialBasisNode& b1 , const MonomialBasisNode& b2);
    Basis plus_basis(const MonomialBasisNode& b1 , const BSplineBasisNode& b2);
    Basis plus_basis(const BSplineBasisNode& b1 , const MonomialBasisNode& b2);
#endif // SWIG
} // namespace spline

#endif /* PLUSUNIVARIATE_H */
