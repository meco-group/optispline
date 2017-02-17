#include "DummyBasis.h"
#include "MonomialBasis.h"
#include "BSplineBasis.h"

namespace spline {

    DummyBasisNode* DummyBasis::get() const {
      return static_cast<DummyBasisNode*>(SharedObject::get());
    };

    DummyBasisNode* DummyBasis::operator->() const { return get(); }

    DummyBasis::DummyBasis() {
        assign_node(new DummyBasisNode());
    }
    std::string DummyBasis::getRepresentation() const { return (*this)->getRepresentation() ; }
    std::string DummyBasisNode::getRepresentation() const {
        return "DummyBasis object";
    }

    Basis DummyBasisNode::operator+ (const Basis& other) const  {
        return other + shared_from_this< DummyBasis >();
    }

    Basis DummyBasisNode::operator+ (const MonomialBasis& other) const  {
        return other.get()->shared_from_this< MonomialBasis >();
    }

    Basis DummyBasisNode::operator+ (const BSplineBasis& other) const  {
        return other.get()->shared_from_this< BSplineBasis >();
    }

    Basis DummyBasisNode::operator* (const Basis& other) const  {
        return other * shared_from_this< DummyBasis >();
    }

    Basis DummyBasisNode::operator* (const MonomialBasis& other) const  {
        return other.get()->shared_from_this< MonomialBasis >();
    }

    Basis DummyBasisNode::operator* (const BSplineBasis& other) const  {
        return other.get()->shared_from_this< BSplineBasis >();
    }

    Basis DummyBasisNode::derivative(int order, AnyTensor& T) const {
        spline_assert(false) ;
        return shared_from_this< Basis >();
    }

}  // namespace spline
