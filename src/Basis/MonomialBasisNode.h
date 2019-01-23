#ifndef CPP_SPLINE_MONOMIALBASISNODE_H_
#define CPP_SPLINE_MONOMIALBASISNODE_H_

#include <vector>
#include <any_tensor.hpp>

#include "Basis.h"
#include "BasisNode.h"

#include "UnivariateBasis.h"
#include "UnivariateBasisNode.h"

namespace spline{

    class MonomialBasis;

    class MonomialBasisNode : public UnivariateBasisNode {

    public:
        MonomialBasisNode(casadi_int degree);

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

        virtual casadi_int dimension() const override;

        template<class T>
        AnyTensor basis_evaluation(const std::vector< T >& x ) const ;

        virtual AnyTensor evaluation_grid() const override;

        virtual Basis derivative(casadi_int order, AnyTensor& T) const override;
        virtual Basis antiderivative(casadi_int order, AnyTensor& T) const override;
        virtual AnyTensor integral(const Interval& domain) const override;
        virtual Basis degree_elevation(casadi_int elevation, AnyTensor& T) const override;
    };

    template<class T>
    AnyTensor MonomialBasisNode::basis_evaluation(const std::vector< T >& x ) const {
        T x_ = x[0];
        casadi_int lenght  = this->dimension();
        std::vector<T> evaluation_basis(lenght);
        for (casadi_int i = 0; i < lenght; ++i) {
              evaluation_basis[i] = pow(x_,i);
        }
        return AnyTensor(vertcat(evaluation_basis));
    }
}  // namespace spline

#endif  // CPP_SPLINE_MONOMIALBASISNODE_H_
