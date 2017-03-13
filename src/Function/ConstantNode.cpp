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
        return coeff_tensor();
        /* spline_assert_message(false, "not implemented: evaluation constant"); */
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
        AnyTensor return_coeff;
        if(shape() == f.shape()){
            Function dummy = transform_to(f.tensor_basis());
            return_coeff = dummy.coeff_tensor() + f.coeff_tensor();
        } else {
            if(is_scalar()) {
                Function dummy = ConstantNode(AnyTensor::repeat(coeff_tensor(), f.shape())).transform_to(f.tensor_basis());
                return_coeff = dummy.coeff_tensor() + f.coeff_tensor();
            } else {
                AnyTensor f_coeff = f.coeff_tensor();
                f_coeff = f_coeff.shape(f.coeff().dimension());

                Function dummy = ConstantNode(AnyTensor::repeat(coeff_tensor(), f.shape())).transform_to(f.tensor_basis());

                int dims = f_coeff.dims().size();
                std::vector<int> a_r = mrange(dims);
                std::vector<int> b_r;
                std::vector<int> c_r = mrange(dims + 2);
                b_r = {c_r[c_r.size() - 2], c_r[c_r.size() - 1]};

                AnyTensor unite = AnyTensor::repeat(AnyTensor::unity(), shape());
                return_coeff =   f_coeff.einstein(unite,a_r,b_r , c_r) ;
                return_coeff = dummy.coeff_tensor() + return_coeff;
            }
        }
        return Function(f.tensor_basis(), return_coeff);
    }

    Function ConstantNode::operator+(const ConstantNode& f) const {
        spline_assert_message(false, "not implemented: operator+(ConstantNode)");
    }


    Function ConstantNode::operator*(const FunNode& f) const {
        return f * *this;
    }

    Function ConstantNode::operator*(const FunctionNode& f) const {
        AnyTensor r_coeff;
        if(is_scalar()){
            r_coeff = f.coeff_tensor() * coeff_tensor();
        } else {
            int dims = f.coeff().dimension().size();
            if(f.is_scalar()){
                std::vector<int> a_r = mrange(dims);
                std::vector<int> b_r;
                std::vector<int> c_r = mrange(dims + 2);
                b_r = {c_r[c_r.size() - 2], c_r[c_r.size() - 1]};

                AnyTensor f_coeff = f.coeff_tensor();
                f_coeff = f_coeff.shape(f.coeff().dimension());
                r_coeff = f_coeff.einstein(coeff_tensor(), a_r, b_r, c_r);
            } else {
                std::vector<int> a_r = mrange(dims + 2);
                std::vector<int> b_r;
                std::vector<int> c_r = mrange(dims + 2);
                b_r = {c_r[c_r.size() - 2], c_r[c_r.size() - 1]};
                r_coeff = f.coeff_tensor().einstein(coeff_tensor(), a_r, b_r, c_r);
            }
        }
        return Function(f.tensor_basis(), r_coeff);
    }

    Function ConstantNode::operator*(const ConstantNode& f) const {
        spline_assert_message(false, "not implemented: operator*(ConstantNode)");
    }


    Function ConstantNode::mtimes(const FunNode& f) const {
        return f.rmtimes(*this);
    }

    Function ConstantNode::mtimes(const FunctionNode& f) const {
        AnyTensor rhs = f.coeff_tensor();
        int n_dims = rhs.dims().size();
        std::vector<int> c_e = mrange(n_dims);
        std::vector<int> a_e = {-n_dims+1, -n_dims+1};
        std::vector<int> b_e = mrange(n_dims);
        b_e[n_dims - 2] = -n_dims - 1;
        AnyTensor data = coeff_tensor().einstein(rhs, a_e, b_e, c_e);
        return Function(f.tensor_basis(), data);
    }

    Function ConstantNode::mtimes(const ConstantNode& f) const {
        AnyTensor data = coeff_tensor().trailing_mtimes(f.coeff_tensor());
        return Function(data);
    }

    Function ConstantNode::rmtimes(const FunctionNode& f) const {
        AnyTensor rhs = f.coeff_tensor();
        int n_dims = rhs.dims().size();
        std::vector<int> c_e = mrange(n_dims);
        std::vector<int> a_e = {-n_dims - 1, -n_dims };
        std::vector<int> b_e = mrange(n_dims);
        b_e[n_dims - 1] = -n_dims - 1 ;
        AnyTensor data = coeff_tensor().einstein(rhs, a_e, b_e, c_e);
        return Function(f.tensor_basis(), data);
    }

    Function ConstantNode::rmtimes(const ConstantNode& f) const {
        AnyTensor data = coeff_tensor().trailing_rmtimes(f.coeff_tensor());
        return Function(data);
    }

    Function ConstantNode::operator-() const {
        return Function(-coeff_tensor());
    }

    Function ConstantNode::transpose() const {
        return Function(coeff().transpose().data());
    }

    Function ConstantNode::trace() const {
        spline_assert_message(false, "not implemented trace");
        std::vector< int > shape_ = shape();
        spline_assert_message(shape_[0] == shape_[1],
                "Trace only defined for square matrices. Dimensions are " << shape_ << ".");

        AnyTensor t = DT(casadi::DM::densify(casadi::DM::eye(shape_[0])));

        return Function( t*coeff_tensor() );
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
