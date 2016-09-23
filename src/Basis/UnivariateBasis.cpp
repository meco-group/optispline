#include <vector>
#include "UnivariatBasis.h"

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

    UnivariatBasisNode::UnivariatBasisNode(SubBasis subbasis) : BasisNode(std::vector< SubBasis > {subbasis}) { }
    UnivariatBasisNode* UnivariatBasis::get() const { return static_cast<UnivariatBasisNode*>(SharedObject::get()); };
    UnivariatBasisNode* UnivariatBasis::operator->() const { return get(); }

    std::string UnivariatBasis::getRepresentation() const { return (*this)->getRepresentation() ; }
    std::string UnivariatBasisNode::getRepresentation() const {
        return "UnivariatBasis object"; 
    }


    int UnivariatBasis::getLenght ( ) const  { return (*this)->getLenght(); } 
    int UnivariatBasisNode::getLenght ( ) const {
         assert(0);
    }
    
    void  UnivariatBasis::setDegree (int degree) { (*this)->setDegree (degree); } 
    void  UnivariatBasisNode::setDegree (int degree) {
         // this->degree = degree;
    }
    
    int  UnivariatBasis::getDegree  ( ) const { return (*this)->getDegree (); } 
    int  UnivariatBasisNode::getDegree  ( ) const {
         return 0 ;
    }

    void UnivariatBasis::assertUnivariatBasis  ( ) const {assert(getDimesion()==1);} 

    SubBasis UnivariatBasis::getUnivariatSubBasis() const{
        assertUnivariatBasis();
        return getSubBasis()[0];
    }
}  // namespace spline

