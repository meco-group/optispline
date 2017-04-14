#include <vector>
#include "Function.h"
#include "FunctionNode.h"
#include "../common.h"

namespace spline {
    FunctionNode::FunctionNode(const Coefficient& coeff) : FunNode(coeff), basis_(TensorBasis()) {}

    FunctionNode::FunctionNode(const TensorBasis& basis, const Coefficient& coeff) : FunNode(coeff), basis_(basis) {}

    AnyTensor FunctionNode::operator()(const AnyTensor& arg, const std::vector< int >& args) const{
        AnyTensor x = arg.squeeze();
        spline_assert_message(x.is_vector(), "Function can not be evaluated on something with dimentions " << x.dims());
        x = x.as_vector();

        std::vector< AnyScalar > x_ = x.unpack_1();
        return tensor_basis()(x_, Argument::from_vector(args), false).inner(coeff().data());
    }

    AnyTensor FunctionNode::list_eval(const AnyTensor& arg, const std::vector< int >& args) const{
        AnyTensor x = arg;
        spline_assert(x.n_dims()<=2);

        if(n_inputs() == 1){
           x = x.as_vector().shape({x.numel(), 1});
        }

        spline_assert(x.n_dims()==2);
        std::vector< std::vector< AnyScalar > > X_ = x.unpack_2();

        std::vector< AnyTensor > tensor = {};
        for (int i = 0; i < X_.size(); i++) {
            tensor.push_back(tensor_basis()(X_[i], Argument::from_vector(args), false));
        }
        AnyTensor packed_tensor = AnyTensor::pack(tensor, 0);
        int shared_dim = packed_tensor.n_dims();
        std::vector<int> a_r = mrange(0, shared_dim);
        std::vector<int> b_r = mrange(1, shared_dim + 2);
        std::vector<int> c_r = { a_r[0] };
        c_r.push_back(b_r[b_r.size() - 2]);
        c_r.push_back(b_r[b_r.size() - 1]);
        packed_tensor = packed_tensor.einstein(coeff_tensor(), a_r, b_r, c_r).squeeze_tailing();
        if(X_.size() == 1){
            std::vector< int > squeeze_dims;
            for(int i = 1; i < packed_tensor.dims().size(); i++) squeeze_dims.push_back(packed_tensor.dims()[i]);
            packed_tensor = packed_tensor.shape(squeeze_dims);
        }
        return packed_tensor;
    }

    Function FunctionNode::partial_eval(const AnyTensor& x, int index) const{
        int n_basis = tensor_basis().n_basis();

        std::vector< Basis > partial_basis = tensor_basis().bases();
        partial_basis.erase(partial_basis.begin() + index);
        std::vector< std::string > partial_arguments = tensor_basis().arguments();
        partial_arguments.erase(partial_arguments.begin() + index);

        std::vector< int > a = mrange(n_basis + 2);
        std::vector< int > b = std::vector< int > {-1-index};
        std::vector< int > c = mrange(n_basis + 2);
        c.erase(c.begin() + index);
        AnyTensor partial_coeff = coeff_tensor().einstein(basis(index)(x),a,b,c);

        return Function(TensorBasis( partial_basis, partial_arguments ), partial_coeff);
    }

    AnyTensor FunctionNode::grid_eval(const std::vector< AnyTensor >& x, const std::vector< int >& args, bool squeeze_return)  const {

        AnyTensor tensor = tensor_basis().grid_eval(x, Argument::from_vector(args), false);
        int n_dims_tensor = tensor.dims().size();
        int n_dims_grid = x.size();
        std::vector< int > a = mrange(n_dims_tensor);
        std::vector< int > b = mrange(n_dims_grid, n_dims_tensor + 2);
        std::vector< int > c = mrange(n_dims_grid);
        c.insert(c.end(), b.end() - 2, b.end());
        tensor = tensor.einstein( coeff_tensor(), a, b, c);

        if (squeeze_return)  return tensor.squeeze();
        return tensor;
    }

    std::string FunctionNode::type() const{
        return "Function";
    }

    std::string FunctionNode::to_string() const{
        return "Function, consisting of a " + basis_.to_string() + "and:\n\t" + coeff_.to_string();
    }

    Function FunctionNode::generic_operation(const Function& f,
            const BasisComposition & bc, const TensorComposition & tc) const  {

        TensorBasis sumBasis = bc(tensor_basis(), f.tensor_basis());
        std::vector< Argument > args = Argument::from_vector(sumBasis.arguments());
        std::vector< AnyTensor > eval_g = sumBasis.evaluation_grid();

        AnyTensor basisEvaluated =  sumBasis.grid_eval(eval_g, args, false);
        AnyTensor thisFunctionEvaluated = shared_from_this<Function>().grid_eval(eval_g, args, false);
        AnyTensor otherFunctionEvaluated = f.grid_eval(eval_g, args, false);

        int n_dims_grid = eval_g.size();
        AnyTensor A = basisEvaluated;

        thisFunctionEvaluated = thisFunctionEvaluated.flatten_first(n_dims_grid);
        otherFunctionEvaluated = otherFunctionEvaluated.flatten_first(n_dims_grid);
        AnyTensor B = tc(thisFunctionEvaluated, otherFunctionEvaluated);

        std::vector< int > elemShape = B.dims();
        elemShape = std::vector<int>(elemShape.begin()+1, elemShape.end());

        A = A.flatten_first(n_dims_grid);
        A = A.flatten_last(n_dims_grid);
        B = B.flatten_last(2);
        AnyTensor C = A.solve(B);

        std::vector< int > shapeCoef = elemShape;
        std::vector< int > shapeBasis = sumBasis.dimension();
        shapeBasis.insert(shapeBasis.end(), shapeCoef.begin(), shapeCoef.end());

        C = C.shape(shapeBasis);
        return Function(sumBasis, C);
    }

    Function FunctionNode::operator+(const FunNode& f) const {
        return f + *this;
    }

    Function FunctionNode::operator+(const FunctionNode& f) const {
        return generic_operation(f.shared_from_this<Function>(),
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs + rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) {
                int n = lhs.n_dims();
                std::vector<int> rep(n, 1);
                if (lhs.dims()[n-2]==1 && lhs.dims()[n-1]==1) {
                    rep[n-2] = rhs.dims()[n-2];
                    rep[n-1] = rhs.dims()[n-1];
                    return AnyTensor::repeat(lhs, rep) + rhs;
                }
                if (rhs.dims()[n-2]==1 && rhs.dims()[n-1]==1) {
                    rep[n-2] = lhs.dims()[n-2];
                    rep[n-1] = lhs.dims()[n-1];
                    return lhs + AnyTensor::repeat(rhs, rep);
                }
                return lhs + rhs;
            });
    }


    Function FunctionNode::operator*(const FunNode& f) const {
        return f * *this;
    }

    Function FunctionNode::operator*(const FunctionNode& f) const {
        return generic_operation(f.shared_from_this<Function>(),
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) {

                int n = lhs.n_dims();
                std::vector<int> rep(n, 1);
                if (lhs.dims()[n-2]==1 && lhs.dims()[n-1]==1) {
                    rep[n-2] = rhs.dims()[n-2];
                    rep[n-1] = rhs.dims()[n-1];
                    return AnyTensor::repeat(lhs, rep) * rhs;
                }
                if (rhs.dims()[n-2]==1 && rhs.dims()[n-1]==1) {
                    rep[n-2] = lhs.dims()[n-2];
                    rep[n-1] = lhs.dims()[n-1];
                    return lhs * AnyTensor::repeat(rhs, rep);
                }
                return lhs * rhs;
            });
    }


    Function FunctionNode::mtimes(const FunNode& f) const {
        return f.rmtimes(*this);
    }

    Function FunctionNode::mtimes(const FunctionNode& f) const {
        return generic_operation(f.shared_from_this<Function>(),
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return lhs.einstein(rhs, {-4, -1, -2}, {-4, -2, -3}, {-4, -1, -3});});
    }


    Function FunctionNode::rmtimes(const FunctionNode& f) const {
        return generic_operation(f.shared_from_this<Function>(),
                [](const TensorBasis& lhs, const TensorBasis& rhs) { return lhs * rhs; },
                [](const AnyTensor& lhs, const AnyTensor& rhs) { return rhs.einstein(lhs, {-4, -1, -2}, {-4, -2, -3}, {-4, -1, -3});});
    }


    Function FunctionNode::operator-() const {
        return Function(tensor_basis(), -coeff_);
    }

    Function FunctionNode::transpose() const {
        return Function(tensor_basis(), coeff().transpose());
    }

    Function FunctionNode::trace() const {
        return Function(tensor_basis(), coeff().trace());
    }

    Function FunctionNode::transform_to(const TensorBasis& basis) const {
        TensorBasis sumBasis = tensor_basis() + basis;
        std::vector< Argument > args = Argument::from_vector(sumBasis.arguments());
        std::vector< AnyTensor > eval_g = sumBasis.evaluation_grid();
        int n_dims_grid = eval_g.size();

        AnyTensor A =  sumBasis.grid_eval(eval_g, args, false);
        AnyTensor B = shared_from_this<Function>().grid_eval(eval_g, args, false);

        A = A.flatten_first(n_dims_grid);
        B = B.flatten_first(n_dims_grid);

        std::vector< int > elemShape = B.dims();
        elemShape = std::vector<int>(elemShape.begin()+1, elemShape.end());

        A = A.flatten_last(n_dims_grid);
        B = B.flatten_last(2);
        AnyTensor C = A.solve(B);

        std::vector< int > shapeCoef = elemShape;
        std::vector< int > shapeBasis = sumBasis.dimension();
        shapeBasis.insert(shapeBasis.end(), shapeCoef.begin(), shapeCoef.end());

        C = C.shape(shapeBasis);
        return Function(sumBasis, C);
    }

    Function FunctionNode::project_to(const TensorBasis& b) const {
        Function b2 = b.basis_functions();
        Function f = reshape(std::vector< int >{1,spline::product(shape())});

        Function b22 = b2.mtimes(b2.transpose());
        Function b2f = b2.mtimes(f); //f already is a row vector

        AnyTensor B22 = b22.integral();
        AnyTensor B2f = b2f.integral();

        AnyTensor C = B22.solve(B2f);

        std::vector< int > M = b.dimension();
        std::vector< int > N = shape();
        std::vector< int > shapeC = M;
        shapeC.insert(shapeC.end(), N.begin(), N.end());
        C = C.shape(shapeC);

        return Function(b,C);
    }

    Function FunctionNode::reshape(const std::vector< int >& shape) const {
        return Function(tensor_basis(), coeff().reshape(shape));
    }

    Function FunctionNode::slice(const AnySlice& i, const AnySlice& j) const {
        return Function(tensor_basis(), coeff_tensor().get_slice(i, j));
    }

    int FunctionNode::n_inputs() const {
        return tensor_basis().n_inputs();
    }

    Function FunctionNode::slice(const AnySlice& i) const {
        return Function(tensor_basis(), coeff_tensor().get_slice(i));
    }

    TensorBasis FunctionNode::tensor_basis() const {
        return basis_;
    }

    Basis FunctionNode::basis() const {
        spline_assert_message(tensor_basis().n_basis()==1,
                ".basis() syntax only works for a 1-D TensorBasis.");
        return tensor_basis().bases()[0];
    }

    Basis FunctionNode::basis(const Argument& index) const {
        return tensor_basis().basis(index);
    }

    Function FunctionNode::insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<int> & arg_ind) const {
        spline_assert(arg_ind.size() == new_knots.size())
            std::vector<AnyTensor> T;
        TensorBasis tbasis = tensor_basis();
        TensorBasis new_tbasis = tbasis.insert_knots(new_knots, Argument::from_vector(arg_ind), T);
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        return Function(new_tbasis, new_coefficient);
    }

    Function FunctionNode::midpoint_refinement(const std::vector<int>& refinement,
            const std::vector< int >& arg_ind) const {
        spline_assert(arg_ind.size() == refinement.size())
            std::vector<AnyTensor> T;
        TensorBasis tbasis = tensor_basis();
        TensorBasis new_tbasis = tbasis.midpoint_refinement(refinement,
                Argument::from_vector(arg_ind), T);
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        return Function(new_tbasis, new_coefficient);
    }

    Function FunctionNode::degree_elevation(const std::vector<int>& elevation,
            const std::vector< int >& arg_ind) const {
        spline_assert(arg_ind.size() == elevation.size());
        std::vector<AnyTensor> T;
        TensorBasis tbasis = tensor_basis();
        TensorBasis new_tbasis = tbasis.degree_elevation(elevation,
                Argument::from_vector(arg_ind), T);
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        return Function(new_tbasis, new_coefficient);
    }

    Function FunctionNode::kick_boundary(const TensorDomain& boundary) const {
        // apply on all directions
        NumericIndexVector arg_ind(tensor_basis().n_basis());
        for (int k=0; k<arg_ind.size(); k++) {
            arg_ind[k] = k;
        }
        return kick_boundary(boundary, arg_ind);
    }

    Function FunctionNode::kick_boundary(const TensorDomain& boundary, const NumericIndex& arg_ind) const {
        return kick_boundary(boundary, NumericIndexVector{arg_ind});
    }

    Function FunctionNode::kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string>& args) const {
        std::vector<AnyTensor> T;
        TensorBasis new_tbasis = tensor_basis().kick_boundary(boundary, args, T);
        NumericIndexVector arg_ind(args.size());
        for (int i=0; i<args.size(); i++) {
            arg_ind[i] = tensor_basis().indexArgument(args[i]);
        }
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        return Function(new_tbasis, new_coefficient);
    }

    Function FunctionNode::kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector& arg_ind) const {
        std::vector<AnyTensor> T;
        TensorBasis new_tbasis = tensor_basis().kick_boundary(boundary, arg_ind, T);
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        return Function(new_tbasis, new_coefficient);
    }

    Function FunctionNode::derivative(const std::vector<int>& orders,
            const std::vector< int >& arg_ind) const {
        spline_assert(orders.size() == arg_ind.size())  // each direction should have an order
            std::vector<AnyTensor> T;
        TensorBasis tbasis = tensor_basis();
        TensorBasis new_tbasis = tbasis.derivative(orders, Argument::from_vector(arg_ind), T);
        NumericIndexVector args(arg_ind.size());
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        return Function(new_tbasis, new_coefficient);
    }


    Function FunctionNode::antiderivative(const std::vector<int>& orders,
            const std::vector< int >& arg_ind) const {
        spline_assert(orders.size() == arg_ind.size())  // each direction should have an order
            std::vector<AnyTensor> T;
        TensorBasis tbasis = tensor_basis();
        TensorBasis new_tbasis = tbasis.antiderivative(orders, Argument::from_vector(arg_ind), T);
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        return Function(new_tbasis, new_coefficient);
    }

    std::vector<Function> FunctionNode::jacobian() const {
        std::vector<Function> Jacobian(n_inputs());
        for (int i=0; i<n_inputs(); i++) {
            Jacobian[i] = derivative({ 1 }, {i});
        }
        return Jacobian;
    }

    AnyTensor FunctionNode::integral() const {
        return integral(tensor_basis().domain());
    }

    AnyTensor FunctionNode::integral(const TensorDomain& domain) const {
        std::vector<AnyTensor> T = tensor_basis().integral(domain);
        NumericIndexVector arg_ind(tensor_basis().n_basis());
        for (int i=0; i<tensor_basis().n_basis(); i++) {
            arg_ind[i] = i;
        }
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        return new_coefficient.data().shape(shape());
    }

    Function FunctionNode::partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args) const {
        std::vector<AnyTensor> T;
        TensorBasis new_tbasis = tensor_basis().partial_integral(domain, args, T);
        NumericIndexVector arg_ind(args.size());
        for (int i=0; i<args.size(); i++) {
            arg_ind[i] = tensor_basis().indexArgument(args[i]);
        }
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        new_coefficient = new_coefficient.rm_direction(arg_ind);
        return Function(new_tbasis, new_coefficient);
    }

    Function FunctionNode::partial_integral(const TensorDomain& domain,
            const NumericIndexVector& arg_ind) const {
        std::vector<AnyTensor> T;
        TensorBasis new_tbasis = tensor_basis().partial_integral(domain, arg_ind, T);
        Coefficient new_coefficient = coeff().transform(T, arg_ind);
        new_coefficient = new_coefficient.rm_direction(arg_ind);
        return Function(new_tbasis, new_coefficient);
    }

}  // namespace spline
