// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#include "Basis.h"
#include "UnivariateBasis.h"
#include "MonomialeBasis.h"

namespace spline {
    MonomialeBasis::MonomialeBasis (int degree) : UnivariateBasis(degree) { }

    std::vector<double> MonomialeBasis::evalBasis (double x) {
        std::vector<double> evaluation_basis(this->getDegree(), 10);
        return evaluation_basis;
    }

//    std::vector<double> MonomialeBasis::evalBasis (std::vector<double> x) {
//        std::vector<double> evaluation_basis(x.size(), 20);
//        return evaluation_basis;
//    }

    int MonomialeBasis::length () {return this->getDegree(); }

} // namespace spline