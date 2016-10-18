#ifndef CPP_SPLINE_FUNCTION_FUNCTION_H
#define CPP_SPLINE_FUNCTION_FUNCTION_H

#include <casadi/casadi.hpp>
#include <string>
#include "../Basis/Basis.h"
#include "../Coefficients/Coefficient.h"
#include <any_tensor.hpp>
namespace spline {
    class Function {


    public :
        Function( const Basis& basis, const Coefficient& coef) : basis(basis), coef(coef) {}

        AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        Function value(const OptistackSolver& sol) const;

        Function operator+(const Function f) const ;
        Function operator*(const Function f) const ;
        Function operator-() const ;
        Function operator-(const Function f) const ;
        Basis getBasis() const {return basis;}
        Coefficient getCoefficient() const {return coef;}
        // Argument& getArgument (){ return getBasis().getArgument();}
    public:
        Basis basis;
        Coefficient coef;

    private:
      typedef std::function<Basis(const Basis&, const Basis&)> BasisComposition;
      typedef std::function<AnyTensor(const AnyTensor&, const AnyTensor&)> TensorComposition;
      Function generic_operation(const Function& f,
          const BasisComposition & bc, const TensorComposition & tc) const ;
    };
} // namespace spline

#endif //CPP_SPLINE_FUNCTION_FUNCTION_H
