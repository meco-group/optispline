#ifndef CPP_SPLINES_VECTORUTILITIES_H
#define CPP_SPLINES_VECTORUTILITIES_H

#include <vector>

namespace spline{

    const std::vector<double> unionKnots(const std::vector<double> kn1,  const std::vector<double> kn2, int degree, int degree1, int degree2) ;
    const std::vector<double> addKnotsUnique(const std::vector<double> knots,  const std::vector<double> newKnots) ;
    const std::vector<double> addKnotsNotUnique(const std::vector<double> knots,  const std::vector<double> newKnots) ;
    const std::vector<double> increaseMultiplicityKnots(const std::vector<double> knots_,  int increase);
}  // namespace spline

#endif //CPP_SPLINES_VECTORUTILITIES_H
