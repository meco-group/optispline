#ifndef SRC_FUNCTION_FUNCTION_H_
#define SRC_FUNCTION_FUNCTION_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include <any_tensor.hpp>
#include "Argument.h"

namespace spline {

class Function {

    public :
        Function(const TensorBasis& basis, const Coefficient& coef);
        Function(const Basis& basis, const Coefficient& coef);
        Function(const AnyTensor& c);
        Function() {}

        static Function Constant(const TensorBasis& basis, const AnyScalar& a,
            const std::vector< int >& size);
        static Function Constant(const TensorBasis& basis, const AnyTensor& t);
        static Function Constant(const Basis& basis, const AnyScalar& a,
            const std::vector< int >& size);
        static Function Constant(const Basis& basis, const AnyTensor& t);

        AnyTensor operator()(const std::vector< AnyScalar >& x) const;

        /* evaluation options */
        /* AnyTensor operator()(const std::map<std::string, AnyScalar >& x) const; */
        /* AnyTensor operator()(const std::map<std::string, std::vector<AnyScalar> >& x) const; */
        /* AnyTensor operator()(const std:vector< std::vector<AnyScalar> >& x, const std::vector< std::string > = std::vector< std::string > {} ) const; */

        /* Function partial(const std::map<std::string, AnyScalar >& x) const; */

        Function slice(const AnySlice& i, const AnySlice& j) const;

        Function operator+(const Function& f) const;
        Function operator+(const AnyTensor& t) const;
        Function operator*(const Function& f) const;
        Function operator*(const AnyTensor& t) const;
        Function pow(int power) const;
        Function operator-() const;
        Function operator-(const Function& f) const;
        Function operator-(const AnyTensor& t) const;
        Function mtimes(const Function& f) const;
        Function mtimes(const AnyTensor& f) const;
        Function rmtimes(const AnyTensor& f) const;
        Function transpose() const;
        Function trace() const;

        static Function vertcat(const std::vector< spline::Function >& f);
        static Function horzcat(const std::vector< spline::Function >& f);
        static Function cat(NumericIndex index, const std::vector< spline::Function >& f);
        static Function blkdiag(const std::vector< spline::Function >& f);

        Basis basis() const;
        Basis basis(const Argument& i) const;
        TensorBasis tensor_basis() const {return basis_;}
        Coefficient coeff() const {return coeff_;}
        AnyTensor coeff_tensor() const {return coeff_.data();}

        AnyTensor data() const {return coeff_tensor().squeeze(); }

        casadi::MX operator<=(const casadi::MX& x) const;
        casadi::MX operator>=(const casadi::MX& x) const;

        bool is_scalar() const { return shape()[0] == 1 && shape()[1] == 1; }

        std::string getRepresentation() const;
        void repr() const { casadi::userOut() << getRepresentation() << std::endl;}
        // std::string& getArgument (){ return basis().getArgument();}

        int n_inputs() const;  // Number of inputs of the function
        std::vector< int > shape() const;  // Shape result obtained after function evaluation

        Function insert_knots(const AnyVector & new_knots) const;
        Function insert_knots(const AnyVector & new_knots, const NumericIndex & arg_ind) const;
        Function insert_knots(const AnyVector & new_knots, const std::string & arg) const;
        Function insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<std::string> & arg) const;
        Function insert_knots(const std::vector<AnyVector> & new_knots,
            const NumericIndexVector & arg_ind) const;

        Function midpoint_refinement(int refinement) const;
        Function midpoint_refinement(int refinement, const NumericIndex & arg_ind) const;
        Function midpoint_refinement(int refinement, const std::string & arg) const;
        Function midpoint_refinement(const std::vector<int> & refinement,
            const std::vector<std::string> & arg) const;
        Function midpoint_refinement(const std::vector<int> & refinement,
            const NumericIndexVector & arg_ind) const;

        Function derivative() const;
        Function derivative(int order) const;
        Function derivative(int order, const std::string& arg) const;
        Function derivative(int order, const NumericIndex& arg_ind) const;
        Function derivative(const std::vector<int>& orders,
            const std::vector<std::string>& args) const;
        Function derivative(const std::vector<int>& orders,
            const NumericIndexVector& arg_ind) const;

        Function antiderivative() const;
        Function antiderivative(int order) const;
        Function antiderivative(int order, const std::string& arg) const;
        Function antiderivative(int order, const NumericIndex& arg_ind) const;
        Function antiderivative(const std::vector<int>& orders,
            const std::vector<std::string>& args) const;
        Function antiderivative(const std::vector<int>& orders,
            const NumericIndexVector& arg_ind) const;

        std::vector<spline::Function> jacobian() const;

        Function transform_to(const TensorBasis& basis) const;
        Function project_to(const TensorBasis& basis) const;

        AnyTensor integral() const;
        AnyTensor integral(const TensorDomain& domain) const;
        Function partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args) const;
        Function partial_integral(const TensorDomain& domain,
         const NumericIndexVector& arg_ind) const;

        Function reshape(const std::vector< int >& shape) const;

    public:
        TensorBasis basis_;
        Coefficient coeff_;

    private:
        static void homogenize_args(Function& f, AnyTensor& t);

        void init(const TensorBasis& basis, const Coefficient& coef);
        typedef std::function<TensorBasis(const TensorBasis&, const TensorBasis&)> BasisComposition;
        typedef std::function<AnyTensor(const AnyTensor&, const AnyTensor&)> TensorComposition;
        Function generic_operation(const Function& f,
                const BasisComposition & bc, const TensorComposition & tc) const;
};


} // namespace spline

#endif // SRC_FUNCTION_FUNCTION_H_
