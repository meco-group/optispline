#ifndef SUBBASIS_H_
#define SUBBASIS_H_

#include <iostream>
#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

// #include <tensor.hpp>
#include <any_tensor.hpp>

namespace spline {
    class SubBasis;
    class SubBasisDummy;
    class SubBSplineBasis;
    class SubMonomialBasis;

#ifndef SWIG

    class SubBasisNode : public SharedObjectNode {
    public:
        virtual SubBasis operator+(const SubBasis& rhs) const;
        virtual SubBasis operator+(const SubBasisDummy& rhs) const;
        virtual SubBasis operator+(const SubMonomialBasis& rhs) const;
        virtual SubBasis operator+(const SubBSplineBasis& rhs) const;

        virtual std::string getRepresentation() const ;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;
        virtual int getDimension() const {return 0;};

        template< class T >
            void assertVectorLenghtCorrect( const std::vector< T >& x) const;

        virtual std::vector< std::vector < AnyScalar > > getEvaluationGrid() const;
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
        virtual SubBasis operator+(const SubBasisDummy& rhs) const;
        virtual SubBasis operator+(const SubMonomialBasis& rhs) const;
        virtual SubBasis operator+(const SubBSplineBasis& rhs) const;
        // virtual SubBasis operator*(const SubBasis& rhs) const;
        // virtual SubBasis operator*(const SubMonomialBasis& rhs) const;
        // virtual SubBasis operator*(const SubBSplineBasis& rhs) const;
        virtual std::string getRepresentation() const ;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;
        int getDimension() const;

        virtual std::vector< std::vector < AnyScalar > > getEvaluationGrid() const;
    };




        template< class T >
        void SubBasisNode::assertVectorLenghtCorrect( const std::vector< T >& x) const{
            assert(x.size() == getDimension());  // imput vector has wrong dimention
        }

}
#endif  // SUBBASIS_H_
