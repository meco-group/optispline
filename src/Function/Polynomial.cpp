#include "Polynomial.h"
#include "../Basis/MonomialeBasis.h"
#include "../Basis/CommonBasis.h"

namespace spline{

    Polynomial::Polynomial (const std::vector< double >& coef) : 
        Function(MonomialeBasis(coef.size()-1), Coefficient(coef)) {
    }

    Polynomial::Polynomial(const std::vector< double >& coef, const Argument argument) : 
        Function(MonomialeBasis(coef.size()-1, argument), Coefficient(coef)) {
    }

									
    Polynomial::Polynomial(const std::vector< double >& coef, const std::string& argumentName) :
        Function(MonomialeBasis(coef.size()-1, Argument(argumentName)), Coefficient(coef)) {
    }
    // Polynomial::Polynomial (const std::vector< double >& coef) { 
    //     basis =  MonomialeBasis(static_cast<int>(coef.size())-1);
    //     coef =  Coefficient(new AnyTensor(coef,{static_cast<int>(coef.size()),1,1}))) ;
    // }
		
} // namespace spline
