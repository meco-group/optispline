// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#include <math.h>       /* pow */

#include <casadi/casadi.hpp> // range

#include "MonomialeBasis.h"

#include "plus.h"
#include "times.h"

namespace spline {
    MonomialeBasis::MonomialeBasis (int degree) : UnivariateBasis(degree) { }

    std::vector<double> MonomialeBasis::evalBasis (double x) const {
        std::vector<double> evaluation_basis(this->length());

        for (int i = 0; i < this->length (); ++i) {
            evaluation_basis[i] = pow (x,i);
        }

        return evaluation_basis;
    }


    int MonomialeBasis::length () const{return this->getDegree() + 1; }

    MonomialeBasis MonomialeBasis::operator+ (const MonomialeBasis &other)const  {
        return plusBasis(*this, other);;
    }

    BSplineBasis MonomialeBasis::operator+ (const BSplineBasis &other)const  {
        return plusBasis(*this, other);
    }

    MonomialeBasis MonomialeBasis::operator* (const MonomialeBasis &other)const  {
        return timesBasis(*this, other);
    }

    BSplineBasis MonomialeBasis::operator* (const BSplineBasis &other)const  {
        return timesBasis(*this, other);
    }

// TODO(ErikLambr) check is grid is valid
    std::vector<double> MonomialeBasis::evaluationGrid () const {
//        std::vector<double> grid = casadi::range(length());
        std::vector<double> grid;
        for( int i = 0; i < length(); i++ )
            grid.push_back( (double)i );
        return grid;
    }
} // namespace spline
