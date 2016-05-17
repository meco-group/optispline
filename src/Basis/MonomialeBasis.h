// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#ifndef MONOMIALBASIS_H_
#define MONOMIALBASIS_H_




#include <vector>
#include "Basis.h"
#include "UnivariateBasis.h"

namespace spline{
   class BSplineBasis;
class MonomialeBasis : public UnivariateBasis {
public:
    MonomialeBasis(){}
    //MonomialeBasis(int degree);

    //    evalution Basis
//   std::vector<double> evalBasis(double x) const;

//    int length() const;

    virtual Basis_ptr operator+(const MonomialeBasis& other) const ;
    virtual Basis_ptr operator+(const BSplineBasis& other) const ;
    virtual Basis_ptr operator+(const Basis& other) const ;

   virtual std::string getRepresentation() const { return "MonomialeBasis object"; };
//   Basis operator*(const MonomialeBasis& other)const ;
//   Basis operator*(const BSplineBasis& other)const ;

 //    virtual std::vector<double> evaluationGrid(void) const;

    };
}  // namespace spline

#endif  // MONOMIALBASIS_H_
