#ifndef CPP_SPLINES_TIMESUNIVARIATE_H
#define CPP_SPLINES_TIMESUNIVARIATE_H

#include "Basis.h"
#include "MonomialBasis.h"
#include "BSplineBasis.h"

namespace spline{
//TODO specify return type
// #ifndef SWIG
    Basis times_basis(const BSplineBasis& b1 , const BSplineBasis& b2);
    Basis times_basis(const MonomialBasis& b1 , const MonomialBasis& b2);
    Basis times_basis(const MonomialBasis& b1 , const BSplineBasis& b2);
    Basis times_basis(const BSplineBasis& b1 , const MonomialBasis& b2);

// #endif // SWIG
} // namespace spline

#endif //CPP_SPLINES_TIMES_H
