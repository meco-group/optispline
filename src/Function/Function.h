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

        Function operator+(const Function f) const;
        
        const Basis& getBasis() const {return basis;}
        Coefficient getCoefficient() {return coef;}
        const Argument& getArgument () const{ return getBasis().getArgument();}
        // Argument& getArgument (){ return getBasis().getArgument();}
    public:
        const Basis basis;
        Coefficient coef;
    };
} // namespace spline

#endif //CPP_SPLINE_FUNCTION_FUNCTION_H
