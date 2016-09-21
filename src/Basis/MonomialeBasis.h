#ifndef MONOMIALBASIS_H_
#define MONOMIALBASIS_H_

#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "Basis.h"
#include "UnivariateBasis.h"

#include "utils/CommonBasis.h"
namespace spline{

    class BSplineBasis;

#ifndef SWIG

    class MonomialeBasisNode : public UnivariateBasisNode {

    public:
        MonomialeBasisNode(int degree) : UnivariateBasisNode(degree) {};

        virtual std::string getRepresentation() const ;
        //    virtual std::vector<double> evaluationGrid(void) const;


        virtual int getLenght() const ;
    template< class T >
    AnyTensor BasisEvalution (const std::vector< T >& x ) const ;
    };

#endif // SWIG

    class MonomialeBasis : public UnivariateBasis {

    public:

#ifndef SWIG
        MonomialeBasis(){}
        MonomialeBasisNode* get() const ;
        MonomialeBasisNode* operator->() const ;
#endif // SWIG
        MonomialeBasis(int degree);


        virtual std::string getRepresentation() const ;
        //   Basis operator*(const MonomialeBasis& other)const ;
        //   Basis operator*(const BSplineBasis& other)const ;

        //    virtual std::vector<double> evaluationGrid(void) const;

#ifndef SWIG
        /// Print a representation of the object to a stream (shorthand)
        inline friend
            std::ostream& operator<<(std::ostream &stream, const MonomialeBasis& obj) {
                return stream << obj.getRepresentation();
            }
#endif // SWIG
    };
    
    template< class T >
    AnyTensor MonomialeBasisNode::BasisEvalution (const std::vector< T >& x ) const {
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


#endif  // MONOMIALBASIS_H_
