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

    AnyTensor Basis::operator() (const AnyVector & x) const {
      std::vector<AnyScalar> a = x.to_scalar_vector();
      (*this)->assert_vector_lenght_correct( a );
      return (*this)->operator()(a);
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

    Basis BasisNode::insert_knots(const AnyVector & new_knots, AnyTensor & T) const {
        spline_assert_message(false, getRepresentation() << " can not insert knots.");
        return Basis();
    }

    Basis BasisNode::midpoint_refinement(int refinement, AnyTensor& T) const {
        spline_assert_message(false, getRepresentation() << " can not refine knots.");
        return Basis();
    }

    Basis Basis::insert_knots(const AnyVector & new_knots, AnyTensor & T) const {
        return (*this)->insert_knots(new_knots, T);
    }

    Basis Basis::midpoint_refinement(int refinement, AnyTensor& T) const {
        return (*this)->midpoint_refinement(refinement, T);
    }


    int Basis::dimension() const { return (*this)->dimension();}
    int Basis::n_inputs() const { return (*this)->n_inputs();}

    Basis Basis::derivative(AnyTensor& T) const {
        // default derivative is with order = 1
        return (*this)->derivative(1, T);
    }

    Basis Basis::derivative(int order, AnyTensor& T) const {
        return (*this)->derivative(order, T);
    }

    Basis BasisNode::derivative(int order, AnyTensor& T) const {
        spline_assert(false);
        return Basis();
    }

    AnyTensor BasisNode::const_coeff_tensor(const AnyTensor& t) const {
        spline_assert(false);
        return AnyTensor();
    }

    AnyTensor Basis::const_coeff_tensor(const AnyTensor& t) const {
        return (*this)->const_coeff_tensor(t);
    }



} // namespace spline
