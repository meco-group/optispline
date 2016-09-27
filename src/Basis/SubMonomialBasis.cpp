#include <math.h>       /* pow */

#include <casadi/casadi.hpp> // range

#include "SubMonomialBasis.h"

#include "operations/operationsBasis.h"

namespace spline {
    std::string SubMonomialBasisNode::getRepresentation() const {
        return "SubMonomialBasis";
    }

    std::string SubMonomialBasis::getRepresentation() const {
        return (*this)->getRepresentation() ;
    }

    SubMonomialBasisNode* SubMonomialBasis::get() const { return static_cast<SubMonomialBasisNode*>(SharedObject::get()); };
    SubMonomialBasisNode* SubMonomialBasis::operator->() const { return get(); }

    SubMonomialBasis::SubMonomialBasis(int degree)  {
        assign_node(new SubMonomialBasisNode(degree));
    }

    SubBasis SubMonomialBasis::operator+ (const SubBasis& other) const {
        assert(false);
	return SubBasis();
	// return other + *this; 
    } 

    SubBasis SubMonomialBasis::operator+ (const SubMonomialBasis& other)const  {
        assert(false);
	return SubBasis();
	// return plusUnivariateUnivariate (*this, other);
    } 
    
    SubBasis SubMonomialBasis::operator+ (const SubBSplineBasis& other)const  {
        assert(false);
	return SubBasis();
	// return plusUnivariateUnivariate (*this, other);
    } 

    SubBasis SubMonomialBasis::operator* (const SubBasis& other) const {
        assert(false);
	return SubBasis();
	// return other * *this; 
    } 

    SubBasis SubMonomialBasis::operator* (const SubMonomialBasis& other)const  {
        assert(false);
	return SubBasis();
	// return timesUnivariateUnivariate (*this, other);
    } 
    
    SubBasis SubMonomialBasis::operator* (const SubBSplineBasis& other)const  {
        assert(false);
	return SubBasis();
	// return timesUnivariateUnivariate (*this, other);
    } 
    
    SubBasis SubMonomialBasisNode::operator+ (const SubBasis& other)const  {
        assert(false);
	return SubBasis();
	// return other + shared_from_this<BSplineBasis>();
    } 

    SubBasis SubMonomialBasisNode::operator* (const SubBasis& other)const  {
        assert(false);
	return SubBasis();
	// return other * shared_from_this<BSplineBasis>();
    } 
    
    AnyTensor SubMonomialBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        // if( 1 ) {
        // if( x.isDouble()) {
            return SubBasisEvalution<double,DM>(x);
        // } else {
        //     return SubBasisEvalution<AnyScalar>(x);
        // }
    }

    ST  SubMonomialBasisNode::operator() (const std::vector< SX > &  x   ) const {
        
        // return SubBasisEvalution<SX,SX>(x);
    }

    MT  SubMonomialBasisNode::operator() (const std::vector< MX > &  x   ) const {
        // return SubBasisEvalution<MX,MX>(x);
    }

    int SubMonomialBasisNode::getLenght () const {
         return getDegree() + 1;
    }
    
    /// TODO(ErikLambr) check is grid is valid
    //   std::vector<double> Monomialestd::shared_ptr<Basis>::evaluationGrid () const {
    ///        std::vector<double> grid = casadi::range(length());
    //       std::vector<double> grid;
    //       for( int i = 0; i < length(); i++ )
    //           grid.push_back( (double)i );
    //       return grid;
    //   }
} // namespace spline
