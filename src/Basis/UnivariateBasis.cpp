#include <vector>
#include "UnivariateBasis.h"
#include "UnivariateBasisNode.h"

#include "operations/operationsBasis.h"
#include "../common.h"

namespace spline {
    UnivariateBasisNode* UnivariateBasis::get() const {
        return static_cast<UnivariateBasisNode*>(SharedObject::get());
    };
    UnivariateBasisNode* UnivariateBasis::operator->() const { return get(); }

    int  UnivariateBasis::degree() const { return (*this)->degree(); }
}  // namespace spline
