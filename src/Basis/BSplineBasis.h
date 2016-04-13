//
// Created by erik on 12/04/16.
//

#ifndef CPP_SPLINE_BSPLINEBASIS_H
#define CPP_SPLINE_BSPLINEBASIS_H

#include <vector>
#include "UnivariateBasis.h"

namespace spline{

class BSplineBasis : public UnivariateBasis{


public:
//    BSplineBasis (std::vector<double >& knots, int degree);
    BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfInternalKnots);

    const std::vector<double> &getKnots ();
    void setKnots (const std::vector<double> &knots) ;
//
//    private:
//        void init(std::vector<double >& knots, int degree);
    std::vector<double> evalBasis(double x);
    int length();
    Basis& plus(Basis& other);

private:
    std::vector<bool> indector(int i, double x);

private:
    std::vector<double> knots;

};


}

#endif //CPP_SPLINE_BSPLINEBASIS_H
