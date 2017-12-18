#include "EmptyBasis.h"
#include "EmptyBasisNode.h"

#include "MonomialBasis.h"
#include "MonomialBasisNode.h"

#include "BSplineBasis.h"
#include "BSplineBasisNode.h"

namespace spline {

    EmptyBasisNode* EmptyBasis::get() const {
      return static_cast<EmptyBasisNode*>(SharedObject::get());
    };

    EmptyBasisNode* EmptyBasis::operator->() const { return get(); }

    EmptyBasis::EmptyBasis() {
        assign_node(new EmptyBasisNode());
    }
}  // namespace spline
