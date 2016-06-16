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
        //Basis (std::vector< Basis> vectorBasis ); 
        //    Basis (int dimension, const std::vector<int> &size, const Argument &argument);

        //   virtual Tensor operator()(const casadi::DM & x) const;
        //    virtual std::vector< Tensor > operator()(const std::vector< casadi::DM >& x) const = 0;

        // int getDimension () const;

        // std::vector<int> getSize () const;

        void setArgument (const Argument& argument);
        void setArgument (const std::string& argumentName);

        Argument& getArgument () const;

        virtual Basis operator+(const Basis& rhs) const;
        virtual Basis operator+(const MonomialeBasis& rhs) const;
        virtual Basis operator+(const BSplineBasis& rhs) const;
        virtual std::string getRepresentation() const ;

        virtual std::vector< Basis > getSubBasis() const;
        void addBasis(Basis basis);

        virtual DT operator()(const std::vector< double >& x) const;
        virtual ST operator()(const std::vector< SX >& x) const;
        virtual MT operator()(const std::vector< MX >& x) const;
    protected:
        Argument& argument;
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
        //Basis (std::vector< Basis> vectorBasis ); 
        //    Basis (int dimension, const std::vector<int> &size, const Argument &argument);

        //   virtual Tensor operator()(const casadi::DM & x) const;
        //    virtual std::vector< Tensor > operator()(const std::vector< casadi::DM >& x) const = 0;

        // int getDimension () const;

        // std::vector<int> getSize () const;
        Basis ();
        Basis (const std::vector< Basis >& allSubBasis);
        void setArgument (const Argument& argument);
        void setArgument (const std::string& argumentName);

        Argument& getArgument () const;

        virtual Basis operator+(const Basis& rhs) const;
        virtual Basis operator+(const MonomialeBasis& rhs) const;
        virtual Basis operator+(const BSplineBasis& rhs) const;
        virtual std::string getRepresentation() const ;

        std::vector< Basis > getSubBasis() const;
        void addBasis(Basis basis);
        DT operator()(const std::vector< double >& x) const;
        ST operator()(const std::vector< SX >& x) const;
        MT operator()(const std::vector< MX >& x) const;
    protected:
        //    Domain domain;
    };
}
#endif  // BASIS_H_
