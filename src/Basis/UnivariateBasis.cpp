#include <vector>
#include "UnivariateBasis.h"

#include "operations/operationsBasis.h"
#include "../common.h"

namespace spline {
    UnivariateBasisNode::UnivariateBasisNode(int degree_) :degree(degree_) { }
    UnivariateBasisNode* UnivariateBasis::get() const {
        return static_cast<UnivariateBasisNode*>(SharedObject::get());
    };
    UnivariateBasisNode* UnivariateBasis::operator->() const { return get(); }

    std::string UnivariateBasis::getRepresentation() const { return (*this)->getRepresentation() ; }
    std::string UnivariateBasisNode::getRepresentation() const {
        return "UnivariateBasis object";
    }

    Basis UnivariateBasisNode::operator+ (const Basis& other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return Basis();
    }

    Basis UnivariateBasisNode::operator+ (const DummyBasis& other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return Basis();
    }

    Basis UnivariateBasisNode::operator+ (const MonomialBasis &other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return Basis();
    }

    Basis UnivariateBasisNode::operator+ (const BSplineBasis &other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return Basis();
    }

    Basis UnivariateBasisNode::operator* (const Basis& other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return Basis();
    }

    Basis UnivariateBasisNode::operator* (const DummyBasis& other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return Basis();
    }

    Basis UnivariateBasisNode::operator* (const MonomialBasis &other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return Basis();
    }

    Basis UnivariateBasisNode::operator* (const BSplineBasis &other) const  {
        spline_assert(false); //  Univariate is "abstract"
        return Basis();
    }


    AnyTensor UnivariateBasisNode::const_coeff_tensor(const AnyTensor& t) const {
        spline_assert(false);
        return AnyTensor();
    }

    int UnivariateBasis::getLength() const  { return (*this)->getLength(); }
    int UnivariateBasisNode::getLength() const {
         assert(0);
    }

    void UnivariateBasis::setDegree(int degree) { (*this)->setDegree(degree); }
    void UnivariateBasisNode::setDegree(int degree) {
         this->degree = degree;
    }

    int  UnivariateBasis::getDegree() const { return (*this)->getDegree (); }
    int  UnivariateBasisNode::getDegree() const {
         return degree ;
    }

    int UnivariateBasisNode::n_inputs() const { return 1; }


    Basis UnivariateBasis::derivative(int order, AnyTensor& T) const {
        return (*this)->univariate_derivative(order, T);
    }
    Basis UnivariateBasis::univariate_derivative(int order, AnyTensor& T) const {
        return (*this)->univariate_derivative(order, T);
    }
    // UnivariateBasisNode implementation is not necessary since you will never make a UnivariateBasis?



}  // namespace spline
