// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#include <math.h>       /* pow */

#include "MonomialeBasis.h"

#include "plus.h"

namespace spline {
    MonomialeBasis::MonomialeBasis (int degree) : UnivariateBasis(degree) { }

    std::vector<double> MonomialeBasis::evalBasis (double x) const {
        std::vector<double> evaluation_basis(this->length());

        for (int i = 0; i < this->length (); ++i) {
            evaluation_basis[i] = pow (x,i);
        }

        return evaluation_basis;
    }

    std::vector< std::vector<double> > MonomialeBasis::evalBasis(const std::vector<double>& x) const {
        return std::vector< std::vector<double> >({{1,2},{3,4,5}});
    }

    int MonomialeBasis::length () const{return this->getDegree() + 1; }

    MonomialeBasis MonomialeBasis::operator+ (const MonomialeBasis &other)const  {
        return plusBasis(*this, other);;
    }

    BSplineBasis MonomialeBasis::operator+ (const BSplineBasis &other)const  {
        return plusBasis(*this, other);
    }





} // namespace spline
