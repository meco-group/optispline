#ifndef CPP_SPLINE_SUBBASISDUMMY
#define CPP_SPLINE_SUBBASISDUMMY

#include <casadi/casadi.hpp>
#include <string>

#include "Basis.h"

namespace spline {

#ifndef SWIG

    class MonomialBasis;
    class BSplineBasis;
    class DummyBasisNode : public BasisNode {
    public:
        virtual Basis operator+(const Basis& other) const ;
        virtual Basis operator+(const MonomialBasis& other) const ;
        virtual Basis operator+(const BSplineBasis& other) const ;

        virtual Basis operator*(const Basis& other) const ;
        virtual Basis operator*(const MonomialBasis& other) const ;
        virtual Basis operator*(const BSplineBasis& other) const ;

        virtual std::string getRepresentation() const ;
        virtual Basis derivative(int order, AnyTensor& T) const override ;
    };


#endif // SWIG
    class DummyBasis : public Basis {
    public:
#ifndef SWIG
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        DummyBasisNode* get() const ;
        DummyBasisNode* operator->() const ;
#endif // SWIG

        DummyBasis() ;
        virtual std::string getRepresentation() const ;
    };
} // namespace spline

#endif //CPP_SPLINE_SUBBASISDUMMY
