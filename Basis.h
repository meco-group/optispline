//
// Created by erik on 08/04/16.
//

#ifndef CPP_SPLINE_BASIS_H
#define CPP_SPLINE_BASIS_H

//namespace spline{


class Basis {
public:
    Basis(int degree) : degree(degree) { }
    int get_degree() const {
        return degree;
    }


private:
    int degree;

};
//} // namespace spline

#endif //CPP_SPLINE_BASIS_H
