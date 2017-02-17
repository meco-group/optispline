#ifndef CPP_SPLINE_FUNCTION_FUNCTION_H
#define CPP_SPLINE_FUNCTION_FUNCTION_H

#include <casadi/casadi.hpp>
#include <string>
#include "../Basis/TensorBasis.h"
#include "../Coefficients/Coefficient.h"
#include <any_tensor.hpp>
#include "Index.h"

namespace spline {
    class Function {


    public :
        Function( const TensorBasis& basis, const Coefficient& coef) ;
        Function( const Basis& basis, const Coefficient& coef) ;

        static Function Constant(const TensorBasis& basis, const AnyScalar& a, const std::vector< int >& size) ;
        static Function Constant(const TensorBasis& basis, const AnyTensor& t) ;
        static Function Constant(const Basis& basis, const AnyScalar& a, const std::vector< int >& size) ;
        static Function Constant(const Basis& basis, const AnyTensor& t);

        AnyTensor operator()(const std::vector< AnyScalar >& x) const ;

        Function operator+(const Function& f) const ;
        Function operator+(const AnyTensor& t) const ;
        Function operator*(const Function& f) const ;
        Function operator*(const AnyTensor& t) const ;
        Function pow(int power) const ;
        Function operator-() const ;
        Function operator-(const Function& f) const ;
        Function operator-(const AnyTensor& t) const ;
        Function mtimes(const Function& f) const ;
        Function mtimes(const AnyTensor& f) const ;
        Function rmtimes(const AnyTensor& f) const ;
        Function transpose() const ;
        Function trace() const ;

        Function vertcat(const std::vector< Function >& f) const ;
        Function horzcat(const std::vector< Function >& f) const ;
        Function blkdiag(const std::vector< Function >& f) const ;

        Basis getBasis() const;
        Basis getBasis(const Index& i) const;
        TensorBasis getTensorBasis() const {return basis;}
        Coefficient getCoefficient() const {return coef;}
        AnyTensor getCoeffTensor() const {return coef.getData();}

        MX operator<=(const MX& x) const;
        MX operator>=(const MX& x) const;

        bool is_scalar() const { return shape()[0]==1 && shape()[1]==1; };

        std::string getRepresentation() const ;
        void repr() const { userOut() << getRepresentation() << std::endl;}
        // Argument& getArgument (){ return getBasis().getArgument();}

        int n_inputs() const;  // Number of inputs of the function
        std::vector< int > shape() const;  // Shape result obtained after function evaluation

        Function insert_knots(const AnyVector & new_knots) const;
        Function insert_knots(const AnyVector & new_knots, const NumericIndex & arg_ind) const;
        Function insert_knots(const AnyVector & new_knots, const Argument & arg) const;
        Function insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<Argument> & arg) const;
        Function insert_knots(const std::vector<AnyVector> & new_knots,
            const std::vector<NumericIndex> & arg_ind) const;

        Function midpoint_refinement(int refinement) const;
        Function midpoint_refinement(int refinement, const NumericIndex & arg_ind) const;
        Function midpoint_refinement(int refinement, const Argument & arg) const;
        Function midpoint_refinement(const std::vector<int> & refinement,
            const std::vector<Argument> & arg) const;
        Function midpoint_refinement(const std::vector<int> & refinement,
            const std::vector<NumericIndex> & arg_ind) const;

        Function derivative(const std::vector<NumericIndex>& direction_ind) const;
        Function derivative(const std::vector<Argument>& directions) const;
        Function derivative(const std::vector<int>& orders, const std::vector<Argument>& directions) const;
        Function derivative(const std::vector<int>& orders, const std::vector<NumericIndex>& direction_ind) const;

        Function transform_to(const TensorBasis& basis) const ;

    public:
        TensorBasis basis;
        Coefficient coef;

    private:
        void init( const TensorBasis& basis, const Coefficient& coef) ;
        typedef std::function<TensorBasis(const TensorBasis&, const TensorBasis&)> BasisComposition;
        typedef std::function<AnyTensor(const AnyTensor&, const AnyTensor&)> TensorComposition;
        Function generic_operation(const Function& f,
                const BasisComposition & bc, const TensorComposition & tc) const ;
    };

} // namespace spline

#endif //CPP_SPLINE_FUNCTION_FUNCTION_H
