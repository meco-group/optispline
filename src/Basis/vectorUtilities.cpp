//
// Created by erik on 19/04/16.
//

#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector
#include "vectorUtilities.h"

namespace spline{

    std::vector<double> unionKnots(const std::vector<double> kn1, const std::vector<double> kn2, int degree, int degree1, int degree2) {
        int count1, count2, count;

        std::vector<double> uniqueKnots(kn1.size() + kn2.size());
        std::vector<double> knots (0);
        std::vector<double> multiple (0);
        std::vector<double>::iterator it;

        it=std::set_union (kn1.begin(),kn1.end(), kn2.begin(),kn2.end(), uniqueKnots.begin());
        uniqueKnots.resize(it-uniqueKnots.begin());
        uniqueKnots.erase( unique( uniqueKnots.begin(), uniqueKnots.end() ), uniqueKnots.end() );

        for (auto const& k: uniqueKnots) {
            count1 = std::count (kn1.begin(), kn1.end(), k);
            count2 = std::count (kn2.begin(), kn2.end(), k);

            count =  std::max(count1 + degree - degree1, count2 + degree - degree2);

            multiple.resize(count);
            std::fill(multiple.begin(), multiple.end(), k);

            knots.insert(knots.end(), multiple.begin(), multiple.end());
        }
        return knots;
    }
}  // namespace

