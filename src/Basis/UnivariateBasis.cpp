//
// Created by erik on 11/04/16.
//

#include <vector>
#include "UnivariateBasis.h"

namespace spline {


    void UnivariateBasis::increaseDegree (int d) {
        degree += d;
    }


    std::vector< std::vector<double> > UnivariateBasis::evalBasis(const std::vector<double>& x) const{
        std::vector< std::vector<double> > r(x.size(), std::vector<double>(length()));

        for (int i = 0; i < x.size(); ++i) {
            r[i] = evalBasis(x[i]);
        }

        return r;
    }


    casadi::DM UnivariateBasis::transformation( const Basis &b) const {
        std::vector<double> grid = evaluatonGrid();
        
	casadi::DM A(evalBasis(grid));
        casadi::DM B(b.evalBasis(grid));

        std::vector<double> grid = evaluationGrid();
        casadi::DM A(std::vector< std::vector<double> >(evalBasis(grid)));
        casadi::DM B(std::vector< std::vector<double> >(b.evalBasis(grid)));
        return casadi::DM::solve(A, B);
    }


}  // namespace spline

