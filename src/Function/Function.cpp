#include <vector>
#include "Function.h"
#include "../Basis/utils/EvaluationGrid.h"

namespace spline {

    AnyTensor Function::operator()(const std::vector< AnyScalar >& x) const{
        return basis(x).inner(coef.data);
    }

    Function Function::value(const OptistackSolver& sol) const {
      return Function(basis, coef.value(sol));
    }

    Function Function::generic_operation(const Function& f,
        const BasisComposition & bc, const TensorComposition & tc) const  {

      Basis sumBasis = bc(getBasis(), f.getBasis());
      EvaluationGrid evaluationGrid = EvaluationGrid(sumBasis);
      std::vector< AnyTensor > basisEvaluated;
      std::vector< AnyTensor > thisFunctionEvaluated;
      std::vector< AnyTensor > otherFunctionEvaluated;
      std::vector< AnyTensor > sumFunctionEvaluated;

      basisEvaluated = evaluationGrid.evaluateEvaluationGrid();
      thisFunctionEvaluated = evaluationGrid.evaluateEvaluationGrid(*this);
      otherFunctionEvaluated = evaluationGrid.evaluateEvaluationGrid(f);

      for (int i = 0; i < basisEvaluated.size(); i++){
          sumFunctionEvaluated.push_back(tc(thisFunctionEvaluated[i], otherFunctionEvaluated[i]));
      }

      AnyTensor A = AnyTensor::pack(basisEvaluated, 0);
      AnyTensor B = AnyTensor::pack(sumFunctionEvaluated, 0);

      int numberEval = basisEvaluated.size();
      int numberBasis = sumBasis.totalNumberSubBasis();
      int numberCoef = coef.getNumberCoefficents();

      std::vector< int > shapeA = {numberEval, numberBasis};
      std::vector< int > shapeB = {numberBasis, numberCoef};
      A = A.shape(shapeA);
      B = B.shape(shapeB);
      AnyTensor C = A.solve(B);

      std::vector< int > shapeCoef = coef.getShape();
      std::vector< int > shape = sumBasis.getShape();
      shape.insert(shape.end(), shapeCoef.begin(), shapeCoef.end());
      C = C.shape(shape);
      return Function(sumBasis, C);
    }

    Function Function::operator+(Function f) const {
      return generic_operation(f,
          [](const Basis& lhs, const Basis& rhs) { return lhs + rhs; },
          [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs + rhs; });
    }


    Function Function::operator*(Function f) const {
      return generic_operation(f,
          [](const Basis& lhs, const Basis& rhs) { return lhs * rhs; },
          [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs * rhs; });
    }


    Function Function::operator-(Function f) const {
        return operator+(-f);
    }

    Function Function::operator-() const {
        return Function(basis, -coef);
    }

}  // namespace spline
