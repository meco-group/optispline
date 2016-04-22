//
// Created by erik on 19/04/16.
//

#include <algorithm>    // std::set_union, std::sort
#include <vector>       // std::vector
#include "vectorUtilities.h"

namespace spline{

    std::vector<double> unionKnots(const std::vector<double> kn1_, const std::vector<double> kn2_, int degree, int degree1, int degree2) {
        int count1, count2, count;

        std::vector<double> kn1 = kn1_;
        std::vector<double> kn2 = kn2_;

        std::vector<double> uniqueKnots(kn1.size() + kn2.size());
        std::vector<double> knots (0);
        std::vector<double> multiple (0);
        std::vector<double>::iterator it;

        std::sort (kn1.begin(),kn1.end());
        std::sort (kn2.begin(),kn2.end());

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


    std::vector<double> addKnotsUnique(const std::vector<double> knots_,  const std::vector<double> newKnots_) {
        int count;

        std::vector<double> knots = knots_;
        std::vector<double> newKnots = newKnots_;

        std::vector<double> uniqueKnots(knots.size() + newKnots.size());
        std::vector<double> returnKnots (0);
        std::vector<double> multiple (0);
        std::vector<double>::iterator it;

        std::sort(knots.begin(),knots.end());
        std::sort(newKnots.begin(),newKnots.end());

        it=std::set_union (knots.begin(),knots.end(), newKnots.begin(),newKnots.end(), uniqueKnots.begin());
        uniqueKnots.resize(it-uniqueKnots.begin());
        uniqueKnots.erase( unique( uniqueKnots.begin(), uniqueKnots.end() ), uniqueKnots.end() );

        for (auto const& k: uniqueKnots) {
            count = std::count (knots.begin(), knots.end(), k) + std::count (newKnots.begin(), newKnots.end(), k);
            multiple.resize(count);
            std::fill(multiple.begin(), multiple.end(), k);

            returnKnots.insert(returnKnots.end(), multiple.begin(), multiple.end());
        }
        return returnKnots;
    }

    std::vector<double> addKnotsNotUnique(const std::vector<double> knots_,  const std::vector<double> newKnots_){
        int count;

        std::vector<double> knots = knots_;
        std::vector<double> newKnots = newKnots_;

        std::vector<double> returnKnots(knots.size() + newKnots.size());
        std::vector<double>::iterator it;

        std::sort(knots.begin(),knots.end());
        std::sort(newKnots.begin(),newKnots.end());

        it=std::set_union (knots.begin(),knots.end(), newKnots.begin(),newKnots.end(), returnKnots.begin());
        returnKnots.resize(it-returnKnots.begin());

        return returnKnots;

    }

}  // namespace

