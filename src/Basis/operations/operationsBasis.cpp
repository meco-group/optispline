#include <vector>
#include <algorithm>
#include "operationsBasis.h"
#include "../TensorBasis.h"
#include "../Basis.h"
#include "../../Function/Argument.h"
#include "../../common.h"

namespace spline {

    TensorBasis plusBasis(const TensorBasis& b1, const TensorBasis& b2) {
        return generic_operation(b1, b2,
                [](const Basis& lhs, const Basis& rhs) { return lhs + rhs; });
    }

    TensorBasis timesBasis(const TensorBasis& b1, const TensorBasis& b2) {
        return generic_operation(b1, b2,
                [](const Basis& lhs, const Basis& rhs) { return lhs * rhs; });
    }

    TensorBasis generic_operation(const TensorBasis& b1, const TensorBasis& b2,
          const BasisComposition& bc) {
        TensorBasis returnBasis = TensorBasis();
        if (b1.hasArguments() && b2.hasArguments()) {
            std::vector< Argument > allArguments = b1.getArguments();
            for (auto &a : b2.getArguments()) {
                if (std::find(allArguments.begin(), allArguments.end(), a) == allArguments.end()) {
                    /* v does not contain x */
                    allArguments.push_back(a);
                }
            }

            for (auto &a : allArguments) {
                Basis subBasis1 = b1.getBasis(a);
                Basis subBasis2 = b2.getBasis(a);
                Basis sum = bc(subBasis1, subBasis2);
                returnBasis.addBasis(sum);
            }

            returnBasis.setArguments(allArguments);

        } else {
            spline_assert_message(b1.n_basis() == b2.n_basis(),
              "Mismatch dimension");
            for (int i = 0; i < b1.n_basis(); i++) {
                Basis subBasis1 = b1.getBasis(i);
                Basis subBasis2 = b2.getBasis(i);
                Basis sum = bc(subBasis1, subBasis2);
                returnBasis.addBasis(sum);
            }

            if (b1.hasArguments()) {
                returnBasis.setArguments(b1.getArguments());
            }

            if (b2.hasArguments()) {
                returnBasis.setArguments(b2.getArguments());
            }

        }
        return returnBasis;
    }
} // namespace spline
