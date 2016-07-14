//
// Created by erik on 11/04/16.
//

#ifndef CPP_SPLINE_FUNCTION_FUNCTION_H
#define CPP_SPLINE_FUNCTION_FUNCTION_H

#include <casadi/casadi.hpp>
#include <string>
#include "../Basis/Basis.h"
#include "../Coefficients/Coefficient.h"
#include <tensor.hpp>
namespace spline {
    typedef DT AnyTensor;
    typedef double AnyScalar;
    class Function {

    public :
        Function( const Basis& basis, const Coefficient& coef) : basis(basis), coef(coef) {} 
        const Basis& basis;
        const Coefficient& coef;

        // AnyTensor operator()(const std::vector< double >& x) const{
        //     return basis(x)*coef.data;
        // }
    
        AnyTensor operator()(const std::vector< AnyScalar >& x) const{
            return basis(x)*coef.data;
        }
    };
} // namespace spline

#endif //CPP_SPLINE_FUNCTION_FUNCTION_H
