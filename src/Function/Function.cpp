#include <vector>
#include "Function.h"

namespace spline {

    AnyTensor Function::operator()(const std::vector< AnyScalar >& x) const{
        return basis(x).inner(coef.data);
    }

    Function Function::operator+(Function f){
        // Basis b = getBasis() + f.getBasis();
        Basis b = getBasis() + f.getBasis();
        Coefficient coef = Coefficient(std::vector< double > {});
        return Function(b,coef);
    }

}  // namespace spline

