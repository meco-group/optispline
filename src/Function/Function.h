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

        AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        const Basis& getBasis(){return basis;}
        const Coefficient& getCoefficient(){return coef;}
    public:
        const Basis basis;
        const Coefficient coef;
    };
} // namespace spline

#endif //CPP_SPLINE_FUNCTION_FUNCTION_H
