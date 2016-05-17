// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#include <math.h>       /* pow */

#include <casadi/casadi.hpp> // range

#include "MonomialeBasis.h"

#include "plus.h"
#include "times.h"

namespace spline {
//    Monomialestd::shared_ptr<Basis>::Monomialestd::shared_ptr<Basis> (int degree) : Univariatestd::shared_ptr<Basis>(degree) { }
//
//   std::vector<double> Monomialestd::shared_ptr<Basis>::evalstd::shared_ptr<Basis> (double x) const {
//       std::vector<double> evaluation_basis(this->length());
//
//       for (int i = 0; i < this->length (); ++i) {
//           evaluation_basis[i] = pow (x,i);
//       }
//
//       return evaluation_basis;
//   }
//

 //   int Monomialestd::shared_ptr<Basis>::length () const{return this->getDegree() + 1; }

    Basis_ptr MonomialeBasis::operator+ (const Basis& other)const  {
        //return plusBasis(*this, other);
        return std::make_shared<Basis>();
    }

    Basis_ptr MonomialeBasis::operator+ (const MonomialeBasis& other)const  {
        return std::make_shared<Basis>();
        //  return plusstd::shared_ptr<Basis>(*this, other);
    }

    Basis_ptr MonomialeBasis::operator+ (const BSplineBasis& other)const  {
        return std::make_shared<Basis>();
        //  return plusstd::shared_ptr<Basis>(*this, other);
    }

//   std::shared_ptr<Basis> Monomialestd::shared_ptr<Basis>::operator* (const Monomialestd::shared_ptr<Basis> &other)const  {
//       return timesstd::shared_ptr<Basis>(*this, other);
//   }
//
//   std::shared_ptr<Basis> Monomialestd::shared_ptr<Basis>::operator* (const BSplinestd::shared_ptr<Basis> &other)const  {
//       return timesstd::shared_ptr<Basis>(*this, other);
//   }
//
/// TODO(ErikLambr) check is grid is valid
//   std::vector<double> Monomialestd::shared_ptr<Basis>::evaluationGrid () const {
///        std::vector<double> grid = casadi::range(length());
//       std::vector<double> grid;
//       for( int i = 0; i < length(); i++ )
//           grid.push_back( (double)i );
//       return grid;
//   }
} // namespace spline
