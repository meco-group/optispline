#ifndef SRC_FUNCTION_FUNCTION_H_
#define SRC_FUNCTION_FUNCTION_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObject.h"

#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include "Argument.h"
#include "FunctionNode.h"

namespace spline {

    class Function : public SharedObject{
    public:
        Function();
        /**
         *
         * Some information about ordering:
         * 
         *  
         * Suppose basis is a univariate BSpline basis, and we want to create a Function that
         * represents a parametrised  2 x 2 matrix.
         * We would need to supply a d x 2 x 2 tensor as coefficient,
         * where d is basis.dimension
         * 
         * For example choose a simple degree-1 basis:
         * b = BSplineBasis([0 1],1,2);
         * 
         * Here, we have d==2.
         * Let's make the coefficient tensor C very concrete by drawing a cube,
         * labeling the axes, and choosing numbers.
         * 
         *       5 ---- 7
         *      /|     /|
         *   k / |    / |
         *    /  6 --/- 8  
         *   1 -/-- 3  /
         * i | /    | /
         *   |/     |/
         *   2 ---- 4
         *       j
         * 
         * C(i,j,k) is a tensor notation with 3 labeled axes.
         * The drawn tensor can be constructed as:
         * C = reshape(1:8,[2,2,2]);
         * Correspondingly, the flattened out version of this tensor, C(:), is just 
         *  1     2     3     4     5     6     7     8
         * Matlab printout would be:
         * 
         * C(:,:,1) =
         *
         *   1     3
         *   2     4
         *
         * C(:,:,2) =
         *
         *   5     7
         *   6     8
         * 
         * Let's make a spline Function with it:
         * F = splines.Function(b,C);
         * 
         * Evaluating F for the parameter==0 corresponds to slicing the tensor at i==1,
         * Evaluating F for the parameter==1 corresponds to slicing the tensor at i==2.
         * 
         * 
         * F.eval(0)
         *   1     5
         *   3     7
         * 
         * Same as squeeze(C(1,:,:))
         * 
         * F.eval(1)
         *   2     6
         *   4     8
         * 
         * Same as squeeze(C(2,:,:))
         * 
         */
        Function(const TensorBasis& basis, const Coefficient& coeff);
        Function(const AnyTensor& tensor);
        Function(const AnyScalar& value, const std::vector< casadi_int >& shape);

#ifndef SWIG
        FunctionNode* get() const ;
        FunctionNode* operator->() const ;
#endif // SWIG

        virtual std::string type() const;

        casadi::MX operator<=(const casadi::MX& x) const;
        casadi::MX operator>=(const casadi::MX& x) const;
        casadi::MX operator==(const casadi::MX& x) const;
        casadi::MX operator<=(const Function& f) const { return ((*this)-f)<=0; }
        casadi::MX operator>=(const Function& f) const { return ((*this)-f)>=0; }
        casadi::MX operator==(const Function& f) const { return ((*this)-f)==0; }

        AnyTensor operator()(const AnyTensor& x, const std::vector< Argument >& args = std::vector< Argument > () ) const;
        AnyTensor list_eval(const AnyTensor& x, const std::vector< Argument >& args = std::vector< Argument > () ) const;
        AnyTensor grid_eval(const std::vector< AnyTensor >& x, const std::vector< Argument >& args = std::vector< Argument > (),  bool squeeze_return = false) const;

        Function partial_eval(const AnyTensor& x, const Argument& args) const;

        Function operator+(const Function& f) const;
        Function operator+(const AnyTensor& t) const;
        Function operator*(const Function& f) const;
        Function operator*(const AnyTensor& t) const;
        Function operator-(const Function& f) const;
        Function operator-(const AnyTensor& t) const;
        Function operator-() const;

        Function mtimes(const Function& f) const;
        Function mtimes(const AnyTensor& f) const;
        Function rmtimes(const AnyTensor& f) const;

        Function pow(casadi_int power) const;
        Function mpow(casadi_int power) const;

        Function slice(const AnySlice& i, const AnySlice& j) const;
        Function slice(const AnySlice& i) const;

        Function transpose() const;
        Function trace() const;

        Function sum(NumericIndex axis) const;
        Function sum() const;

        static Function vertcat(const std::vector< spline::Function >& f);
        static Function horzcat(const std::vector< spline::Function >& f);
        static Function blockcat(const std::vector< std::vector<spline::Function> >& f);
        static Function cat(NumericIndex index, const std::vector< spline::Function >& f);
        static Function blkdiag(const std::vector< spline::Function >& f);

        Coefficient coeff() const;
        AnyTensor coeff_tensor() const;
        AnyTensor data() const;

        Basis basis() const;
        Basis basis(const Argument& i) const;
        TensorBasis tensor_basis() const;
        TensorDomain domain() const;

        bool is_scalar() const;
        bool is_vector() const;
        bool is_column() const;

        std::vector< casadi_int > shape() const;  // Shape result obtained after function evaluation
        Function reshape(const std::vector< casadi_int >& shape) const;

        Function transform_to(const Basis& basis) const;
        Function transform_to(const TensorBasis& basis) const;
        Function project_to(const Basis& basis) const;
        Function project_to(const TensorBasis& basis) const;

        casadi_int n_inputs() const;  // Number of inputs of the function

        Function insert_knots(const AnyVector & new_knots, const Argument& arg = Argument()) const;
        Function insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<Argument> & arg) const;

        Function midpoint_refinement(casadi_int refinement = 1, const Argument& arg = Argument()) const;
        Function midpoint_refinement(const std::vector<casadi_int>& refinement,
            const std::vector< Argument >& arg_ind) const;

        Function degree_elevation(casadi_int order = 1, const Argument& arg = Argument()) const;
        Function degree_elevation(const std::vector<casadi_int>& orders,
            const std::vector< Argument >& arg_ind) const;

        Function kick_boundary(const TensorDomain& boundary) const;
        Function kick_boundary(const TensorDomain& boundary, const NumericIndex& arg_ind) const;
        Function kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string> & args) const;
        Function kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector & arg_ind) const;

        Function derivative(casadi_int order = 1, const Argument& arg = Argument()) const;
        Function derivative(const std::vector<casadi_int>& orders,
            const std::vector< Argument >& arg_ind) const;

        Function antiderivative(casadi_int order = 1, const Argument& arg = Argument()) const;
        Function antiderivative(const std::vector<casadi_int>& orders,
            const std::vector< Argument >& arg_ind) const;

        std::vector<spline::Function> jacobian() const;
        std::vector<std::vector<spline::Function> > hessian() const;
        spline::Function jacobian_matrix() const;
        spline::Function hessian_matrix() const;

        AnyTensor integral() const;
        AnyTensor integral(const TensorDomain& domain) const;
        Function partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args) const;
        Function partial_integral(const TensorDomain& domain,
         const NumericIndexVector& arg_ind) const;

        /** \brief Construct a piece-wise linear function from x/y data */
        static Function linear(const AnyVector & x, const AnyVector & y);

        casadi::Function to_casadi() const;

        AnyTensor fast_eval(const AnyTensor& xy) const;
        casadi::DM fast_jac(const AnyTensor& xy) const;

    private:
        template<class T>
        std::vector<T> vectorize(const Argument& arg, const T& e) const {
            std::vector< T > ret;
            if(arg.is_all()){
                ret = std::vector<T>(tensor_basis().n_basis(), e);
            } else {
                ret = { e };
            }
            return ret;
        }

        std::vector<casadi_int> vectorize(const Argument& arg) const;
    };

} // namespace spline

#endif //       SRC_FUNCTION_FUNCTION_H_
