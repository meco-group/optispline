#include "TensorBasis.h"
#include "TensorBasisConstant.h"
#include "EmptyBasis.h"
#include "BSplineBasis.h"
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

    TensorBasisNode::TensorBasisNode(const std::vector< Basis >& bases)
      : bases_(bases), allArguments(std::vector<std::string>{})
      {}

    TensorBasisNode::TensorBasisNode(const std::vector< Basis >& bases,
        const std::vector< std::string >& args) :
        bases_(bases), allArguments(args)
    {}

    int TensorBasis::n_basis() const { return (*this)->n_basis(); }
    int TensorBasisNode::n_basis() const {
        return bases_.size();
    }

    std::vector< std::string > TensorBasis::arguments() const {
        return (*this)->arguments();
    }
    std::vector< std::string > TensorBasisNode::arguments() const {
        return allArguments;
    }

    std::string TensorBasis::argument(int index) const {
        return (*this)->argument(index);
    }
    std::string TensorBasisNode::argument(int index) const {
        return allArguments[index];
    }

    int TensorBasis::indexArgument(std::string a) const { return (*this)->indexArgument(a); }
    int TensorBasisNode::indexArgument(std::string a) const {
        auto it = std::find(allArguments.begin(), allArguments.end(), a);
        int index;
        if (it == allArguments.end()) {
            index = -1;
        } else {
            index = std::distance(allArguments.begin(), it);
        }
        return  index;
    }

    bool TensorBasis::hasArguments() const { return (*this)->hasArguments();}
    bool TensorBasisNode::hasArguments() const {
        return allArguments.size() > 0;
    }

    std::vector< Basis > TensorBasis::bases() const { return (*this)->bases (); }
    std::vector< Basis > TensorBasisNode::bases() const {
        return bases_;
    }

    Basis TensorBasis::basis() const {
        return (*this)->basis();
    }

    Basis TensorBasisNode::basis() const {
        spline_assert(n_basis() == 1);
        return bases()[0];
    }

    Basis TensorBasis::basis(const std::string& a) const {
        return (*this)->basis(a);
    }

    Basis TensorBasisNode::basis(const std::string& a) const {
        int index = indexArgument(a);
        return basis(index);
    }

    Basis TensorBasis::basis(const Argument& index) const {
        return (*this)->basis(index);
    }

    Basis TensorBasisNode::basis(const Argument& index) const {
        int ind = index.concrete(arguments());
        spline_assert(ind < n_basis());
        if (ind < 0) {
            return EmptyBasis();
        }
        return bases()[ind];
    }

    TensorBasis TensorBasis::add_basis(TensorBasis tensor_basis)
    const {return (*this)->add_basis(tensor_basis);}
    TensorBasis TensorBasisNode::add_basis(TensorBasis tensor_basis) const {
        std::vector< Basis > new_bases = bases();
        for ( auto &subBasis : tensor_basis.bases() ) {
            new_bases.push_back(subBasis);
        }
        return TensorBasis(new_bases, arguments());
    }

    TensorBasis TensorBasis::add_basis(Basis basis) const {return (*this)->add_basis(basis);}
    TensorBasis TensorBasisNode::add_basis(Basis basis) const {
        std::vector< Basis > new_bases = bases();
        new_bases.push_back(basis);
        return TensorBasis(new_bases, arguments());
    }

    TensorBasis TensorBasis::substitute_bases(const std::vector<Argument>& indices,
        const std::vector<Basis>& bases) const {
        return (*this)->substitute_bases(indices, bases);
    }

    TensorBasis TensorBasisNode::substitute_bases(const std::vector<Argument>& indices,
        const std::vector<Basis>& bases) const {
        spline_assert(indices.size() == bases.size());
        std::vector<Basis> new_bases(0);
        for (int i = 0; i < n_basis(); i++) {
            int j;
            for (j = 0; j < indices.size(); j++) {
                if (i == indices[j].concrete(arguments())) {
                    break;
                }
            }
            if (j < indices.size()) {
                new_bases.push_back(bases[j]);
            } else {
                new_bases.push_back(bases_[i]);
            }
        }
        return TensorBasis(new_bases, allArguments);
    }

    std::vector<int> TensorBasis::dimension() const { return (*this)->dimension ();}
    std::vector<int> TensorBasisNode::dimension() const {
        std::vector<int> shape;
        for (auto const& b : bases()) {
            shape.push_back(b.dimension());
        }
        return shape;
    }

    TensorDomain TensorBasisNode::domain() const {
        std::vector<Domain> doms(n_basis());
        for (int i=0; i<n_basis(); i++) {
            doms[i] = basis(i).domain();
        }
        if (hasArguments()) {
            return TensorDomain(doms, arguments());
        } else {
            return TensorDomain(doms);
        }
    }

    TensorDomain TensorBasis::domain() const { return (*this)->domain(); }

    std::string TensorBasis::type() const { return (*this)->type() ;}
    std::string TensorBasisNode::type() const { return "TensorBasis";}

    std::string TensorBasis::to_string() const { return (*this)->to_string() ;}
    std::string TensorBasisNode::to_string() const {
        const std::string n_basis = (bases_.size()==1) ? "basis" : "bases";

        std::string str_basis;  // basis info
        std::vector<std::string> args = arguments();
        if (args.size() == 0){
            for (int i=0 ; i<bases_.size(); i++) {
                str_basis += "\t";
                str_basis += bases_[i].to_string();
                // str_basis += " over "
                // str_basis += domain(i).to_string();
                str_basis += "\n";
            }
            return "TensorBasis containing "  + std::to_string(bases_.size()) + " " +
                    n_basis + ":\n" + str_basis;
        }
        else{
            for (int i=0 ; i<bases_.size(); i++) {
                str_basis += "\t";
                str_basis += args[i] + ": ";
                str_basis += bases_[i].to_string();
                str_basis += "\n";
            }
            return "TensorBasis containing "  + std::to_string(bases_.size()) + " " + n_basis + " in " +
                std::to_string(allArguments.size()) + " arguments: \n " + str_basis;
        }
    }

    std::ostream& operator<<(std::ostream &stream, const TensorBasis& base) {
        return stream << base.to_string();
    }

    TensorBasis TensorBasis::operator+ (const TensorBasis& other) const {
        if(other.type() == "TensorBasisConstant"){
            return (*this)->operator+((TensorBasisConstant) other);
        }
        return (*this)->operator+(other);
    }
    TensorBasis TensorBasisNode::operator+ (const TensorBasis& other) const {
        return plus_tensor_basis(shared_from_this<TensorBasis>(), other);
    }

    TensorBasis TensorBasis::operator+ (const TensorBasisConstant& other) const {
        return (*this)->operator+(other);
    }
    TensorBasis TensorBasisNode::operator+ (const TensorBasisConstant& other) const {
        return shared_from_this<TensorBasis>();
    }

    TensorBasis TensorBasis::operator* (const TensorBasis& other) const {
        return (*this)->operator*(other);
    }
    TensorBasis TensorBasisNode::operator* (const TensorBasis& other) const {
        return times_tensor_basis(shared_from_this<TensorBasis>(), other);
    }

    TensorBasis TensorBasis::operator* (const TensorBasisConstant& other) const {
        return (*this)->operator+(other);
    }
    TensorBasis TensorBasisNode::operator* (const TensorBasisConstant& other) const {
        return shared_from_this<TensorBasis>();
    }

    AnyTensor TensorBasis::operator() (const std::vector< AnyScalar > &  x, const std::vector< Argument >& arg_ind) const {
        return (*this)->operator()(x, Argument::concrete(arg_ind, *this));
    }
    AnyTensor TensorBasisNode::operator() (const std::vector< AnyScalar > &  x, const std::vector< int >& arg_ind) const {

        spline_assert(x.size() == n_inputs());
        AnyTensor ret = AnyTensor::unity();

        std::vector< int > input_border_ = input_border();

        for (int i = 0; i < n_basis(); i++) {
            Basis b = basis(i);
            int index_arg = arg_ind[i];
            std::vector< AnyScalar > input = {};

            for (int i = input_border_[index_arg]; i < input_border_[index_arg + 1]; ++i) {
                input.push_back(x[i]);
            }
            ret = ret.outer_product(b(input));
        }
        return ret;
    }

    bool TensorBasis::operator==(const TensorBasis& rhs) const {
        if(this->get() == rhs.get()) return true;
        return (*this)->operator==(rhs);
    }
    bool TensorBasisNode::operator==(const TensorBasis& rhs) const {
        if( this->n_basis() != rhs.n_basis()) return false;
        for( int i = 0; i < n_basis(); i++){
            if( !( this->bases()[i] == rhs.bases()[i])) return false;
        }
        return true;
    }

    int TensorBasis::totalNumberBasisFunctions() const {
        return (*this)->totalNumberBasisFunctions();
    }
    int TensorBasisNode::totalNumberBasisFunctions() const {
        return spline::product(dimension());
    }

    int TensorBasis::n_inputs() const {
        return (*this)->n_inputs();
    }
    int TensorBasisNode::n_inputs() const {
        int r = 0;
        for (auto const& b : bases()) {
            r += b.n_inputs();
        }
        return r;
    }

    AnyTensor TensorBasis::const_coeff_tensor(const AnyTensor& t) const {
        return (*this)->const_coeff_tensor(t);
    }
    AnyTensor TensorBasisNode::const_coeff_tensor(const AnyTensor& t) const {
        AnyTensor prod = AnyTensor::unity();

        for (int i = 0; i < n_basis(); i++) {
            AnyTensor temp = bases_[i].const_coeff_tensor(AnyTensor(AnyScalar(1)));
            prod = prod.outer_product(temp.shape(std::vector< int >(1, temp.numel())));
        }

        return prod.outer_product(t);
    }

    TensorBasis TensorBasis::insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<std::string>& args, std::vector<AnyTensor> & T) const {
        NumericIndexVector arg_ind(args.size());
        for (int i = 0; i < args.size(); i++) {
            arg_ind[i] = indexArgument(args[i]);
        }
        return (*this)->insert_knots(new_knots, arg_ind, T);
    }

    TensorBasis TensorBasis::insert_knots(const std::vector<AnyVector> & new_knots,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const {
        return (*this)->insert_knots(new_knots, arg_ind, T);
    }

    TensorBasis TensorBasisNode::insert_knots(const std::vector<AnyVector>& new_knots,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const {
        spline_assert(arg_ind.size() == new_knots.size());
        std::vector<Basis> new_bases(arg_ind.size());
        std::vector<AnyTensor> T_(arg_ind.size());
        for (int i = 0; i < arg_ind.size(); i++) {
            new_bases[i] = basis(arg_ind[i]).insert_knots(new_knots[i], T_[i]);
        }
        T = T_;
        return substitute_bases(Argument::from_vector(arg_ind), new_bases);
    }

    TensorBasis TensorBasis::midpoint_refinement(const std::vector<int> & refinement,
            const std::vector<std::string>& args, std::vector<AnyTensor> & T) const {
        NumericIndexVector arg_ind(args.size());
        for (int i = 0; i < args.size(); i++) {
            arg_ind[i] = indexArgument(args[i]);
        }
        return (*this)->midpoint_refinement(refinement, arg_ind, T);
    }

    TensorBasis TensorBasis::midpoint_refinement(const std::vector<int> & refinement,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const {
        return (*this)->midpoint_refinement(refinement, arg_ind, T);
    }

    TensorBasis TensorBasisNode::midpoint_refinement(const std::vector<int>& refinement,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const {
        spline_assert(arg_ind.size() == refinement.size());
        std::vector<Basis> new_bases(arg_ind.size());
        std::vector<AnyTensor> T_(arg_ind.size());
        for (int i = 0; i < arg_ind.size(); i++) {
            new_bases[i] = basis(arg_ind[i]).midpoint_refinement(refinement[i], T_[i]);
        }
        T = T_;
        return substitute_bases(Argument::from_vector(arg_ind), new_bases);
    }

    TensorBasis TensorBasis::degree_elevation(const std::vector<int>& elevation,
            const std::vector<std::string>& args, std::vector<AnyTensor> & T) const {
        NumericIndexVector arg_ind(args.size());
        for (int i = 0; i < args.size(); i++) {
            arg_ind[i] = indexArgument(args[i]);
        }
        return (*this)->degree_elevation(elevation, arg_ind, T);
    }

    TensorBasis TensorBasis::degree_elevation(const std::vector<int>& elevation,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const {
        return (*this)->degree_elevation(elevation, arg_ind, T);
    }

    TensorBasis TensorBasisNode::degree_elevation(const std::vector<int>& elevation,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const {
        spline_assert(arg_ind.size() == elevation.size());
        std::vector<Basis> new_bases(arg_ind.size());
        std::vector<AnyTensor> T_(arg_ind.size());
        for (int i = 0; i < arg_ind.size(); i++) {
            new_bases[i] = basis(arg_ind[i]).degree_elevation(elevation[i], T_[i]);
        }
        T = T_;
        return substitute_bases(Argument::from_vector(arg_ind), new_bases);
    }

    TensorBasis TensorBasisNode::kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const {
        spline_assert(arg_ind.size() == boundary.n_domains());
        std::vector<Basis> new_bases(arg_ind.size());
        std::vector<AnyTensor> T_(arg_ind.size());
        for (int i = 0; i < arg_ind.size(); i++) {
            new_bases[i] = basis(arg_ind[i]).kick_boundary(boundary.domain(i), T_[i]);
        }
        T = T_;
        return substitute_bases(Argument::from_vector(arg_ind), new_bases);
    }

    TensorBasis TensorBasisNode::kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string>& args, std::vector<AnyTensor> & T) const {
        spline_assert(boundary.n_domains() == args.size());
        NumericIndexVector arg_ind(args.size());
        for (int i=0; i<args.size(); i++) {
            arg_ind[i] = indexArgument(args[i]);
        }
        if (boundary.hasArguments()) { // order domain
            std::vector<Domain> doms(args.size());
            for (int i=0; i<args.size(); i++) {
                doms[i] = boundary.domain(args[i]);
            }
            TensorDomain boundary2 = TensorDomain(doms, args);
            return partial_integral(boundary2, arg_ind, T);
        }
        return kick_boundary(boundary, arg_ind, T);
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
        return (*this)->derivative(std::vector<int>(1, arg.size()), Argument::concrete(arg, arguments()), T);
    }

    TensorBasis TensorBasis::derivative(const std::vector<int>& order,
      const std::vector<Argument>& arg, std::vector<AnyTensor>& T) const {
        return (*this)->derivative(order, Argument::concrete(arg, arguments()), T);
    }

    TensorBasis TensorBasis::antiderivative(const std::vector<Argument>& arg,
        std::vector<AnyTensor>& T) const {
        return (*this)->antiderivative(std::vector<int>(1, arg.size()), Argument::concrete(arg, arguments()), T);
    }

    TensorBasis TensorBasis::antiderivative(const std::vector<int>& order,
      const std::vector<Argument>& arg, std::vector<AnyTensor>& T) const {
        return (*this)->antiderivative(order, Argument::concrete(arg, arguments()), T);
    }

    TensorBasis TensorBasisNode::derivative(const std::vector<int>& orders,
        const NumericIndexVector& arg_ind, std::vector<AnyTensor>& T) const {
        // Call derivative on basis, for corresponding direction
        std::vector<Basis> new_bases(arg_ind.size());
        std::vector<AnyTensor> T_(arg_ind.size());
        for (int i = 0; i < arg_ind.size(); i++) {
            new_bases[i] = basis(arg_ind[i]).derivative(orders[i], T_[i]);
        }
        T = T_;
        return substitute_bases(Argument::from_vector(arg_ind), new_bases);
    }

    TensorBasis TensorBasisNode::antiderivative(const std::vector<int>& orders,
        const NumericIndexVector& arg_ind, std::vector<AnyTensor>& T) const {
        // Call antiderivative on basis, for corresponding direction
        std::vector<Basis> new_bases(arg_ind.size());
        std::vector<AnyTensor> T_(arg_ind.size());
        for (int i = 0; i < arg_ind.size(); i++) {
            new_bases[i] = basis(arg_ind[i]).antiderivative(orders[i], T_[i]);
        }
        T = T_;
        return substitute_bases(Argument::from_vector(arg_ind), new_bases);
    }

    std::vector<AnyTensor> TensorBasisNode::integral(const TensorDomain& dom) const {
        spline_assert(dom.n_domains() == n_basis());
        std::vector<AnyTensor> ret(n_basis());
        for (int i=0; i<n_basis(); i++) {
            if (dom.hasArguments()) {
                ret[i] = basis(i).integral(dom.domain(argument(i)));
            } else {
                ret[i] = basis(i).integral(dom.domain(i));
            }
        }
        return ret;
    }

    TensorBasis TensorBasisNode::partial_integral(const TensorDomain& dom,
        const NumericIndexVector& arg_ind, std::vector<AnyTensor>& T) const {
        spline_assert(dom.n_domains() == arg_ind.size());
        std::vector<Basis> bases(0);
        std::vector<std::string> args(0);
        std::vector<AnyTensor> T_(arg_ind.size());
        for (int i=0; i<arg_ind.size(); i++) {
            T_[i] = basis(arg_ind[i]).integral(dom.domain(i));
        }
        int i;
        for (int j=0; j<n_basis(); j++) {
            for (i=0; i<arg_ind.size(); i++) {
                if (arg_ind[i] == j) {
                    break;
                }
            }
            if (i == arg_ind.size()) {
                bases.push_back(basis(j));
                if (hasArguments()) {
                    args.push_back(argument(j));
                }
            }
        }
        T = T_;
        return TensorBasis(bases, args);
    }

    TensorBasis TensorBasisNode::partial_integral(const TensorDomain& dom,
        const std::vector<std::string>& args, std::vector<AnyTensor>& T) const {
        spline_assert(dom.n_domains() == args.size());
        NumericIndexVector arg_ind(args.size());
        for (int i=0; i<args.size(); i++) {
            arg_ind[i] = indexArgument(args[i]);
        }
        if (dom.hasArguments()) { // order domain
            std::vector<Domain> doms(args.size());
            for (int i=0; i<args.size(); i++) {
                doms[i] = dom.domain(args[i]);
            }
            TensorDomain dom2 = TensorDomain(doms, args);
            return partial_integral(dom2, arg_ind, T);
        }
        return partial_integral(dom, arg_ind, T);
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

    spline::Function TensorBasisNode::basis_functions() const {
        // Construct coefficient based on outer product of eye's
        AnyTensor C = DT(casadi::DM::densify(casadi::DM::eye(dimension()[0])));
        std::vector< int > dims_even = std::vector< int >{0};
        std::vector< int > dims_odd = std::vector< int >{1};
        for(int i = 1; i < n_basis(); i++) {
            C = C.outer_product(DT(casadi::DM::densify(casadi::DM::eye(dimension()[i]))));
            dims_even.push_back(2*i);
            dims_odd.push_back(2*i+1);
        }

        // Scramble dimensions of coefficient
        dims_even.insert(dims_even.end(), dims_odd.begin(), dims_odd.end());
        C = C.reorder_dims(dims_even);

        // reshape tensor-valued function to vector-valued
        std::vector< int > shape = C.dims();
        shape.erase(shape.begin() + shape.size()/2, shape.end());
        shape.push_back(product(shape));
        C = C.shape(shape);

        spline::Function basis_functions_ = spline::Function(shared_from_this<TensorBasis>(), Coefficient(C));
        return basis_functions_;
    }

    AnyTensor TensorBasis::project_to(const TensorBasis& b) const {
        return (*this)->project_to(b);
    }

    AnyTensor TensorBasisNode::project_to(const TensorBasis& b) const {
        Function b1 = basis_functions();
        Function b2 = b.basis_functions();

        Function b21  = b2.mtimes(b1.transpose());
        Function b22  = b2.mtimes(b2.transpose());

        AnyTensor B21 = b21.integral();
        AnyTensor B22 = b22.integral();

        AnyTensor T = B22.solve(B21);

        std::vector< int > M = b.dimension();
        std::vector< int > N = dimension();
        std::vector< int > shapeT = M;
        shapeT.insert(shapeT.end(), N.begin(), N.end());

        return T.shape(shapeT);
    }

    TensorBasis TensorBasis::transform_to(const TensorBasis& b, std::vector<AnyTensor>& T) const {
        return (*this)->transform_to(b, T);
    }

    TensorBasis TensorBasisNode::transform_to(const TensorBasis& tb, std::vector<AnyTensor>& T) const {
        spline_assert(n_basis() == tb.n_basis());
        std::vector<Basis> new_bases(n_basis());
        std::vector<AnyTensor> T_(n_basis());
        for (int i = 0; i <n_basis(); i++) {
            new_bases[i] = basis(i).transform_to(tb.basis(i), T_[i]);
        }
        T = T_;
        if (hasArguments()) {
            return TensorBasis(new_bases, arguments());
        } else {
            return TensorBasis(new_bases);
        }
    }


    std::vector< int > TensorBasis::get_permutation(const TensorBasis& grid) const{
        return (*this)->get_permutation(grid);
    }

    std::vector< int > TensorBasisNode::get_permutation(const TensorBasis& grid_basis) const{
        std::vector< int > index;
        if(grid_basis.hasArguments() && hasArguments()){
            for(auto & a : grid_basis.arguments()){
                index.push_back(indexArgument(a));
            }
        }else{
            spline_assert(grid_basis.n_basis() == n_basis());
            for(int i = 0; i < grid_basis.n_basis(); i++){
                index.push_back(i);
            }
        }
        return index;
    }

    std::vector< int > TensorBasisNode::input_border() const {
        std::vector< int > s = { 0 };
        for(int i = 0; i < n_basis() ; i++){
            s.push_back(s[i] + basis(i).n_inputs());
        }
    return  s;
    }

    std::vector<int> TensorBasis::vectorize(const Argument& arg) const{
        if(arg.is_all()){
            return casadi::range(n_basis());
        } else {
            return {arg.concrete(arguments())};
        }
    }

} // namespace spline
