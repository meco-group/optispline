#include "TensorBasis.h"
#include "TensorBasisNode.h"
#include "EmptyBasis.h"
#include "BSplineBasis.h"
#include "BSplineBasisNode.h"
#include "../common.h"
#include "operations/operationsBasis.h"
#include "../Function/Function.h"

namespace spline {

    TensorBasisNode* TensorBasis::get() const {
        return static_cast<TensorBasisNode*>(SharedObject::get());
    }

    TensorBasisNode* TensorBasis::operator->() const { return get(); }

    TensorBasis::TensorBasis() {
        assign_node(new TensorBasisNode(std::vector< Basis >{}));
    }

    TensorBasis::TensorBasis(const std::vector< TensorBasis >& allBasis) {
        std::vector< Basis > bases_ {};
        for ( auto &basis : allBasis ) {
            for ( auto &subBasis : basis.bases() ) {
                bases_.push_back(subBasis);
            }
        }
        assign_node(new TensorBasisNode(bases_));
    }

    TensorBasis::TensorBasis(const Basis & bases_) {
        assign_node(new TensorBasisNode(std::vector< Basis > {bases_}));
    }

    TensorBasis::TensorBasis(const std::vector< Basis >& bases_) {
        assign_node(new TensorBasisNode(bases_));
    }

    TensorBasis::TensorBasis(const Basis& bases_, const std::string& args) {
        assign_node(new TensorBasisNode(std::vector< Basis > {bases_}, std::vector< std::string > {args}));
    }

    TensorBasis::TensorBasis(const TensorBasis& tensor, const std::vector< std::string >& args) {
        assign_node(new TensorBasisNode(tensor.bases(), args));
    }

    TensorBasis::TensorBasis(const std::vector< Basis >& bases_,
            const std::vector< std::string >& args) {
        assign_node(new TensorBasisNode(bases_, args));
    }

    casadi_int TensorBasis::n_basis() const { return (*this)->n_basis(); }
    std::vector< std::string > TensorBasis::arguments() const {
        return (*this)->arguments();
    }
    std::vector<Argument> TensorBasis::arguments_temp() const {
        return (*this)->arguments_temp();
    }
    std::string TensorBasis::argument(casadi_int index) const {
        return (*this)->argument(index);
    }
    casadi_int TensorBasis::index_argument(const Argument& a) const { return (*this)->index_argument(a); }
    casadi_int TensorBasis::indexArgument(const std::string& a) const { return (*this)->indexArgument(a); }
    bool TensorBasis::valid_argument_list(const std::vector<Argument>& args) const { return (*this)->valid_argument_list(args);}
    bool TensorBasis::valid_argument(const Argument& a) const{ return (*this)->valid_argument(a);}
    bool TensorBasis::hasArguments() const { return (*this)->hasArguments();}
    std::vector< Basis > TensorBasis::bases() const { return (*this)->bases (); }
    Basis TensorBasis::basis() const {
        return (*this)->basis();
    }

    Basis TensorBasis::basis(const std::string& a) const {
        return (*this)->basis(a);
    }

    Basis TensorBasis::basis(const Argument& index) const {
        return (*this)->basis(index);
    }

    TensorBasis TensorBasis::add_basis(TensorBasis tensor_basis)
        const {return (*this)->add_basis(tensor_basis);}
    TensorBasis TensorBasis::add_basis(Basis basis) const {return (*this)->add_basis(basis);}
    TensorBasis TensorBasis::substitute_bases(const std::vector<Argument>& indices,
            const std::vector<Basis>& bases) const {
        return (*this)->substitute_bases(indices, bases);
    }

    std::vector<casadi_int> TensorBasis::dimension() const { return (*this)->dimension ();}
    TensorDomain TensorBasis::domain() const { return (*this)->domain(); }

    std::string TensorBasis::type() const { return (*this)->type() ;}
    TensorBasis TensorBasis::operator+ (const TensorBasis& other) const {
        return (*this)->operator+(other);
    }
    TensorBasis TensorBasis::operator* (const TensorBasis& other) const {
        return (*this)->operator*(other);
    }
    AnyTensor TensorBasis::operator() (const std::vector< AnyScalar > &  x,
            const std::vector< Argument >& arg_ind, bool reorder_output) const {
        /* spline_assert_message(x.size() == arg_ind.size() || n_basis() == 0, */
        /*         "Can evaluate list of " + std::to_string(n_inputs()) + " inputs. Got " + */
        /*         std::to_string(x.size())); */
        if(arg_ind.size() != 0){
            spline_assert_message(x.size() == arg_ind.size(),
                    "Can evaluate vector and argument list have different length.");
            spline_assert_message(this->valid_argument_list(arg_ind),
                    "List of argument does not cover all required arguments.");
        }
        spline_assert_message(x.size() >= n_inputs(),
                "Can evaluate vector and argument list have different length.");
        return (*this)->operator()(x, Argument::concrete(arg_ind, *this), reorder_output);
    }
    std::vector< AnyTensor >  TensorBasis::evaluation_grid () const {
        return (*this)->evaluation_grid();
    }

    AnyTensor TensorBasis::grid_eval (const std::vector< AnyTensor > &  x,
            const std::vector< Argument >& arg_ind, bool reorder_output) const {
        return (*this)->grid_eval(x, Argument::concrete(arg_ind, *this), reorder_output);
    }

    bool TensorBasis::operator==(const TensorBasis& rhs) const {
        if(this->get() == rhs.get()) return true;
        return (*this)->operator==(rhs);
    }
    casadi_int TensorBasis::totalNumberBasisFunctions() const {
        return (*this)->totalNumberBasisFunctions();
    }
    casadi_int TensorBasis::n_inputs() const {
        return (*this)->n_inputs();
    }
    std::vector< casadi_int > TensorBasis::n_inputs_list() const {
        return (*this)->n_inputs_list();
    }
    AnyTensor TensorBasis::const_coeff_tensor(const AnyTensor& t) const {
        return (*this)->const_coeff_tensor(t);
    }
    TensorBasis TensorBasis::insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<Argument>& args, std::vector<AnyTensor> & T) const {
        return (*this)->insert_knots(new_knots, Argument::concrete(args, arguments()), T);
    }

    TensorBasis TensorBasis::midpoint_refinement(const std::vector<casadi_int> & refinement,
            const std::vector<Argument>& args, std::vector<AnyTensor> & T) const {
        return (*this)->midpoint_refinement(refinement, Argument::concrete(args, arguments()), T);
    }

    TensorBasis TensorBasis::degree_elevation(const std::vector<casadi_int>& elevation,
            const std::vector<Argument>& args, std::vector<AnyTensor> & T) const {
        return (*this)->degree_elevation(elevation, Argument::concrete(args, arguments()), T);
    }

    TensorBasis TensorBasis::kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const {
        return (*this)->kick_boundary(boundary, arg_ind, T);
    }

    TensorBasis TensorBasis::kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string>& args, std::vector<AnyTensor> & T) const {
        return (*this)->kick_boundary(boundary, args, T);
    }

    TensorBasis TensorBasis::derivative(const std::vector<Argument>& arg,
            std::vector<AnyTensor>& T) const {
        return (*this)->derivative(std::vector<casadi_int>(1, arg.size()), Argument::concrete(arg, arguments()), T);
    }

    TensorBasis TensorBasis::derivative(const std::vector<casadi_int>& order,
            const std::vector<Argument>& arg, std::vector<AnyTensor>& T) const {
        return (*this)->derivative(order, Argument::concrete(arg, arguments()), T);
    }

    TensorBasis TensorBasis::antiderivative(const std::vector<Argument>& arg,
            std::vector<AnyTensor>& T) const {
        return (*this)->antiderivative(std::vector<casadi_int>(1, arg.size()), Argument::concrete(arg, arguments()), T);
    }

    TensorBasis TensorBasis::antiderivative(const std::vector<casadi_int>& order,
            const std::vector<Argument>& arg, std::vector<AnyTensor>& T) const {
        return (*this)->antiderivative(order, Argument::concrete(arg, arguments()), T);
    }

    std::vector<AnyTensor> TensorBasis::integral(const TensorDomain& domain) const {
        return (*this)->integral(domain);
    }

    TensorBasis TensorBasis::partial_integral(const TensorDomain& domain,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor>& T) const {
        return (*this)->partial_integral(domain, arg_ind, T);
    }

    TensorBasis TensorBasis::partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args, std::vector<AnyTensor>& T) const {
        return (*this)->partial_integral(domain, args, T);
    }

    spline::Function TensorBasis::basis_functions() const {
        return (*this)->basis_functions();
    }

    AnyTensor TensorBasis::project_to(const TensorBasis& b) const {
        return (*this)->project_to(b);
    }

    TensorBasis TensorBasis::transform_to(const TensorBasis& b, std::vector<AnyTensor>& T) const {
        return (*this)->transform_to(b, T);
    }

    std::vector< casadi_int > TensorBasis::get_permutation(const TensorBasis& grid) const{
        return (*this)->get_permutation(grid);
    }

    std::vector<casadi_int> TensorBasis::vectorize(const Argument& arg) const{
        if(arg.is_all()){
            return casadi::range(n_basis());
        } else {
            return {arg.concrete(arguments())};
        }
    }

} // namespace spline
