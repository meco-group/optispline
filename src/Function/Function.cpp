#include <vector>
#include "Function.h"
#include "../Basis/utils/EvaluationGrid.h"

namespace spline {

    AnyTensor Function::operator()(const std::vector< AnyScalar >& x) const{
        return basis(x).inner(coef.data);
    }

    Function Function::operator+(Function f){
        Basis sumBasis = getBasis() + f.getBasis();
        EvaluationGrid evaluationGrid = EvaluationGrid(sumBasis);
        std::vector< AnyTensor > basisEvaluated;
        std::vector< AnyTensor > thisFunctionEvaluated;
        std::vector< AnyTensor > otherFunctionEvaluated;
        std::vector< AnyTensor > sumFunctionEvaluated;

        basisEvaluated = evaluationGrid.evaluateEvaluationGrid();
        thisFunctionEvaluated = evaluationGrid.evaluateEvaluationGrid(*this);
        otherFunctionEvaluated = evaluationGrid.evaluateEvaluationGrid(f);

        for(int i = 0; i < basisEvaluated.size(); i++){
            sumFunctionEvaluated.push_back(thisFunctionEvaluated[i] + otherFunctionEvaluated[i]);
        }

        AnyTensor A = AnyTensor::pack(basisEvaluated, 0);
        AnyTensor B = AnyTensor::pack(sumFunctionEvaluated, 0);

        int numberEval = basisEvaluated.size();
        int numberBasis = sumBasis.totalNumberSubBasis();
        int numberCoef = coef.getNumberCoefficents();

        std::vector< int > shapeA = {numberEval,numberBasis};
        std::vector< int > shapeB = {numberBasis,numberCoef};
        A = A.shape(shapeA);
        B = B.shape(shapeB);
        AnyTensor C = AnyTensor::solve(A,B);

        std::vector< int > shapeCoef = coef.getShape();
        std::vector< int > shape = sumBasis.getShape();
        shape.insert(shape.end(), shapeCoef.begin(), shapeCoef.end());
        C = C.shape(shape);
        return Function(sumBasis,C);
    }


    Function Function::operator*(Function f){
        Basis sumBasis = getBasis() * f.getBasis();
        EvaluationGrid evaluationGrid = EvaluationGrid(sumBasis);
        std::vector< AnyTensor > basisEvaluated;
        std::vector< AnyTensor > thisFunctionEvaluated;
        std::vector< AnyTensor > otherFunctionEvaluated;
        std::vector< AnyTensor > sumFunctionEvaluated;

        basisEvaluated = evaluationGrid.evaluateEvaluationGrid();
        thisFunctionEvaluated = evaluationGrid.evaluateEvaluationGrid(*this);
        otherFunctionEvaluated = evaluationGrid.evaluateEvaluationGrid(f);

        for(int i = 0; i < basisEvaluated.size(); i++){
            sumFunctionEvaluated.push_back(thisFunctionEvaluated[i] * otherFunctionEvaluated[i]);
        }

        AnyTensor A = AnyTensor::pack(basisEvaluated, 0);
        AnyTensor B = AnyTensor::pack(sumFunctionEvaluated, 0);

        int numberEval = basisEvaluated.size();
        int numberBasis = sumBasis.totalNumberSubBasis();
        int numberCoef = coef.getNumberCoefficents();

        std::vector< int > shapeA = {numberEval,numberBasis};
        std::vector< int > shapeB = {numberBasis,numberCoef};
        A = A.shape(shapeA);
        B = B.shape(shapeB);
        AnyTensor C = AnyTensor::solve(A,B);

        std::vector< int > shapeCoef = coef.getShape();
        std::vector< int > shape = sumBasis.getShape();
        shape.insert(shape.end(), shapeCoef.begin(), shapeCoef.end());
        C = C.shape(shape);
        return Function(sumBasis,C);
    }

}  // namespace spline

