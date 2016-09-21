#ifndef SUBMONOMIALBASIS_H_
#define SUBMONOMIALBASIS_H_

#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "SubBasis.h"
#include "SubUnivariateBasis.h"

#include "utils/CommonBasis.h"
namespace spline{

    class SubBSplineBasis;

#ifndef SWIG

    class SubMonomialeBasisNode : public SubUnivariateBasisNode {

    public:
        SubMonomialeBasisNode(int degree) : SubUnivariateBasisNode(degree) {};

        virtual std::string getRepresentation() const ;
        //    virtual std::vector<double> evaluationGrid(void) const;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;
        virtual ST operator()(const std::vector< SX >& x) const ;
        virtual MT operator()(const std::vector< MX >& x) const ;

        virtual SubBasis operator+(const SubBasis& other) const ;
        virtual SubBasis operator*(const SubBasis& other) const ;
        virtual int getLenght() const ;
    template< class T >
    AnyTensor SubBasisEvalution (const std::vector< T >& x ) const ;
    };

#endif // SWIG

    class SubMonomialeBasis : public SubUnivariateBasis {

    public:

#ifndef SWIG
        SubMonomialeBasis(){}
        SubMonomialeBasisNode* get() const ;
        SubMonomialeBasisNode* operator->() const ;
#endif // SWIG
        SubMonomialeBasis(int degree);

        virtual SubBasis operator+(const SubMonomialeBasis& other) const ;
        virtual SubBasis operator+(const SubBSplineBasis& other) const ;
        virtual SubBasis operator+(const SubBasis& other) const ;

        virtual SubBasis operator*(const SubMonomialeBasis& other) const ;
        virtual SubBasis operator*(const SubBSplineBasis& other) const ;
        virtual SubBasis operator*(const SubBasis& other) const ;

        virtual std::string getRepresentation() const ;
        //   Basis operator*(const MonomialeBasis& other)const ;
        //   Basis operator*(const BSplineBasis& other)const ;

        //    virtual std::vector<double> evaluationGrid(void) const;

#ifndef SWIG
        /// Print a representation of the object to a stream (shorthand)
        inline friend
            std::ostream& operator<<(std::ostream &stream, const SubMonomialeBasis& obj) {
                return stream << obj.getRepresentation();
            }
#endif // SWIG
    };
    
    template< class T >
    AnyTensor SubMonomialeBasisNode::SubBasisEvalution (const std::vector< T >& x ) const {
        assert(x.size()==1);
        T x_ = x[0];
        int lenght  = this->getLenght();
        std::vector<T> evaluation_basis(lenght);
        for (int i = 0; i < lenght; ++i) {
              evaluation_basis[i] = pow(x_,i);
        }
        return AnyTensor(evaluation_basis,{lenght});
    }

}  // namespace spline


#endif  // SUBMONOMIALBASIS_H_
