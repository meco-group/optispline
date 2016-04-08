// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#ifndef POLYNOMIALBASIS_H_
#define POLYNOMIALBASIS_H_

#include <vector>
#include "Basis.h"
namespace spline {
class PolynomialBasis : public Basis {
public:
    PolynomialBasis(int degree);
    std::vector<double> f(double x);
};
}  // namespace spline

#endif  // POLYNOMIALBASIS_H_
