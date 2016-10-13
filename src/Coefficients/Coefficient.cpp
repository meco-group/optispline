#include "Coefficient.h"

namespace spline {

    Coefficient::Coefficient (const std::vector< double >& v) :
        data(DT(DM(v),std::vector<int>{static_cast<int>(v.size()),1,1})){
    }
}  // namespace spline

