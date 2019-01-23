#ifndef SRC_BASIS_MONOMIALBASIS_H_
#define SRC_BASIS_MONOMIALBASIS_H_

#include <vector>
#include <any_tensor.hpp>

#include "Basis.h"
#include "MonomialBasisNode.h"
#include "UnivariateBasis.h"

namespace spline {

class MonomialBasis : public UnivariateBasis {
 public:
#ifndef SWIG
    MonomialBasis() {}
    MonomialBasisNode* get() const;
    MonomialBasisNode* operator->() const;
#endif  // SWIG
    MonomialBasis(casadi_int degree);
};

}  // namespace spline

#endif  // SRC_BASIS_MONOMIALBASIS_H_
