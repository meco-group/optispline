#include "Polynomial.h"
#include "../Basis/MonomialBasis.h"

namespace spline {

    Polynomial::Polynomial(const std::vector< double >& coef) :
        Function(MonomialBasis(coef.size()-1), Coefficient(coef)) {
    }

    Polynomial::Polynomial(const std::vector< double >& coef, Argument& a) :
        Function(TensorBasis(MonomialBasis(coef.size()-1), std::vector< Argument > {a}), Coefficient(coef)) { }

    Polynomial::Polynomial(const std::vector< double >& coef, const std::string &name) :
        Function(TensorBasis(MonomialBasis(coef.size()-1), std::vector< Argument > { Argument(name)}), Coefficient(coef)) {
        /* std::vector< Argument > argumentList = { Argument(name)}; */
        /* getTensorBasis().setArguments(argumentList); */
    }

} // namespace spline
