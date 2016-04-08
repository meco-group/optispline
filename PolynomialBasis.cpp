//
// Created by erik on 08/04/16.
//


#include "PolynomialBasis.h"

//namespace spline{
PolynomialBasis::PolynomialBasis(int degree) : Basis(degree) { }

std::vector<double> PolynomialBasis::f(double x) {
    std::vector<double> evaluation_basis (this->get_degree(), 10);

//        for (int i = 0; i <= this->get_degree(); ++i) {
//
//        }
    
    return evaluation_basis;
}



//} // namespace spline
