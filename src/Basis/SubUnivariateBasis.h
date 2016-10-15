#ifndef CPP_SPLINE_SUBUNIVARIATEBASIS_H
#define CPP_SPLINE_SUBUNIVARIATEBASIS_H

#include <casadi/casadi.hpp>
#include <string>

#include "../common.h"
#include "SubBasis.h"

namespace spline {

    class SubMonomialBasis;
    class SubBSplineBasis;

#ifndef SWIG

    class SubUnivariateBasisNode : public SubBasisNode {
    public:
        virtual SubBasis operator+(const SubMonomialBasis& other) const ;
        virtual SubBasis operator+(const SubBSplineBasis& other) const ;
        virtual SubBasis operator+(const SubBasis& other) const ;
        virtual SubBasis operator+(const SubBasisDummy& other) const ;

        virtual SubBasis operator*(const SubMonomialBasis& other) const ;
        virtual SubBasis operator*(const SubBSplineBasis& other) const ;
        virtual SubBasis operator*(const SubBasis& other) const ;
        virtual SubBasis operator*(const SubBasisDummy& other) const ;

        SubUnivariateBasisNode(int degree);

        virtual std::string getRepresentation() const ;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const {spline_assert(0); return DT();};

        virtual int getLength() const ;
        int getDegree () const ;
        void setDegree (int degree);
        int getDimension() const;
        std::vector< int > getShape() const {
            std::cout << getLength() << std::endl;
            return std::vector< int > {getLength()};}

        virtual void getEvaluationGrid(std::vector< std::vector < AnyScalar > > * eg) const {spline_assert(0);};
    protected:
        int degree;
    };

#endif // SWIG

    class SubUnivariateBasis : public SubBasis {
    public:
#ifndef SWIG
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        SubUnivariateBasis() { };
        SubUnivariateBasisNode* get() const ;
        SubUnivariateBasisNode* operator->() const ;
#endif // SWIG

        virtual std::string getRepresentation() const ;

        int getLength() const ;
        int getDegree () const ;
        void setDegree (int degree);
    };
} // namespace spline

#endif //CPP_SPLINE_SUBUNIVARIATEBASIS_H
