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

    UnivariateBasisNode::UnivariateBasisNode(SubBasis subbasis) : BasisNode(std::vector< SubBasis > {subbasis}) { }
    UnivariateBasisNode* UnivariateBasis::get() const { return static_cast<UnivariateBasisNode*>(SharedObject::get()); };
    UnivariateBasisNode* UnivariateBasis::operator->() const { return get(); }

    std::string UnivariateBasis::getRepresentation() const { return (*this)->getRepresentation() ; }
    std::string UnivariateBasisNode::getRepresentation() const {
        return "UnivariateBasis object"; 
    }


    int UnivariateBasis::getLength ( ) const  { return (*this)->getLength(); } 
    int UnivariateBasisNode::getLength ( ) const {
        return getSubBasis().size();
    }
    
    void  UnivariateBasis::setDegree (int degree) { (*this)->setDegree (degree); } 
    void  UnivariateBasisNode::setDegree (int degree) {
         // this->degree = degree;
    }
    
    int  UnivariateBasis::getDegree  ( ) const { return (*this)->getDegree (); } 
    int  UnivariateBasisNode::getDegree  ( ) const {
         return 0 ;
    }

    void UnivariateBasisNode::assertUnivariateBasis  ( ) const {assert(getDimension()==1);} 
    void UnivariateBasis::assertUnivariateBasis  ( ) const {assert(getDimension()==1);} 

    SubBasis UnivariateBasis::getSubUnivariateBasis() const{
        assertUnivariateBasis();
        return getSubBasis()[0];
    }
}  // namespace spline

