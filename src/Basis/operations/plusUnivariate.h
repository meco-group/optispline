#ifndef PLUSUNIVARIATE_H
#define PLUSUNIVARIATE_H

#include "../Function/Argument.h"
#include "Basis.h"
#include "MonomialeBasis.h"
#include "BSplineBasis.h"

namespace spline{

// #ifndef SWIG
    Basis plusBasis(const BSplineBasis& b1 , const BSplineBasis& b2);
    Basis plusBasis(const MonomialeBasis& b1 , const MonomialeBasis& b2);
    Basis plusBasis(const MonomialeBasis& b1 , const BSplineBasis& b2);
    Basis plusBasis(const BSplineBasis& b1 , const MonomialeBasis& b2);

// #endif // SWIG
} // namespace spline

#endif /* PLUSUNIVARIATE_H */
