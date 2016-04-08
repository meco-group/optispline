//
// Created by erik on 08/04/16.
//

#ifndef CPP_SPLINE_POLYNOMIALBASIS_H
#define CPP_SPLINE_POLYNOMIALBASIS_H

//namespace spline {

class PolynomialBasis : public Basis {
public:
    PolynomialBasis(int degree);

    vector<double> f(double x);


};
//} // namespace spline

#endif //CPP_SPLINE_POLYNOMIALBASIS_H
