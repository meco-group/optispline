#include "Polynomial.h"
#include "../Basis/MonomialeBasis.h"
#include "../Basis/CommonBasis.h"

namespace spline{

    Polynomial::Polynomial (const std::vector< double >& coef) : 
        Function(MonomialeBasis(coef.size()-1,
                 AnyTensor(coef,{coef.size(),1,1}){
    }
} // namespace spline
