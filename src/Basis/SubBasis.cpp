#include "SubBasis.h"
#include "operations/operationsBasis.h"
#include "../common.h"	

namespace spline {

    SubBasisNode* SubBasis::get() const { return static_cast<SubBasisNode*>(SharedObject::get()); };
    SubBasisNode* SubBasis::operator->() const { return get(); }

    SubBasis::SubBasis (){
        assign_node(new SubBasisNode());
    }
 
    std::string SubBasisNode::getRepresentation() const {return "SubBasis";};
    std::string SubBasis::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::ostream& operator<<(std::ostream &stream, const SubBasis& base){
        return stream << base.getRepresentation();
    }

    
    SubBasis SubBasis::operator+ (const SubBasis& other) const { return (*this)->operator+(other);}
    SubBasis SubBasisNode::operator+ (const SubBasis& other) const { 
        spline_assert(false);
        return SubBasis();
    }    

    SubBasis SubBasis::operator+ (const SubBasisDummy& other) const { return (*this)->operator+(other);}
    SubBasis SubBasisNode::operator+ (const SubBasisDummy& other) const { 
        spline_assert(false);
        return SubBasis();
    }

    SubBasis SubBasis::operator+ (const SubMonomialBasis& other) const { return (*this)->operator+(other); }
    SubBasis SubBasisNode::operator+ (const SubMonomialBasis& other) const {
        spline_assert(false);
        return SubBasis();
    } 

    SubBasis SubBasis::operator+ (const SubBSplineBasis& other) const {return (*this)->operator+(other);}
    SubBasis SubBasisNode::operator+ (const SubBSplineBasis& other) const {
        spline_assert(false);
        return SubBasis();
    } 

    AnyTensor SubBasis::operator() (const std::vector< AnyScalar > & x) const { return (*this)->operator()(x); }
    AnyTensor SubBasisNode::operator() (const std::vector< AnyScalar > & x) const {
        assert(false);		//Abstract
        return AnyTensor();
    }

    int SubBasis::getDimension() const{ return (*this)->getDimension();}
} // namespace spline
