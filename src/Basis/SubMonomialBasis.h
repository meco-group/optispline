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

    class SubMonomialBasisNode : public SubUnivariateBasisNode {

    public:
        SubMonomialBasisNode(int degree) : SubUnivariateBasisNode(degree) {};

        virtual SubBasis operator+(const SubMonomialBasis& other) const ;
        virtual SubBasis operator+(const SubBSplineBasis& other) const ;
        virtual SubBasis operator+(const SubBasis& other) const ;
        virtual SubBasis operator+(const SubBasisDummy& other) const ;

        virtual std::string getRepresentation() const ;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        virtual int getLength() const ;
        template<class T>
        AnyTensor SubBasisEvalution (const std::vector< T >& x ) const ;

        virtual std::vector< std::vector < double > > getEvaluationGrid() const;
    };

#endif // SWIG

    class SubMonomialBasis : public SubUnivariateBasis {

    public:

#ifndef SWIG
        SubMonomialBasis(){}
        SubMonomialBasisNode* get() const ;
        SubMonomialBasisNode* operator->() const ;
#endif // SWIG
        SubMonomialBasis(int degree);

        virtual std::string getRepresentation() const ;

#ifndef SWIG
        /// Print a representation of the object to a stream (shorthand)
        inline friend
            std::ostream& operator<<(std::ostream &stream, const SubMonomialBasis& obj) {
                return stream << obj.getRepresentation();
            }
#endif // SWIG
    };
    
    template<class T>
    AnyTensor SubMonomialBasisNode::SubBasisEvalution (const std::vector< T >& x ) const {
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
