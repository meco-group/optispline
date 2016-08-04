#include <math.h>       /* pow */

#include <casadi/casadi.hpp> // range

#include "MonomialeBasis.h"

#include "plus.h"
#include "times.h"

namespace spline {
    std::string MonomialeBasisNode::getRepresentation() const {
        return "MonomialeBasis";
    }

    std::string MonomialeBasis::getRepresentation() const {
        return (*this)->getRepresentation() ;
    }

    MonomialeBasisNode* MonomialeBasis::get() const { return static_cast<MonomialeBasisNode*>(SharedObject::get()); };
    MonomialeBasisNode* MonomialeBasis::operator->() const { return get(); }

    MonomialeBasis::MonomialeBasis(int degree)  {
        assign_node(new MonomialeBasisNode(degree));
    }

    MonomialeBasis::MonomialeBasis(int degree, Argument argument)  {
        assign_node(new MonomialeBasisNode(degree, argument));
    }

    Basis MonomialeBasis::operator+ (const Basis& other) const {
        return other + *this; 
    } 

    Basis MonomialeBasis::operator+ (const MonomialeBasis& other)const  {
        return plusUnivariateUnivariate (*this, other);
    } 
    
    Basis MonomialeBasis::operator+ (const BSplineBasis& other)const  {
        return plusUnivariateUnivariate (*this, other);
    } 
    
    Basis MonomialeBasisNode::operator+ (const Basis& other)const  {
        return other + shared_from_this<BSplineBasis>();
        // return plusUnivariateUnivariate (*this, other);
        // return Basis();
    } 
    
    AnyTensor MonomialeBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        if( 1 ) {
        // if( x.isDouble()) {
            return BasisEvalution<double>(x);
        } else {
            return BasisEvalution<AnyScalar>(x);
        }
    }

    ST  MonomialeBasisNode::operator() (const std::vector< SX > &  x   ) const {
        assert(x.size()==1);
        return ST(vertcat(x),{3}); 
    }

    MT  MonomialeBasisNode::operator() (const std::vector< MX > &  x   ) const {
        assert(x.size()==1);
        return MT(vertcat(x),{3}); 
    }

    int MonomialeBasisNode::getLenght () const {
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
