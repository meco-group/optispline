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
      (*this)->assert_vector_lenght_correct( x );
      return (*this)->operator()(x);
    }

    AnyTensor BasisNode::operator() (const std::vector< AnyScalar > & x) const {
        assert(false); //Abstract
        return AnyTensor();
    }

    std::vector< std::vector < AnyScalar > > Basis::getEvaluationGrid() const {
      return (*this)->getEvaluationGrid();
    }
    std::vector< std::vector < AnyScalar > > BasisNode::getEvaluationGrid() const {
      return std::vector< std::vector < AnyScalar > >();
    }

    int Basis::dimension() const { return (*this)->dimension();}
    int Basis::n_inputs() const { return (*this)->n_inputs();}

    AnyTensor BasisNode::const_coeff_tensor(const AnyTensor& t) const {
        spline_assert(false);
        return AnyTensor();
    }

    AnyTensor Basis::const_coeff_tensor(const AnyTensor& t) const {
        return (*this)->const_coeff_tensor(t);
    }

} // namespace spline
