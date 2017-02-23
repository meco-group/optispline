#include "Polynomial.h"
#include "../Basis/MonomialBasis.h"

namespace spline {

    Polynomial::Polynomial(const std::vector< double >& coef) :
        Function(MonomialBasis(coef.size()-1), Coefficient(coef)) {
    }

    Polynomial::Polynomial(const std::vector< double >& coef, std::string& a) :
        Function(TensorBasis(MonomialBasis(coef.size()-1), std::vector< std::string > {a}), Coefficient(coef)) { }

    Polynomial::Polynomial(const std::vector< double >& coef, const std::string &name) :
        Function(TensorBasis(MonomialBasis(coef.size()-1), std::vector< std::string > { std::string(name)}), Coefficient(coef)) {
    }

} // namespace spline
