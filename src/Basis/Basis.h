#ifndef BASIS_H_
#define BASIS_H_

#include <iostream>
#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "../Function/Argument.h"
#include <tensor.hpp>
//#include "../Math/Domain.h"

namespace spline {
    class Basis;
    class BSplineBasis;
    class MonomialeBasis;

#ifndef SWIG

    class BasisNode : public SharedObjectNode {
    public:
        BasisNode (const std::vector< Basis >& allSubBasis);
        BasisNode (const std::vector< Basis >& allSubBasis, Argument argument);
        // int getDimension () const;

        // std::vector<int> getSize () const;

        void setArgument (const Argument& argument);
        void setArgument (const std::string& argumentName);

        Argument& getArgument ();
        const Argument& getArgument () const;

        virtual Basis operator+(const Basis& rhs) const;
        // virtual Basis operator+(const MonomialeBasis& rhs) const;
        // virtual Basis operator+(const BSplineBasis& rhs) const;
        virtual std::string getRepresentation() const ;

        virtual std::vector< Basis > getSubBasis() const;
        virtual Basis getSubBasis( int index ) const;
        void addBasis(Basis basis);

        virtual DT operator()(const std::vector< double >& x) const;
        virtual ST operator()(const std::vector< SX >& x) const;
        virtual MT operator()(const std::vector< MX >& x) const;
    protected:
        Argument argument;
        std::vector< Basis > allSubBasis;
        //    Domain domain;
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
        Basis (const std::vector< Basis >& allSubBasis);
        void setArgument (const Argument& argument);
        void setArgument (const std::string& argumentName);

        Argument& getArgument ();
        const Argument& getArgument () const;

        virtual Basis operator+(const Basis& rhs) const;
        virtual Basis operator+(const MonomialeBasis& rhs) const;
        virtual Basis operator+(const BSplineBasis& rhs) const;
        virtual std::string getRepresentation() const ;

        std::vector< Basis > getSubBasis() const;
        Basis getSubBasis( int index ) const;
        void addBasis(Basis basis);
        DT operator()(const std::vector< double >& x) const;
        ST operator()(const std::vector< SX >& x) const;
        MT operator()(const std::vector< MX >& x) const;
    protected:
        //    Domain domain;
    };
}
#endif  // BASIS_H_
