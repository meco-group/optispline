#ifndef SRC_FUNCTION_FUNCTION_H_
#define SRC_FUNCTION_FUNCTION_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include <any_tensor.hpp>
#include "GenericFunction.h"
#include "Argument.h"

namespace spline {
class Constant;

class Function : public GenericFunction {

    public :
        Function(const TensorBasis& basis, const Coefficient& coef);
        Function(const Basis& basis, const Coefficient& coef);
        Function(const AnyTensor& c);
        Function() {}

        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const override;

        virtual Function operator+(const Function& f) const override;
        virtual Function operator+(const Constant& f) const override;
        virtual Function operator*(const Function& f) const override;
        virtual Function operator*(const Constant& f) const override;
        virtual Function operator-() const override;
        virtual Function operator-(const AnyTensor& t) const override;

        virtual Function mtimes(const GenericFunction& f) const override;
        virtual Function mtimes(const Function& f) const override;
        virtual Function mtimes(const Constant& f) const override;
        virtual Function mtimes(const AnyTensor& f) const override;
        virtual Function rmtimes(const AnyTensor& f) const override;

        virtual Function transpose() const override;
        virtual Function trace() const override;

        virtual std::string type() const override;
        virtual std::string to_string() const override;

        virtual Function transform_to(const Basis& basis) const override;
        virtual Function transform_to(const TensorBasis& basis) const override;
        virtual Function project_to(const Basis& basis) const override;
        virtual Function project_to(const TensorBasis& basis) const override;

        virtual Function slice(const AnySlice& i, const AnySlice& j) const override;
        virtual Function slice(const AnySlice& i) const override;
        Function reshape(const std::vector< int >& shape) const;

        Basis basis() const;
        Basis basis(const Argument& i) const;
        virtual TensorBasis tensor_basis() const override {return basis_;}
        TensorDomain domain() const {return basis_.domain();}

        void repr() const { casadi::userOut() << to_string() << std::endl;}
        // std::string& getArgument (){ return basis().getArgument();}

        int n_inputs() const;  // Number of inputs of the function

        Function insert_knots(const AnyVector & new_knots) const;
        Function insert_knots(const AnyVector & new_knots, const NumericIndex & arg_ind) const;
        Function insert_knots(const AnyVector & new_knots, const std::string & arg) const;
        Function insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<std::string> & arg) const;
        Function insert_knots(const std::vector<AnyVector> & new_knots,
            const NumericIndexVector & arg_ind) const;

        Function midpoint_refinement(int refinement) const;
        Function midpoint_refinement(int refinement, const NumericIndex & arg_ind) const;
        Function midpoint_refinement(int refinement, const std::string & arg) const;
        Function midpoint_refinement(const std::vector<int> & refinement,
            const std::vector<std::string> & arg) const;
        Function midpoint_refinement(const std::vector<int> & refinement,
            const NumericIndexVector & arg_ind) const;

        Function degree_elevation(int elevation) const;
        Function degree_elevation(int elevation, const NumericIndex& arg_ind) const;
        Function degree_elevation(int elevation, const std::string& arg) const;
        Function degree_elevation(const std::vector<int> & elevation,
            const std::vector<std::string> & args) const;
        Function degree_elevation(const std::vector<int> & elevation,
            const NumericIndexVector & arg_ind) const;

        Function kick_boundary(const TensorDomain& boundary) const;
        Function kick_boundary(const TensorDomain& boundary, const NumericIndex& arg_ind) const;
        Function kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string> & args) const;
        Function kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector & arg_ind) const;

        Function derivative() const;
        Function derivative(int order) const;
        Function derivative(int order, const std::string& arg) const;
        Function derivative(int order, const NumericIndex& arg_ind) const;
        Function derivative(const std::vector<int>& orders,
            const std::vector<std::string>& args) const;
        Function derivative(const std::vector<int>& orders,
            const NumericIndexVector& arg_ind) const;

        Function antiderivative() const;
        Function antiderivative(int order) const;
        Function antiderivative(int order, const std::string& arg) const;
        Function antiderivative(int order, const NumericIndex& arg_ind) const;
        Function antiderivative(const std::vector<int>& orders,
            const std::vector<std::string>& args) const;
        Function antiderivative(const std::vector<int>& orders,
            const NumericIndexVector& arg_ind) const;

        std::vector<spline::Function> jacobian() const;

        AnyTensor integral() const;
        AnyTensor integral(const TensorDomain& domain) const;
        Function partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args) const;
        Function partial_integral(const TensorDomain& domain,
         const NumericIndexVector& arg_ind) const;
    public:
        TensorBasis basis_;

    private:
        static void homogenize_args(Function& f, AnyTensor& t);

        void init(const TensorBasis& basis, const Coefficient& coef);
        typedef std::function<TensorBasis(const TensorBasis&, const TensorBasis&)> BasisComposition;
        typedef std::function<AnyTensor(const AnyTensor&, const AnyTensor&)> TensorComposition;
        Function generic_operation(const Function& f,
                const BasisComposition & bc, const TensorComposition & tc) const;
};


} // namespace spline

#endif // SRC_FUNCTION_FUNCTION_H_
