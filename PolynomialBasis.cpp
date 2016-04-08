// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#include "PolynomialBasis.h"


namespace spline {
 PolynomialBasis::PolynomialBasis(int degree) : Basis(degree) {}

 std::vector<double> PolynomialBasis::f(double x) {
    std::vector<double> evaluation_basis(this->get_degree(), 10);
    return evaluation_basis;
 }

}  // namespace spline
