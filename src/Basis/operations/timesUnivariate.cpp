#include <vector>
#include "timesUnivariate.h"
#include "../utils/vectorUtilities.h"

namespace spline{

    SubBasis timesBasis(const SubBSplineBasis& b1 , const SubBSplineBasis& b2){
        int newDegree = b1.getDegree() + b2.getDegree();
        const std::vector<double> v = unionKnots(b1.getKnots(), b2.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
        return SubBSplineBasis(v, newDegree) ;
    }

    SubBasis timesBasis(const SubBSplineBasis& b1 , const SubMonomialBasis& b2){
        //    TODO experimantal implementation
        int newDegree = b1.getDegree() + b2.getDegree();
        const std::vector<double> v = unionKnots(b1.getKnots(), b1.getKnots(), newDegree, b1.getDegree(), b2.getDegree());
        return SubBSplineBasis(v, newDegree) ;
    }

    SubBasis timesBasis(const SubMonomialBasis& b1 , const SubBSplineBasis& b2){
        return timesBasis(b2, b1);
    }

    SubBasis timesBasis(const SubMonomialBasis& b1 , const SubMonomialBasis& b2){
        int newDegree = b1.getDegree() + b2.getDegree();
        return SubMonomialBasis(newDegree);
    }

}