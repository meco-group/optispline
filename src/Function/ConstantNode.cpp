#include <vector>
#include "ConstantNode.h"
#include "FunctionNode.h"
#include "../Basis/utils/EvaluationGrid.h"
#include "../Basis/TensorBasisConstant.h"
#include "../common.h"

namespace spline {
    ConstantNode::ConstantNode(const AnyTensor& coeff) {
        coeff_ = coeff;
    }

    AnyTensor ConstantNode::operator()(const AnyTensor& x, const std::vector< std::string >& args) const{
        spline_assert_message(false, "not implemented: evaluation constant");
        /* return packed_tensor.einstein(coeff().data(), a_r, b_r, c_r).squeeze(); */
    }

    std::string ConstantNode::type() const{
        return "Constant";
    }

    std::string ConstantNode::to_string() const{
        return "Constant\t" + coeff_.to_string();
    }

    Function ConstantNode::operator+(const FunNode& f) const {
        return f + *this;
    }

    Function ConstantNode::operator+(const FunctionNode& f) const {
        Function dummy = transform_to(f.tensor_basis());
        return Function(f.tensor_basis(), dummy.coeff_tensor() + f.coeff_tensor());
    }

    Function ConstantNode::operator+(const ConstantNode& f) const {
        spline_assert_message(false, "not implemented: operator+(ConstantNode)");
    }


    Function ConstantNode::operator*(const FunNode& f) const {
        return f * *this;
    }

    Function ConstantNode::operator*(const FunctionNode& f) const {
        return Function(f.tensor_basis(), coeff_tensor() * f.coeff_tensor());
    }

    Function ConstantNode::operator*(const ConstantNode& f) const {
        spline_assert_message(false, "not implemented: operator*(ConstantNode)");
    }


    Function ConstantNode::mtimes(const FunNode& f) const {
        return f.mtimes(*this);
    }

    Function ConstantNode::mtimes(const FunctionNode& f) const {
        /* Coefficient c = coeff(); */
        /* return Function(c.transform(t.reorder_dims({1, 0}), 0)); */
    }

    Function ConstantNode::mtimes(const ConstantNode& f) const {
        spline_assert_message(false, "not implemented: mtimes(ConstantNode)");
    }

    Function ConstantNode::operator-() const {
        return Function(-coeff_tensor());
    }

    Function ConstantNode::transpose() const {
        return Function(coeff().transpose().data());
    }

    Function ConstantNode::trace() const {
       spline_assert_message(false, "not implemented trace");
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
    }

    Function ConstantNode::transform_to(const TensorBasis& basis) const {
        return Function(basis, basis.const_coeff_tensor(coeff_tensor()));
    }

    Function ConstantNode::project_to(const TensorBasis& basis) const {
        return Function(basis, basis.const_coeff_tensor(coeff_tensor()));
    }

    Function ConstantNode::reshape(const std::vector< int >& shape) const {
        return Function(coeff().reshape(shape).data());
    }

    Function ConstantNode::slice(const AnySlice& i, const AnySlice& j) const {
        return Function(coeff_tensor().get_slice(i, j));
    }

    Function ConstantNode::slice(const AnySlice& i) const {
        return Function(coeff_tensor().get_slice(i));
    }

    TensorBasis ConstantNode::tensor_basis() const {
        return TensorBasisConstant();
    }

}  // namespace spline
