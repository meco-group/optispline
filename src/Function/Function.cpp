#include <vector>
#include "Function.h"

namespace spline {

    AnyTensor Function::operator()(const std::vector< AnyScalar >& x) const{
        return basis(x).inner(coef.data);
    }

    const Function Function::operator+(const Function f) const{
        // Basis b = getBasis() + f.getBasis();
        Basis b = basis + f.basis;
        Coefficient coef = Coefficient(std::vector< double > {});
        return Function(b,coef);
    }

    Argument& Function::getArgument (){
        return getBasis().getArgument();
    }
}  // namespace spline

