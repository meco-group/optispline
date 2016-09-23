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
        MonomialeBasisNode(int degree) ;

        virtual std::string getRepresentation() const ;
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

#ifndef SWIG
        /// Print a representation of the object to a stream (shorthand)
        inline friend
            std::ostream& operator<<(std::ostream &stream, const MonomialeBasis& obj) {
                return stream << obj.getRepresentation();
            }
#endif // SWIG
    };
    
}  // namespace spline

#endif  // MONOMIALBASIS_H_
