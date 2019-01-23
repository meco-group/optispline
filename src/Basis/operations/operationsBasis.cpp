#include <vector>
#include <algorithm>
#include "operationsBasis.h"
#include "../TensorBasis.h"
#include "../Basis.h"
#include "../../common.h"

namespace spline {

    TensorBasis plus_tensor_basis(const TensorBasis& b1, const TensorBasis& b2) {
        return generic_operation(b1, b2,
                [](const Basis& lhs, const Basis& rhs) { return lhs + rhs; });
    }

    TensorBasis times_tensor_basis(const TensorBasis& b1, const TensorBasis& b2) {
        return generic_operation(b1, b2,
                [](const Basis& lhs, const Basis& rhs) { return lhs * rhs; });
    }

    TensorBasis generic_operation(const TensorBasis& b1, const TensorBasis& b2,
          const BasisComposition& bc) {
        TensorBasis returnBasis = TensorBasis();
        // one of the basis is empty (working with a constant)
        if (b1.n_basis() == 0) return b2;
        if (b2.n_basis() == 0) return b1;

        if (b1.hasArguments() && b2.hasArguments()) {
            std::vector< std::string > allArguments = b2.arguments();
            for (auto &a : b1.arguments()) {
                if (std::find(allArguments.begin(), allArguments.end(), a) == allArguments.end()) {
                    /* v does not contain argument a */
                    allArguments.push_back(a);
                }
            }

            for (auto &a : allArguments) {
                Basis subBasis1 = b1.basis(a);
                Basis subBasis2 = b2.basis(a);
                Basis sum = bc(subBasis1, subBasis2);
                returnBasis = returnBasis.add_basis(sum);
            }

            returnBasis = TensorBasis(returnBasis, allArguments);

        } else {
            spline_assert_message(b1.n_basis() == b2.n_basis(),
              "Mismatch dimension");
            for (casadi_int i = 0; i < b1.n_basis(); i++) {
                Basis subBasis1 = b1.basis(i);
                Basis subBasis2 = b2.basis(i);
                Basis sum = bc(subBasis1, subBasis2);
                returnBasis = returnBasis.add_basis(sum);
            }

            if (b1.hasArguments()) {
                returnBasis = TensorBasis(returnBasis, b1.arguments());
            }

            if (b2.hasArguments()) {
                returnBasis = TensorBasis(returnBasis, b2.arguments());
            }

        }
        return returnBasis;
    }
} // namespace spline
