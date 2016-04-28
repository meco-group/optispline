//
// Created by erik on 11/04/16.
//

#include <vector>
#include "Coefficient.h"

namespace spline {

    Coefficient::Coefficient () {}

    Coefficient::Coefficient (double d) {
        const casadi::DM c(d);
        initCoefficient(c);
    }


    Coefficient::Coefficient (const std::vector< std::vector< double > > m) {
        const casadi::DM c(m);
        initCoefficient(c);
    }

    Coefficient::Coefficient (const casadi::DM &coef) {
        initCoefficient(coef);
    }

    void Coefficient::initCoefficient (const casadi::DM &coef) {

    }

    const casadi::DM &Coefficient::getCoef () const {
        return coef;
    }

    void Coefficient::setCoef (const casadi::DM &coef) {
        Coefficient::coef = coef;
    }

    const std::vector<int> Coefficient::getShape () const{
        const std::vector<int> r = {coef.size1(), coef.size2()};
        return r;
    }

    Coefficient Coefficient::operator+ (Coefficient & other) const{
        Coefficient sum();
        sum.setCoef(coef + other.getCoef());
    }

}  // namespace spline

