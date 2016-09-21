#include <math.h>       /* pow */

#include <casadi/casadi.hpp> // range

#include "SubMonomialeBasis.h"

#include "operations/operationsBasis.h"

namespace spline {
    std::string SubMonomialeBasisNode::getRepresentation() const {
        return "SubMonomialeBasis";
    }

    std::string SubMonomialeBasis::getRepresentation() const {
        return (*this)->getRepresentation() ;
    }

    SubMonomialeBasisNode* SubMonomialeBasis::get() const { return static_cast<SubMonomialeBasisNode*>(SharedObject::get()); };
    SubMonomialeBasisNode* SubMonomialeBasis::operator->() const { return get(); }

    SubMonomialeBasis::SubMonomialeBasis(int degree)  {
        assign_node(new SubMonomialeBasisNode(degree));
    }

    SubBasis SubMonomialeBasis::operator+ (const SubBasis& other) const {
        assert(false);
	return SubBasis();
	// return other + *this; 
    } 

    SubBasis SubMonomialeBasis::operator+ (const SubMonomialeBasis& other)const  {
        assert(false);
	return SubBasis();
	// return plusUnivariateUnivariate (*this, other);
    } 
    
    SubBasis SubMonomialeBasis::operator+ (const SubBSplineBasis& other)const  {
        assert(false);
	return SubBasis();
	// return plusUnivariateUnivariate (*this, other);
    } 

    SubBasis SubMonomialeBasis::operator* (const SubBasis& other) const {
        assert(false);
	return SubBasis();
	// return other * *this; 
    } 

    SubBasis SubMonomialeBasis::operator* (const SubMonomialeBasis& other)const  {
        assert(false);
	return SubBasis();
	// return timesUnivariateUnivariate (*this, other);
    } 
    
    SubBasis SubMonomialeBasis::operator* (const SubBSplineBasis& other)const  {
        assert(false);
	return SubBasis();
	// return timesUnivariateUnivariate (*this, other);
    } 
    
    SubBasis SubMonomialeBasisNode::operator+ (const SubBasis& other)const  {
        assert(false);
	return SubBasis();
	// return other + shared_from_this<BSplineBasis>();
    } 

    SubBasis SubMonomialeBasisNode::operator* (const SubBasis& other)const  {
        assert(false);
	return SubBasis();
	// return other * shared_from_this<BSplineBasis>();
    } 
    
    AnyTensor SubMonomialeBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        if( 1 ) {
        // if( x.isDouble()) {
            return SubBasisEvalution<double>(x);
        } else {
            return SubBasisEvalution<AnyScalar>(x);
        }
    }

    ST  SubMonomialeBasisNode::operator() (const std::vector< SX > &  x   ) const {
        assert(x.size()==1);
        return ST(vertcat(x),{3}); 
    }

    MT  SubMonomialeBasisNode::operator() (const std::vector< MX > &  x   ) const {
        assert(x.size()==1);
        return MT(vertcat(x),{3}); 
    }

    int SubMonomialeBasisNode::getLenght () const {
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
