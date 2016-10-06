#ifndef BASIS_H_
#define BASIS_H_

#include <iostream>
#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "SubBasis.h"
#include "../Function/Argument.h"
// #include <tensor.hpp>
#include <any_tensor.hpp>

namespace spline {
    class Basis;
    class BSplineBasis;
    class MonomialBasis;

#ifndef SWIG

    class BasisNode : public SharedObjectNode {
    public:
        BasisNode (const std::vector< Basis >& allBasis);
        BasisNode (const std::vector< SubBasis >& allSubBasis);
        int getDimension () const;

        std::vector<int> getSize () const;

        void setArguments (const std::vector< Argument >& argument);
        std::vector< Argument > getArguments() const;

        Argument getSubArgument( int index ) const;
        int indexArgument(Argument a);

        bool hasArguments() const;
        bool hasValidNumberOfArguments() const;

        virtual std::string getRepresentation() const ;

        virtual std::vector< SubBasis > getSubBasis() const;
        virtual Basis getSubBasis( int index ) const;

        void addBasis(Basis basis);
        void addBasis(SubBasis basis);

        Basis operator+(const Basis& rhs) const;
        // Basis operator*(const Basis& rhs) const;

        AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        virtual BSplineBasis castBSpline() const;
    // protected:
        std::vector< SubBasis > allSubBasis;
        std::vector< Argument > allArguments;
    };

#endif // SWIG

    class Basis : public SharedObject{
    public:

#ifndef SWIG

        BasisNode* get() const ;
        BasisNode* operator->() const ;

#endif // SWIG
        int getDimension () const;

        std::vector<int> getSize () const;
        Basis ();
        Basis (const std::vector< SubBasis >& allSubBasis);
        Basis (const std::vector< Basis >& allBasis);

        void setArguments (const std::vector< spline::Argument >& argument);
        std::vector< spline::Argument > getArguments() const;

        spline::Argument getSubArgument( int index ) const;
        int indexArgument(spline::Argument a);

        bool hasArguments() const;
        bool hasValidNumberOfArguments() const;

        virtual std::string getRepresentation() const ;

        std::vector< SubBasis > getSubBasis() const;
        Basis getSubBasis( int index ) const;


        void addBasis(Basis basis);
        void addBasis(SubBasis basis);

        Basis operator+(const Basis& rhs) const;
        // Basis operator*(const Basis& rhs) const;

        AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        virtual BSplineBasis castBSpline() const;
    };
}
#endif  // BASIS_H_
