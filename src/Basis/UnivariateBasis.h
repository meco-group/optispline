//
// Created by erik on 11/04/16.
//

#ifndef CPP_SPLINE_UNIVARIATEBASIS_H
#define CPP_SPLINE_UNIVARIATEBASIS_H

#include "Basis.h"

#include <casadi/casadi.hpp>

#include <string>

namespace spline {
    class UnivariateBasis : public Basis {
    public:
//        UnivariateBasis (int degree);
        UnivariateBasis (int degree, std::string argument = std::string("__"));


        //    evalution Basis
        virtual std::vector<double> evalBasis(double x) const = 0;
        virtual std::vector< std::vector<double> > evalBasis(const std::vector<double>& x) const;

        virtual int length() const = 0;
//        virtual Basis plus(Basis &other) = 0;

        int getDegree () const { return degree;}
        void setDegree (int degree);
        void increaseDegree(int d);

        casadi::DM transformation( const Basis &b) const;

        virtual std::vector<double> evaluationGrid (void) const = 0;

        void setArguments (const std::string argument);


    private:
        void initUnivariateBasis (int degree, std::string argument);


    protected:
        int degree;


    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
