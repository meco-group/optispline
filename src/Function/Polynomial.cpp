#include "Polynomial.h"
#include "../Basis/MonomialeBasis.h"
#include "../Basis/CommonBasis.h"

namespace spline{

    Polynomial::Polynomial (const std::vector< double >& coef) : 
        Function(*(new MonomialeBasis(coef.size()-1)), Coefficient(coef)) {
    }

    Polynomial::Polynomial(const std::vector< double >& coef, const Argument argument) : 
        Function(*(new MonomialeBasis(coef.size()-1, argument)), Coefficient(coef)) {
    }

    Polynomial::Polynomial(const std::vector< double >& coef, const std::string& argumentName) :
        Function(*(new MonomialeBasis(coef.size()-1, Argument(argumentName))), Coefficient(coef)) {
    }
} // namespace spline
