#ifndef SRC_BASIS_TENSORBASIS_H_
#define SRC_BASIS_TENSORBASIS_H_

#include <iostream>
#include <string>
#include <vector>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "Basis.h"
#include "../Function/Argument.h"
#include "../Function/NumericIndex.h"
#include "../Domain/TensorDomain.h"

namespace spline {

class TensorBasis;
class BSplineBasis;
class MonomialBasis;
class Function;

#ifndef SWIG

class TensorBasisNode : public SharedObjectNode {
    public:
        TensorBasisNode(const std::vector< Basis >& bases_);
        TensorBasisNode(const std::vector< Basis >& bases_, const std::vector< std::string >& args);

        virtual std::string type() const;
        virtual std::string to_string() const override;

        virtual int n_basis() const;
        virtual std::vector<int> dimension() const;
        virtual int n_inputs() const;

        std::vector< std::string > arguments() const;

        std::string argument(int index) const;
        int indexArgument(std::string a) const;

        bool hasArguments() const;

        TensorDomain domain() const;

        std::vector< Basis > bases() const;
        TensorBasis bases(int index) const;
        Basis basis() const;
        Basis basis(const std::string& a) const;
        virtual Basis basis(const Argument& index) const;
        // Basis bases( std::string a) const;

        TensorBasis add_basis(TensorBasis basis) const;
        TensorBasis add_basis(Basis basis) const;

        TensorBasis substitute_bases(const std::vector<Argument>& arg_ind,
            const std::vector<Basis>& bases) const;

        virtual TensorBasis operator+(const TensorBasis& rhs) const;
        virtual TensorBasis operator*(const TensorBasis& rhs) const;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x, const std::vector< int >& arg_ind, bool reorder_output) const;

        virtual AnyTensor grid_eval(const std::vector< AnyTensor >& x, const std::vector< int >& arg_ind, bool reorder_output) const;
        virtual std::vector< AnyTensor >  evaluation_grid() const;

        virtual bool operator==(const TensorBasis& rhs) const;

        int totalNumberBasisFunctions() const;
        AnyTensor const_coeff_tensor(const AnyTensor& t) const;

        TensorBasis insert_knots(const std::vector<AnyVector> & new_knots,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const;
        TensorBasis midpoint_refinement(const std::vector<int> & refinement,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const;

        TensorBasis derivative(const std::vector<int>& orders,
            const std::vector<int>& arg_ind, std::vector<AnyTensor>& T) const;
        TensorBasis antiderivative(const std::vector<int>& orders,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor>& T) const;
        std::vector<AnyTensor> integral(const TensorDomain& domain) const;
        TensorBasis partial_integral(const TensorDomain& domain,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor>& T) const;
        TensorBasis partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args, std::vector<AnyTensor>& T) const;

        TensorBasis degree_elevation(const std::vector<int>& elevation,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const;

        TensorBasis kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string>& args, std::vector<AnyTensor> & T) const;
        TensorBasis kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor> & T) const;

        AnyTensor project_to(const TensorBasis& b) const ;
        TensorBasis transform_to(const TensorBasis& b, std::vector<AnyTensor>& T) const;
        spline::Function basis_functions() const ;

        virtual std::vector< int > get_permutation(const TensorBasis& grid) const;
        std::vector< int > input_border() const;
    // protected:
        std::vector< Basis > bases_;
        std::vector< std::string > allArguments;


};

#endif // SWIG

class TensorBasis : public SharedObject{
public:
#ifndef SWIG

        TensorBasisNode* get() const;
        TensorBasisNode* operator->() const;

#endif // SWIG
        TensorBasis();
        TensorBasis(const Basis& bases_);
        TensorBasis(const Basis& bases_, const std::string& args);
        TensorBasis(const TensorBasis& tensor, const std::vector< std::string >& args);
        TensorBasis(const std::vector< Basis >& bases_);
        TensorBasis(const std::vector< Basis >& bases_, const std::vector< std::string >& args);
        TensorBasis(const std::vector< TensorBasis >& allBasis);

#ifndef SWIG
        inline friend
    std::ostream& operator<<(std::ostream &stream, const TensorBasis& base) {
        return stream << base.to_string();
    }
#endif // SWIG

        virtual std::string type() const;

        int n_basis() const;  // Number of bases, building up the TensorBasis
        int n_inputs() const; // Total number of inputs, over all bases
        std::vector<int> dimension() const; // Vector containing number of basis functions in each basis

        std::vector< std::string > arguments() const;

        std::string argument(int index) const;
        int indexArgument(std::string a) const;

        bool hasArguments() const;

        TensorDomain domain() const;

        Basis basis() const;
        Basis basis(const std::string& a) const;
        Basis basis(const Argument& index) const;
        // Basis operator[](std::string a) const;
        // Basis operator[](int index) const;

        std::vector< Basis > bases() const;
        // TensorBasis tensor_basis() const;

        TensorBasis add_basis(TensorBasis basis) const;
        TensorBasis add_basis(Basis basis) const;

        TensorBasis substitute_bases(const std::vector<Argument>& arg_ind,
            const std::vector<Basis>& bases) const;

        TensorBasis operator+(const TensorBasis& rhs) const;
        TensorBasis operator*(const TensorBasis& rhs) const;

        AnyTensor operator()(const std::vector< AnyScalar >& x, const std::vector< Argument >& arg_ind = {}, bool reorder_output = true) const;
        AnyTensor grid_eval(const std::vector< AnyTensor >& x, const std::vector< Argument >& arg_ind = {}, bool reorder_output = true) const;
        std::vector< AnyTensor >  evaluation_grid() const;

        virtual bool operator==(const TensorBasis& rhs) const;

        int totalNumberBasisFunctions() const;

        AnyTensor const_coeff_tensor(const AnyTensor& t) const;

        TensorBasis insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<Argument>& args, std::vector<AnyTensor> & SWIG_OUTPUT(T)) const;
        TensorBasis midpoint_refinement(const std::vector<int> & refinement,
            const std::vector<Argument>& args, std::vector<AnyTensor> & SWIG_OUTPUT(T)) const;
        TensorBasis degree_elevation(const std::vector<int>& elevation,
            const std::vector<Argument>& args, std::vector<AnyTensor>& SWIG_OUTPUT(T)) const;
        TensorBasis kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string>& args, std::vector<AnyTensor>& SWIG_OUTPUT(T)) const;
        TensorBasis kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor>& SWIG_OUTPUT(T)) const;


        TensorBasis derivative(const std::vector<Argument>& arg,
          std::vector<AnyTensor>& SWIG_OUTPUT(T)) const; // default order = 1
        TensorBasis derivative(const std::vector<int>& order,
          const std::vector<Argument>& arg, std::vector<AnyTensor>& SWIG_OUTPUT(T)) const;

        TensorBasis antiderivative(const std::vector<Argument>& arg,
          std::vector<AnyTensor>& SWIG_OUTPUT(T)) const; // default order = 1
        TensorBasis antiderivative(const std::vector<int>& order,
          const std::vector<Argument>& arg, std::vector<AnyTensor>& SWIG_OUTPUT(T)) const;

        std::vector<AnyTensor> integral(const TensorDomain& domain) const;
        TensorBasis partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args, std::vector<AnyTensor>& SWIG_OUTPUT(T)) const;
        TensorBasis partial_integral(const TensorDomain& domain,
            const NumericIndexVector& arg_ind, std::vector<AnyTensor>& SWIG_OUTPUT(T)) const;

        AnyTensor project_to(const TensorBasis& b) const ;
        TensorBasis transform_to(const TensorBasis& b, std::vector<AnyTensor>& SWIG_OUTPUT(T)) const;
        spline::Function basis_functions() const ;

        std::vector< int > get_permutation(const TensorBasis& grid) const;
      private:
          template<class T>
          std::vector<T> vectorize(const Argument& arg, const T& e) const {
              std::vector< T > ret;
              if(arg.is_all()){
                  ret = std::vector<T>(n_basis(), e);
              } else {
                  ret = { e };
              }
              return ret;
          }

          std::vector<int> vectorize(const Argument& arg) const;

};

}   // namespace spline


#endif  // SRC_BASIS_TENSORBASIS_H_
