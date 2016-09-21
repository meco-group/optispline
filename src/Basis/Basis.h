#ifndef BASIS_H_
#define BASIS_H_

#include <iostream>
#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "../Function/Argument.h"
#include "SubBasis.h"
#include <tensor.hpp>

namespace spline {
    class Basis;
    class BSplineBasis;
    class MonomialeBasis;

#ifndef SWIG

    class BasisNode : public SharedObjectNode {
    public:
        BasisNode (const std::vector< SubBasis >& allSubBasis);
        // int getDimension () const;

        // std::vector<int> getSize () const;


        Argument& getArgument ();
        const Argument& getArgument () const;

        virtual std::string getRepresentation() const ;

        // virtual std::vector< SubBasis > getSubBasis() const;
        virtual Basis getSubBasis( int index ) const;

        // virtual std::vector< Argument > getSubArgument() const;
        // virtual Argument getSubArgument( int index ) const;
        void addBasis(Basis basis);
        void addBasis(SubBasis basis);

        virtual DT operator()(const std::vector< double >& x) const;
        virtual ST operator()(const std::vector< SX >& x) const;
        virtual MT operator()(const std::vector< MX >& x) const;

        int indexArgument(Argument a);
    protected:
        std::vector< SubBasis > allSubBasis;
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
        Basis (const std::vector< SubBasis >& allSubBasis);
        void setArgument (const Argument& argument);
        void setArgument (const std::string& argumentName);

        Argument& getArgument ();
        const Argument& getArgument () const;

        virtual Basis operator+(const Basis& rhs) const;
        virtual Basis operator*(const Basis& rhs) const;
        virtual std::string getRepresentation() const ;

        std::vector< SubBasis > getSubBasis() const;
        SubBasis getSubBasis( int index ) const;

        // std::vector< Argument > getSubArgument ()const;
        // Argument getSubArgument ( int index ) const;

        void addBasis(Basis basis);
        void addBasis(SubBasis basis);

        DT operator()(const std::vector< double >& x) const;
        ST operator()(const std::vector< SX >& x) const;
        MT operator()(const std::vector< MX >& x) const;

        int indexArgument(Argument a);
    };
}
#endif  // BASIS_H_
