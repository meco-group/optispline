#ifndef PLUSUNIVARIATE_H
#define PLUSUNIVARIATE_H

#include "SubBasis.h"
#include "SubMonomialBasis.h"
#include "SubBSplineBasis.h"

namespace spline{
//TODO specify return type
// #ifndef SWIG
    SubBasis plusBasis(const SubBSplineBasis& b1 , const SubBSplineBasis& b2);
    SubBasis plusBasis(const SubMonomialBasis& b1 , const SubMonomialBasis& b2);
    SubBasis plusBasis(const SubMonomialBasis& b1 , const SubBSplineBasis& b2);
    SubBasis plusBasis(const SubBSplineBasis& b1 , const SubMonomialBasis& b2);

// #endif // SWIG
} // namespace spline

#endif /* PLUSUNIVARIATE_H */
