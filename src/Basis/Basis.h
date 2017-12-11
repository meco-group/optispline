#ifndef SRC_BASIS_BASIS_H_
#define SRC_BASIS_BASIS_H_

#include <iostream>
#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "BasisNode.h"
#include "../common.h"
#include "../Domain/Domain.h"

// #include <tensor.hpp>
#include <any_tensor.hpp>

namespace spline {

    class EmptyBasis;
    class EmptyBasisNode;

    class BSplineBasis;
    class BSplineBasisNode;

    class MonomialBasis;
    class MonomialBasisNode;

    class Function;

    class Basis : public SharedObject{
    public:

#ifndef SWIG
        BasisNode* get() const ;
        BasisNode* operator->() const ;
#endif // SWIG

        Basis operator+(const Basis& rhs) const;
        Basis operator*(const Basis& rhs) const;

        AnyTensor operator()(const AnyTensor& x) const;
        AnyTensor list_eval(const AnyTensor& x) const;

        bool operator==(const Basis& rhs) const;

        std::string type() const;

        Domain domain() const;
        AnyTensor evaluation_grid() const;

        Basis insert_knots(const AnyVector & new_knots, AnyTensor & SWIG_OUTPUT(T)) const;
        Basis midpoint_refinement(int refinement, AnyTensor& SWIG_OUTPUT(T)) const;
        Basis degree_elevation(int elevation, AnyTensor& SWIG_OUTPUT(T)) const;
        Basis kick_boundary(const Domain& boundary, AnyTensor& SWIG_OUTPUT(T)) const;

        AnyTensor project_to(const Basis& b) const ;
        Basis transform_to(const Basis& b, AnyTensor& SWIG_OUTPUT(T)) const ;
        spline::Function basis_functions() const;

        int dimension() const;
        int n_inputs() const;

        Basis derivative(AnyTensor& SWIG_OUTPUT(T)) const;  // default order = 1
        Basis derivative(int order, AnyTensor& SWIG_OUTPUT(T)) const;

        Basis antiderivative(AnyTensor& SWIG_OUTPUT(T)) const;  // default order = 1
        Basis antiderivative(int order, AnyTensor& SWIG_OUTPUT(T)) const;

        AnyTensor integral(const Domain& domain) const;

        AnyTensor const_coeff_tensor(const AnyTensor& t) const ;

    };

}  // namespace spline
#endif  // SRC_BASIS_BASIS_H_
