//
// Created by erik on 18/04/16.
//

#ifndef CPP_SPLINES_PLUS_H
#define CPP_SPLINES_PLUS_H


#include "BSplineBasis.h"
#include "MonomialeBasis.h"


namespace spline{

    BSplineBasis plusBasis(const BSplineBasis& b1 , const BSplineBasis& b2);
    MonomialeBasis plusBasis(const MonomialeBasis& b1 , const MonomialeBasis& b2);
    BSplineBasis plusBasis(const MonomialeBasis& b1 , const BSplineBasis& b2);
    BSplineBasis plusBasis(const BSplineBasis& b1 , const MonomialeBasis& b2);


} // namespace spline
#endif //CPP_SPLINES_PLUS_H
