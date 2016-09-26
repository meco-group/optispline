#ifndef CPP_SPLINES_TIMES_H
#define CPP_SPLINES_TIMES_H

#include "SubBasis.h"
#include "SubMonomialBasis.h"
#include "SubBSplineBasis.h"

namespace spline{
//TODO specify return type
// #ifndef SWIG
    SubBasis timesBasis(const SubBSplineBasis& b1 , const SubBSplineBasis& b2);
    SubBasis timesBasis(const SubMonomialBasis& b1 , const SubMonomialBasis& b2);
    SubBasis timesBasis(const SubMonomialBasis& b1 , const SubBSplineBasis& b2);
    SubBasis timesBasis(const SubBSplineBasis& b1 , const SubMonomialBasis& b2);

// #endif // SWIG
} // namespace spline

#endif //CPP_SPLINES_TIMES_H
