#ifndef CPP_SPLINE_UNIVARIATEBASIS_H
#define CPP_SPLINE_UNIVARIATEBASIS_H

#include <casadi/casadi.hpp>
#include <string>

#include "Basis.h"
#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {

    class MonomialBasis;
    class BSplineBasis;

#ifndef SWIG

    class UnivariateBasisNode : public BasisNode {
    public:
        //   casadi::DM transformation( const Basis &b) const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        UnivariateBasisNode(SubBasis subbasis);

        virtual std::string getRepresentation() const ;

        virtual int getLength() const ;
        int getDegree () const ;
        void setDegree (int degree);
    protected:
#ifndef SWIG
        void assertUnivariateBasis() const;
#endif
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

#ifndef SWIG
        void assertUnivariateBasis() const;
        SubBasis getSubUnivariateBasis() const;
#endif
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
