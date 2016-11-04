#ifndef CPP_SPLINE_SUBUNIVARIATEBASIS_H
#define CPP_SPLINE_SUBUNIVARIATEBASIS_H

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

        virtual std::string getRepresentation() const ;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const {spline_assert(0); return DT();};

        virtual int getLength() const ;
        int getDegree () const ;
        void setDegree (int degree);
        int getDimension() const;
        std::vector< int > getShape() const { return {getLength()};}

        virtual void getEvaluationGrid(std::vector< std::vector < AnyScalar > > * eg) const {spline_assert(0);};
    protected:
        int degree;
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

        virtual std::string getRepresentation() const ;


        int getLength() const ;
        int getDegree () const ;
        void setDegree (int degree);
    };
} // namespace spline

#endif //CPP_SPLINE_SUBUNIVARIATEBASIS_H
