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

    std::string BasisNode::to_string() const {return "A Basis object on " + domain().to_string();};

    Domain BasisNode::domain() const {return domain_;}
    Domain Basis::domain() const {return (*this)->domain();}

    Basis Basis::operator+ (const Basis& other) const { return (*this)->operator+(*other.get());}

    Basis Basis::operator* (const Basis& other) const { return (*this)->operator*(*other.get());}

    bool Basis::operator==(const Basis& rhs) const {
        if (this->get() == rhs.get()) return true;
        return (*this)->operator==(*rhs.get());
    }

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

    AnyTensor Basis::operator() (const AnyTensor& arg) const {
        AnyTensor x = arg.squeeze();
        if (x.is_vector()) x = x.as_vector();
        spline_assert(x.n_dims()<=2);

        if (x.n_dims()==1) {
          if (x.dims()[0] == n_inputs()) {
            std::vector<AnyScalar> a = x.unpack_1();
            return (*this)->operator()(a);
          } else {
            x = x.shape({x.numel(), 1});
          }
        }

        if(type() != "EmptyBasis"){
            spline_assert_message(x.dims()[1] == n_inputs(),
                    "Can evaluate list of " + std::to_string(n_inputs()) + " inputs. Got " +
                    std::to_string(x.dims()[0])+ " by " + std::to_string(x.dims()[1]));
        }

        std::vector< AnyTensor > ret ;
        std::vector< std::vector< AnyScalar > > unpacked_x = x.unpack_2();

        for (int i = 0; i < unpacked_x.size(); i++) {
            ret.push_back((*this)->operator()(unpacked_x[i]));
        }
        return AnyVector::pack(ret, 0);
    }

    AnyTensor Basis::list_eval(const AnyTensor& arg) const {

        AnyTensor x = arg;
        spline_assert(x.n_dims()<=2);
        if (x.is_vector()) x = x.shape({x.numel(), 1});
        if(type() != "EmptyBasis"){
            spline_assert_message(x.dims()[1] == n_inputs(),
                    "Can evaluate list of " + std::to_string(n_inputs()) + " inputs. Got " +
                    std::to_string(x.dims()[0])+ " by " + std::to_string(x.dims()[1]));
        }

        std::vector< AnyTensor > ret ;
        std::vector< std::vector< AnyScalar > > unpacked_x = x.unpack_2();

        for (int i = 0; i < unpacked_x.size(); i++) {
            ret.push_back((*this)->operator()(unpacked_x[i]));
        }
        return AnyVector::pack(ret, 0);
    }

    void BasisNode::assert_vector_lenght_correct(const AnyVector& x) const {
        spline_assert_message(x.size() == n_inputs(), "Input vector has wrong dimension.");
    }

    void BasisNode::assert_vector_lenght_correct(const AnyTensor& x) const {
        spline_assert_message(x.dims()[1] == n_inputs(), "Input vector has wrong dimension.");
    }

    AnyTensor Basis::evaluation_grid() const {
      return (*this)->evaluation_grid();
    }
    AnyTensor BasisNode::evaluation_grid() const {
      return AnyTensor();
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

        Domain d_int = domain().intersection(b.domain());

        AnyTensor B21 = b21.integral(d_int);
        AnyTensor B22 = b22.integral(d_int);

        AnyTensor T = B22.solve(B21);
        return T;
    }

    AnyTensor Basis::project_to(const Basis& b) const {
        return (*this)->project_to(b);
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

    Basis Basis::transform_to(const Basis& b, AnyTensor& T) const {
        return (*this)->transform_to(b, T);
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

    AnyTensor Basis::const_coeff_tensor(const AnyTensor& t) const {
        return (*this)->const_coeff_tensor(t);
    }
    AnyTensor BasisNode::const_coeff_tensor(const AnyTensor& t) const {
        spline_assert(false);
        return AnyTensor();
    }

} // namespace spline
