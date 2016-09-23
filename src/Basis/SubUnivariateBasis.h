#ifndef CPP_SPLINE_SUBUNIVARIATEBASIS_H
#define CPP_SPLINE_SUBUNIVARIATEBASIS_H

#include <casadi/casadi.hpp>
#include <string>

#include "SubBasis.h"
#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {

    class SubMonomialeBasis;
    class SubBSplineBasis;

#ifndef SWIG

    class SubUnivariatBasisNode : public SubBasisNode {
    public:
        //   casadi::DM transformation( const Basis &b) const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        SubUnivariatBasisNode(int degree);

        virtual std::string getRepresentation() const ;

        virtual DT operator()(const std::vector< double >& x) const { assert(0);};
        virtual ST operator()(const std::vector< SX >& x) const { assert(0);};
        virtual MT operator()(const std::vector< MX >& x) const { assert(0);};

//TODO addBasis         
        virtual int getLenght() const ;
        int getDegree () const ;
        void setDegree (int degree);
    protected:
        int degree;
    };

#endif // SWIG

    class SubUnivariatBasis : public SubBasis {
    public:
#ifndef SWIG
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        SubUnivariatBasis() { };
        SubUnivariatBasisNode* get() const ;
        SubUnivariatBasisNode* operator->() const ;
#endif // SWIG
        virtual SubBasis operator+ (const SubBasis& other) const;
        virtual SubBasis operator+ (const SubMonomialeBasis& other) const;
        virtual SubBasis operator+ (const SubBSplineBasis& other) const;

        virtual SubBasis operator* (const SubBasis& other) const;
        virtual SubBasis operator* (const SubMonomialeBasis& other) const;
        virtual SubBasis operator* (const SubBSplineBasis& other) const;

        virtual std::string getRepresentation() const ;

        int getLenght() const ;
        int getDegree () const ;
        void setDegree (int degree);
    };
} // namespace spline

#endif //CPP_SPLINE_SUBUNIVARIATEBASIS_H
