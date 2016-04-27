//
// Created by erik on 22/04/16.
//

#ifndef CPP_SPLINES_TENSORBASIS_H
#define CPP_SPLINES_TENSORBASIS_H

#include <vector>
#include <string>
#include "Basis.h"

namespace spline{

    class TensorBasis : public Basis{

    public:
        TensorBasis (std::vector< Basis* > basis, std::string argument = std::string("__"));
        TensorBasis (std::vector< Basis* > basis, std::vector< std::string > argument);

        virtual std::vector<double> evalBasis (double x) const;
        virtual std::vector< std::vector<double> > evalBasis (const std::vector<double>& x) const;
        virtual std::vector< std::vector< std::vector<double> > > evalBasis (const std::vector< std::vector<double> >& x) const;

        int length() const {return 0;}
//        virtual TensorBasis plus(TensorBasis &other);

        int getDimensions () const;

    private:
        std::vector< Basis*> basis;

    }; //  TensorBasis
} // namespace splien

#endif //CPP_SPLINES_TENSORBASIS_H
