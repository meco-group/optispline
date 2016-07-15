#include "Coefficient.h"

namespace spline {

    Coefficient::Coefficient (const std::vector<AnyScalar>& v) : 
        data(AnyTensor(DM(v),std::vector<int>{static_cast<int>(v.size()),1,1})){
    }
}  // namespace spline

