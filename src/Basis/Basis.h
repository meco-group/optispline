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
    class Basis;
    class DummyBasis;
    class BSplineBasis;
    class MonomialBasis;

#ifndef SWIG

    class BasisNode : public SharedObjectNode {
    public:
        virtual Basis operator+(const Basis& rhs) const;
        virtual Basis operator+(const DummyBasis& rhs) const;
        virtual Basis operator+(const MonomialBasis& rhs) const;
        virtual Basis operator+(const BSplineBasis& rhs) const;
        virtual Basis operator*(const Basis& rhs) const;
        virtual Basis operator*(const DummyBasis& rhs) const;
        virtual Basis operator*(const MonomialBasis& rhs) const;
        virtual Basis operator*(const BSplineBasis& rhs) const;

        virtual std::string getRepresentation() const ;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;
        virtual int getDimension() const {return 0;};
        virtual std::vector< int > getShape() const {return std::vector< int > {};}

        template< class T >
            void assertVectorLenghtCorrect( const std::vector< T >& x) const;

        virtual void getEvaluationGrid(std::vector< std::vector < AnyScalar > > * eg) const;
    };

#endif // SWIG

    class Basis : public SharedObject{
    public:

#ifndef SWIG

        BasisNode* get() const ;
        BasisNode* operator->() const ;

#endif // SWIG
        // int getDimension () const;

        // std::vector<int> getSize () const;
        Basis ();

        virtual Basis operator+(const Basis& rhs) const;
        virtual Basis operator+(const DummyBasis& rhs) const;
        virtual Basis operator+(const MonomialBasis& rhs) const;
        virtual Basis operator+(const BSplineBasis& rhs) const;
        virtual Basis operator*(const Basis& rhs) const;
        virtual Basis operator*(const DummyBasis& rhs) const;
        virtual Basis operator*(const MonomialBasis& rhs) const;
        virtual Basis operator*(const BSplineBasis& rhs) const;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;
        int getDimension() const;
        std::vector< int > getShape() const ;

        virtual std::string getRepresentation() const ;
        virtual void getEvaluationGrid(std::vector< std::vector < AnyScalar > > * eg) const;
        
      
        virtual void foo() const {};
    };

    template< class T >
        void BasisNode::assertVectorLenghtCorrect( const std::vector< T >& x) const{
            assert(x.size() == getDimension());  // imput vector has wrong dimention
        }

}
#endif  // SUBBASIS_H_
