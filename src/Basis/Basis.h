#ifndef SUBBASIS_H_
#define SUBBASIS_H_

#include <iostream>
#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"
#include "../common.h"

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

        template< class T >
            void assert_vector_lenght_correct( const std::vector< T >& x) const;

        virtual std::vector< std::vector < AnyScalar > > getEvaluationGrid() const;

        virtual AnyTensor const_coeff_tensor(const AnyTensor& t) const ;

        virtual int dimension() const {return 0;};  // Number of basis functions in the basis
        virtual int n_inputs() const {return 0;};  // Number of inputs of the basis

        // virtual Basis derivative(int order, int direction) const;
        virtual Basis insert_knots(const std::vector<AnyScalar> & new_knots, AnyTensor & T) const;
    };

#endif // SWIG

    class Basis : public SharedObject{
    public:

#ifndef SWIG

        BasisNode* get() const ;
        BasisNode* operator->() const ;

#endif // SWIG
        // std::vector<int> getSize() const;
        Basis();

        virtual Basis operator+(const Basis& rhs) const;
        virtual Basis operator+(const DummyBasis& rhs) const;
        virtual Basis operator+(const MonomialBasis& rhs) const;
        virtual Basis operator+(const BSplineBasis& rhs) const;
        virtual Basis operator*(const Basis& rhs) const;
        virtual Basis operator*(const DummyBasis& rhs) const;
        virtual Basis operator*(const MonomialBasis& rhs) const;
        virtual Basis operator*(const BSplineBasis& rhs) const;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        virtual std::string getRepresentation() const ;
        virtual std::vector< std::vector < AnyScalar > > getEvaluationGrid() const;

        Basis insert_knots(const std::vector<AnyScalar> & new_knots, AnyTensor & SWIG_OUTPUT(T)) const;
        int dimension() const;
        int n_inputs() const;

        // virtual Basis derivative(int order, int direction) const;
        AnyTensor const_coeff_tensor(const AnyTensor& t) const ;
    };

    template< class T >
        void BasisNode::assert_vector_lenght_correct( const std::vector< T >& x) const{
            spline_assert_message(x.size() == n_inputs(), "imput vector has wrong dimention");  // imput vector has wrong dimention
        }

}  // namespace spline
#endif  // SUBBASIS_H_
