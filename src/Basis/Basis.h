// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#ifndef BASIS_H_
#define BASIS_H_

#include <vector>

namespace spline {
class Basis {
public:
    virtual std::vector<double> evalBasis(double x) = 0;
//    std::vector<double> evalBasis(std::vector<double> x);
    virtual int length() const= 0;
    virtual Basis &plus(Basis &other) = 0;
};
}
#endif  // BASIS_H_
