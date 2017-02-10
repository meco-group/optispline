#include "Polynomial.h"
#include "../Basis/MonomialBasis.h"

namespace spline {

    Polynomial::Polynomial(const std::vector< double >& coef) :
        Function(MonomialBasis(coef.size()-1), Coefficient(coef)) {
    }

    Polynomial::Polynomial(const std::vector< double >& coef, Argument a) :
        Function(MonomialBasis(coef.size()-1), Coefficient(coef)) {
        getTensorBasis().setArguments(std::vector< Argument > {a});
    }

    Polynomial::Polynomial(const std::vector< double >& coef, const std::string &name) :
        Function(MonomialBasis(coef.size()-1), Coefficient(coef)) {
        std::vector< Argument > argumentList = { Argument(name)};
        getTensorBasis().setArguments(argumentList);
    }

} // namespace spline
