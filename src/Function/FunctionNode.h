#ifndef SRC_FUNCTION_FUNCTIONNODE_H_
#define SRC_FUNCTION_FUNCTIONNODE_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include <any_tensor.hpp>

#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include "Argument.h"
#include "FunNode.h"

namespace spline {

#ifndef SWIG
    class FunctionNode : public FunNode{
    public :
        FunctionNode() {};
        FunctionNode(const TensorBasis& basis, const Coefficient& coeff);

        virtual std::string type() const override;
        virtual std::string to_string() const override;

        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const override;

        virtual Function partial_eval(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const override;

        virtual Function operator-() const override;

        virtual Function operator+(const FunNode& f) const override;
        virtual Function operator+(const FunctionNode& f) const override;
        virtual Function operator+(const ConstantNode& f) const override;

        virtual Function operator*(const FunNode& f) const override;
        virtual Function operator*(const FunctionNode& f) const override;
        virtual Function operator*(const ConstantNode& f) const override;

        virtual Function mtimes(const FunNode& f) const override;
        virtual Function mtimes(const FunctionNode& f) const override;
        virtual Function mtimes(const ConstantNode& f) const override;
        virtual Function rmtimes(const FunctionNode& f) const override;
        virtual Function rmtimes(const ConstantNode& f) const override;

        virtual Function reshape(const std::vector< int >& shape) const override;

        virtual Function slice(const AnySlice& i, const AnySlice& j) const override;
        virtual Function slice(const AnySlice& i) const override;

        virtual Function transpose() const override;
        virtual Function trace() const override;

        virtual Function transform_to(const TensorBasis& basis) const override;
        virtual Function project_to(const TensorBasis& basis) const override;

        virtual TensorBasis tensor_basis() const override;
        virtual Basis basis() const override;
        virtual Basis basis(const Argument& i) const override;

        virtual int n_inputs() const override;  // Number of inputs of the function

        virtual Function insert_knots(const AnyVector & new_knots) const override;
        virtual Function insert_knots(const AnyVector & new_knots, const NumericIndex & arg_ind) const override;
        virtual Function insert_knots(const AnyVector & new_knots, const std::string & arg) const override;
        virtual Function insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<std::string> & arg) const override;
        virtual Function insert_knots(const std::vector<AnyVector> & new_knots,
            const NumericIndexVector & arg_ind) const override;

        virtual Function midpoint_refinement(const std::vector<int>& refinement,
                const std::vector< int >& arg_ind) const override;

        virtual Function degree_elevation(const std::vector<int>& elivations,
                const std::vector< int >& arg_ind) const override;

        virtual Function kick_boundary(const TensorDomain& boundary) const override;
        virtual Function kick_boundary(const TensorDomain& boundary, const NumericIndex& arg_ind) const override;
        virtual Function kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string> & args) const override;
        virtual Function kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector & arg_ind) const override;

        virtual Function derivative(const std::vector<int>& orders,
            const std::vector< int >& arg_ind) const override;

        virtual Function antiderivative(const std::vector<int>& orders,
                const std::vector< int >& arg_ind) const override;

        virtual std::vector<spline::Function> jacobian() const override;

        virtual AnyTensor integral() const override;
        virtual AnyTensor integral(const TensorDomain& domain) const override;
        virtual Function partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args) const override;
        virtual Function partial_integral(const TensorDomain& domain,
         const NumericIndexVector& arg_ind) const override;

    protected:
        TensorBasis basis_;

        typedef std::function<TensorBasis(const TensorBasis&, const TensorBasis&)> BasisComposition;
        typedef std::function<AnyTensor(const AnyTensor&, const AnyTensor&)> TensorComposition;
        Function generic_operation(const Function& f,
                const BasisComposition & bc, const TensorComposition & tc) const;
};

#endif //       SWIG

} // namespace spline

#endif //       SRC_FUNCTION_FUNCTIONNODE_H_

