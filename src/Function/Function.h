#ifndef CPP_SPLINE_FUNCTION_FUNCTION_H
#define CPP_SPLINE_FUNCTION_FUNCTION_H

#include <casadi/casadi.hpp>
#include <string>
#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include <any_tensor.hpp>
namespace spline {
    class Function {


    public :
        Function( const TensorBasis& basis, const Coefficient& coef) : basis(basis), coef(coef) {}
        Function( const Basis& basis, const Coefficient& coef) : basis(TensorBasis(basis)), coef(coef) {}


        AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        Function operator+(const Function f) const ;
        Function operator*(const Function f) const ;
        Function operator-() const ;
        Function operator-(const Function f) const ;
        TensorBasis getBasis() const {return basis;}
        Coefficient getCoefficient() const {return coef;}

        MX operator<=(const MX& x) const;
        MX operator>=(const MX& x) const;
        
        std::string getRepresentation() const ;
        void repr() const { userOut() << getRepresentation() << std::endl;}
        // Argument& getArgument (){ return getBasis().getArgument();}
    public:
        TensorBasis basis;
        Coefficient coef;

    private:
      typedef std::function<TensorBasis(const TensorBasis&, const TensorBasis&)> BasisComposition;
      typedef std::function<AnyTensor(const AnyTensor&, const AnyTensor&)> TensorComposition;
      Function generic_operation(const Function& f,
          const BasisComposition & bc, const TensorComposition & tc) const ;
    };
} // namespace spline

#endif //CPP_SPLINE_FUNCTION_FUNCTION_H
