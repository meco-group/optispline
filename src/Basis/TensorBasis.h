#ifndef TENSORBASIS_H_
#define TENSORBASIS_H_

#include <iostream>
#include <string>
#include <vector>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "Basis.h"
#include "../Function/Argument.h"
#include "../Function/Index.h"

namespace spline {
    class TensorBasis;
    class BSplineBasis;
    class MonomialBasis;

#ifndef SWIG

    class TensorBasisNode : public SharedObjectNode {
    public:
        TensorBasisNode(const std::vector< Basis >& allSubBasis);
        TensorBasisNode(const std::vector< Basis >& allSubBasis, const std::vector< Argument >& args);

        int n_basis() const;
        std::vector<int> dimension() const;
        int n_inputs() const;

        void setArguments(const std::vector< Argument >& argument);
        std::vector< Argument > getArguments() const;

        Argument getSubArgument(int index) const;
        int indexArgument(Argument a) const;

        bool hasArguments() const;

        virtual std::string getRepresentation() const ;

        std::vector< Basis > getSubBasis() const;
        TensorBasis getSubBasis(int index) const;
        // Basis getBasis( int index ) const;
        // Basis getSubBasis( Argument a) const;

        TensorBasis addBasis(TensorBasis basis) const ;
        TensorBasis addBasis(Basis basis) const ;

        TensorBasis operator+(const TensorBasis& rhs) const;
        TensorBasis operator*(const TensorBasis& rhs) const;

        AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        int totalNumberBasisFunctions() const;

        // Basis derivative(int order, int direction, AnyTensor& T) const;

    // protected:
        std::vector< Basis > allSubBasis;
        std::vector< Argument > allArguments;
    };

#endif // SWIG

    class TensorBasis : public SharedObject{
    public:

#ifndef SWIG

        TensorBasisNode* get() const ;
        TensorBasisNode* operator->() const ;

#endif // SWIG
        TensorBasis();
        TensorBasis(const Basis& allSubBasis);
        TensorBasis(const std::vector< Basis >& allSubBasis);
        TensorBasis(const std::vector< TensorBasis >& allBasis);
        TensorBasis(const Basis& allSubBasis, const std::vector< Argument > args);
        TensorBasis(const std::vector< Basis >& allSubBasis, const std::vector< Argument > args);
        TensorBasis(const std::vector< TensorBasis >& allBasis, const std::vector< Argument > args);

        int n_basis() const;  // Number of bases, building up the TensorBasis
        int n_inputs() const; // Total number of inputs, over all bases
        std::vector<int> dimension() const; // Vector containing number of basis functions in each basis

        TensorBasis setArguments(const std::vector< spline::Argument >& argument) const;
        std::vector< spline::Argument > getArguments() const;

        spline::Argument getSubArgument(int index) const;
        int indexArgument(Argument a) const;

        bool hasArguments() const;

        virtual std::string getRepresentation() const ;

        Basis getBasis() const;
        Basis getBasis(Argument a) const;
        Basis getBasis(const Index& index) const;
        // Basis operator[](Argument a) const;
        // Basis operator[](int index) const;

        std::vector< Basis > getSubBasis() const;
        // TensorBasis getTensorBasis() const;

        TensorBasis addBasis(TensorBasis basis) const ;
        TensorBasis addBasis(Basis basis) const ;

        TensorBasis operator+(const TensorBasis& rhs) const;
        TensorBasis operator*(const TensorBasis& rhs) const;

        AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        int totalNumberBasisFunctions() const;

        // Basis derivative(int order, int direction, AnyTensor& T) const;
    };
}
#endif  // TENSORBASIS_H_
