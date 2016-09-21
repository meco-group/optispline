#include <vector>
#include "UnivariateBasis.h"

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"
#include "operations/operationsBasis.h"

namespace spline {

    //  /    TODO(ErikLambre) check order
    //     casadi::DM Univariatestd::shared_ptr<Basis>::transformation( const std::shared_ptr<Basis> &b) const {
    //         std::vector<double> grid = evaluationGrid();
    //         casadi::DM A(evalstd::shared_ptr<Basis>(grid));
    //         casadi::DM B(b.evalstd::shared_ptr<Basis>(grid));
    //         return casadi::DM::solve(A, B);
    //     }
    //  
    //  

    UnivariateBasisNode::UnivariateBasisNode(int degree_) : BasisNode(std::vector< SubBasis > {}) { }
    UnivariateBasisNode* UnivariateBasis::get() const { return static_cast<UnivariateBasisNode*>(SharedObject::get()); };
    UnivariateBasisNode* UnivariateBasis::operator->() const { return get(); }

    std::string UnivariateBasis::getRepresentation() const { return (*this)->getRepresentation() ; }
    std::string UnivariateBasisNode::getRepresentation() const {
        return "UnivariateBasis object"; 
    }


    int UnivariateBasis::getLenght ( ) const  { return (*this)->getLenght(); } 
    int UnivariateBasisNode::getLenght ( ) const {
         assert(0);
    }
    
    void  UnivariateBasis::setDegree (int degree) { (*this)->setDegree (degree); } 
    void  UnivariateBasisNode::setDegree (int degree) {
         // this->degree = degree;
    }
    
    int  UnivariateBasis::getDegree  ( ) const { return (*this)->getDegree (); } 
    int  UnivariateBasisNode::getDegree  ( ) const {
         return 0 ;
    }
}  // namespace spline

