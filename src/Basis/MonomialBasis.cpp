#include <math.h>       /* pow */

#include <casadi/casadi.hpp> // range

#include "MonomialBasis.h"
#include "SubMonomialBasis.h"

#include "operations/operationsBasis.h"

namespace spline {
    std::string MonomialBasisNode::getRepresentation() const {
        return "MonomialBasis";
    }

    std::string MonomialBasis::getRepresentation() const {
        return (*this)->getRepresentation() ;
    }

    MonomialBasisNode* MonomialBasis::get() const { return static_cast<MonomialBasisNode*>(SharedObject::get()); };
    MonomialBasisNode* MonomialBasis::operator->() const { return get(); }

    MonomialBasisNode::MonomialBasisNode(int degree) : UnivariateBasisNode(SubMonomialBasis(degree)) {};
    MonomialBasis::MonomialBasis(int degree)  {
        assign_node(new MonomialBasisNode(degree));
    }

} // namespace spline
