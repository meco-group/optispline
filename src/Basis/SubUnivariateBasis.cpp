#include <vector>
#include "SubUnivariateBasis.h"

#include "operations/operationsBasis.h"
#include "../common.h"

namespace spline {
    SubUnivariateBasisNode::SubUnivariateBasisNode(int degree_) :degree(degree_){ }
    SubUnivariateBasisNode* SubUnivariateBasis::get() const { return static_cast<SubUnivariateBasisNode*>(SharedObject::get()); };
    SubUnivariateBasisNode* SubUnivariateBasis::operator->() const { return get(); }

    std::string SubUnivariateBasis::getRepresentation() const { return (*this)->getRepresentation() ; }
    std::string SubUnivariateBasisNode::getRepresentation() const {
        return "SubUnivariateBasis object";
    }

    SubBasis SubUnivariateBasisNode::operator+ (const SubBasis& other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return SubBasis();
    }

    SubBasis SubUnivariateBasisNode::operator+ (const SubBasisDummy& other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return SubBasis();
    }

    SubBasis SubUnivariateBasisNode::operator+ (const SubMonomialBasis &other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return SubBasis();
    }

    SubBasis SubUnivariateBasisNode::operator+ (const SubBSplineBasis &other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return SubBasis();
    }

    SubBasis SubUnivariateBasisNode::operator* (const SubBasis& other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return SubBasis();
    }

    SubBasis SubUnivariateBasisNode::operator* (const SubBasisDummy& other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return SubBasis();
    }

    SubBasis SubUnivariateBasisNode::operator* (const SubMonomialBasis &other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return SubBasis();
    }

    SubBasis SubUnivariateBasisNode::operator* (const SubBSplineBasis &other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return SubBasis();
    }

    int SubUnivariateBasis::getLength ( ) const  { return (*this)->getLength(); }
    int SubUnivariateBasisNode::getLength ( ) const {
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

