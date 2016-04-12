//
// Created by erik on 12/04/16.
//

#include <math.h>       /* pow */

#include "BSplineBasis.h"

namespace spline{

//
//    BSplineBasis::BSplineBasis (std::vector<double> &knots, int degree) : UnivariateBasis(degree), knots(&knots){
//    }
//

    BSplineBasis::BSplineBasis (const std::vector<double> &bounds, int degree, int numberOfIntervals) : UnivariateBasis(degree) {
        int nuberOfKnots = 2*degree + numberOfIntervals;
        std::vector<double> *knots = new std::vector<double>(nuberOfKnots);

        for (int i = 0; i < degree; ++i) {
            (*knots)[i] = bounds[0];
            (*knots)[nuberOfKnots - i - 1] = bounds[1];
        }

        for (int i = 0; i < numberOfIntervals; ++i) {
            (*knots)[degree + 1 + i] = i/(numberOfIntervals-1);
        }

        this->setKnot(*knots);
//        BSplineBasis(*knots,degree);

    }

    std::vector<double> &BSplineBasis::getKnot () {
        return *knots;
    }

    void BSplineBasis::setKnot (std::vector<double> &knots) {
        BSplineBasis::knots = &knots;
    }

    std::vector<double> BSplineBasis::evalBasis (double x) {
        return std::vector<double>();
    }

    int BSplineBasis::length () {
        return knots->size() - getDegree() - 1;
    }

    Basis& BSplineBasis::plus (Basis &other) {
        return other;
    }


}
