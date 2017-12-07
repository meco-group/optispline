#ifndef SRC_BASIS_MONOMIALBASIS_H_
#define SRC_BASIS_MONOMIALBASIS_H_

#include <vector>
#include <any_tensor.hpp>

#include "Basis.h"
#include "UnivariateBasis.h"

namespace spline {

class MonomialBasisNode;

class MonomialBasis : public UnivariateBasis {
 public:
#ifndef SWIG
    MonomialBasis() {}
    MonomialBasisNode* get() const;
    MonomialBasisNode* operator->() const;
#endif  // SWIG
    MonomialBasis(int degree);
};

}  // namespace spline

#endif  // SRC_BASIS_MONOMIALBASIS_H_
