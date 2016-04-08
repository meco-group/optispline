//
// Created by erik on 08/04/16.
//

#ifndef CPP_SPLINE_POLYNOMIALBASIS_H
#define CPP_SPLINE_POLYNOMIALBASIS_H

#include <vector>
#include "Basis.h"
//namespace spline {

class PolynomialBasis : public Basis {
public:
    PolynomialBasis(int degree);

    std::vector<double> f(double x);

};
//} // namespace spline

#endif //CPP_SPLINE_POLYNOMIALBASIS_H
