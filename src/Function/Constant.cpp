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

    /* GenericFunction Constant::operator+(const Function& f) const { */
    /*     return generic_operation(f, */
    /*             [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs + rhs; }, */
    /*             [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs + rhs; }); */
    /* } */

    /* GenericFunction Constant::operator+(const AnyTensor& t) const { */
    /*     if (t.is_scalar() && t.dims()!=shape()) */
    /*         return operator+(AnyTensor::repeat(t.as_scalar(), shape())); */
    /*     return operator+(Constant::Constant(this->tensor_basis(), t)); */
    /* } */

    /* GenericFunction Constant::operator*(const Function& f) const { */
    /*     return generic_operation(f, */
    /*             [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; }, */
    /*             [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs * rhs; }); */
    /* } */

    /* GenericFunction Constant::operator*(const AnyTensor& rhs) const { */
    /*     Function f = *this; */
    /*     AnyTensor t = rhs; */
    /*     homogenize_args(f, t); */

    /*     std::vector< int > tdims = f.coeff_tensor().dims(); */
    /*     tdims.pop_back(); */
    /*     tdims.pop_back(); */

    /*     AnyTensor transf = AnyTensor::repeat(AnyScalar(1), tdims); */
    /*     transf = transf.outer_product(t); */
    /*     AnyTensor data = f.coeff_tensor()*transf; */

    /*     return Function(f.tensor_basis(), Coefficient(data)); */
    /* } */

    /* GenericFunction Constant::mtimes(const Constant& f) const { */
    /* } */

    /* GenericFunction Constant::mtimes(const Function& f) const { */
    /*     return generic_operation(f, */
    /*             [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; }, */
    /*             [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.mtimes(rhs);}); */
    /* } */

    /* GenericFunction Constant::mtimes(const AnyTensor& t) const { */
    /*     if (t.is_scalar()) return operator*(t); */
    /*     spline_assert(t.n_dims() == 2); */
    /*     Coefficient c = coeff(); */
    /*     int dir = n_inputs() + 1; //0 based, 2nd matrix dimension */
    /*     return Constant(c.transform(t.reorder_dims({1, 0}), dir)); */
    /* } */

    GenericFunction Constant::rmtimes(const AnyTensor& t) const {
        if (t.is_scalar()) return operator*(t);
        return Constant(t).mtimes(*this);
    }

    GenericFunction Constant::operator-() const {
        return Constant(-coeff_);
    }

    GenericFunction Constant::transpose() const {
        return Constant(coeff().transpose());
    }

    /* GenericFunction Constant::trace() const { */
    /*     std::vector< int > shape_ = shape(); */
    /*     spline_assert_message(shape_[0] == shape_[1], */
    /*             "Trace only defined for square matrices. Dimensions are " << shape_ << "."); */

    /*     AnyTensor t = DT(casadi::DM::densify(casadi::DM::eye(shape_[0]))); */
    /*     Function fdiag = operator*(t); //keep diagonal entries only */

    /*     Coefficient cdiag = fdiag.coeff(); */
    /*     AnyTensor ones = AnyTensor::repeat(AnyScalar(1), std::vector< int >{1, shape_[0]}); */
    /*     cdiag = cdiag.transform(ones, cdiag.dimension().size()); //sum over all columns */
    /*     cdiag = cdiag.transform(ones, cdiag.dimension().size()+1); //sum over all rows */

    /*     return Function(tensor_basis(), cdiag); */
    /* } */

    GenericFunction Constant::transform_to(const Basis& basis) const {
        return transform_to(TensorBasis(basis));
    }

    GenericFunction Constant::transform_to(const TensorBasis& basis) const {
        return Function(basis, basis.const_coeff_tensor(T));
    }

    GenericFunction Constant::project_to(const Basis& basis) const {
        return project_to(TensorBasis(basis));
    }

    GenericFunction Constant::project_to(const TensorBasis& b) const {
        return Function(basis, basis.const_coeff_tensor(T));
    }

    GenericFunction Constant::reshape(const std::vector< int >& shape) const {
        return Constant(coeff().reshape(shape));
    }

    GenericFunction Constant::slice(const AnySlice& i, const AnySlice& j) const {
        return Constant(coeff_tensor().get_slice(i, j));
    }

    GenericFunction Constant::slice(const AnySlice& i) const {
        return Constant(coeff_tensor().get_slice(i));
    }

    TensorBasis Constant::tensor_basis() const {
        return TensorBasisConstant();
    }
}  // namespace spline
