// Copyright 2016 Erik
//
// Created by erik on 08/04/16.
//

#include "Basis.h"
#include "plus.h"

namespace spline {

    Basis::Basis(){
        std::vector< Basis_ptr > allSubBasis;
    }

    //Basis::Basis(std::vector<  Basis > allSubBasis) : allSubBasis(allSubBasis){}
    
//    std::shared_ptr<Basis>::std::shared_ptr<Basis> (int dimension, const std::vector<int> &size, const Argument &argument) : dimension(dimension),
 //                                                                                          size(size),
//                                                                                           argument(argument) { }

    int Basis::getDimension () const {
        return allSubBasis.size();
    }

//    void Basis::addArgument (const Argument &argument) {
//        this->argument = argument;
//    }
//
//    void Basis::addArgument (const std::string& argument) {
//        this->argument = Argument(argument);
//    }

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
   std::vector<int> Basis::getSize () const {
       std::vector<int> size;
       std::vector<int> sizeSubBasis;
       for(int i = 0; i < getDimension(); i++){
           sizeSubBasis = allSubBasis[i]->getSize(); 
           for(int j = 0; j < sizeSubBasis.size(); j++){
                size.push_back(sizeSubBasis[j]);
           }
       }
       return size;
   }
    
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
