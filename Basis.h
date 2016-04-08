//
// Created by erik on 08/04/16.
//

#ifndef CPP_SPLINE_BASIS_H
#define CPP_SPLINE_BASIS_H


class Basis {
public:
    Basis(int degree) : degree(degree) { }

    int getDegree() const {
        return degree;
    }

private:
    int degree;

};


#endif //CPP_SPLINE_BASIS_H
