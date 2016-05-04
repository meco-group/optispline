// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#include "Basis.h"
#include "plus.h"

namespace spline {

    Basis::Basis () { }

//    Basis::Basis(std::vector< Basis> vectorBasis) : vectorBasis(vectorBasis){}
    
//    Basis::Basis (int dimension, const std::vector<int> &size, const Argument &argument) : dimension(dimension),
 //                                                                                          size(size),
//                                                                                           argument(argument) { }

//    int Basis::getDimension () const {
//        return vectorBasis.size();
//    }
//
//    void Basis::setArgument (const Argument &argument) {
//        this->argument = argument;
//    }
//
//    void Basis::setArgument (const std::string& argument) {
//        this->argument = Argument(argument);
//    }
//
//    Argument Basis::getArgument () const {
//        return argument;
//    }
//
//   std::vector<int> Basis::getSize () const {
//       std::vector<int> size;
//       std::vector<int> sizeSubBasis;
//       for(int i = 0; i < getDimension(); i++){
//           sizeSubBasis = vectorBasis.getSize(); 
//           for(int j = 0; j < sizeSubBasis.size(); j++){
//           size.push_back(sizeSubBasis(j));
//       }
//       return size;
//   }
    
    Basis Basis::operator+ (const Basis &other) const {
         return BSplineBasis();
        //        return other + *this;
    }

    Basis Basis::operator+ (const MonomialeBasis &other) const {
return Basis();//        return plusBasis(*this, other);
    }

    Basis Basis::operator+ (const BSplineBasis &other) const{
return Basis();//        return plusBasis(*this, other);
    }

} // namespace spline
