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

    SubBasis SubMonomialBasis::operator+ (const SubBasis& other) const {(*this)->operator+(other);}
    SubBasis SubMonomialBasisNode::operator+ (const SubBasis& other) const {
        std::cout << "subMono + sub " << getRepresentation() << " " << other.getRepresentation() << std::endl;  
        return other + shared_from_this<SubMonomialBasis>();
    } 

    SubBasis SubMonomialBasis::operator+ (const SubBSplineBasis& other) const {(*this)->operator+(other);}
    SubBasis SubMonomialBasisNode::operator+ (const SubBSplineBasis& other) const {
        return plusSubBasis (shared_from_this<SubBSplineBasis>(), other);
    } 

    SubBasis SubMonomialBasis::operator+ (const SubMonomialBasis& other) const {(*this)->operator+(other);}
    SubBasis SubMonomialBasisNode::operator+ (const SubMonomialBasis& other) const {
        std::cout << "subMono + subMono " << getRepresentation() << " " << other.getRepresentation() << std::endl;  
        return plusSubBasis (shared_from_this<SubMonomialBasis>(), other);
    } 


    // SubBasis SubMonomialBasis::operator* (const SubBasis& other) const {
    //     assert(false);
	// return SubBasis();
	// // return other * *this; 
    // } 
    //
    // SubBasis SubMonomialBasis::operator* (const SubMonomialBasis& other)const  {
    //     assert(false);
	// return SubBasis();
	// // return timesUnivariateUnivariate (*this, other);

    //
    // SubBasis SubMonomialBasis::operator* (const SubBSplineBasis& other)const  {
    //     assert(false);
	// return SubBasis();
	// // return timesUnivariateUnivariate (*this, other);
    // } 
    
    // SubBasis SubMonomialBasisNode::operator+ (const SubBasis& other)const  {
    //     assert(false);
	// return SubBasis();
	// // return other + shared_from_this<BSplineBasis>();
    // } 
    //
    // SubBasis SubMonomialBasisNode::operator* (const SubBasis& other)const  {
    //     assert(false);
	// return SubBasis();
	// // return other * shared_from_this<BSplineBasis>();
    // } 
    
    AnyTensor SubMonomialBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        assert(x.size()==getDimension()); 
        if(AnyScalar::is_double(x)) {
            return SubBasisEvalution<double>(AnyScalar::vector_double(x));
        } else {
            return SubBasisEvalution<AnyScalar>(x);
        }
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
