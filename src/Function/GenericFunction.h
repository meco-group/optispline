#ifndef SRC_FUNCTION_GENERICFUNCTION_H_
#define SRC_FUNCTION_GENERICFUNCTION_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include <any_tensor.hpp>
#include "Argument.h"

namespace spline {

    class Constant;
    class Function;

class GenericFunction {

    public :
        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const = 0;

        casadi::MX operator<=(const casadi::MX& x) const;
        casadi::MX operator>=(const casadi::MX& x) const;

        virtual Function slice(const AnySlice& i, const AnySlice& j) const = 0;
        virtual Function slice(const AnySlice& i) const = 0;

        virtual Function operator+(const Function& f) const = 0;
        virtual Function operator+(const Constant& f) const = 0;
        Function operator+(const AnyTensor& t) const;
        virtual Function operator*(const Function& f) const = 0;
        virtual Function operator*(const Constant& f) const = 0;
        Function operator*(const AnyTensor& t) const;

        virtual Function operator-() const = 0;
        Function operator-(const GenericFunction& f) const;
        Function operator-(const AnyTensor& t) const;

        virtual Function mtimes(const Function& f) const = 0;
        virtual Function mtimes(const Constant& f) const = 0;
        Function mtimes(const AnyTensor& f) const;
        Function rmtimes(const AnyTensor& f) const;

        Function pow(int power) const;

        virtual Function transpose() const = 0;
        virtual Function trace() const = 0;

        static Function vertcat(const std::vector< spline::Function >& f);
        static Function horzcat(const std::vector< spline::Function >& f);
        static Function cat(NumericIndex index, const std::vector< spline::Function >& f);
        static Function blkdiag(const std::vector< spline::Function >& f);

        virtual std::string type() const;
        virtual std::string to_string() const;

        Coefficient coeff() const {return coeff_;}
        AnyTensor coeff_tensor() const {return coeff_.data();}

        virtual TensorBasis tensor_basis() const = 0;

        AnyTensor data() const {return coeff_tensor().squeeze(); }

        bool is_scalar() const { return shape()[0] == 1 && shape()[1] == 1; }

        void repr() const { casadi::userOut() << to_string() << std::endl;}
        // std::string& getArgument (){ return basis().getArgument();}

        std::vector< int > shape() const;  // Shape result obtained after function evaluation

        virtual Function transform_to(const Basis& basis) const = 0;
        virtual Function transform_to(const TensorBasis& basis) const = 0;
        virtual Function project_to(const Basis& basis) const = 0;
        virtual Function project_to(const TensorBasis& basis) const = 0;

        /* virtual GenericFunction reshape(const std::vector< int >& shape) const = 0; */
    public:
        Coefficient coeff_;

    private:
        static void homogenize_args(Function& f, AnyTensor& t);
};


} // namespace spline

#endif // SRC_FUNCTION_GENERICFUNCTION_H_
