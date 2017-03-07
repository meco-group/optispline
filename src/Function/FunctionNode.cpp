#include <vector>
#include "FunctionNode.h"
#include "ConstantNode.h"
#include "../Basis/utils/EvaluationGrid.h"
#include "../Basis/TensorBasisConstant.h"
#include "../common.h"

namespace spline {

    AnyTensor FunctionNode::operator()(const AnyTensor& x, const std::vector< std::string >& args) const{
        spline_assert_message(false, "not implemented: evaluation constant");
        /* return packed_tensor.einstein(coeff().data(), a_r, b_r, c_r).squeeze(); */
    }

    std::string FunctionNode::type() const{
        return "Constant";
    }

    std::string FunctionNode::to_string() const{
        return "Constant\t" + coeff_.to_string();
    }

    Function FunctionNode::operator+(const FunNode& f) const {
        return f + *this;
    }

    Function FunctionNode::operator+(const FunctionNode& f) const {
        Function dummy = transform_to(f.tensor_basis());
        /* return Function(f.tensor_basis(), dummy.coeff() + f.coeff()); */
    }

    Function FunctionNode::operator+(const ConstantNode& f) const {
        return f + *this;
    }


    Function FunctionNode::operator*(const FunNode& f) const {
        return f * *this;
    }

    Function FunctionNode::operator*(const FunctionNode& f) const {
        /* return Function(f.tensor_basis(), coeff_tensor() * f.coeff_tensor()); */
    }

    Function FunctionNode::operator*(const ConstantNode& f) const {
        return f * *this;
    }


    Function FunctionNode::mtimes(const FunNode& f) const {
        return f.mtimes(*this);
    }

    Function FunctionNode::mtimes(const FunctionNode& f) const {
    }

    Function FunctionNode::mtimes(const ConstantNode& f) const {
        return f.mtimes(*this);
    }

    Function FunctionNode::operator-() const {
        return Function(tensor_basis(), -coeff_);
    }

    Function FunctionNode::transpose() const {
        return Function(tensor_basis(), coeff().transpose());
    }

    Function FunctionNode::trace() const {
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

    Function FunctionNode::transform_to(const TensorBasis& basis) const {
    }

    Function FunctionNode::project_to(const TensorBasis& b) const {
    }

/*     Function FunctionNode::reshape(const std::vector< int >& shape) const { */
/*         return Function(tensor_basis(), coeff().reshape(shape)); */
/*     } */

    Function FunctionNode::slice(const AnySlice& i, const AnySlice& j) const {
        return Function(tensor_basis(), coeff_tensor().get_slice(i, j));
    }

    Function FunctionNode::slice(const AnySlice& i) const {
        return Function(tensor_basis(), coeff_tensor().get_slice(i));
    }

    TensorBasis FunctionNode::tensor_basis() const {
        return TensorBasisConstant();
    }

}  // namespace spline
