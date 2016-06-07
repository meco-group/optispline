#ifndef BASIS_H_
#define BASIS_H_

#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "../Function/Argument.h"
//#include "../Math/Tensor.h"
//#include "../Math/Domain.h"

namespace spline {
    class Basis;
    class BSplineBasis;
    class MonomialeBasis;

#ifndef SWIG

    class BasisNode : public SharedObjectNode {
    public:
        BasisNode ();
        //Basis (std::vector< Basis> vectorBasis ); 
        //    Basis (int dimension, const std::vector<int> &size, const Argument &argument);

        //   virtual Tensor operator()(const casadi::DM & x) const;
        //    virtual std::vector< Tensor > operator()(const std::vector< casadi::DM >& x) const = 0;

        // int getDimension () const;

        // std::vector<int> getSize () const;

        //      void setArgument (const Argument& argument);
        //      void setArgument (const std::string& argumentName);

        //    void addArgument (const Argument& argument);
        //    void addArgument (const std::string& argumentName);
        //Argument getArgument () const;

        virtual Basis operator+(const Basis& rhs) const;
        virtual Basis operator+(const MonomialeBasis& rhs) const;
        virtual Basis operator+(const BSplineBasis& rhs) const;
        virtual std::string getRepresentation() const ;
    protected:
        // std::vector< Basis_ptr > allSubBasis;
        // Argument argument;
        //    Domain domain;
    };

#endif // SWIG

    class Basis : public SharedObject{
    public:

#ifndef SWIG
        Basis (){};
        BasisNode* get() const ;
        BasisNode* operator->() const ;

#endif // SWIG
        //Basis (std::vector< Basis> vectorBasis ); 
        //    Basis (int dimension, const std::vector<int> &size, const Argument &argument);

        //   virtual Tensor operator()(const casadi::DM & x) const;
        //    virtual std::vector< Tensor > operator()(const std::vector< casadi::DM >& x) const = 0;

        // int getDimension () const;

        // std::vector<int> getSize () const;

        //      void setArgument (const Argument& argument);
        //      void setArgument (const std::string& argumentName);

        //    void addArgument (const Argument& argument);
        //    void addArgument (const std::string& argumentName);
        //Argument getArgument () const;

        virtual Basis operator+(const Basis& rhs) const;
        virtual Basis operator+(const MonomialeBasis& rhs) const;
        virtual Basis operator+(const BSplineBasis& rhs) const;
        virtual std::string getRepresentation() const ;
    protected:
        // std::vector< Basis_ptr > allSubBasis;
        // Argument argument;
        //    Domain domain;
    };
}
#endif  // BASIS_H_
