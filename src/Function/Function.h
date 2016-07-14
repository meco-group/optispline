//
// Created by erik on 11/04/16.
//

#ifndef CPP_SPLINE_FUNCTION_FUNCTION_H
#define CPP_SPLINE_FUNCTION_FUNCTION_H

#include <casadi/casadi.hpp>
#include <string>
#include "../Basis/Basis.h"
#include "../Coefficients/Coefficient.h"
namespace spline {
    typedef DT AnyTensor;
    class Function {

    public :
        Function( const Basis& basis, const Coefficient& coef) : basis(basis), coef(coef) {} 
        const Basis& basis;
        const Coefficient& coef;

        template<class T >
        Tensor< T > operator()(const std::vector< T >& x) const{
            return basis(x)*coef.data;
        }
    };
} // namespace spline

#endif //CPP_SPLINE_FUNCTION_FUNCTION_H
