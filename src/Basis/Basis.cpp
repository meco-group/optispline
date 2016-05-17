// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#include "Basis.h"
#include "plus.h"

namespace spline {

    Basis::Basis(){}

    Basis::Basis(std::vector< Basis_ptr> allSubBasis) : allSubBasis(allSubBasis){}
    
//    std::shared_ptr<Basis>::std::shared_ptr<Basis> (int dimension, const std::vector<int> &size, const Argument &argument) : dimension(dimension),
 //                                                                                          size(size),
//                                                                                           argument(argument) { }

//    int std::shared_ptr<Basis>::getDimension () const {
//        return vectorstd::shared_ptr<Basis>.size();
//    }
//
//    void std::shared_ptr<Basis>::setArgument (const Argument &argument) {
//        this->argument = argument;
//    }
//
//    void std::shared_ptr<Basis>::setArgument (const std::string& argument) {
//        this->argument = Argument(argument);
//    }
//
//    Argument std::shared_ptr<Basis>::getArgument () const {
//        return argument;
//    }
//
//   std::vector<int> std::shared_ptr<Basis>::getSize () const {
//       std::vector<int> size;
//       std::vector<int> sizeSubstd::shared_ptr<Basis>;
//       for(int i = 0; i < getDimension(); i++){
//           sizeSubstd::shared_ptr<Basis> = vectorstd::shared_ptr<Basis>.getSize(); 
//           for(int j = 0; j < sizeSubstd::shared_ptr<Basis>.size(); j++){
//           size.push_back(sizeSubstd::shared_ptr<Basis>(j));
//       }
//       return size;
//   }
    
    Basis_ptr Basis::operator+ (const Basis& rhs) const {
         return std::make_shared<Basis>();
        //        return rhs + *this;
    }

    Basis_ptr Basis::operator+ (const MonomialeBasis& rhs) const {
        return std::make_shared<Basis>();//        return plusstd::shared_ptr<Basis>(*this, rhs);
    }

    Basis_ptr Basis::operator+ (const BSplineBasis& rhs) const{
        return std::make_shared<Basis>();//        return plusstd::shared_ptr<Basis>(*this, rhs);
    }

} // namespace spline
