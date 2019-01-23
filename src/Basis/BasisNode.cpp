#include "BasisNode.h"
#include "operations/operationsBasis.h"
#include "../Function/Function.h"
#include "../common.h"

namespace spline {

    BasisNode::BasisNode(const Domain& domain) : domain_(domain) { }

    std::string BasisNode::type() const { return "Basis";}

    std::string BasisNode::to_string() const {return "A Basis object on " + domain().to_string();};

    Domain BasisNode::domain() const {return domain_;}
    bool BasisNode::operator==(const BasisNode& rhs) const {
      spline_assert_message(false, "operator== not implemented on " << type() << " and " << type());
      return false;
    }

    bool BasisNode::operator==(const EmptyBasisNode& rhs) const {
      spline_assert_message(false, "operator== not implemented on " << type() << " and " << type());
      return false;
    }

    bool BasisNode::operator==(const MonomialBasisNode& rhs) const {
      spline_assert_message(false, "operator== not implemented on " << type() << " and " << type());
      return false;
    }

    bool BasisNode::operator==(const BSplineBasisNode& rhs) const {
      spline_assert_message(false, "operator== not implemented on " << type() << " and " << type());
      return false;
    }

    AnyTensor BasisNode::operator() (const std::vector< AnyScalar > & x) const {
        assert(false); //Abstract
        return AnyTensor();
    }

    void BasisNode::assert_vector_lenght_correct(const AnyVector& x) const {
        spline_assert_message(x.size() == n_inputs(), "Input vector has wrong dimension.");
    }

    void BasisNode::assert_vector_lenght_correct(const AnyTensor& x) const {
        spline_assert_message(x.dims()[1] == n_inputs(), "Input vector has wrong dimension.");
    }

    AnyTensor BasisNode::evaluation_grid() const {
      return AnyTensor();
    }

    Basis BasisNode::insert_knots(const AnyVector & new_knots, AnyTensor & T) const {
        spline_assert_message(false, type() << " can not insert knots.");
        return Basis();
    }

    Basis BasisNode::midpoint_refinement(casadi_int refinement, AnyTensor& T) const {
        spline_assert_message(false, type() << " can not refine knots.");
        return Basis();
    }

    Basis BasisNode::degree_elevation(casadi_int elevation, AnyTensor& T) const {
        spline_assert_message(false, type() << " can not elevate the degree.");
        return Basis();
    }

    Basis BasisNode::kick_boundary(const Domain& boundary, AnyTensor& T) const {
        spline_assert_message(false, type() << " can not kick its boundaries.");
        return Basis();
    }

    AnyTensor BasisNode::project_to(const Basis& b) const {
        Function b1 = basis_functions();
        Function b2 = b.basis_functions();

        Function b21  = b2.mtimes(b1.transpose());
        Function b22  = b2.mtimes(b2.transpose());

        Domain d_int = domain().intersection(b.domain());

        AnyTensor B21 = b21.integral(d_int);
        AnyTensor B22 = b22.integral(d_int);

        AnyTensor T = B22.solve(B21);
        return T;
    }

    Basis BasisNode::transform_to(const Basis& b, AnyTensor& T) const {
        Basis basis = shared_from_this<Basis>();
        Basis union_basis = basis + b;
        AnyTensor eval_grid = union_basis.evaluation_grid();

        std::vector< AnyTensor > pre_step{ AnyTensor::unity()};
        std::vector< AnyTensor > union_basis_eval;
        std::vector< AnyTensor > basis_eval;
        for(auto const & point : eval_grid.unpack_2()){
            AnyTensor points = AnyVector(point);
            union_basis_eval.push_back(union_basis(points));
            basis_eval.push_back(basis(points));
        }
        AnyTensor A = AnyTensor::pack(union_basis_eval, 0);
        AnyTensor B = AnyTensor::pack(basis_eval, 0);
        T = A.solve(B);
        return union_basis;
    }

    Function BasisNode::basis_functions() const {
        AnyTensor t = DT(casadi::DM::densify(casadi::DM::eye(dimension())));
        return Function(shared_from_this<Basis>(), Coefficient(t));
    }

    Basis BasisNode::derivative(casadi_int order, AnyTensor& T) const {
        spline_assert(false);
        return Basis();
    }

    Basis BasisNode::antiderivative(casadi_int order, AnyTensor& T) const {
        spline_assert(false);
        return Basis();
    }

    AnyTensor BasisNode::integral(const Domain& domain) const {
        spline_assert(false);
        return AnyTensor();
    }

    AnyTensor BasisNode::const_coeff_tensor(const AnyTensor& t) const {
        spline_assert(false);
        return AnyTensor();
    }

} // namespace spline
