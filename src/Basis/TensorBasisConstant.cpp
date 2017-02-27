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

    int TensorBasisConstantNode::n_basis() const {
        return 0;
    }

    /* std::vector<int> TensorBasisConstantNode::dimension() const { */
    /*     std::vector<int> shape; */
    /*     return shape; */
    /* } */

    std::string TensorBasisConstantNode::getRepresentation() const {
        return "TensorBasisConstant";
    }

    std::ostream& operator<<(std::ostream &stream, const TensorBasisConstant& base) {
        return stream << base.getRepresentation();
    }

    AnyTensor TensorBasisConstantNode::operator() (const std::vector< AnyScalar > &  x) const {
        return AnyScalar(1.0);
    }

    int TensorBasisConstantNode::n_inputs() const {
        spline_assert(false);
        return -1;
    }

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
} // namespace spline
