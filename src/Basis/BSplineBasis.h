//
// Created by erik on 12/04/16.
//

#ifndef CPP_SPLINE_BSPLINEBASIS_H
#define CPP_SPLINE_BSPLINEBASIS_H

#include <vector>
#include "Basis.h"
#include "UnivariateBasis.h"

namespace spline{

class BSplineBasis : UnivariateBasis{


public:
    BSplineBasis (std::vector<double >& knots, int degree);
    BSplineBasis (std::vector<double >& bounds, int degree, int numberOfInternalKnots);

    std::vector<double> &getKnot ();
    void setKnot (std::vector<double> &knots) ;
//
//    private:
//        void init(std::vector<double >& knots, int degree);
    std::vector<double> evalBasis(double x);
    int length();
    Basis* plus(Basis* other);



private:
    std::vector<double> *knots;
};


}

#endif //CPP_SPLINE_BSPLINEBASIS_H
