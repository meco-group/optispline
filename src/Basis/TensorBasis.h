//
// Created by erik on 22/04/16.
//

#ifndef CPP_SPLINES_TENSORBASIS_H
#define CPP_SPLINES_TENSORBASIS_H

namespace spline{

#include <vector>
#include "Basis.h"

    class TensorBasis : public Basis{


        TensorBasis (vector< Basis > basis) : basis(basis) {}

        //    evalution Basis
        //TODO eval of double, useless in tensor
//        virtual std::vector<double> evalBasis(double x) const = 0;
        virtual std::vector< std::vector<double> > evalBasis(const std::vector<double>& x) const;
        virtual std::vector< std::vector< std::vector<double> > > evalBasis(const std::vector< std::vector<double> >& x) const;
        
        virtual TensorBasis plus(TensorBasis &other);

    private:
        vector< Basis > basis;

    };


    }

#endif //CPP_SPLINES_TENSORBASIS_H
