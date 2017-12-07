#ifndef CPP_SPLINE_EMPTYBASISNODE_H_
#define CPP_SPLINE_EMPTYBASISNODE_H_

#include <casadi/casadi.hpp>
#include <string>

#include "BasisNode.h"

#include "EmptyBasis.h"

namespace spline {

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
} // namespace spline

#endif //CPP_SPLINE_EMPTYBASISNODE_H_
