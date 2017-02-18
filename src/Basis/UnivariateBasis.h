#ifndef CPP_SPLINE_UNIVARIATEBASIS_H
#define CPP_SPLINE_UNIVARIATEBASIS_H

#include <casadi/casadi.hpp>
#include <string>

#include "../common.h"
#include "Basis.h"

namespace spline {

    class MonomialBasis;
    class BSplineBasis;

#ifndef SWIG

    class UnivariateBasisNode : public BasisNode {
    public:
        UnivariateBasisNode(int degree);

        virtual AnyTensor const_coeff_tensor(const AnyTensor& t) const = 0;
        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const = 0;

        int degree () const ;
        int n_inputs() const;

        virtual Basis derivative(int order, AnyTensor& T) const = 0;
        virtual Basis antiderivative(int order, AnyTensor& T) const = 0;

        virtual std::vector< std::vector < AnyScalar > > getEvaluationGrid() const = 0;

        int dimension() const {return length();}
        virtual int length() const = 0;
    protected:
        int degree_;
    };

#endif // SWIG

    class UnivariateBasis : public Basis {
    public:
#ifndef SWIG
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        UnivariateBasis() { };
        UnivariateBasisNode* get() const ;
        UnivariateBasisNode* operator->() const ;
#endif // SWIG

        int degree () const ;

    protected:
        int length() const ;
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
