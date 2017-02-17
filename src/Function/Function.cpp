#include <vector>
#include "Function.h"
#include "../Basis/utils/EvaluationGrid.h"
#include "../common.h"
namespace spline {
    Function::Function(const TensorBasis& basis, const Coefficient& coef) {
        init(basis, coef);
    }

    Function::Function(const Basis& basis, const Coefficient& coef) {
        init(TensorBasis(basis), coef);
    }

    void Function::init(const TensorBasis& basis_, const Coefficient& coef_) {
        std::vector< int > dim_basis = basis_.dimension();
        std::vector< int > dim_coef = coef_.getData().dims();

        int total_size_coef = dim_coef.size();
        spline_assert_message(dim_basis.size() <= total_size_coef,
          "Dimensions of basis " << dim_basis << " and coefficient " <<
          dim_coef << " can not be connected.");
        spline_assert_message(dim_basis.size() + 2 >= total_size_coef,
          "Dimensions of basis " << dim_basis << " and coefficient " <<
          dim_coef << " can not be connected.");
        for (int i = 0; i < dim_basis.size(); i++) {
            spline_assert_message(dim_basis[i] == dim_coef[i],
                "Mismatch of dimention " + std::to_string(i) + " between basis and coefficient: "
                << "Got basis " << dim_basis << " and coefficient" << dim_coef <<".");
        }

        basis = basis_;
        coef = coef_.add_trival_dimension(2 + dim_basis.size() - dim_coef.size());
    }

    Function Function::Constant(const TensorBasis& basis, const AnyScalar& a, const std::vector< int >& size) {
        AnyTensor value = AnyTensor::repeat(AnyTensor(a), size);
        return Function::Constant(basis, value);
    }

    Function Function::Constant(const TensorBasis& basis, const AnyTensor& t) {
        Coefficient coeff = Coefficient(basis.const_coeff_tensor(t));
        return Function(basis, coeff);
    }

    Function Function::Constant(const Basis& basis, const AnyScalar& a, const std::vector< int >& size) {
        AnyTensor value = AnyTensor::repeat(AnyTensor(a), size);
        return Function::Constant(basis, value);
    }

    Function Function::Constant(const Basis& basis, const AnyTensor& t) {
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
          AnyTensor lhs = thisFunctionEvaluated[i];
          AnyTensor rhs = otherFunctionEvaluated[i];
          if(lhs.dims() == std::vector< int > {1, 1}){
              lhs = lhs.shape({});
          }
          if(rhs.dims() == std::vector< int > {1, 1}){
              rhs = rhs.shape({});
          }
          sumFunctionEvaluated.push_back(tc(lhs, rhs));
      }

      AnyTensor A = AnyTensor::pack(basisEvaluated, 0);
      AnyTensor B = AnyTensor::pack(sumFunctionEvaluated, 0);

      int numberEval = basisEvaluated.size();
      int numberBasis = sumBasis.totalNumberBasisFunctions();
      std::vector< int > elemShape = sumFunctionEvaluated[0].dims();
      int numberCoef = (elemShape.size() == 0)? 1: elemShape[0]*elemShape[1];

      std::vector< int > shapeA = {numberEval, numberBasis};
      std::vector< int > shapeB = {numberBasis, numberCoef};
      A = A.shape(shapeA);
      B = B.shape(shapeB);
      AnyTensor C = A.solve(B);

      std::vector< int > shapeCoef = elemShape;
      std::vector< int > shapeBasis = sumBasis.dimension();
      shapeBasis.insert(shapeBasis.end(), shapeCoef.begin(), shapeCoef.end());

      C = C.shape(shapeBasis);
      return Function(sumBasis, C);
    }

    Function Function::operator+(const Function& f) const {
      return generic_operation(f,
          [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs + rhs; },
          [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs + rhs; });
    }

    Function Function::operator+(const AnyTensor& t) const {
        if (t.is_scalar() && !is_scalar()) return operator+(AnyTensor::repeat(t.as_scalar(), shape()));
        return operator+(Function::Constant(this->getTensorBasis(), t));
    }

    Function Function::operator*(const Function& f) const {
      return generic_operation(f,
          [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
          [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs * rhs; });
    }

    Function Function::operator*(const AnyTensor& t) const {
        if (t.is_scalar() && !is_scalar()) return operator*(AnyTensor::repeat(t.as_scalar(), shape()));
        
        std::vector< int > tdims = getCoeffTensor().dims();
        tdims.pop_back();
        tdims.pop_back();

        AnyTensor transf = AnyTensor::repeat(AnyScalar(1), tdims);
        transf = transf.outer_product(t);
        AnyTensor data = getCoeffTensor()*transf;
        
        return Function(getTensorBasis(), Coefficient(data));
    }

    Function Function::mtimes(const Function& f) const {
      return generic_operation(f,
          [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
          [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs);});
    }

    Function Function::mtimes(const AnyTensor& t) const {
        if (t.is_scalar() && !is_scalar()) return operator*(AnyTensor::repeat(t.as_scalar(), t.dims()));
        spline_assert(t.n_dims() == 2);
        Coefficient c = getCoefficient();
        int dir = n_inputs() + 1; //0 based, 2nd matrix dimension
        return Function(getTensorBasis(), c.transform(t.reorder_dims({1,0}), dir));
    }

    Function Function::rmtimes(const AnyTensor& t) const {
        if (t.is_scalar()) return operator*(t);
        return Function::Constant(this->getTensorBasis(), t).mtimes(*this);
    }

    Function Function::pow(int power) const {
        spline_assert(power >= 0);
        Function f = *this;
        Function fpow = *this;

        if(power == 0){
            fpow = Function::Constant(getTensorBasis(), 1, shape());
        } else {
            for (int i = 1; i < power; i++) {
                fpow = fpow*f;
            }
        }

        return fpow;
    }

    Function Function::operator-(const Function& f) const {
        return operator+(-f);
    }

    Function Function::operator-(const AnyTensor& t) const {
        return operator+(-t);
    }

    Function Function::operator-() const {
        return Function(basis, -coef);
    }

    Function Function::transpose() const {
        return Function(getTensorBasis(), getCoefficient().transpose());
    }

    std::string Function::getRepresentation() const {return "Function";};

    int Function::n_inputs() const {
        return basis.n_inputs();
    }

    std::vector< int > Function::shape() const {
        return coef.shape();
    }

    Function Function::insert_knots(const AnyVector & new_knots) const {
      spline_assert_message(getTensorBasis().n_basis() == 1,
        "I don't know the direction for knot insertion. Please supply argument.")
      return insert_knots(std::vector<AnyVector>{new_knots}, std::vector<NumericIndex>{0});
    }

    Function Function::insert_knots(const AnyVector & new_knots,
      const NumericIndex & arg_ind) const {
      return insert_knots(std::vector<AnyVector>{new_knots}, std::vector<NumericIndex>{arg_ind});
    }

    Function Function::insert_knots(const AnyVector & new_knots, const Argument & arg) const {
      return insert_knots(std::vector<AnyVector>{new_knots}, std::vector<Argument>{arg});
    }

    Function Function::insert_knots(const std::vector<AnyVector> & new_knots,
      const std::vector<Argument> & arg) const {
      std::vector<NumericIndex> arg_ind(arg.size());
      for (int i=0; i<arg.size(); i++) {
        arg_ind[i] = getTensorBasis().indexArgument(arg[i]);
      }
      return insert_knots(new_knots, arg_ind);
    }

    Function Function::insert_knots(const std::vector<AnyVector> & new_knots,
      const std::vector<NumericIndex> & arg_ind) const {
      spline_assert(arg_ind.size() == new_knots.size())
      std::vector<AnyTensor> T;
      TensorBasis tbasis = getTensorBasis();
      TensorBasis new_tbasis = tbasis.insert_knots(new_knots, T, arg_ind);
      Coefficient new_coefficient = getCoefficient().transform(T, arg_ind);
      return Function(new_tbasis, new_coefficient);
    }

    Function Function::midpoint_refinement(int refinement) const {
      // apply on all directions
      std::vector<NumericIndex> arg_ind(getTensorBasis().n_basis());
      std::vector<int> refs(getTensorBasis().n_basis());
      for (int k=0; k<arg_ind.size(); k++) {
        arg_ind[k] = k;
        refs[k] = refinement;
      }
      return midpoint_refinement(refs, arg_ind);
    }

    Function Function::midpoint_refinement(int refinement, const NumericIndex & arg_ind) const {
      return midpoint_refinement(std::vector<int>{refinement}, std::vector<NumericIndex>{arg_ind});
    }

    Function Function::midpoint_refinement(int refinement, const Argument & arg) const {
      return midpoint_refinement(std::vector<int>{refinement}, std::vector<Argument>{arg});
    }

    Function Function::midpoint_refinement(const std::vector<int> & refinement,
      const std::vector<Argument> & arg) const {
      std::vector<NumericIndex> arg_ind(arg.size());
      for (int i=0; i<arg.size(); i++) {
        arg_ind[i] = getTensorBasis().indexArgument(arg[i]);
      }
      return midpoint_refinement(refinement, arg_ind);
    }

    Function Function::midpoint_refinement(const std::vector<int> & refinement,
      const std::vector<NumericIndex> & arg_ind) const {
      spline_assert(arg_ind.size() == refinement.size())
      std::vector<AnyTensor> T;
      TensorBasis tbasis = getTensorBasis();
      TensorBasis new_tbasis = tbasis.midpoint_refinement(refinement, T, arg_ind);
      Coefficient new_coefficient = getCoefficient().transform(T, arg_ind);
      return Function(new_tbasis, new_coefficient);
    }

    Function Function::derivative(const std::vector<NumericIndex>& direction_ind) const {
        // default derivative is with order = 1
        std::vector<int> orders(direction_ind.size(), 1);
        return derivative(orders, direction_ind);
    }

    Function Function::derivative(const std::vector<Argument>& directions) const {
        // default derivative is with order = 1
        std::vector<int> orders(directions.size(), 1);
        std::vector<NumericIndex> direction_ind(directions.size());
        for (int i=0; i<directions.size(); i++){
            direction_ind[i] = getTensorBasis().indexArgument(directions[i]);
        }
        return derivative(orders, direction_ind);
    }

    Function Function::derivative(const std::vector<int>& orders, const std::vector<Argument>& directions) const {
        std::vector<NumericIndex> direction_ind(directions.size());
        for (int i=0; i<directions.size(); i++){
            direction_ind[i] = getTensorBasis().indexArgument(directions[i]);
        }
        return derivative(orders, direction_ind);
    }

    Function Function::derivative(const std::vector<int>& orders, const std::vector<NumericIndex>& direction_ind) const {
        spline_assert(orders.size() == direction_ind.size())  // each direction should have an order
        std::vector<AnyTensor> T;
        TensorBasis tbasis = getTensorBasis();
        TensorBasis new_tbasis = tbasis.derivative(orders, direction_ind, T);
        std::vector<NumericIndex> directions(direction_ind.size());
        Coefficient new_coefficient = getCoefficient().transform(T, direction_ind);
        return Function(new_tbasis, new_coefficient);
    }

}  // namespace spline
