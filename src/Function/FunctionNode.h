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

    class FunctionNode : public FunNode{
    public :
        FunctionNode() {};
        FunctionNode(const TensorBasis& basis, const Coefficient& coeff);

        virtual std::string type() const override;
        virtual std::string to_string() const override;

        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const override;

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

        virtual Function slice(const AnySlice& i, const AnySlice& j) const override;
        virtual Function slice(const AnySlice& i) const override;

        virtual Function transpose() const override;
        virtual Function trace() const override;

        virtual Function transform_to(const TensorBasis& basis) const override;
        virtual Function project_to(const TensorBasis& basis) const override;

        virtual TensorBasis tensor_basis() const override;
    protected:
        TensorBasis basis_;
};
} // namespace spline

#endif //       SRC_FUNCTION_FUNCTIONNODE_H_

