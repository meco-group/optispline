#include "TensorBasisConstant.h"
#include "EmptyBasis.h"
#include "BSplineBasis.h"
#include "../common.h"
#include "operations/operationsBasis.h"
#include "../Function/Function.h"

namespace spline {

    TensorBasisConstantNode* TensorBasisConstant::get() const {
        return static_cast<TensorBasisConstantNode*>(SharedObject::get());
    }

    TensorBasisConstantNode* TensorBasisConstant::operator->() const { return get(); }

    TensorBasisConstant::TensorBasisConstant() {
        assign_node(new TensorBasisConstantNode());
    }

    TensorBasisConstant::TensorBasisConstant(const TensorBasis& tb) {
        assign_node(new TensorBasisConstantNode());
    }

    /* int TensorBasisConstantNode::n_basis() const { */
    /*     return 0; */
    /* } */

    /* std::vector<int> TensorBasisConstantNode::dimension() const { */
    /*     std::vector<int> shape; */
    /*     return shape; */
    /* } */

    std::string TensorBasisConstantNode::to_string() const {
        return "TensorBasisConstant";
    }

    std::ostream& operator<<(std::ostream &stream, const TensorBasisConstant& base) {
        return stream << base.to_string();
    }

    AnyTensor TensorBasisConstantNode::operator() (const std::vector< AnyScalar > &  x) const {
        return AnyScalar(1.0);
    }

    /* int TensorBasisConstantNode::n_inputs() const { */
    /*     spline_assert(false); */
    /*     return -1; */
    /* } */

    TensorBasis TensorBasisConstantNode::operator+ (const TensorBasis& other) const {
        return other;
    }

    TensorBasis TensorBasisConstantNode::operator+ (const TensorBasisConstant& other) const {
        return other;
    }

    TensorBasis TensorBasisConstantNode::operator* (const TensorBasis& other) const {
        return other;
    }

    TensorBasis TensorBasisConstantNode::operator* (const TensorBasisConstant& other) const {
        return other;
    }





    /* TensorBasis TensorBasisConstantNode::transform_to(const TensorBasis& tb, std::vector<AnyTensor>& T) const { */
    /*     spline_assert(n_basis() == tb.n_basis()); */
    /*     std::vector<Basis> new_bases(n_basis()); */
    /*     std::vector<AnyTensor> T_(n_basis()); */
    /*     for (int i = 0; i <n_basis(); i++) { */
    /*         new_bases[i] = basis(i).transform_to(tb.basis(i), T_[i]); */
    /*     } */
    /*     T = T_; */
    /*     if (hasArguments()) { */
    /*         return TensorBasis(new_bases, arguments()); */
    /*     } else { */
    /*         return TensorBasis(new_bases); */
    /*     } */
    /* } */
    /* /1* Basis TensorBasisNode::basis(const Argument& index) const { *1/ */
    /* /1*     int ind = index.concrete(arguments()); *1/ */
    /* /1*     if (ind == 0) { *1/ */
    /* /1*         return EmptyBasis(); *1/ */
    /* /1*     } *1/ */
    /* /1*     return EmptyBasis(); *1/ */
    /* /1* } *1/ */

    /* spline::Function TensorBasisConstantNode::basis_functions() const { */
    /*     // Construct coefficient based on outer product of eye's */
    /*     AnyTensor C = DT(casadi::DM::densify(casadi::DM::eye(dimension()[0]))); */
    /*     std::vector< int > dims_even = std::vector< int >{0}; */
    /*     std::vector< int > dims_odd = std::vector< int >{1}; */
    /*     for(int i = 1; i < n_basis(); i++) { */
    /*         C = C.outer_product(DT(casadi::DM::densify(casadi::DM::eye(dimension()[i])))); */
    /*         dims_even.push_back(2*i); */
    /*         dims_odd.push_back(2*i+1); */
    /*     } */

    /*     // Scramble dimensions of coefficient */
    /*     dims_even.insert(dims_even.end(), dims_odd.begin(), dims_odd.end()); */
    /*     C = C.reorder_dims(dims_even); */

    /*     // reshape tensor-valued function to vector-valued */
    /*     std::vector< int > shape = C.dims(); */
    /*     shape.erase(shape.begin() + shape.size()/2, shape.end()); */
    /*     shape.push_back(product(shape)); */
    /*     C = C.shape(shape); */

    /*     spline::Function basis_functions_ = spline::Function(shared_from_this<TensorBasis>(), Coefficient(C)); */
    /*     return basis_functions_; */
    /* } */

    std::vector< int > TensorBasisConstantNode::get_permutation(const TensorBasis& grid_basis) const{
        std::vector< int > index;
        for(int i = 0; i < grid_basis.n_basis(); i++){
            index.push_back(-1);
        }
        return index;
    }
} // namespace spline
