//
// Created by erik on 12/04/16.
//

#include <stdio.h>
#include <math.h>       /* pow */

#include "BSplineBasis.h"

#include "plus.h"
#include "times.h"
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
            knots[degree + i] = (double)i/(numberOfIntervals-1);
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


    std::vector< std::vector<double> > BSplineBasis::evalBasis(const std::vector<double>& x) const {
        return std::vector< std::vector<double> >({{1,2},{3,4,5}});
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


} // namespace spline
