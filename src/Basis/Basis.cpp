#include "Basis.h"
#include "operations/operationsBasis.h"
#include "../Function/Function.h"
#include "../common.h"

namespace spline {

    BasisNode::BasisNode(const Domain& domain) : domain_(domain) { }

    BasisNode* Basis::get() const { return static_cast<BasisNode*>(SharedObject::get()); };
    BasisNode* Basis::operator->() const { return get(); }

    std::string BasisNode::getRepresentation() const {return "Basis";};
    std::string Basis::getRepresentation() const { return (*this)->getRepresentation() ;};

    Domain BasisNode::domain() const {return domain_;}
    Domain Basis::domain() const {return (*this)->domain();}

    Basis Basis::operator+ (const Basis& other) const { return (*this)->operator+(other);}

    Basis Basis::operator+ (const EmptyBasis& other) const { return (*this)->operator+(other);}
    Basis BasisNode::operator+ (const EmptyBasis& other) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::operator+ (const MonomialBasis& other) const { return (*this)->operator+(other); }
    Basis Basis::operator+ (const BSplineBasis& other) const {return (*this)->operator+(other);}

    Basis Basis::operator* (const Basis& other) const { return (*this)->operator*(other);}
    Basis Basis::operator* (const EmptyBasis& other) const { return (*this)->operator*(other);}
    Basis BasisNode::operator* (const EmptyBasis& other) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::operator* (const MonomialBasis& other) const { return (*this)->operator*(other); }
    Basis Basis::operator* (const BSplineBasis& other) const {return (*this)->operator*(other);}

    AnyTensor Basis::operator() (const AnyVector & x) const {
      std::vector<AnyScalar> a = x.to_scalar_vector();
      (*this)->assert_vector_lenght_correct(a);
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

    Function BasisNode::basis_functions() const {
        AnyTensor t = DT(casadi::DM::densify(casadi::DM::eye(dimension())));
        return Function(shared_from_this<Basis>(), Coefficient(t));
    }
    
    Function Basis::basis_functions() const {
        return (*this)->basis_functions();
    }


    int Basis::dimension() const { return (*this)->dimension();}
    int Basis::n_inputs() const { return (*this)->n_inputs();}

    Basis Basis::derivative(AnyTensor& T) const {
        return (*this)->derivative(1, T);
    }

    Basis Basis::derivative(int order, AnyTensor& T) const {
        return (*this)->derivative(order, T);
    }

    Basis BasisNode::derivative(int order, AnyTensor& T) const {
        spline_assert(false);
        return Basis();
    }

    Basis Basis::antiderivative(AnyTensor& T) const {
        return (*this)->antiderivative(1, T);
    }

    Basis Basis::antiderivative(int order, AnyTensor& T) const {
        return (*this)->antiderivative(order, T);
    }

    Basis BasisNode::antiderivative(int order, AnyTensor& T) const {
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
