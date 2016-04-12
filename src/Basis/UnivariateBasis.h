//
// Created by erik on 11/04/16.
//

#ifndef CPP_SPLINE_UNIVARIATEBASIS_H
#define CPP_SPLINE_UNIVARIATEBASIS_H

#include "Basis.h"

namespace spline {
    class UnivariateBasis : public Basis {
    public:
        UnivariateBasis (int degree) : degree(degree) {}

        virtual std::vector<double> evalBasis(double x) = 0;
        virtual int length() = 0;
        virtual Basis &plus(Basis &other) = 0;

        int getDegree () const {
            return degree;
        }

    private:
        int degree;
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
