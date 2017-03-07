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

    std::string Polynomial::type() const{
    	return "Polynomial";
    }

    std::string Polynomial::to_string() const{
    	return "Polynomial, consisting of:\n" + tensor_basis().to_string() + "and:\n\t " + coeff().to_string();
    }

} // namespace spline
