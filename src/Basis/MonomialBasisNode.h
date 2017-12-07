#ifndef CPP_SPLINE_MONOMIALBASISNODE_H_
#define CPP_SPLINE_MONOMIALBASISNODE_H_

#include <any_tensor.hpp>
#include <vector>

#include "Basis.h"
#include "UnivariateBasis.h"

#include "MonomialBasisNode.h"

namespace spline{

    class MonomialBasisNode : public UnivariateBasisNode {

    public:
        MonomialBasisNode(int degree);

        virtual std::string type() const override {return "MonomialBasis";}
        virtual std::string to_string() const override ;

        virtual Basis operator+(const MonomialBasisNode& other) const override;
        virtual Basis operator+(const BSplineBasisNode& other) const override;
        virtual Basis operator+(const BasisNode& other) const override;
        virtual Basis operator+(const EmptyBasisNode& other) const override;

        virtual Basis operator*(const MonomialBasisNode& other) const override;
        virtual Basis operator*(const BSplineBasisNode& other) const override;
        virtual Basis operator*(const BasisNode& other) const override;
        virtual Basis operator*(const EmptyBasisNode& other) const override;

        virtual bool operator==(const MonomialBasisNode& other) const override;
        virtual bool operator==(const BSplineBasisNode& other) const override;
        virtual bool operator==(const BasisNode& other) const override;
        virtual bool operator==(const EmptyBasisNode& other) const override;

        virtual AnyTensor const_coeff_tensor(const AnyTensor& t) const override ;
        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const override;

        virtual int dimension() const override;

        template<class T>
        AnyTensor basis_evaluation(const std::vector< T >& x ) const ;

    virtual AnyTensor evaluation_grid() const override;

        virtual Basis derivative(int order, AnyTensor& T) const override;
        virtual Basis antiderivative(int order, AnyTensor& T) const override;
        virtual AnyTensor integral(const Interval& domain) const override;
        virtual Basis degree_elevation(int elevation, AnyTensor& T) const override;
    };

}  // namespace spline

#endif  // CPP_SPLINE_MONOMIALBASISNODE_H_
