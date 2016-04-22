//
// Created by erik on 12/04/16.
//

#include <math.h>       /* pow */
#include <numeric>      // std::accumulate -> greville

#include "BSplineBasis.h"

#include "plus.h"
#include "times.h"
#include "vectorUtilities.h"

namespace spline{

    BSplineBasis::BSplineBasis (const std::vector<double> &knots, int degree) : UnivariateBasis(degree), knots(knots) {}

    BSplineBasis::BSplineBasis (const std::vector<double> &bounds, int degree, int numberOfIntervals) : UnivariateBasis(degree) {
        int numberOfKnots = 2*degree + numberOfIntervals;
        knots.resize(numberOfKnots, 0);

        for (int i = 0; i < degree; ++i) {
            knots[i] = bounds[0];
            knots[numberOfKnots - i - 1] = bounds[1];
        }

        for (int i = 0; i < numberOfIntervals; ++i) {
            knots[degree + i] = bounds[0] + (bounds[1] - bounds[0]) * (double)i/(numberOfIntervals-1);
        }

        setKnots(knots);
    }

    BSplineBasis::~BSplineBasis () {
    }

    const std::vector<double> &BSplineBasis::getKnots () const {
        return knots;
    }


    void BSplineBasis::setKnots (const std::vector<double> &knots) {
        BSplineBasis::knots = knots;
    }

    std::vector<double> BSplineBasis::evalBasis (double x) const {
        double b;
        double bottom;
        double basis[degree+1][knots.size()-1];

        for (int i=0; i<(knots.size()-1); i++){
            if((i < degree+1) and (knots[0] == knots[i])){
                basis[0][i] = ((x >= knots[i]) and (x <= knots[i+1]));
            }else{
                basis[0][i] = ((x > knots[i]) and (x <= knots[i+1]));
            }
        }

        for (int d=1; d<(degree+1); d++){
            for (int i=0; i < length(); i++){
                b = 0;
                bottom = knots[i+d] - knots[i];
                if (bottom != 0){
                    b = (x - knots[i])*basis[d-1][i]/bottom;
                }
                bottom = knots[i+d+1] - knots[i+1];
                if (bottom != 0){
                    b += (knots[i+d+1] - x)*basis[d-1][i+1]/bottom;
                }
                basis[d][i] = b;
            }
        }

        std::vector<double> r(length());

        for (int i = 0; i < length(); ++i) {
            r[i] = basis[degree][i];
        }
        return r;
    }

    int BSplineBasis::length () const{
        return knots.size() - degree - 1;
    }

    std::vector<bool> BSplineBasis::indector (int i, double x) {
        std::vector<bool> r(knots.size() - 1);

        for (int i = 0; i < knots.size() - 1; ++i) {
           r[i] =  (x >= knots[i]) * (x < knots[i + 1]);
        }

        return r;
    }

    BSplineBasis BSplineBasis::operator+ (const MonomialeBasis &other) const {
        return plusBasis(*this, other);
    }

    BSplineBasis BSplineBasis::operator+ (const BSplineBasis &other) const{
        return plusBasis(*this, other);
    }

    BSplineBasis BSplineBasis::operator* (const MonomialeBasis &other) const {
        return timesBasis(*this, other);
    }

    BSplineBasis BSplineBasis::operator* (const BSplineBasis &other) const{
        return timesBasis(*this, other);
    }

    std::vector<double> BSplineBasis::greville () const {
        int d = degree;
        if(d == 1){
            d =1;
        }

        std::vector<double> g (length());
        double s;
        for (int i = 0; i < length(); ++i) {
            g[i] = std::accumulate(knots.begin()+i+1,knots.begin()+i+d+1, 0.0) / (d);
        }

        return g;
    }

    std::vector<double> BSplineBasis::evaluationGrid (void) const {
        return greville();
    }


    BSplineBasis BSplineBasis::addKnots(const std::vector<double> newKnots, bool unique) const {
        std::vector<double> kn;
        if (unique){
            kn = addKnotsUnique(knots, newKnots);
        } else {
            kn = addKnotsNotUnique(knots, newKnots);
        }
        return BSplineBasis(kn, degree);
    }

} // namespace spline
