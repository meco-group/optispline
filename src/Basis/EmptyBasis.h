#ifndef CPP_SPLINE_SUBBASISDUMMY
#define CPP_SPLINE_SUBBASISDUMMY

#include <casadi/casadi.hpp>
#include <string>

#include "Basis.h"

namespace spline {

#ifndef SWIG

    class EmptyBasisNode : public BasisNode {
      public:
        virtual std::string type() const override {return "EmptyBasis";}
        virtual std::string to_string() const override;

        virtual Basis operator+(const BasisNode& other) const override;
        virtual Basis operator+(const EmptyBasisNode& other) const override;
        virtual Basis operator+(const MonomialBasisNode& other) const override;
        virtual Basis operator+(const BSplineBasisNode& other) const override;

        virtual Basis operator*(const BasisNode& other) const override;
        virtual Basis operator*(const EmptyBasisNode& other) const override;
        virtual Basis operator*(const MonomialBasisNode& other) const override;
        virtual Basis operator*(const BSplineBasisNode& other) const override;

        virtual bool operator==(const MonomialBasisNode& other) const override;
        virtual bool operator==(const BSplineBasisNode& other) const override;
        virtual bool operator==(const BasisNode& other) const override;
        virtual bool operator==(const EmptyBasisNode& other) const override;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const override;
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
