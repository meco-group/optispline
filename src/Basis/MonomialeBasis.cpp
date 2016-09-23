#include <math.h>       /* pow */

#include <casadi/casadi.hpp> // range

#include "MonomialeBasis.h"
#include "SubMonomialeBasis.h"

#include "operations/operationsBasis.h"

namespace spline {
    std::string MonomialeBasisNode::getRepresentation() const {
        return "MonomialeBasis";
    }

    std::string MonomialeBasis::getRepresentation() const {
        return (*this)->getRepresentation() ;
    }

    MonomialeBasisNode* MonomialeBasis::get() const { return static_cast<MonomialeBasisNode*>(SharedObject::get()); };
    MonomialeBasisNode* MonomialeBasis::operator->() const { return get(); }

    MonomialeBasisNode::MonomialeBasisNode(int degree) : UnivariatBasisNode(SubMonomialeBasis(degree)) {};
    MonomialeBasis::MonomialeBasis(int degree)  {
        assign_node(new MonomialeBasisNode(degree));
    }

} // namespace spline
