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

    // TODO(ErikLambre) this is wrong
    std::vector< std::vector<double> > TensorBasis::evalBasis (const std::vector<double>& x) const {
        std::vector< std::vector<double> > r;
        for (int i = 0; i < getDimensions(); ++i) {
            r.push_back((basis[i])->evalBasis(x[i]));
        }
        return r;
    }



    // TODO(ErikLambre) make it work
    std::vector< std::vector< std::vector<double> > > TensorBasis::evalBasis (const std::vector< std::vector<double> >& x) const {
        std::vector< std::vector< std::vector<double> > > r;
        for (int i = 0; i < x.size(); ++i) {
		r.push_back(TensorBasis::evalBasis(x[i]));
        }
        return r;
    }

int TensorBasis::getDimensions () const {
    return basis.size();
}


} // namespace spline

