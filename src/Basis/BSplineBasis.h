//
// Created by erik on 12/04/16.
//

#ifndef CPP_SPLINE_BSPLINEBASIS_H
#define CPP_SPLINE_BSPLINEBASIS_H


class MonomialeBasis;
class BSplineBasis;

#include <vector>
#include "UnivariateBasis.h"
#include "MonomialeBasis.h"


namespace spline{

class BSplineBasis : public UnivariateBasis{


public:
    BSplineBasis (const std::vector<double >& knots, int degree);
    BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfInternalKnots);

    ~BSplineBasis ();

    const std::vector<double> &getKnots () const;
    void setKnots (const std::vector<double> &knots) ;

    //    evalution Basis
    std::vector<double> evalBasis(double x) const;
    std::vector<double> operator()(double x) const { return evalBasis(x); };
    std::vector< std::vector<double> > evalBasis(const std::vector<double>& x) const ;
    std::vector< std::vector<double> > operator()(const std::vector<double>& x) const { return evalBasis(x); };

    int length() const;

    BSplineBasis operator+(const MonomialeBasis& other) const ;
    BSplineBasis operator+(const BSplineBasis& other) const ;

    BSplineBasis operator*(const MonomialeBasis& other) const ;
    BSplineBasis operator*(const BSplineBasis& other) const ;

    std::vector<double> greville () const;

private:
    std::vector<bool> indector(int i, double x);

private:
    std::vector<double> knots;

};


}

#endif //CPP_SPLINE_BSPLINEBASIS_H
