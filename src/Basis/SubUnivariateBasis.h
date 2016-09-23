#ifndef CPP_SPLINE_SUBUNIVARIATEBASIS_H
#define CPP_SPLINE_SUBUNIVARIATEBASIS_H

#include <casadi/casadi.hpp>
#include <string>

#include "SubBasis.h"
#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {

    class SubMonomialBasis;
    class SubBSplineBasis;

#ifndef SWIG

    class SubUnivariateBasisNode : public SubBasisNode {
    public:
        //   casadi::DM transformation( const Basis &b) const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        SubUnivariateBasisNode(int degree);

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

    class SubUnivariateBasis : public SubBasis {
    public:
#ifndef SWIG
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        SubUnivariateBasis() { };
        SubUnivariateBasisNode* get() const ;
        SubUnivariateBasisNode* operator->() const ;
#endif // SWIG
        virtual SubBasis operator+ (const SubBasis& other) const;
        virtual SubBasis operator+ (const SubMonomialBasis& other) const;
        virtual SubBasis operator+ (const SubBSplineBasis& other) const;

        virtual SubBasis operator* (const SubBasis& other) const;
        virtual SubBasis operator* (const SubMonomialBasis& other) const;
        virtual SubBasis operator* (const SubBSplineBasis& other) const;

        virtual std::string getRepresentation() const ;

        int getLenght() const ;
        int getDegree () const ;
        void setDegree (int degree);
    };
} // namespace spline

#endif //CPP_SPLINE_SUBUNIVARIATEBASIS_H
