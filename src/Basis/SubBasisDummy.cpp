#include "SubBasisDummy.h"
#include "SubMonomialBasis.h"
#include "SubBSplineBasis.h"

namespace spline {

    SubBasisDummyNode* SubBasisDummy::get() const { return static_cast<SubBasisDummyNode*>(SharedObject::get()); };
    SubBasisDummyNode* SubBasisDummy::operator->() const { return get(); }

    SubBasisDummy::SubBasisDummy (){
        assign_node(new SubBasisDummyNode());
    }
    std::string SubBasisDummy::getRepresentation() const { return (*this)->getRepresentation() ; }
    std::string SubBasisDummyNode::getRepresentation() const {
        return "SubBasisDummy object";
    }

    SubBasis SubBasisDummyNode::operator+ (const SubBasis& other) const  {
        return other + shared_from_this< SubBasisDummy >();
    }

    SubBasis SubBasisDummyNode::operator+ (const SubMonomialBasis& other) const  {
        return other.get()->shared_from_this< SubMonomialBasis >();
    }

    SubBasis SubBasisDummyNode::operator+ (const SubBSplineBasis& other) const  {
        return other.get()->shared_from_this< SubBSplineBasis >();
    } 
    //
    // SubBasis SubBasisDummyNode::operator+ (const SubMonomialBasis &other) const  {
    //     return other;
    // } 
    //
    // SubBasis SubBasisDummyNode::operator+ (const SubBSplineBasis &other) const  {
    //     return other;
    // } 
    //
}  // namespace spline

