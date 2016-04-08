//
// Created by erik on 08/04/16.
//

#include "PolynomialBasis.h"

//namespace spline{
PolynomialBasis::PolynomialBasis(int degree) : Basis(degree) { }

vector<double> PolynomialBasis::f(double x) {
    std::vector<double> evaluation_basis(this->getDegree(), 10);

//        for (int i = 0; i <= this->getDegree(); ++i) {
//
//        }
    return evaluation_basis;
}



//} // namespace spline
