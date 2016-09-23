#ifndef CPP_SPLINES_TIMES_H
#define CPP_SPLINES_TIMES_H



#include "BSplineBasis.h"
#include "MonomialBasis.h"


namespace spline{

    BSplineBasis timesBasis(const BSplineBasis& b1 , const BSplineBasis& b2);
    MonomialBasis timesBasis(const MonomialBasis& b1 , const MonomialBasis& b2);
    BSplineBasis timesBasis(const MonomialBasis& b1 , const BSplineBasis& b2);
    BSplineBasis timesBasis(const BSplineBasis& b1 , const MonomialBasis& b2);


} // namespace spline
#endif //CPP_SPLINES_TIMES_H
