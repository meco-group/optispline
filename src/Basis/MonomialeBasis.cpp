#include <math.h>       /* pow */

#include <casadi/casadi.hpp> // range

#include "MonomialeBasis.h"

#include "operations/operationsBasis.h"

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

       
    // AnyTensor MonomialeBasisNode::operator() (const std::vector<AnyScalar> & x) const {
    //     if( 1 ) {
    //     // if( x.isDouble()) {
    //         return BasisEvalution<double>(x);
    //     } else {
    //         return BasisEvalution<AnyScalar>(x);
    //     }
    // }


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
