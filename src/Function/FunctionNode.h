#ifndef SRC_FUNCTION_FUNCTIONNODE_H_
#define SRC_FUNCTION_FUNCTIONNODE_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObjectNode.h"
#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include "Argument.h"

namespace spline {

#ifndef SWIG
    class Function;

    class FunctionNode : public SharedObjectNode{
    public :
        FunctionNode() {};
        FunctionNode(const Coefficient& coeff);
        FunctionNode(const TensorBasis& basis, const Coefficient& coeff);

        virtual std::string type() const;
        virtual std::string to_string() const;

        Coefficient coeff() const {return coeff_;}
        AnyTensor coeff_tensor() const {return coeff_.data();}
        AnyTensor data() const {return coeff_tensor().squeeze(); }

        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< Argument >& args ) const;
        virtual AnyTensor list_eval(const AnyTensor& x, const std::vector< Argument >& args) const;
        virtual AnyTensor grid_eval(const std::vector< AnyTensor >& x, const std::vector< Argument >& args,  bool squeeze_return) const;

        virtual Function partial_eval(const AnyTensor& x, const Argument& args) const;

        Function operator+(const Function& f) const;
        virtual Function operator+(const FunctionNode& f) const;
        virtual Function operator-() const;

        Function operator*(const Function& f) const;
        virtual Function operator*(const FunctionNode& f) const;

        Function mtimes(const Function& f) const;
        virtual Function mtimes(const FunctionNode& f) const;
        virtual Function rmtimes(const FunctionNode& f) const;

        bool is_scalar() const;
        std::vector< int > shape() const;  // Shape result obtained after function evaluation
        virtual Function reshape(const std::vector< int >& shape) const;

        virtual Function slice(const AnySlice& i, const AnySlice& j) const;
        virtual Function slice(const AnySlice& i) const;

        virtual Function transpose() const;
        virtual Function trace() const;

        virtual Function transform_to(const TensorBasis& basis) const;
        virtual Function project_to(const TensorBasis& basis) const;

        virtual TensorBasis tensor_basis() const;
        virtual Basis basis() const;
        virtual Basis basis(const Argument& i) const;

        virtual int n_inputs() const;  // Number of inputs of the function

        virtual Function insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<int> & arg) const;

        virtual Function midpoint_refinement(const std::vector<int>& refinement,
                const std::vector< int >& arg_ind) const;

        virtual Function degree_elevation(const std::vector<int>& elivations,
                const std::vector< int >& arg_ind) const;

        virtual Function kick_boundary(const TensorDomain& boundary) const;
        virtual Function kick_boundary(const TensorDomain& boundary, const NumericIndex& arg_ind) const;
        virtual Function kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string> & args) const;
        virtual Function kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector & arg_ind) const;

        virtual Function derivative(const std::vector<int>& orders,
            const std::vector< int >& arg_ind) const;

        virtual Function antiderivative(const std::vector<int>& orders,
                const std::vector< int >& arg_ind) const;

        virtual std::vector<spline::Function> jacobian() const;

        virtual AnyTensor integral() const;
        virtual AnyTensor integral(const TensorDomain& domain) const;
        virtual Function partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args) const;
        virtual Function partial_integral(const TensorDomain& domain,
         const NumericIndexVector& arg_ind) const;

        TensorDomain domain() const;
    protected:
        Coefficient coeff_;
        TensorBasis basis_;

        typedef std::function<TensorBasis(const TensorBasis&, const TensorBasis&)> BasisComposition;
        typedef std::function<AnyTensor(const AnyTensor&, const AnyTensor&)> TensorComposition;
        Function generic_preproces_operation(const Function& f,
                const BasisComposition & bc, const TensorComposition & tc) const;
        Function generic_operation(const Function& f,
                const BasisComposition & bc, const TensorComposition & tc) const;
};

#endif //       SWIG

} // namespace spline

#endif //       SRC_FUNCTION_FUNCTIONNODE_H_
