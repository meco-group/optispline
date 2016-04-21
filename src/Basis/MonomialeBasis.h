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

    int length() const;

    MonomialeBasis operator+(const MonomialeBasis& other)const ;
    BSplineBasis operator+(const BSplineBasis& other)const ;

    MonomialeBasis operator*(const MonomialeBasis& other)const ;
    BSplineBasis operator*(const BSplineBasis& other)const ;
    };
}  // namespace spline

#endif  // MONOMIALBASIS_H_
