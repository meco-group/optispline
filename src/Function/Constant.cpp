#include <vector>
#include "Function.h"
#include "../Basis/utils/EvaluationGrid.h"
#include "../Basis/TensorBasisConstant.h"
#include "../common.h"

namespace spline {

    Constant::Constant(const AnyTensor& c) {
        spline_assert_message(c.dims().size() <= 2, "Constant has dimention higher than 2");
        /* std::vector< int > new_dims = c.dims(); */
        /* new_dims.insert(new_dims.begin(), 1); */
        /* coeff_ = c.shape(new_dims); */
        coeff_ = c;
    }

    AnyTensor Constant::operator()(const AnyTensor& x, const std::vector< std::string >& args) const{
        spline_assert_message(false, "not implemented: evaluation conastan");
        return packed_tensor.einstein(coeff().data(), a_r, b_r, c_r).squeeze();
    }

    std::string Constant::type() const{
        return "Constant";
    }

    std::string Constant::to_string() const{
        return "Constant\t" + coeff_.to_string();
    }

    GenericFunction Constant::operator+(const Function& f) const {
        return generic_operation(f,
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs + rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs + rhs; });
    }

    GenericFunction Constant::operator+(const AnyTensor& t) const {
        if (t.is_scalar() && t.dims()!=shape())
            return operator+(AnyTensor::repeat(t.as_scalar(), shape()));
        return operator+(Constant::Constant(this->tensor_basis(), t));
    }

    GenericFunction Constant::operator*(const Function& f) const {
        return generic_operation(f,
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs * rhs; });
    }

    GenericFunction Constant::operator*(const AnyTensor& rhs) const {
        Function f = *this;
        AnyTensor t = rhs;
        homogenize_args(f, t);

        std::vector< int > tdims = f.coeff_tensor().dims();
        tdims.pop_back();
        tdims.pop_back();

        AnyTensor transf = AnyTensor::repeat(AnyScalar(1), tdims);
        transf = transf.outer_product(t);
        AnyTensor data = f.coeff_tensor()*transf;

        return Function(f.tensor_basis(), Coefficient(data));
    }

    GenericFunction Constant::mtimes(const Function& f) const {
        return generic_operation(f,
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs);});
    }

    GenericFunction Constant::mtimes(const AnyTensor& t) const {
        if (t.is_scalar()) return operator*(t);
        spline_assert(t.n_dims() == 2);
        Coefficient c = coeff();
        int dir = n_inputs() + 1; //0 based, 2nd matrix dimension
        return Function(tensor_basis(), c.transform(t.reorder_dims({1, 0}), dir));
    }

    GenericFunction Constant::rmtimes(const AnyTensor& t) const {
        if (t.is_scalar()) return operator*(t);
        return Constant::Constant(this->tensor_basis(), t).mtimes(*this);
    }

    GenericFunction Constant::operator-() const {
        return Function(basis_, -coeff_);
    }

    GenericFunction Constant::transpose() const {
        return Function(tensor_basis(), coeff().transpose());
    }

    GenericFunction Constant::trace() const {
        std::vector< int > shape_ = shape();
        spline_assert_message(shape_[0] == shape_[1],
                "Trace only defined for square matrices. Dimensions are " << shape_ << ".");

        AnyTensor t = DT(casadi::DM::densify(casadi::DM::eye(shape_[0])));
        Function fdiag = operator*(t); //keep diagonal entries only

        Coefficient cdiag = fdiag.coeff();
        AnyTensor ones = AnyTensor::repeat(AnyScalar(1), std::vector< int >{1, shape_[0]});
        cdiag = cdiag.transform(ones, cdiag.dimension().size()); //sum over all columns
        cdiag = cdiag.transform(ones, cdiag.dimension().size()+1); //sum over all rows

        return Function(tensor_basis(), cdiag);
    }

    GenericFunction Constant::transform_to(const Basis& basis) const {
        return transform_to(TensorBasis(basis));
    }

    GenericFunction Constant::transform_to(const TensorBasis& basis) const {
        if(basis_.type() == "TensorBasisConstant"){
            AnyTensor T = coeff_.rm_direction( std::vector< int > {0} ).data();
            return Function(basis, basis.const_coeff_tensor(T));
        }

        TensorBasis unionBasis = tensor_basis() + basis;
        EvaluationGrid evaluationGrid = EvaluationGrid(unionBasis);
        std::vector< AnyTensor > basisEvaluated;
        std::vector< AnyTensor > thisFunctionEvaluated;

        basisEvaluated = evaluationGrid.eval();
        thisFunctionEvaluated = evaluationGrid.eval(*this);

        AnyTensor A = AnyTensor::pack(basisEvaluated, 0);
        AnyTensor B = AnyTensor::pack(thisFunctionEvaluated, 0);
        int numberEval = basisEvaluated.size();
        int numberBasis = unionBasis.totalNumberBasisFunctions();
        std::vector< int > elemShape = thisFunctionEvaluated[0].dims();
        int numberCoef = spline::product(elemShape);

        std::vector< int > shapeA = {numberEval, numberBasis};
        std::vector< int > shapeB = {numberBasis, numberCoef};
        A = A.shape(shapeA);
        B = B.shape(shapeB);
        AnyTensor C = A.solve(B);

        std::vector< int > shapeCoef = elemShape;
        std::vector< int > shapeBasis = unionBasis.dimension();
        shapeBasis.insert(shapeBasis.end(), shapeCoef.begin(), shapeCoef.end());

        C = C.shape(shapeBasis);
        return Function(unionBasis, C);
    }

    GenericFunction Constant::project_to(const Basis& basis) const {
        return project_to(TensorBasis(basis));
    }

    GenericFunction Constant::project_to(const TensorBasis& b) const {
        Function b2 = b.basis_functions();
        Function f = reshape(std::vector< int >{1,spline::product(shape())});

        Function b22 = b2.mtimes(b2.transpose());
        Function b2f = b2.mtimes(f); //f already is a row vector

        AnyTensor B22 = b22.integral();
        AnyTensor B2f = b2f.integral();

        AnyTensor C = B22.solve(B2f);

        std::vector< int > M = b.dimension();
        std::vector< int > N = shape();
        std::vector< int > shapeC = M;
        shapeC.insert(shapeC.end(), N.begin(), N.end());
        C = C.shape(shapeC);

        return Function(b,C);
    }

    GenericFunction Constant::reshape(const std::vector< int >& shape) const {
        return Function(tensor_basis(), coeff().reshape(shape));
    }

    GenericFunction Constant::slice(const AnySlice& i, const AnySlice& j) const {
        return Function(tensor_basis(), coeff_tensor().get_slice(i, j));
    }

    GenericFunction Constant::slice(const AnySlice& i) const {
        return Function(tensor_basis(), coeff_tensor().get_slice(i));
    }
}  // namespace spline
