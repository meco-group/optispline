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

    class UnivariateBasisNode : public BasisNode {
    public:
        //   casadi::DM transformation( const Basis &b) const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        UnivariateBasisNode(int degree);
        UnivariateBasisNode(int degree, Argument argument);
        virtual Basis operator+ (const Basis& other) const;
        virtual Basis operator+ (const MonomialeBasis& other) const;
        virtual Basis operator+ (const BSplineBasis& other) const;
        virtual std::string getRepresentation() const ;

        virtual std::vector< Basis > getSubBasis() const;
        virtual DT operator()(const std::vector< double >& x) const { assert(0);};
        virtual ST operator()(const std::vector< SX >& x) const { assert(0);};
        virtual MT operator()(const std::vector< MX >& x) const { assert(0);};

//TODO addBasis         
        virtual void addBasis(Basis basis){}
        virtual int getLenght() const ;
        int getDegree () const ;
        void setDegree (int degree);
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
        virtual Basis operator+ (const Basis& other) const;
        virtual Basis operator+ (const MonomialeBasis& other) const;
        virtual Basis operator+ (const BSplineBasis& other) const;

        virtual std::string getRepresentation() const ;

        int getLenght() const ;
        int getDegree () const ;
        void setDegree (int degree);
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
