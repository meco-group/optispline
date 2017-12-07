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

    template<class T>
    AnyTensor MonomialBasisNode::basis_evaluation(const std::vector< T >& x ) const {
        T x_ = x[0];
        int lenght  = this->dimension();
        std::vector<T> evaluation_basis(lenght);
        for (int i = 0; i < lenght; ++i) {
              evaluation_basis[i] = pow(x_,i);
        }
        return AnyTensor(vertcat(evaluation_basis));
    }

}  // namespace spline

#endif  // SUBMONOMIALBASIS_H_
