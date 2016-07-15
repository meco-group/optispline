//
// Created by erik on 11/04/16.
//

#include <vector>
#include "Function.h"

namespace spline {

    AnyTensor Function::operator()(const std::vector< AnyScalar >& x) const{
        return basis(x).inner(coef.data);
    }

}  // namespace spline

