#ifndef SUBMONOMIALBASIS_H_
#define SUBMONOMIALBASIS_H_

#include <any_tensor.hpp>
#include <vector>

#include "Basis.h"
#include "UnivariateBasis.h"

namespace spline{

#ifndef SWIG

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

        virtual std::vector< std::vector < AnyScalar > > getEvaluationGrid() const override;

    virtual AnyTensor evaluation_grid() const override;

        virtual Basis derivative(int order, AnyTensor& T) const override;
        virtual Basis antiderivative(int order, AnyTensor& T) const override;
        virtual AnyTensor integral(const Interval& domain) const override;
        virtual Basis degree_elevation(int elevation, AnyTensor& T) const override;
    };

#endif // SWIG

    class MonomialBasis : public UnivariateBasis {

    public:

#ifndef SWIG
        MonomialBasis(){}
        MonomialBasisNode* get() const ;
        MonomialBasisNode* operator->() const ;
#endif // SWIG
        MonomialBasis(int degree);

    };

    template<class T>
    AnyTensor MonomialBasisNode::basis_evaluation(const std::vector< T >& x ) const {
        T x_ = x[0];
        int lenght  = this->dimension();
        std::vector<T> evaluation_basis(lenght);
        for (int i = 0; i < lenght; ++i) {
              evaluation_basis[i] = pow(x_,i);
        }
        return AnyTensor(vertcat(evaluation_basis));
    }

}  // namespace spline

#endif  // SUBMONOMIALBASIS_H_
