#ifndef SRC_FUNCTION_CONSTANTNODE_H_
#define SRC_FUNCTION_CONSTANTNODE_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include <any_tensor.hpp>

#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include "Argument.h"
#include "FunNode.h"

namespace spline {

    class Function;
    class FunctionNode;

    class ConstantNode : public FunNode{
    public:
        ConstantNode(const AnyTensor& coeff);

        virtual std::string type() const;
        virtual std::string to_string() const;

        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const;

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

        virtual TensorBasis tensor_basis() const override;
        /* virtual TensorDomain domain() const override; */

        virtual Function transform_to(const TensorBasis& basis) const override;
        virtual Function project_to(const TensorBasis& basis) const override;

    };

} // namespace spline

#endif //       SRC_FUNCTION_CONSTANTNODE_H_
