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
        int numberOfKnots = 2*degree + numberOfIntervals;
        knots.resize(numberOfKnots);

        for (int i = 0; i < degree; ++i) {
            (knots)[i] = bounds[0];
            (knots)[numberOfKnots - i - 1] = bounds[1];
        }

        for (int i = 0; i < numberOfIntervals; ++i) {
            (knots)[degree + 1 + i] = i/(numberOfIntervals-1);
        }

        this->setKnots(knots);
    }

    const std::vector<double> &BSplineBasis::getKnots () {
        return knots;
    }

    void BSplineBasis::setKnots (const std::vector<double> &knots) {
        BSplineBasis::knots = knots;
    }

    std::vector<double> BSplineBasis::evalBasis (double x) {
        return std::vector<double>();
    }

    int BSplineBasis::length () {
        return knots.size() - getDegree() - 1;
    }

    Basis& BSplineBasis::plus (Basis &other) {
        return other;
    }


}
