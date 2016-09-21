#ifndef SUBBASIS_H_
#define SUBBASIS_H_

#include <iostream>
#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include <tensor.hpp>
//#include "../Math/Domain.h"

namespace spline {
    class SubBasis;
    class SubBSplineBasis;
    class SubMonomialeBasis;

#ifndef SWIG

    class SubBasisNode : public SharedObjectNode {
    public:
        virtual std::string getRepresentation() const ;

        virtual DT operator()(const std::vector< double >& x) const;
        virtual ST operator()(const std::vector< SX >& x) const;
        virtual MT operator()(const std::vector< MX >& x) const;

    };

#endif // SWIG

    class SubBasis : public SharedObject{
    public:

#ifndef SWIG

        SubBasisNode* get() const ;
        SubBasisNode* operator->() const ;

#endif // SWIG
        // int getDimension () const;

        // std::vector<int> getSize () const;
        SubBasis ();

        virtual SubBasis operator+(const SubBasis& rhs) const;
        virtual SubBasis operator*(const SubBasis& rhs) const;
        virtual std::string getRepresentation() const ;

        DT operator()(const std::vector< double >& x) const;
        ST operator()(const std::vector< SX >& x) const;
        MT operator()(const std::vector< MX >& x) const;

    };
}
#endif  // SUBBASIS_H_
