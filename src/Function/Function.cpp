#include <vector>
#include "Function.h"
#include "../Basis/utils/EvaluationGrid.h"

namespace spline {

    AnyTensor Function::operator()(const std::vector< AnyScalar >& x) const{
        return basis(x).inner(coef.data);
    }

    Function Function::operator+(Function f){
        // Basis b = getBasis() + f.getBasis();
        Basis b = getBasis();
        // Coefficient coef = Coefficient(std::vector< double > {});
        EvaluationGrid evaluationGrid = EvaluationGrid(b);
        // return Function(b,coef);
    }

}  // namespace spline

