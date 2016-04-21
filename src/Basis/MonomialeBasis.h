// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#ifndef MONOMIALBASIS_H_
#define MONOMIALBASIS_H_


class MonomialeBasis;
class BSplineBasis;

#include <vector>
#include "UnivariateBasis.h"



namespace spline {
    class BSplineBasis;

class MonomialeBasis : public UnivariateBasis {
public:
    MonomialeBasis(int degree);

    //    evalution Basis
    std::vector<double> evalBasis(double x) const;
    std::vector<double> operator()(double x) const { return evalBasis(x); };
    std::vector< std::vector<double> > evalBasis(const std::vector<double>& x) const ;
    std::vector< std::vector<double> > operator()(const std::vector<double>& x) const { return evalBasis(x); };


    int length() const;

    MonomialeBasis operator+(const MonomialeBasis& other)const ;
    BSplineBasis operator+(const BSplineBasis& other)const ;

    MonomialeBasis operator*(const MonomialeBasis& other)const ;
    BSplineBasis operator*(const BSplineBasis& other)const ;
    };
}  // namespace spline

#endif  // MONOMIALBASIS_H_
