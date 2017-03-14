#ifndef SRC_FUNCTION_FUNCTION_H_
#define SRC_FUNCTION_FUNCTION_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObject.h"

#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include "Argument.h"
#include "FunNode.h"

namespace spline {

    class Function : public SharedObject{
        public:
        Function();
        Function(const TensorBasis& basis, const Coefficient& coeff);
        Function(const AnyTensor& tensor);
        Function(const AnyScalar& value, const std::vector< int >& shape);

#ifndef SWIG
        FunNode* get() const ;
        FunNode* operator->() const ;
#endif // SWIG

        virtual std::string type() const;
        virtual std::string to_string() const;

        casadi::MX operator<=(const casadi::MX& x) const;
        casadi::MX operator>=(const casadi::MX& x) const;

        AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const;

        Function operator+(const Function& f) const;
        Function operator+(const AnyTensor& t) const;
        Function operator*(const Function& f) const;
        Function operator*(const AnyTensor& t) const;
        Function operator-(const Function& f) const;
        Function operator-(const AnyTensor& t) const;
        Function operator-() const;

        Function mtimes(const Function& f) const;
        Function mtimes(const AnyTensor& f) const;
        Function rmtimes(const AnyTensor& f) const;

        Function pow(int power) const;
        Function mpow(int power) const;

        Function slice(const AnySlice& i, const AnySlice& j) const;
        Function slice(const AnySlice& i) const;

        Function transpose() const;
        Function trace() const;

        static Function vertcat(const std::vector< spline::Function >& f);
        static Function horzcat(const std::vector< spline::Function >& f);
        static Function cat(NumericIndex index, const std::vector< spline::Function >& f);
        static Function blkdiag(const std::vector< spline::Function >& f);

        Coefficient coeff() const;
        AnyTensor coeff_tensor() const;
        AnyTensor data() const;

        Basis basis() const;
        Basis basis(const Argument& i) const;
        TensorBasis tensor_basis() const;
        TensorDomain domain() const;

        bool is_scalar() const;

        void repr() const { casadi::userOut() << to_string() << std::endl;}

        std::vector< int > shape() const;  // Shape result obtained after function evaluation
        Function reshape(const std::vector< int >& shape) const;

        Function transform_to(const Basis& basis) const;
        Function transform_to(const TensorBasis& basis) const;
        Function project_to(const Basis& basis) const;
        Function project_to(const TensorBasis& basis) const;

        int n_inputs() const;  // Number of inputs of the function

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

        Function degree_elevation(int elevation) const;
        Function degree_elevation(int elevation, const NumericIndex& arg_ind) const;
        Function degree_elevation(int elevation, const std::string& arg) const;
        Function degree_elevation(const std::vector<int> & elevation,
            const std::vector<std::string> & args) const;
        Function degree_elevation(const std::vector<int> & elevation,
            const NumericIndexVector & arg_ind) const;

        Function kick_boundary(const TensorDomain& boundary) const;
        Function kick_boundary(const TensorDomain& boundary, const NumericIndex& arg_ind) const;
        Function kick_boundary(const TensorDomain& boundary,
            const std::vector<std::string> & args) const;
        Function kick_boundary(const TensorDomain& boundary,
            const NumericIndexVector & arg_ind) const;

        Function derivative() const;
        Function derivative(int order) const;
        Function derivative(int order, const std::string& arg) const;
        Function derivative(int order, const NumericIndex& arg_ind) const;
        Function derivative(int order, const Argument& arg = Argument()) const;
        Function derivative(const std::vector<int>& orders,
            const std::vector< Argument >& arg_ind) const;

        Function antiderivative() const;
        Function antiderivative(int order) const;
        Function antiderivative(int order, const std::string& arg) const;
        Function antiderivative(int order, const NumericIndex& arg_ind) const;
        Function antiderivative(const std::vector<int>& orders,
            const std::vector<std::string>& args) const;
        Function antiderivative(const std::vector<int>& orders,
            const NumericIndexVector& arg_ind) const;

        std::vector<spline::Function> jacobian() const;

        AnyTensor integral() const;
        AnyTensor integral(const TensorDomain& domain) const;
        Function partial_integral(const TensorDomain& domain,
            const std::vector<std::string>& args) const;
        Function partial_integral(const TensorDomain& domain,
         const NumericIndexVector& arg_ind) const;



#ifndef SWIG
            inline friend
                std::ostream& operator<<(std::ostream &stream, const Function& obj) {
                    return stream << obj.to_string();
                }
#endif // SWIG

    private:
        template<class T>
        std::vector<T> vectorize(const Argument& arg, const T& e) const {
            std::vector< T > ret;
            if(arg.is_all()){
                ret = std::vector<T>(tensor_basis().n_basis(), e);
            } else {
                ret = { e };
            }
            return ret;
        }

        std::vector<int> vectorize(const Argument& arg) const;

    };

} // namespace spline

#endif //       SRC_FUNCTION_FUNCTION_H_
