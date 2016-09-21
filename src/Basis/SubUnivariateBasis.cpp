#include <vector>
#include "SubUnivariateBasis.h"

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

    SubUnivariateBasisNode::SubUnivariateBasisNode(int degree_) :degree(degree_){ }
    SubUnivariateBasisNode* SubUnivariateBasis::get() const { return static_cast<SubUnivariateBasisNode*>(SharedObject::get()); };
    SubUnivariateBasisNode* SubUnivariateBasis::operator->() const { return get(); }

    std::string SubUnivariateBasis::getRepresentation() const { return (*this)->getRepresentation() ; }
    std::string SubUnivariateBasisNode::getRepresentation() const {
        return "SubUnivariateBasis object"; 
    }

    SubBasis SubUnivariateBasis::operator+ (const SubBasis& other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariateBasis::operator+ (const SubMonomialeBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariateBasis::operator+ (const SubBSplineBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariateBasis::operator* (const SubBasis& other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariateBasis::operator* (const SubMonomialeBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariateBasis::operator* (const SubBSplineBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    int UnivariateBasis::getLenght ( ) const  { return (*this)->getLenght(); } 
    int UnivariateBasisNode::getLenght ( ) const {
         assert(0);
    }
    
    void  UnivariateBasis::setDegree (int degree) { (*this)->setDegree (degree); } 
    void  UnivariateBasisNode::setDegree (int degree) {
         this->degree = degree;
    }
    
    int  UnivariateBasis::getDegree  ( ) const { return (*this)->getDegree (); } 
    int  UnivariateBasisNode::getDegree  ( ) const {
         return degree ;
    }
}  // namespace spline

