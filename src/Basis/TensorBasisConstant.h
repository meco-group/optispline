#ifndef SRC_BASIS_TENSORBASISCONSTANT_H_
#define SRC_BASIS_TENSORBASISCONSTANT_H_

#include <iostream>
#include <string>
#include <vector>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "Basis.h"
#include "TensorBasis.h"
#include "../Function/Argument.h"
#include "../Function/NumericIndex.h"
#include "../Domain/TensorDomain.h"

namespace spline {

class TensorBasisConstant;
class BSplineBasis;
class MonomialBasis;
class Function;

#ifndef SWIG

class TensorBasisConstantNode : public TensorBasisNode {
    public:
        TensorBasisConstantNode() : TensorBasisNode(std::vector< Basis > {}) {};

        /* virtual Basis basis(const Argument& index) const override; */

        virtual std::string type() const {return "TensorBasisConstant";}

        /* virtual int n_basis() const override; */
        /* virtual std::vector<int> dimension() const override; */
        /* virtual int n_inputs() const override; */

        /* std::string argument(int index) const; */
        /* int indexArgument(std::string a) const; */

        /* bool hasArguments() const; */

        virtual std::string to_string() const override;

        virtual TensorBasis operator+(const TensorBasis& rhs) const override;
        virtual TensorBasis operator+(const TensorBasisConstant& rhs) const override;
        virtual TensorBasis operator*(const TensorBasis& rhs) const override;
        virtual TensorBasis operator*(const TensorBasisConstant& rhs) const override;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x, const std::vector< int >& arg_ind) const override;

        /* int totalNumberBasisFunctions() const; */

        /* spline::Function basis_functions() const ; */
        virtual std::vector< int > get_permutation(const TensorBasis& grid) const override;

};

#endif // SWIG

class TensorBasisConstant : public TensorBasis{
public:
#ifndef SWIG
        TensorBasisConstant();
        TensorBasisConstant(const TensorBasis& tb);
        TensorBasisConstantNode* get() const;
        TensorBasisConstantNode* operator->() const;
#endif // SWIG
};

}   // namespace spline

#endif  // SRC_BASIS_TENSORBASISCONSTAN_H_
