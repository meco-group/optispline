#ifndef SRC_FUNCTION_CONSTANT_H_
#define SRC_FUNCTION_CONSTANT_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include <any_tensor.hpp>
#include "GenericFunction.h"
#include "Argument.h"

namespace spline {

class Constant : public GenericFunction {

    public :
        Constant(const AnyTensor& c);
        Constant(const AnyScalar& c, const std::vector< int >& sha);

        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const override;

        virtual Function slice(const AnySlice& i, const AnySlice& j) const override;
        virtual Function slice(const AnySlice& i) const override;

        virtual Function operator+(const GenericFunction& f) const override;
        virtual Function operator+(const Function& f) const override;
        virtual Function operator+(const Constant& f) const override;
        virtual Function operator+(const AnyTensor& t) const override;
        virtual Function operator*(const GenericFunction& f) const override;
        virtual Function operator*(const Function& f) const override;
        virtual Function operator*(const Constant& f) const override;
        virtual Function operator*(const AnyTensor& t) const override;

        virtual Function operator-() const override;

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

        virtual TensorBasis tensor_basis() const override;
};

} // namespace spline

#endif // SRC_FUNCTION_CONSTANT_H_
