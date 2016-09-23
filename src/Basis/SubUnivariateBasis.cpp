#include <vector>
#include "SubUnivariatBasis.h"

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

    SubUnivariatBasisNode::SubUnivariatBasisNode(int degree_) :degree(degree_){ }
    SubUnivariatBasisNode* SubUnivariatBasis::get() const { return static_cast<SubUnivariatBasisNode*>(SharedObject::get()); };
    SubUnivariatBasisNode* SubUnivariatBasis::operator->() const { return get(); }

    std::string SubUnivariatBasis::getRepresentation() const { return (*this)->getRepresentation() ; }
    std::string SubUnivariatBasisNode::getRepresentation() const {
        return "SubUnivariatBasis object"; 
    }

    SubBasis SubUnivariatBasis::operator+ (const SubBasis& other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariatBasis::operator+ (const SubMonomialeBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariatBasis::operator+ (const SubBSplineBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariatBasis::operator* (const SubBasis& other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariatBasis::operator* (const SubMonomialeBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariatBasis::operator* (const SubBSplineBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    int SubUnivariatBasis::getLenght ( ) const  { return (*this)->getLenght(); } 
    int SubUnivariatBasisNode::getLenght ( ) const {
         assert(0);
    }
    
    void  SubUnivariatBasis::setDegree (int degree) { (*this)->setDegree (degree); } 
    void  SubUnivariatBasisNode::setDegree (int degree) {
         this->degree = degree;
    }
    
    int  SubUnivariatBasis::getDegree  ( ) const { return (*this)->getDegree (); } 
    int  SubUnivariatBasisNode::getDegree  ( ) const {
         return degree ;
    }
}  // namespace spline

