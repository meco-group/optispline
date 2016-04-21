//
// Created by erik on 11/04/16.
//

#ifndef CPP_SPLINE_UNIVARIATEBASIS_H
#define CPP_SPLINE_UNIVARIATEBASIS_H

#include "Basis.h"

#include <casadi/casadi.hpp>

namespace spline {
    class UnivariateBasis : public Basis {
    public:
        UnivariateBasis (int degree) : degree(degree) {}

        //    evalution Basis
        virtual std::vector<double> evalBasis(double x) const = 0;
        virtual std::vector< std::vector<double> > evalBasis(const std::vector<double>& x) const;

        virtual int length() const = 0;
//        virtual Basis plus(Basis &other) = 0;

        int getDegree () const { return degree;}
        void increaseDegree(int d);


        casadi::DM transformation( const Basis &b) const;

        virtual std::vector<double> evaluatonGrid(void) const {};
    protected:
        int degree;
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
