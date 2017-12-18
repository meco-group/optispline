#include "MonomialBasis.h"
#include "MonomialBasisNode.h"

/* #include "operations/operationsBasis.h" */

namespace spline {

    MonomialBasisNode* MonomialBasis::get() const {
        return static_cast<MonomialBasisNode*>(SharedObject::get());
    }
    MonomialBasisNode* MonomialBasis::operator->() const { return get(); }

    MonomialBasis::MonomialBasis(int degree)  {
        assign_node(new MonomialBasisNode(degree));
    }
}  // namespace spline
