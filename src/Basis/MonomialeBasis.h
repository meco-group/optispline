#ifndef MONOMIALBASIS_H_
#define MONOMIALBASIS_H_

#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "Basis.h"
#include "UnivariateBasis.h"

namespace spline{

    class BSplineBasis;

#ifndef SWIG

    class MonomialeBasisNode : public UnivariateBasisNode {

    public:
        MonomialeBasisNode(){}
        // MonomialeBasisNode(int degree) : UnivariateBasisNode(degree) {};

        virtual std::string getRepresentation() const ;
        //   Basis operator*(const MonomialeBasis& other)const ;
        //   Basis operator*(const BSplineBasis& other)const ;

        //    virtual std::vector<double> evaluationGrid(void) const;

        virtual DT operator()(const std::vector< double >& x) const;
        virtual ST operator()(const std::vector< SX >& x) const ;
        virtual MT operator()(const std::vector< MX >& x) const ;

        virtual int getLenght() const ;
    };

#endif // SWIG

    class MonomialeBasis : public UnivariateBasis {

    public:

#ifndef SWIG
        MonomialeBasisNode* get() const ;
        MonomialeBasisNode* operator->() const ;
#endif // SWIG
        MonomialeBasis(int degree);

        virtual Basis operator+(const MonomialeBasis& other) const ;
        virtual Basis operator+(const BSplineBasis& other) const ;
        virtual Basis operator+(const Basis& other) const ;

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
}  // namespace spline


#endif  // MONOMIALBASIS_H_
