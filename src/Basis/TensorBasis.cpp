//
// Created by erik on 22/04/16.
//

#include "TensorBasis.h"

namespace spline{

    TensorBasis::TensorBasis (std::vector <Basis*> basis, std::string argument){
        std::vector<std::string> argumentList = {argument};
        TensorBasis(basis,argumentList);
    }

    TensorBasis::TensorBasis (std::vector <Basis*> basis, std::vector <std::string> argument) : basis(basis) {
        setArguments(argument);
    }

    std::vector<double> TensorBasis::evalBasis (double x) const {
        return std::vector<double>();
    }

    std::vector< std::vector<double> > TensorBasis::evalBasis (const std::vector<double>& x) const {
        std::vector< std::vector<double> > r;
        for (int i = 0; i < getDimensions(); ++i) {
            r.push_back((basis[i])->evalBasis(x[i]));
        }
        return r;
    }

    int TensorBasis::getDimensions () const {
        return basis.size();
    }


    // TODO(ErikLambre) make it work
    std::vector< std::vector< std::vector<double> > > TensorBasis::evalBasis (const std::vector< std::vector<double> >& x) const {

        std::vector< std::vector< std::vector<double> > > r;
//        std::vector< std::vector<double> > r_;
        for (int i = 0; i < x.size(); ++i) {
//            r_.clear();
//            for (int j = 0; j < TensorBasis::getDimensions(); ++j) {
//                r_.push_back((basis[j]).evalBasis(x[i][j]));
//            }


            r.push_back(TensorBasis::evalBasis(x[i]));
//            r.push_back(r_);
        }
        return r;



//
//
//        std::vector< std::vector< std::vector<double> > > r;
//        std::vector< std::vector<double> > r_;
//        std::vector< std::vector<double> > r__;
//        for (int i = 0; i < x.size(); ++i) {
//
////            r_.resize(basis[j].getDimensions(), basis[j].getLenght());
//            r_.clear();
//            for (int j = 0; j < getDimensions(); ++j) {
//                for (int k = 0; k < basis[j].getLenght(); ++k) {
//
//                }
//                r_.push_back(basis[j].evalBasis(x[i]));
//            }
//            r.push_back(r_);
//        }
//        return r;
    }
} // namespace spline
