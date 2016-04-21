//
// Created by erik on 18/04/16.
//

#ifndef CPP_SPLINES_TIMES_H
#define CPP_SPLINES_TIMES_H


#include "BSplineBasis.h"
#include "MonomialeBasis.h"


namespace spline{

    BSplineBasis timesBasis(const BSplineBasis& b1 , const BSplineBasis& b2);
    MonomialeBasis timesBasis(const MonomialeBasis& b1 , const MonomialeBasis& b2);
    BSplineBasis timesBasis(const MonomialeBasis& b1 , const BSplineBasis& b2);
    BSplineBasis timesBasis(const BSplineBasis& b1 , const MonomialeBasis& b2);


} // namespace spline
#endif //CPP_SPLINES_TIMES_H
