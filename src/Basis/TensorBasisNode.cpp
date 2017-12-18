#include "TensorBasis.h"
#include "TensorBasisNode.h"
#include "EmptyBasis.h"
#include "BSplineBasis.h"
#include "../common.h"
#include "operations/operationsBasis.h"
#include "../Function/Function.h"

namespace spline {

    TensorBasisNode::TensorBasisNode(const std::vector< Basis >& bases)
        : bases_(bases), allArguments(std::vector<std::string>{})
        {}

    TensorBasisNode::TensorBasisNode(const std::vector< Basis >& bases,
            const std::vector< std::string >& args) :
        bases_(bases), allArguments(args)
    {}

    int TensorBasisNode::n_basis() const {
        return bases_.size();
    }

    std::vector< std::string > TensorBasisNode::arguments() const {
        return allArguments;
    }

    std::vector<Argument> TensorBasisNode::arguments_temp() const {
        std::vector< Argument > arg_temp;
        for(auto& a : arguments()) arg_temp.push_back(Argument(a));
        return arg_temp;
    }

    std::string TensorBasisNode::argument(int index) const {
        return allArguments[index];
    }

    int TensorBasisNode::index_argument(const Argument& a) const {
        return Argument::concrete(a, shared_from_this<TensorBasis>());
    }

    int TensorBasisNode::indexArgument(const std::string& a) const {
        return Argument::concrete(a, shared_from_this<TensorBasis>());
    }

    bool TensorBasisNode::valid_argument_list(const std::vector<Argument>& args) const {
        if(n_basis() == 0) return true;
        std::vector< int > number_occurence = n_inputs_list();
        std::vector< int > index = Argument::concrete(args, shared_from_this<TensorBasis>());
        for ( auto &i : index){
            if (i >= 0 && i < n_basis()) number_occurence[i]--;
        }
        return std::all_of(number_occurence.begin(), number_occurence.end(), [](int i){ return i == 0; });
    }

    bool TensorBasisNode::valid_argument(const Argument& a) const{
        int index = index_argument(a);
        if (index < 0) return false;
        if (index >= n_basis()) return false;
        return true;
    }

    bool TensorBasisNode::hasArguments() const {
        return ( allArguments.size() > 0 );
    }

    std::vector< Basis > TensorBasisNode::bases() const {
        return bases_;
    }

    Basis TensorBasisNode::basis() const {
        spline_assert(n_basis() == 1);
        return bases()[0];
    }

    Basis TensorBasisNode::basis(const std::string& a) const {
        int index = indexArgument(a);
        return basis(index);
    }

    Basis TensorBasisNode::basis(const Argument& index) const {
        int ind = index.concrete(arguments());
        if (ind < 0 || ind >= n_basis()) {
            return EmptyBasis();
        }
        return bases()[ind];
    }

    TensorBasis TensorBasisNode::add_basis(TensorBasis tensor_basis) const {
        std::vector< Basis > new_bases = bases();
        for ( auto &subBasis : tensor_basis.bases() ) {
            new_bases.push_back(subBasis);
        }
        return TensorBasis(new_bases, arguments());
    }

    TensorBasis TensorBasisNode::add_basis(Basis basis) const {
        std::vector< Basis > new_bases = bases();
        new_bases.push_back(basis);
        return TensorBasis(new_bases, arguments());
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

    std::string TensorBasisNode::type() const { return "TensorBasis";}

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
            return "TensorBasis containing " + std::to_string(bases_.size()) +
                " " + n_basis + " in " + std::to_string(allArguments.size()) +
                " arguments: \n " + str_basis;
        }
    }

    TensorBasis TensorBasisNode::operator+ (const TensorBasis& other) const {
        return plus_tensor_basis(shared_from_this<TensorBasis>(), other);
    }

    TensorBasis TensorBasisNode::operator* (const TensorBasis& other) const {
        return times_tensor_basis(shared_from_this<TensorBasis>(), other);
    }

    AnyTensor TensorBasisNode::operator() (const std::vector< AnyScalar > &  x,
            const std::vector< int >& arg_ind, bool reorder_output) const {

        AnyTensor ret = AnyTensor::unity();

        if(n_basis() == 0) return ret;
        std::vector< int > input_border_ = input_border();
        std::vector< AnyScalar > x_ = x;
        std::vector< int > arg_ind_ = arg_ind;

        for(int i = x.size() - 1; i >= 0; i--){
            if(arg_ind_[i] < 0){
                x_.erase(x_.begin() + i);
                arg_ind_.erase(arg_ind_.begin() + i);
            }
        }

        for (int i = 0; i < n_basis(); i++) {
            Basis b = basis(i);
            int index_arg = arg_ind_[i];
            std::vector< AnyScalar > input = {};

            for (int j = input_border_[index_arg]; j < input_border_[index_arg + 1]; ++j) {
                input.push_back(x_[j]);
            }
            ret = ret.outer_product(b(AnyVector(input)));
        }
        if(reorder_output) return ret.reorder_dims(arg_ind_);
        return ret;
    }

    std::vector< AnyTensor >  TensorBasisNode::evaluation_grid () const {
        std::vector< AnyTensor > ret;
        for(auto& b : bases()){
            ret.push_back(b.evaluation_grid());
        }
        return ret;
    }

    AnyTensor TensorBasisNode::grid_eval (const std::vector< AnyTensor > &  x,
            const std::vector< int >& arg_ind_, bool reorder_output) const {
        if(n_basis() == 0){// constant
            std::vector< int > dims_grid(x.size());
            for(int i = 0; i < x.size(); i++) dims_grid[i] = x[i].dims()[0];
            return AnyTensor::ones(dims_grid);
        }
        int length_argument = arg_ind_.size();
        std::vector< int > arg_ind;
        if(length_argument == 0){// no argument list is given
            for(int i = 0; i < x.size(); i++) arg_ind.push_back(i);
            length_argument = x.size();
        } else {
            arg_ind = arg_ind_;
        }
        spline_assert(x.size() == length_argument);
        AnyTensor ret = AnyTensor::unity();

        std::vector< int > reorder(2*length_argument ,0);

        for (int i = 0; i < length_argument; i++) {
            ret = ret.outer_product(basis(arg_ind[i]).list_eval(x[i]));
            reorder[i] = 2*i;
            reorder[i + length_argument] = 2*i + 1;
        }

        if(reorder_output){
            ret = ret.reorder_dims(reorder);
            std::vector< int > dims = ret.dims();
            for (int i = 0; i < length_argument; i++) {
                if(arg_ind[i] < 0){
                    dims.erase(dims.begin() + length_argument + i );
                }
            }
            return ret.shape(dims);
        }

        int dim_to_pop = 0;
        for (int i = 0; i < length_argument; i++) {
            if(arg_ind[i] < 0){
                dim_to_pop++;
                reorder[2*length_argument - dim_to_pop] = 2*i + 1;
            } else {
                reorder[length_argument+arg_ind[i]] = 2*i + 1;
            }
        }
        ret = ret.reorder_dims(reorder);
        std::vector< int > dims = ret.dims();
        dims.erase(dims.end() - dim_to_pop, dims.end());
        return ret.shape(dims);
    }

    bool TensorBasisNode::operator==(const TensorBasis& rhs) const {
        if( this->n_basis() != rhs.n_basis()) return false;
        for( int i = 0; i < n_basis(); i++){
            if( !( this->bases()[i] == rhs.bases()[i])) return false;
        }
        return true;
    }

    int TensorBasisNode::totalNumberBasisFunctions() const {
        return spline::product(dimension());
    }

    int TensorBasisNode::n_inputs() const {
        int r = 0;
        for (auto const& b : bases()) {
            r += b.n_inputs();
        }
        return r;
    }

    std::vector< int > TensorBasisNode::n_inputs_list() const {
        std::vector< int > ret {n_basis()};
        for (int i =0 ; i < n_basis(); i++) {
            ret[i] = basis(i).n_inputs();
        }
        return ret;
    }

    AnyTensor TensorBasisNode::const_coeff_tensor(const AnyTensor& t) const {
        AnyTensor prod = AnyTensor::unity();

        for (int i = 0; i < n_basis(); i++) {
            AnyTensor temp = bases_[i].const_coeff_tensor(AnyTensor(AnyScalar(1)));
            prod = prod.outer_product(temp.shape(std::vector< int >(1, temp.numel())));
        }

        return prod.outer_product(t);
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

    TensorBasis TensorBasisNode::midpoint_refinement(const std::vector<int>& refinement,
            const std::vector<int>& arg_ind, std::vector<AnyTensor> & T) const {
        spline_assert(arg_ind.size() == refinement.size());
        std::vector<Basis> new_bases(arg_ind.size());
        std::vector<AnyTensor> T_(arg_ind.size());
        for (int i = 0; i < arg_ind.size(); i++) {
            new_bases[i] = basis(arg_ind[i]).midpoint_refinement(refinement[i], T_[i]);
        }
        T = T_;
        return substitute_bases(Argument::from_vector(arg_ind), new_bases);
    }

    TensorBasis TensorBasisNode::degree_elevation(const std::vector<int>& elevation,
            const std::vector<int>& arg_ind, std::vector<AnyTensor> & T) const {
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

    TensorBasis TensorBasisNode::transform_to(const TensorBasis& tb, std::vector<AnyTensor>& T) const {
        spline_assert(n_basis() == tb.n_basis());
        std::vector<Basis> new_bases(n_basis());
        std::vector<AnyTensor> T_(n_basis());
        std::vector< int > args_other = Argument::concrete(Argument::from_vector(tb.arguments()), shared_from_this<TensorBasis>());
        for (int i = 0; i <n_basis(); i++) {
            new_bases[i] = basis(i).transform_to(tb.basis(args_other[i]), T_[args_other[i]]);
        }
        T = T_;
        if (hasArguments()) {
            return TensorBasis(new_bases, arguments());
        } else {
            return TensorBasis(new_bases);
        }
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

} // namespace spline
