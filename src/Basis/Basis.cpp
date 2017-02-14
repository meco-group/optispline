#include "Basis.h"
#include "operations/operationsBasis.h"
#include "../common.h"

namespace spline {

    BasisNode* Basis::get() const { return static_cast<BasisNode*>(SharedObject::get()); };
    BasisNode* Basis::operator->() const { return get(); }

    Basis::Basis() {
        assign_node(new BasisNode());
    }

    std::string BasisNode::getRepresentation() const {return "Basis";};
    std::string Basis::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::ostream& operator<<(std::ostream &stream, const Basis& base) {
        return stream << base.getRepresentation();
    }

    Basis Basis::operator+ (const Basis& other) const { return (*this)->operator+(other);}
    Basis BasisNode::operator+ (const Basis& other) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::operator+ (const DummyBasis& other) const { return (*this)->operator+(other);}
    Basis BasisNode::operator+ (const DummyBasis& other) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::operator+ (const MonomialBasis& other) const { return (*this)->operator+(other); }
    Basis BasisNode::operator+ (const MonomialBasis& other) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::operator+ (const BSplineBasis& other) const {return (*this)->operator+(other);}
    Basis BasisNode::operator+ (const BSplineBasis& other) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::operator* (const Basis& other) const { return (*this)->operator*(other);}
    Basis BasisNode::operator* (const Basis& other) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::operator* (const DummyBasis& other) const { return (*this)->operator*(other);}
    Basis BasisNode::operator* (const DummyBasis& other) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::operator* (const MonomialBasis& other) const { return (*this)->operator*(other); }
    Basis BasisNode::operator* (const MonomialBasis& other) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::operator* (const BSplineBasis& other) const {return (*this)->operator*(other);}
    Basis BasisNode::operator* (const BSplineBasis& other) const {
        spline_assert(false);
        return Basis();
    }

    AnyTensor Basis::operator() (const std::vector< AnyScalar > & x) const {
      return (*this)->operator()(x);
    }

    AnyTensor BasisNode::operator() (const std::vector< AnyScalar > & x) const {
        assert(false); //Abstract
        return AnyTensor();
    }

    int Basis::n_inputs() const { return (*this)->n_inputs();}
    std::vector< int > Basis::getShape() const { return (*this)->getShape();}

    void Basis::getEvaluationGrid(std::vector< std::vector < AnyScalar > > * eg) const {
      (*this)->getEvaluationGrid(eg);
    }
    void BasisNode::getEvaluationGrid(std::vector< std::vector < AnyScalar > > * eg) const {}

    int Basis::dimension() const { return (*this)->dimension();}
} // namespace spline
