#include "Polynomial.h"
#include "../Basis/MonomialeBasis.h"
#include "../Basis/utils/CommonBasis.h"

namespace spline{

    Polynomial::Polynomial (const std::vector< double >& coef) : 
        Function(*(new MonomialeBasis(coef.size()-1)), Coefficient(coef)) {
    }


} // namespace spline
