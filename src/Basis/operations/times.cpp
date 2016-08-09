//
// Created by erik on 18/04/16.
//
#include <vector>
#include "times.h"
#include "vectorUtilities.h"


namespace spline{

BSplinestd::shared_ptr<Basis> timesstd::shared_ptr<Basis> (const BSplinestd::shared_ptr<Basis> &b1, const BSplinestd::shared_ptr<Basis> &b2) {

    int newDegree = b1.getDegree() + b2.getDegree();
    const std::vector<double> v = unionKnots(b1.getKnots(), b2.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
    return BSplinestd::shared_ptr<Basis>(v, newDegree) ;
}

BSplinestd::shared_ptr<Basis> timesstd::shared_ptr<Basis> (const BSplinestd::shared_ptr<Basis> &b1, const Monomialestd::shared_ptr<Basis> &b2) {
//    TODO experimantal implementation
    int newDegree = b1.getDegree() + b2.getDegree();
    const std::vector<double> v = unionKnots(b1.getKnots(), b1.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
    return BSplinestd::shared_ptr<Basis>(v, newDegree) ;
}

     BSplinestd::shared_ptr<Basis> timesstd::shared_ptr<Basis> (const Monomialestd::shared_ptr<Basis> &b1, const BSplinestd::shared_ptr<Basis> &b2) {
    return timesstd::shared_ptr<Basis>(b2, b1);
}


     Monomialestd::shared_ptr<Basis> timesstd::shared_ptr<Basis> (const Monomialestd::shared_ptr<Basis> &b1, const Monomialestd::shared_ptr<Basis> &b2) {

         int newDegree = b1.getDegree() + b2.getDegree();
         return Monomialestd::shared_ptr<Basis>(newDegree);
}

}
