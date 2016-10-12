#include <vector>
#include "Function.h"
#include "../Basis/utils/EvaluationGrid.h"

namespace spline {

    AnyTensor Function::operator()(const std::vector< AnyScalar >& x) const{
        return basis(x).inner(coef.data);
    }

    Function Function::operator+(Function f){
        Basis b = getBasis();
        EvaluationGrid evaluationGrid = EvaluationGrid(b);

        // return Function(b,coef);
        //
        //     casadi::DM Univariatestd::shared_ptr<Basis>::transformation( const std::shared_ptr<Basis> &b) const {
    //         std::vector<double> grid = evaluationGrid();
    //         casadi::DM A(evalstd::shared_ptr<Basis>(grid));
    //         casadi::DM B(b.evalstd::shared_ptr<Basis>(grid));
    //         return casadi::DM::solve(A, B);
//     }
    }

}  // namespace spline

