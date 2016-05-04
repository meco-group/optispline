// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#ifndef BASIS_H_
#define BASIS_H_

#include <string>
#include <vector>
//#include "BSplineBasis.h"
//#include "MonomialeBasis.h"
//#include "../Function/Argument.h"
//#include "../Math/Tensor.h"
//#include "../Math/Domain.h"

namespace spline {
    class BSplineBasis;
    class MonomialeBasis;

class Basis {
public:
      Basis ();
//    Basis (std::vector< Basis> vectorBasis ); 
//    Basis (int dimension, const std::vector<int> &size, const Argument &argument);

 //   virtual Tensor operator()(const casadi::DM & x) const;
//    virtual std::vector< Tensor > operator()(const std::vector< casadi::DM >& x) const = 0;

 //   int getDimension () const;

 //   std::vector<int> getSize () const;

//    void setArgument (const Argument& argument);
//    void setArgument (const std::string& argumentName);
//    Argument getArgument () const;

    virtual Basis operator+(const MonomialeBasis& other) const;
    virtual Basis operator+(const BSplineBasis& other) const;
    virtual Basis operator+(const Basis& other) const;
    virtual std::string getRepresentation() const { return "Basis object"; };
protected:
//    std::vector< Basis > vectorBasis;
    //Argument argument;
//    Domain domain;
};
}
#endif  // BASIS_H_
