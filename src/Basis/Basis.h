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
//    evalution Basis
    virtual std::vector<double> evalBasis(double x) const = 0;
    std::vector<double> operator()(double x) const { return evalBasis(x); };
    virtual std::vector< std::vector<double> > evalBasis(const std::vector<double>& x) const = 0;
    std::vector< std::vector<double> > operator()(std::vector<double>& x) const { return evalBasis(x); };

    virtual int length() const= 0;
//    virtual Basis &plus(Basis &other) = 0;
};
}
#endif  // BASIS_H_
