#ifndef OPERATIONSBASISG
#define OPERATIONSBASISG

#include <functional>

#include "plus.h"
#include "times.h"

namespace spline{
    class Basis;
    class TensorBasis;
    typedef std::function<Basis(const Basis&, const Basis&)> BasisComposition;
    TensorBasis generic_operation(const TensorBasis& b1, const TensorBasis& b2, const BasisComposition& bc);
    TensorBasis plusTensorBasis(const TensorBasis& b1, const TensorBasis& b2 );
    TensorBasis timesTensorBasis(const TensorBasis& b1, const TensorBasis& b2 );
}
#endif //  OPERATIONSBASISG
