#include <vector>
#include "Function.h"
#include "../Basis/utils/EvaluationGrid.h"
#include "../common.h"
namespace spline {

    Function Constant(const TensorBasis& basis, const AnyScalar& a, const std::vector< int >& size) const {
        AnyTensor value = repeat(AnyTensor(a), size);
        return Function::Constant(value, basis);
    }

    Function Constant(const TensorBasis& basis, const AnyTensor& t)  const {
        std::vector< Basis > subbasis = TensorBasis.getSubBasis();
        Function f = Function::Constant(t, subbasis[0]);
        for (int i = 1; i < TensorBasis.getNumberOfSubBasis(); i++) {
            f *= Function::Constant(value, size, subbasis[i]);
        }
        return f;
    }

    Function Constant(const Basis& basis, const AnyScalar& a, const std::vector< int >& size) const {
        AnyTensor value = repeat(AnyTensor(a), size);
        return Function:::Constant(value, basis);
    }

    Function Constant(const Basis& basis, const AnyTensor& t) const {
        Coefficient coeff = Coefficient(basis.const_coeff_tensor(t));
        return Function(basis, coeff);
    }

    AnyTensor Function::operator()(const std::vector< AnyScalar >& x) const {
        return basis(x).inner(coef.getData());
    }

    MX Function::operator<=(const MX& x) const {
      return getCoefficient().getData().as_MT().data()<=x;
    }
    MX Function::operator>=(const MX& x) const {
      return getCoefficient().getData().as_MT().data()>=x;
    }

    Basis Function::getBasis() const {
      spline_assert_message(basis.n_basis()==1,
          ".getBasis() syntax only works for a 1-D TensorBasis.");
      return basis.getSubBasis()[0];
    }
    Basis Function::getBasis(const Index& index) const {
      return basis.getBasis(index);
    }

    Function Function::generic_operation(const Function& f,
        const BasisComposition & bc, const TensorComposition & tc) const  {

      TensorBasis sumBasis = bc(getTensorBasis(), f.getTensorBasis());
      EvaluationGrid evaluationGrid = EvaluationGrid(sumBasis);
      std::vector< AnyTensor > basisEvaluated;
      std::vector< AnyTensor > thisFunctionEvaluated;
      std::vector< AnyTensor > otherFunctionEvaluated;
      std::vector< AnyTensor > sumFunctionEvaluated;

      basisEvaluated = evaluationGrid.evaluateEvaluationGrid();
      thisFunctionEvaluated = evaluationGrid.evaluateEvaluationGrid(*this);
      otherFunctionEvaluated = evaluationGrid.evaluateEvaluationGrid(f);

      for (int i = 0; i < basisEvaluated.size(); i++) {
          sumFunctionEvaluated.push_back(tc(thisFunctionEvaluated[i], otherFunctionEvaluated[i]));
      }

      AnyTensor A = AnyTensor::pack(basisEvaluated, 0);
      AnyTensor B = AnyTensor::pack(sumFunctionEvaluated, 0);

      int numberEval = basisEvaluated.size();
      int numberBasis = sumBasis.totalNumberBasisFunctions();
      int numberCoef = coef.getNumberCoefficents();

      std::vector< int > shapeA = {numberEval, numberBasis};
      std::vector< int > shapeB = {numberBasis, numberCoef};
      A = A.shape(shapeA);
      B = B.shape(shapeB);
      AnyTensor C = A.solve(B);

      std::vector< int > shapeCoef = coef.dimension();
      std::vector< int > shape = sumBasis.dimension();
      shape.insert(shape.end(), shapeCoef.begin(), shapeCoef.end());
      C = C.shape(shape);
      return Function(sumBasis, C);
    }

    Function Function::operator+(const Function& f) const {
      return generic_operation(f,
          [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs + rhs; },
          [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs + rhs; });
    }

    Function Function::operator+(const AnyScalar& a) const {
        return operator+(Function::Constant(a, this->getTensorBasis(), this->shape()));
    }

    Function Function::operator+(const AnyTensor& t) const {
        return operator+(Function::Constant(t, this->getTensorBasis()));
    }

    Function Function::operator*(const Function& f) const {
      return generic_operation(f,
          [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
          [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs * rhs; });
    }


    Function Function::operator-(const Function& f) const {
        return operator+(-f);
    }

    Function Function::operator-() const {
        return Function(basis, -coef);
    }

    std::string Function::getRepresentation() const {return "Function";};

    int Function::n_inputs() const{
        return basis.n_inputs();
    }

    std::vector< int > Function::shape() const{
        return coef.shape();
    }

}  // namespace spline
