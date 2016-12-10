#include <vector>
#include "times.h"
#include "../utils/vectorUtilities.h"

namespace spline{

    Basis timesSubBasis(const BSplineBasis& b1 , const BSplineBasis& b2){
        int newDegree = b1.getDegree() + b2.getDegree();
        AnyVector v = unionKnots(b1.getKnots(), b2.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
        return BSplineBasis(v, newDegree) ;
    }

    Basis timesSubBasis(const BSplineBasis& b1 , const MonomialBasis& b2){
        //    TODO experimantal implementation
        int newDegree = b1.getDegree() + b2.getDegree();
        AnyVector v = unionKnots(b1.getKnots(), b1.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
        return BSplineBasis(v, newDegree) ;
    }

    Basis timesSubBasis(const MonomialBasis& b1 , const BSplineBasis& b2){
        return timesSubBasis(b2, b1);
    }

    Basis timesSubBasis(const MonomialBasis& b1 , const MonomialBasis& b2){
        int newDegree = b1.getDegree() + b2.getDegree();
        return MonomialBasis(newDegree);
    }

}
