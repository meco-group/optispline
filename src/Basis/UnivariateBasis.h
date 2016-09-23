#ifndef CPP_SPLINE_UNIVARIATEBASIS_H
#define CPP_SPLINE_UNIVARIATEBASIS_H

#include <casadi/casadi.hpp>
#include <string>

#include "Basis.h"
#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {

    class MonomialeBasis;
    class BSplineBasis;

#ifndef SWIG

    class UnivariatBasisNode : public BasisNode {
    public:
        //   casadi::DM transformation( const Basis &b) const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        UnivariatBasisNode(SubBasis subbasis);

        virtual std::string getRepresentation() const ;

        virtual int getLenght() const ;
        int getDegree () const ;
        void setDegree (int degree);
    protected:
    };

#endif // SWIG

    class UnivariatBasis : public Basis {
    public:
#ifndef SWIG
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        UnivariatBasis() { };
        UnivariatBasisNode* get() const ;
        UnivariatBasisNode* operator->() const ;
#endif // SWIG

        virtual std::string getRepresentation() const ;

        int getLenght() const ;
        int getDegree () const ;
        void setDegree (int degree);

#ifndef SWIG
        void assertUnivariatBasis() const;
        SubBasis getUnivariatSubBasis() const;
#endif
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
