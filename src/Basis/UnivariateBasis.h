//
// Created by erik on 11/04/16.
//

#ifndef CPP_SPLINE_UNIVARIATEBASIS_H
#define CPP_SPLINE_UNIVARIATEBASIS_H

#include "Basis.h"

#include <casadi/casadi.hpp>

#include <string>

namespace spline {
class MonomialeBasis;
class BSplineBasis;
class UnivariateBasis : public Basis {
public:
    UnivariateBasis(){}
//   UnivariateBasis (int dimension, const std::vector<int> &size, const Argument &argument, int degree);
//   UnivariateBasis (int degree);
//   UnivariateBasis (int degree, std::string argument);
//
//   //    evalution Basis
//   std::vector<double> operator()(double x) const;
//   virtual std::vector< std::vector<double> > operator()(const std::vector<double>& x) const = 0;
///    std::vector< double > operator()(const std::vector<double, 1>& x) const;
//   std::vector< std::vector< std::vector<double> > > operator()(const std::vector< std::vector<double> >& x) const;
//
///        virtual Basis plus(Basis &other) = 0;
//
//   int getDegree () const { return degree;}
//   void setDegree (int degree);
//   void increaseDegree(int d);
//
//   int getLenght() const ;
//
//   casadi::DM transformation( const Basis &b) const;
//
//   virtual std::vector<double> evaluationGrid (void) const = 0;

    virtual Basis operator+ (const Basis &other) const;
    virtual Basis operator+ (const MonomialeBasis &other) const;
    virtual Basis operator+ (const BSplineBasis &other) const;
    virtual std::string getRepresentation() const { return "UnivariateBasis object"; };

protected:
    int degree;
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
