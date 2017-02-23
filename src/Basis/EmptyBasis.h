#ifndef CPP_SPLINE_SUBBASISDUMMY
#define CPP_SPLINE_SUBBASISDUMMY

#include <casadi/casadi.hpp>
#include <string>

#include "Basis.h"

namespace spline {

#ifndef SWIG
    class MonomialBasis;
    class BSplineBasis;
    class EmptyBasisNode : public BasisNode {
      public:
        virtual std::string type() const override {return "EmptyBasis";}

        virtual Basis operator+(const Basis& other) const override;
        virtual Basis operator+(const MonomialBasis& other) const override;
        virtual Basis operator+(const BSplineBasis& other) const override;

        virtual Basis operator*(const Basis& other) const override;
        virtual Basis operator*(const MonomialBasis& other) const override;
        virtual Basis operator*(const BSplineBasis& other) const override;

        virtual std::string getRepresentation() const override;
    };
#endif // SWIG

    class EmptyBasis : public Basis {
    public:
#ifndef SWIG
        EmptyBasisNode* get() const ;
        EmptyBasisNode* operator->() const ;
#endif // SWIG

        EmptyBasis() ;

    };
} // namespace spline

#endif //CPP_SPLINE_SUBBASISDUMMY
