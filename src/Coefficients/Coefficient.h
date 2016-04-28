//
// Created by erik on 11/04/16.
//

#ifndef CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
#define CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H

#include <casadi/casadi.hpp>
#include <string>
#include <vector>

namespace spline {
    class Coefficient {


    public:
        Coefficient ();
        Coefficient (double d);
        Coefficient (const std::vector< std::vector< double > > m);
        Coefficient (const casadi::DM &coef);

        void initCoefficient (const casadi::DM &coef);

        const casadi::DM & getCoef () const;
        void setCoef (const casadi::DM &coef);
        const std::vector< int > getShape() const;

        Coefficient operator+ (Coefficient & other) const;

    private:
        casadi::DM coef;



    };
} // namespace spline

#endif //CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
