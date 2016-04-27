// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#ifndef BASIS_H_
#define BASIS_H_

#include <string>
#include <vector>

namespace spline {
class Basis {
public:
//    evalution Basis
    virtual std::vector<double> evalBasis(double x) const = 0;
    std::vector<double> operator()(double x) const { return evalBasis(x); };
    virtual std::vector< std::vector<double> > evalBasis(const std::vector<double>& x) const = 0;
    std::vector< std::vector<double> > operator()(const std::vector<double>& x) const { return evalBasis(x); };

//    Basis* plus(Basis &other);
//

    virtual int length() const= 0;


    int getDimensions () const;
    void setDimensions (int dimensions);

    std::vector<std::string > getArguments () const;
    virtual void setArguments (const std::vector <std::string> arguments);


protected:
    int dimension;
    std::vector< std::string > arguments;
};
}
#endif  // BASIS_H_
