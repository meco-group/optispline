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

    
    SubBasis SubBasis::operator+ (const SubBasis& other) const { (*this)->operator+(other);}
    SubBasis SubBasisNode::operator+ (const SubBasis& other) const { 
        spline_assert(false);
    }

    SubBasis SubBasis::operator+ (const SubMonomialBasis& other) const {(*this)->operator+(other);}
    SubBasis SubBasisNode::operator+ (const SubMonomialBasis& other) const {
        spline_assert(false);
    } 

    SubBasis SubBasis::operator+ (const SubBSplineBasis& other) const {(*this)->operator+(other);}
    SubBasis SubBasisNode::operator+ (const SubBSplineBasis& other) const {
        spline_assert(false);
    } 

    // SubBasis SubBasis::operator* (const SubBasis& other) const { 
	// assert(false);
	// return SubBasis();
    //     // return timesMultivariate(*this, other);
    // }
    //
    // SubBasis SubBasis::operator* (const SubMonomialBasis& other) const {
	// assert(false);
	// return SubBasis();
    //     // return timesMultivariate(*this, other);
    // } 
    //
    // SubBasis SubBasis::operator* (const SubBSplineBasis& other) const {
	// assert(false);
	// return SubBasis();
    //     // return timesMultivariate(*this, other);
    // } 

    AnyTensor  SubBasis::operator() (const std::vector< AnyScalar > &  x ) const {
        return (*this)->operator()(x);
    }

    AnyTensor  SubBasisNode::operator() (const std::vector< AnyScalar > &  x   ) const {
	assert(false);		//Abstract
    }
   
    // ST  SubBasis::operator()  (const std::vector< SX > &  x   ) const {
    //     return (*this)->operator() (x);
    // }
    //
    // ST  SubBasisNode::operator()  (const std::vector< SX > &  x   ) const {
    //     ST ret(1,{});
	// assert(false);		//Abstract
    //     return ret;
    // }
    //
    // MT  SubBasis::operator()  (const std::vector< MX > &  x   ) const {
    //     return (*this)->operator() (x);
    // }
    //
    // MT  SubBasisNode::operator()  (const std::vector< MX > &  x   ) const {
    //     MT ret(1,{});
	// assert(false);		//Abstract
    //     return ret;
    // }

    int SubBasis::getDimension() const{(*this)->getDimension();}
} // namespace spline
