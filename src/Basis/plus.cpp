//
// Created by erik on 18/04/16.
//
#include <vector>
#include "plus.h"


namespace spline{

BSplineBasis plusBasis (const BSplineBasis &b1, const BSplineBasis &b2) {

    int newDegree = std::max(b1.getDegree(), b2.getDegree());
    const std::vector<double> v {0.0, 1.0};
    return BSplineBasis(v, 0, 0 ) ;
}

    BSplineBasis plusBasis (const BSplineBasis &b1, const MonomialeBasis &b2) {
    const std::vector<double> v {0.0, 1.0};
    return BSplineBasis(v, 0, 0 ) ;
}


     BSplineBasis plusBasis (const MonomialeBasis &b1, const BSplineBasis &b2) {
    return plusBasis(b2, b1);
}


     MonomialeBasis plusBasis (const MonomialeBasis &b1, const MonomialeBasis &b2) {
    return MonomialeBasis(6);
}

}