//
// Created by erik on 18/04/16.
//
#include <vector>
#include "plus.h"
#include "vectorUtilities.h"


namespace spline{

BSplineBasis plusBasis (const BSplineBasis &b1, const BSplineBasis &b2) {
    int newDegree = std::max(b1.getDegree(), b2.getDegree());
    const std::vector<double> v = unionKnots(b1.getKnots(), b2.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
    return BSplineBasis(v, newDegree) ;
}

BSplineBasis plusBasis (const BSplineBasis &b1, const MonomialeBasis &b2) {
//    TODO experimantal implementation
    int newDegree = b1.getDegree() + b2.getDegree();
    const std::vector<double> v = unionKnots(b1.getKnots(), b1.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
    return BSplineBasis(v, newDegree) ;
}

BSplineBasis plusBasis (const MonomialeBasis &b1, const BSplineBasis &b2) {
    return plusBasis(b2, b1);
}

MonomialeBasis plusBasis (const MonomialeBasis &b1, const MonomialeBasis &b2) {
  int newDegree = std::max(b1.getDegree(), b2.getDegree());
  return MonomialeBasis(newDegree);
}

}  // namespace spline
