#ifndef SUBMONOMIALBASIS_H_
#define SUBMONOMIALBASIS_H_

#include <any_tensor.hpp>
#include <vector>

#include "Basis.h"
#include "UnivariateBasis.h"

namespace spline{

    class MonomialBasisNode;

    class MonomialBasis : public UnivariateBasis {

    public:

#ifndef SWIG
        MonomialBasis(){}
        MonomialBasisNode* get() const ;
        MonomialBasisNode* operator->() const ;
#endif // SWIG
        MonomialBasis(int degree);

    };

}  // namespace spline

#endif  // SUBMONOMIALBASIS_H_
