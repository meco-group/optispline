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
        virtual Basis operator+(const MonomialBasis& other) const ;
        virtual Basis operator+(const BSplineBasis& other) const ;
        virtual Basis operator+(const Basis& other) const ;
        virtual Basis operator+(const DummyBasis& other) const ;

        virtual Basis operator*(const MonomialBasis& other) const ;
        virtual Basis operator*(const BSplineBasis& other) const ;
        virtual Basis operator*(const Basis& other) const ;
        virtual Basis operator*(const DummyBasis& other) const ;

        UnivariateBasisNode(int degree);

        virtual AnyTensor const_coeff_tensor(const AnyTensor& t) const override ;

        virtual std::string getRepresentation() const override;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const {spline_assert(0); return DT();};

        int degree () const ;
        void setDegree (int degree);
        int n_inputs() const;

        virtual Basis derivative(int order, AnyTensor& T) const = 0;
        virtual Basis antiderivative(int order, AnyTensor& T) const = 0;

        virtual std::vector< std::vector < AnyScalar > > getEvaluationGrid() const {spline_assert(0); return std::vector< std::vector < AnyScalar > >(); };

        int dimension() const {return getLength();}
        virtual int getLength() const ;
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
        void setDegree (int degree);


    protected:
        int getLength() const ;
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
