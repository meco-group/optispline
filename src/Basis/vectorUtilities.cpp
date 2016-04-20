//
// Created by erik on 19/04/16.
//

#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector
#include "vectorUtilities.h"

namespace spline{

    std::vector<double> unionKnots(const std::vector<double> kn1, const std::vector<double> kn2, int degree) {
        std::vector<double> v(kn1.size() + kn2.size());
        std::vector<double>::iterator it;
        it=std::set_union (kn1.begin(),kn1.end(), kn2.begin(),kn2.end(), v.begin());
        v.resize(it-v.begin());
        return v;
    }
}  // namespace

