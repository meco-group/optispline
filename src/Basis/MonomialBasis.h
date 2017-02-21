#ifndef SUBMONOMIALBASIS_H_
#define SUBMONOMIALBASIS_H_

#include <any_tensor.hpp>
#include <vector>

#include "Basis.h"
#include "UnivariateBasis.h"

namespace spline{

    class BSplineBasis;

#ifndef SWIG

    class MonomialBasisNode : public UnivariateBasisNode {

    public:
        MonomialBasisNode(int degree) : UnivariateBasisNode(degree) {};

        virtual std::string type() const override {return "MonomialBasis";}

        virtual Basis operator+(const MonomialBasis& other) const override;
        virtual Basis operator+(const BSplineBasis& other) const override;
        virtual Basis operator+(const Basis& other) const override;
        virtual Basis operator+(const EmptyBasis& other) const override;

        virtual Basis operator*(const MonomialBasis& other) const override;
        virtual Basis operator*(const BSplineBasis& other) const override;
        virtual Basis operator*(const Basis& other) const override;
        virtual Basis operator*(const EmptyBasis& other) const override;

        virtual std::string getRepresentation() const override ;

        virtual AnyTensor const_coeff_tensor(const AnyTensor& t) const override ;
        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const override;

        virtual int length() const override;
        template<class T>
        AnyTensor SubBasisEvalution(const std::vector< T >& x ) const ;

        virtual std::vector< std::vector < AnyScalar > > getEvaluationGrid() const override;

        virtual Basis derivative(int order, AnyTensor& T) const override;
        virtual Basis antiderivative(int order, AnyTensor& T) const override;
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
    AnyTensor MonomialBasisNode::SubBasisEvalution(const std::vector< T >& x ) const {
        T x_ = x[0];
        int lenght  = this->length();
        std::vector<T> evaluation_basis(lenght);
        for (int i = 0; i < lenght; ++i) {
              evaluation_basis[i] = pow(x_,i);
        }
        return AnyTensor(vertcat(evaluation_basis));
    }

}  // namespace spline

#endif  // SUBMONOMIALBASIS_H_
