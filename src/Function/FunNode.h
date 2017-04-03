#ifndef SRC_FUNCTION_FUNNODE_H_
#define SRC_FUNCTION_FUNNODE_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObjectNode.h"

#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include "Argument.h"
#include "../Domain/TensorDomain.h"

namespace spline {
    class Function;
    class FunctionNode;
    class ConstantNode;

    class FunNode : public SharedObjectNode{
        public:
            FunNode() {}
            FunNode(const Coefficient& coeff) : coeff_(coeff) {}

            virtual std::string type() const;
            virtual std::string to_string() const;

            virtual AnyTensor operator()(const AnyTensor& x, const std::vector< int >& args)  const = 0;

            virtual Function partial_eval(const AnyTensor& x, int args)  const = 0;

            Function operator+(const Function& f) const;
            Function operator*(const Function& f) const;
            Function mtimes(const Function& f) const;

            virtual Function operator-() const = 0;

            virtual Function operator+(const FunNode& f) const = 0;
            virtual Function operator+(const FunctionNode& f) const = 0;
            virtual Function operator+(const ConstantNode& f) const = 0;

            virtual Function operator*(const FunNode& f) const = 0;
            virtual Function operator*(const FunctionNode& f) const = 0;
            virtual Function operator*(const ConstantNode& f) const = 0;

            virtual Function mtimes(const FunNode& f) const = 0;
            virtual Function mtimes(const FunctionNode& f) const = 0;
            virtual Function mtimes(const ConstantNode& f) const = 0;
            virtual Function rmtimes(const FunctionNode& f) const = 0;
            virtual Function rmtimes(const ConstantNode& f) const = 0;

            virtual Function slice(const AnySlice& i, const AnySlice& j) const = 0;
            virtual Function slice(const AnySlice& i) const = 0;

            virtual Function transpose() const = 0;
            virtual Function trace() const = 0;

            Coefficient coeff() const {return coeff_;}
            AnyTensor coeff_tensor() const {return coeff_.data();}
            AnyTensor data() const {return coeff_tensor().squeeze(); }


            virtual Basis basis() const;
            virtual Basis basis(const Argument& i) const;
            virtual TensorBasis tensor_basis() const = 0;
            virtual TensorDomain domain() const;

            bool is_scalar() const;

            void repr() const { casadi::userOut() << to_string() << std::endl;}

            std::vector< int > shape() const;  // Shape result obtained after function evaluation
            virtual Function reshape(const std::vector< int >& shape) const = 0;

            virtual Function transform_to(const TensorBasis& basis) const = 0;
            virtual Function project_to(const TensorBasis& basis) const = 0;

            virtual int n_inputs() const;  // Number of inputs of the functio

            virtual Function insert_knots(const std::vector<AnyVector> & new_knots,
                    const std::vector< int >& arg_ind) const;

        virtual Function midpoint_refinement(const std::vector<int>& refinement,
                const std::vector< int >& arg_ind) const;

        virtual Function degree_elevation(const std::vector<int>& orders,
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

        protected:
            Coefficient coeff_;
    };

} // namespace spline

#endif //       SRC_FUNCTION_FUNNODE_H_
