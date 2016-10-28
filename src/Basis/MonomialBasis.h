#ifndef SUBMONOMIALBASIS_H_
#define SUBMONOMIALBASIS_H_

#include <vector>

#include "Basis.h"
#include "UnivariateBasis.h"

#include "utils/CommonBasis.h"

namespace spline{

    class BSplineBasis;

#ifndef SWIG

    class MonomialBasisNode : public UnivariateBasisNode {

    public:
        MonomialBasisNode(int degree) : UnivariateBasisNode(degree) {};

        virtual Basis operator+(const MonomialBasis& other) const ;
        virtual Basis operator+(const BSplineBasis& other) const ;
        virtual Basis operator+(const Basis& other) const ;
        virtual Basis operator+(const DummyBasis& other) const ;

        virtual Basis operator*(const MonomialBasis& other) const ;
        virtual Basis operator*(const BSplineBasis& other) const ;
        virtual Basis operator*(const Basis& other) const ;
        virtual Basis operator*(const DummyBasis& other) const ;

        virtual std::string getRepresentation() const ;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        virtual int getLength() const ;
        template<class T>
        AnyTensor SubBasisEvalution (const std::vector< T >& x ) const ;

        virtual void getEvaluationGrid(std::vector< std::vector < AnyScalar > > * eg) const;
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

        virtual std::string getRepresentation() const ;

        virtual void foo() const {};
        
#ifndef SWIG
        /// Print a representation of the object to a stream (shorthand)
        inline friend
            std::ostream& operator<<(std::ostream &stream, const MonomialBasis& obj) {
                return stream << obj.getRepresentation();
            }
#endif // SWIG
    };

    template<class T>
    AnyTensor MonomialBasisNode::SubBasisEvalution (const std::vector< T >& x ) const {
        T x_ = x[0];
        int lenght  = this->getLength();
        std::vector<T> evaluation_basis(lenght);
        for (int i = 0; i < lenght; ++i) {
              evaluation_basis[i] = pow(x_,i);
        }
        return AnyTensor(vertcat(evaluation_basis));
    }

}  // namespace spline

#endif  // SUBMONOMIALBASIS_H_
