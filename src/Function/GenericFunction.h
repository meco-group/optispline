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
        virtual AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const;

        virtual Function slice(const AnySlice& i, const AnySlice& j) const;
        virtual Function slice(const AnySlice& i) const;

        virtual Function operator+(const Function& f) const;
        virtual Function operator+(const AnyTensor& t) const;
        virtual Function operator*(const Function& f) const;
        virtual Function operator*(const AnyTensor& t) const;
        virtual Function pow(int power) const;
        virtual Function operator-() const;
        virtual Function operator-(const Function& f) const;
        virtual Function operator-(const AnyTensor& t) const;
        virtual Function mtimes(const Function& f) const;
        virtual Function mtimes(const AnyTensor& f) const;
        virtual Function rmtimes(const AnyTensor& f) const;
        virtual Function transpose() const;
        virtual Function trace() const;

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

        virtual Function transform_to(const Basis& basis) const;
        virtual Function transform_to(const TensorBasis& basis) const;
        virtual Function project_to(const Basis& basis) const;
        virtual Function project_to(const TensorBasis& basis) const;

        virtual Function reshape(const std::vector< int >& shape) const;
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
