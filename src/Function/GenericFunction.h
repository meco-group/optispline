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

class GenericFunction {

    public :
        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const = 0;

        casadi::MX operator<=(const casadi::MX& x) const;
        casadi::MX operator>=(const casadi::MX& x) const;

        virtual GenericFunction slice(const AnySlice& i, const AnySlice& j) const = 0;
        virtual GenericFunction slice(const AnySlice& i) const = 0;

        virtual GenericFunction operator+(const GenericFunction& f) const = 0;
        virtual GenericFunction operator+(const Function& f) const = 0;
        virtual GenericFunction operator+(const Constant& f) const = 0;
        virtual GenericFunction operator+(const AnyTensor& t) const = 0;
        virtual GenericFunction operator*(const GenericFunction& f) const = 0;
        virtual GenericFunction operator*(const Function& f) const = 0;
        virtual GenericFunction operator*(const Constant& f) const = 0;
        virtual GenericFunction operator*(const AnyTensor& t) const = 0;
        virtual GenericFunction operator-() const = 0;
        virtual GenericFunction operator-(const GenericFunction& f) const;
        virtual GenericFunction operator-(const AnyTensor& t) const = 0;
        virtual GenericFunction mtimes(const GenericFunction& f) const = 0;
        virtual GenericFunction mtimes(const Function& f) const = 0;
        virtual GenericFunction mtimes(const Constant& f) const = 0;
        virtual GenericFunction mtimes(const AnyTensor& f) const = 0;
        virtual GenericFunction rmtimes(const AnyTensor& f) const = 0;
        virtual GenericFunction pow(int power) const = 0;
        virtual GenericFunction transpose() const = 0;
        virtual GenericFunction trace() const = 0;

        static Function vertcat(const std::vector< spline::Function >& f);
        static Function horzcat(const std::vector< spline::Function >& f);
        static Function cat(NumericIndex index, const std::vector< spline::Function >& f);
        static Function blkdiag(const std::vector< spline::Function >& f);

        virtual std::string type() const;
        virtual std::string to_string() const;

        Coefficient coeff() const {return coeff_;}
        AnyTensor coeff_tensor() const {return coeff_.data();}

        AnyTensor data() const {return coeff_tensor().squeeze(); }

        casadi::MX operator<=(const casadi::MX& x) const;
        casadi::MX operator>=(const casadi::MX& x) const;

        bool is_scalar() const { return shape()[0] == 1 && shape()[1] == 1; }

        void repr() const { casadi::userOut() << to_string() << std::endl;}
        // std::string& getArgument (){ return basis().getArgument();}

        std::vector< int > shape() const;  // Shape result obtained after function evaluation

        virtual GenericFunction transform_to(const Basis& basis) const = 0;
        virtual GenericFunction transform_to(const TensorBasis& basis) const = 0;
        virtual GenericFunction project_to(const Basis& basis) const = 0;
        virtual GenericFunction project_to(const TensorBasis& basis) const = 0;

        virtual GenericFunction reshape(const std::vector< int >& shape) const = 0;
    public:
        Coefficient coeff_;

    private:
        static void homogenize_args(Function& f, AnyTensor& t);

        typedef std::function<TensorBasis(const TensorBasis&, const TensorBasis&)> BasisComposition;
        typedef std::function<AnyTensor(const AnyTensor&, const AnyTensor&)> TensorComposition;
        Function generic_operation(const Function& f,
                const BasisComposition & bc, const TensorComposition & tc) const;
};


} // namespace spline

#endif // SRC_FUNCTION_GENERICFUNCTION_H_
