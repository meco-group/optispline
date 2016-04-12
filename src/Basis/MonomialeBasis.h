// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#ifndef MONOMIALBASIS_H_
#define MONOMIALBASIS_H_

#include <vector>
#include "UnivariateBasis.h"

namespace spline {
    class MonomialeBasis : public UnivariateBasis {
public:
    MonomialeBasis(int degree);
    std::vector<double> evalBasis(double x);
//    std::vector<double> evalBasis(std::vector<double> x);
    int  length();
    Basis* plus(Basis* other);
    };
}  // namespace spline

#endif  // MONOMIALBASIS_H_
