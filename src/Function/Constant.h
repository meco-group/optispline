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
        Constant(const AnyConstant& c, const std::vector< int >& sha);

        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const override;

        /* virtual GenericFunction operator+(const GenericFunction& f) const override; */
        /* virtual GenericFunction operator+(const Function& f) const override; */
        /* virtual GenericFunction operator+(const Conastant& f) const override; */
        /* virtual GenericFunction operator+(const AnyTensor& t) const override; */
        /* virtual GenericFunction operator*(const GenericFunction& f) const override; */
        /* virtual GenericFunction operator*(const Function& f) const override; */
        /* virtual GenericFunction operator*(const Constant& f) const override; */
        /* virtual GenericFunction operator*(const AnyTensor& t) const override; */
        virtual GenericFunction operator-() const override;

        /* virtual GenericFunction mtimes(const GenericFunction& f) const override; */
        /* virtual GenericFunction mtimes(const Function& f) const override; */
        /* virtual GenericFunction mtimes(const Constant& f) const override; */
        /* virtual GenericFunction mtimes(const AnyTensor& f) const override; */
        /* virtual GenericFunction rmtimes(const AnyTensor& f) const override; */

        virtual GenericFunction transpose() const override;
        /* virtual GenericFunction trace() const override; */

        virtual std::string type() const override override;
        virtual std::string to_string() const override override;

        virtual GenericFunction transform_to(const Basis& basis) const override;
        virtual GenericFunction transform_to(const TensorBasis& basis) const override;
        virtual GenericFunction project_to(const Basis& basis) const override;
        virtual GenericFunction project_to(const TensorBasis& basis) const override;

        virtual GenericFunction reshape(const std::vector< int >& shape) const override;

        virtual GenericFunction slice(const AnySlice& i, const AnySlice& j) const override;
        virtual GenericFunction slice(const AnySlice& i) const override;

        virtual TensorBasis tensor_basis() const override;
};

} // namespace spline

#endif // SRC_FUNCTION_CONSTANT_H_
