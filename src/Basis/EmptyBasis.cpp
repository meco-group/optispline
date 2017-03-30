#include "EmptyBasis.h"
#include "MonomialBasis.h"
#include "BSplineBasis.h"

namespace spline {

    EmptyBasisNode* EmptyBasis::get() const {
      return static_cast<EmptyBasisNode*>(SharedObject::get());
    };

    EmptyBasisNode* EmptyBasis::operator->() const { return get(); }

    EmptyBasis::EmptyBasis() {
        assign_node(new EmptyBasisNode());
    }

    std::string EmptyBasisNode::to_string() const { return "An EmptyBasis object"; }


    Basis EmptyBasisNode::operator+ (const BasisNode& other) const  {
        return other + *this;
    }

    Basis EmptyBasisNode::operator+ (const EmptyBasisNode& other) const  {
        return shared_from_this< EmptyBasis >();
    }

    Basis EmptyBasisNode::operator+ (const MonomialBasisNode& other) const  {
        return other.shared_from_this< MonomialBasis >();
    }

    Basis EmptyBasisNode::operator+ (const BSplineBasisNode& other) const  {
        return other.shared_from_this< BSplineBasis >();
    }

    Basis EmptyBasisNode::operator* (const BasisNode& other) const  {
        return other * *this;
    }

    Basis EmptyBasisNode::operator* (const EmptyBasisNode& other) const  {
        return shared_from_this< EmptyBasis >();
    }

    Basis EmptyBasisNode::operator* (const MonomialBasisNode& other) const  {
        return other.shared_from_this< MonomialBasis >();
    }

    Basis EmptyBasisNode::operator* (const BSplineBasisNode& other) const  {
        return other.shared_from_this< BSplineBasis >();
    }

    bool EmptyBasisNode::operator== (const BasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    bool EmptyBasisNode::operator== (const EmptyBasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    bool EmptyBasisNode::operator== (const BSplineBasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    bool EmptyBasisNode::operator== (const MonomialBasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    AnyTensor EmptyBasis::operator() (const AnyVector & x) const {
        std::vector<AnyScalar> a = x.to_scalar_vector();
        return (*this)->operator()(a);
    }
    AnyTensor EmptyBasisNode::operator() (const std::vector< AnyScalar > & x) const {
        return AnyScalar(1.0);
    }
}  // namespace spline
