#include "Basis.h"
#include "operations/operationsBasis.h"
#include "../Function/Function.h"
#include "../common.h"

namespace spline {

    BasisNode::BasisNode(const Domain& domain) : domain_(domain) { }

    BasisNode* Basis::get() const { return static_cast<BasisNode*>(SharedObject::get()); };
    BasisNode* Basis::operator->() const { return get(); }

    std::string Basis::type() const { return (*this)->type() ;}
    std::string BasisNode::type() const { return "Basis";}

    std::string Basis::to_string() const { return (*this)->to_string() ;};
    std::string BasisNode::to_string() const {return "A Basis object on " + domain().to_string();};

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
        spline_assert_message(false, type() << " can not insert knots.");
        return Basis();
    }

    Basis Basis::insert_knots(const AnyVector & new_knots, AnyTensor & T) const {
        return (*this)->insert_knots(new_knots, T);
    }

    Basis BasisNode::midpoint_refinement(int refinement, AnyTensor& T) const {
        spline_assert_message(false, type() << " can not refine knots.");
        return Basis();
    }

    Basis Basis::midpoint_refinement(int refinement, AnyTensor& T) const {
        return (*this)->midpoint_refinement(refinement, T);
    }

    Basis BasisNode::degree_elevation(int elevation, AnyTensor& T) const {
        spline_assert_message(false, type() << " can not elevate the degree.");
        return Basis();
    }

    Basis Basis::degree_elevation(int elevation, AnyTensor & T) const {
        return (*this)->degree_elevation(elevation, T);
    }

    Basis BasisNode::kick_boundary(const Domain& boundary, AnyTensor& T) const {
        spline_assert_message(false, type() << " can not kick its boundaries.");
        return Basis();
    }

    Basis Basis::kick_boundary(const Domain& boundary, AnyTensor & T) const {
        return (*this)->kick_boundary(boundary, T);
    }

    AnyTensor BasisNode::project_to(const Basis& b) const {
        Function b1 = basis_functions();
        Function b2 = b.basis_functions();

        Function b21  = b2.mtimes(b1.transpose());
        Function b22  = b2.mtimes(b2.transpose());

        AnyTensor B21 = b21.integral();
        AnyTensor B22 = b22.integral();

        AnyTensor T = B22.solve(B21);
        return T;
    }

    AnyTensor Basis::project_to(const Basis& b) const {
        return (*this)->project_to(b);
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

    AnyTensor BasisNode::integral(const Domain& domain) const {
        spline_assert(false);
        return AnyTensor();
    }

    AnyTensor Basis::integral(const Domain& domain) const {
        return (*this)->integral(domain);
    }

    AnyTensor BasisNode::const_coeff_tensor(const AnyTensor& t) const {
        spline_assert(false);
        return AnyTensor();
    }

    AnyTensor Basis::const_coeff_tensor(const AnyTensor& t) const {
        return (*this)->const_coeff_tensor(t);
    }

} // namespace spline
