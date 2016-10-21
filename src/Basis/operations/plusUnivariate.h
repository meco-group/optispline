#ifndef PLUSUNIVARIATE_H
#define PLUSUNIVARIATE_H

#include "Basis.h"
#include "MonomialBasis.h"
#include "BSplineBasis.h"

namespace spline{
//TODO specify return type
// #ifndef SWIG
    Basis plusSubBasis(const BSplineBasis& b1 , const BSplineBasis& b2);
    Basis plusSubBasis(const MonomialBasis& b1 , const MonomialBasis& b2);
    Basis plusSubBasis(const MonomialBasis& b1 , const BSplineBasis& b2);
    Basis plusSubBasis(const BSplineBasis& b1 , const MonomialBasis& b2);

// #endif // SWIG
} // namespace spline

#endif /* PLUSUNIVARIATE_H */
