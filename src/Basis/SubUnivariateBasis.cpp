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

    SubBasis SubUnivariateBasis::operator+ (const SubMonomialBasis &other) const  {
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

    SubBasis SubUnivariateBasis::operator* (const SubMonomialBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    SubBasis SubUnivariateBasis::operator* (const SubBSplineBasis &other) const  {
        assert(false); //  Univariate is "abstract"
        return SubBasis();
    } 

    int SubUnivariateBasis::getLenght ( ) const  { return (*this)->getLenght(); } 
    int SubUnivariateBasisNode::getLenght ( ) const {
         assert(0);
    }
    
    void  SubUnivariateBasis::setDegree (int degree) { (*this)->setDegree (degree); } 
    void  SubUnivariateBasisNode::setDegree (int degree) {
         this->degree = degree;
    }
    
    int  SubUnivariateBasis::getDegree  ( ) const { return (*this)->getDegree (); } 
    int  SubUnivariateBasisNode::getDegree  ( ) const {
         return degree ;
    }

    int SubUnivariateBasisNode::getDimension() const { return 1; }
}  // namespace spline

