#ifndef SRC_FUNCTION_CONSTANT_H_
#define SRC_FUNCTION_CONSTANT_H_

#include <string>
#include <vector>
#include <casadi/casadi.hpp>
#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include <any_tensor.hpp>
#include "GenericFunction.h"
#include "Argument.h"

namespace spline {

class Constant : public GenericFunction {

    public :
        Constant(const AnyTensor& c);

        AnyTensor operator()(const AnyTensor& x, const std::vector< std::string >& args = std::vector< std::string > () ) const;
        /* evaluation options */
        /* AnyTensor operator()(const std::map<std::string, AnyScalar >& x) const; */
        /* AnyTensor operator()(const std::map<std::string, std::vector<AnyScalar> >& x) const; */
        /* AnyTensor operator()(const std:vector< std::vector<AnyScalar> >& x, const std::vector< std::string > = std::vector< std::string > {} ) const; */

        /* Function partial(const std::map<std::string, AnyScalar >& x) const; */

        Function slice(const AnySlice& i, const AnySlice& j) const;
        Function slice(const AnySlice& i) const;

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

        virtual std::string type() const override;
        virtual std::string to_string() const override;


        bool is_scalar() const { return shape()[0] == 1 && shape()[1] == 1; }

        void repr() const { casadi::userOut() << to_string() << std::endl;}
        // std::string& getArgument (){ return basis().getArgument();}

        Function transform_to(const Basis& basis) const;
        Function transform_to(const TensorBasis& basis) const;
        Function project_to(const Basis& basis) const;
        Function project_to(const TensorBasis& basis) const;

        Function reshape(const std::vector< int >& shape) const;

};


} // namespace spline

#endif // SRC_FUNCTION_CONSTANT_H_
