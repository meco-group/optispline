#include <vector>
#include "Function.h"
#include "../Basis/utils/EvaluationGrid.h"

namespace spline {

    AnyTensor Function::operator()(const std::vector< AnyScalar >& x) const{
        return basis(x).inner(coef.data);
    }

    Function Function::operator+(Function f){
        Basis b = getBasis() + f.getBasis();
        EvaluationGrid evaluationGrid = EvaluationGrid(b);
        std::vector< AnyTensor > basisEvaluated;
        std::vector< AnyTensor > thisFunctionEvaluated;
        std::vector< AnyTensor > otherFunctionEvaluated;
        std::vector< AnyTensor > sumFunctionEvaluated;

        evaluationGrid.evaluateEvaluationGrid(&basisEvaluated);
        evaluationGrid.evaluateEvaluationGrid(&thisFunctionEvaluated, *this);
        evaluationGrid.evaluateEvaluationGrid(&otherFunctionEvaluated, f);

        for(int i = 0; i < basisEvaluated.size(); i++){
            sumFunctionEvaluated.push_back(thisFunctionEvaluated[i] + otherFunctionEvaluated[i]);
        }

        std::vector< int > shape;


        AnyTensor A = AnyTensor::pack(basisEvaluated, 0);
        AnyTensor B = AnyTensor::pack(sumFunctionEvaluated, 0);

        AnyTensor C = AnyTensor::solve(A,B);

        return Function(b,C);




            // casadi::DM::solve(A, B);
    }

}  // namespace spline

