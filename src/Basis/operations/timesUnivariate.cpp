#include <vector>
#include "times.h"
#include "vectorUtilities.h"

namespace spline{

    BSplineBasis timesBasis(const BSplineBasis& b1 , const BSplineBasis& b2){
        int newDegree = b1.getDegree() + b2.getDegree();
        const std::vector<double> v = unionKnots(b1.getKnots(), b2.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
        return BSplineBasis(v, newDegree) ;
    }

    BSplineBasis timesBasis(const BSplineBasis& b1 , const MonomialeBasis& b2){
        //    TODO experimantal implementation
        int newDegree = b1.getDegree() + b2.getDegree();
        const std::vector<double> v = unionKnots(b1.getKnots(), b1.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
        return BSplineBasis(v, newDegree) ;
    }

    BSplineBasis timesBasis(const MonomialeBasis& b1 , const BSplineBasis& b2){
        return timesBasis(b2, b1);
    }

    MonomialeBasis timesBasis(const MonomialeBasis& b1 , const MonomialeBasis& b2){
        int newDegree = b1.getDegree() + b2.getDegree();
        return MonomialeBasis(newDegree);
    }

}
