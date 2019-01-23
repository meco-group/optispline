#include "Basis.h"
#include "BasisNode.h"
#include "operations/operationsBasis.h"
#include "../Function/Function.h"
#include "../common.h"

namespace spline {

    BasisNode* Basis::get() const { return static_cast<BasisNode*>(SharedObject::get()); };
    BasisNode* Basis::operator->() const { return get(); }

    std::string Basis::type() const { return (*this)->type() ;}
    Domain Basis::domain() const {return (*this)->domain();}

    Basis Basis::operator+ (const Basis& other) const { return (*this)->operator+(*other.get());}

    Basis Basis::operator* (const Basis& other) const { return (*this)->operator*(*other.get());}

    bool Basis::operator==(const Basis& rhs) const {
        if (this->get() == rhs.get()) return true;
        return (*this)->operator==(*rhs.get());
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

        for (casadi_int i = 0; i < unpacked_x.size(); i++) {
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

        for (casadi_int i = 0; i < unpacked_x.size(); i++) {
            ret.push_back((*this)->operator()(unpacked_x[i]));
        }
        return AnyVector::pack(ret, 0);
    }

    AnyTensor Basis::evaluation_grid() const {
      return (*this)->evaluation_grid();
    }
    Basis Basis::insert_knots(const AnyVector & new_knots, AnyTensor & T) const {
        return (*this)->insert_knots(new_knots, T);
    }

    Basis Basis::midpoint_refinement(casadi_int refinement, AnyTensor& T) const {
        return (*this)->midpoint_refinement(refinement, T);
    }

    Basis Basis::degree_elevation(casadi_int elevation, AnyTensor & T) const {
        return (*this)->degree_elevation(elevation, T);
    }

    Basis Basis::kick_boundary(const Domain& boundary, AnyTensor & T) const {
        return (*this)->kick_boundary(boundary, T);
    }

    AnyTensor Basis::project_to(const Basis& b) const {
        return (*this)->project_to(b);
    }

    Basis Basis::transform_to(const Basis& b, AnyTensor& T) const {
        return (*this)->transform_to(b, T);
    }

    Function Basis::basis_functions() const {
        return (*this)->basis_functions();
    }

    casadi_int Basis::dimension() const { return (*this)->dimension();}
    casadi_int Basis::n_inputs() const { return (*this)->n_inputs();}

    Basis Basis::derivative(AnyTensor& T) const {
        return (*this)->derivative(1, T);
    }

    Basis Basis::derivative(casadi_int order, AnyTensor& T) const {
        return (*this)->derivative(order, T);
    }

    Basis Basis::antiderivative(AnyTensor& T) const {
        return (*this)->antiderivative(1, T);
    }

    Basis Basis::antiderivative(casadi_int order, AnyTensor& T) const {
        return (*this)->antiderivative(order, T);
    }

    AnyTensor Basis::integral(const Domain& domain) const {
        return (*this)->integral(domain);
    }

    AnyTensor Basis::const_coeff_tensor(const AnyTensor& t) const {
        return (*this)->const_coeff_tensor(t);
    }

} // namespace spline
