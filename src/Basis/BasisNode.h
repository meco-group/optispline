#ifndef CPP_SPLINE_BASISNODE_H_
#define CPP_SPLINE_BASISNODE_H_

#include <iostream>
#include <string>
#include <vector>

#include "../SharedObject/SharedObjectNode.h"
#include "../common.h"
#include "../Domain/Domain.h"

// #include <tensor.hpp>
#include <any_tensor.hpp>

#include "Basis.h"

namespace spline {

    class EmptyBasis;
    class EmptyBasisNode;

    class BSplineBasis;
    class BSplineBasisNode;

    class MonomialBasis;
    class MonomialBasisNode;

    class Function;

    class BasisNode : public SharedObjectNode {
        public:
            BasisNode(const Domain& domain);
            BasisNode() { }

            virtual Basis operator+(const BasisNode& rhs) const = 0;
            virtual Basis operator+(const EmptyBasisNode& rhs) const = 0;
            virtual Basis operator+(const MonomialBasisNode& rhs) const = 0;
            virtual Basis operator+(const BSplineBasisNode& rhs) const = 0;

            virtual Basis operator*(const BasisNode& rhs) const = 0;
            virtual Basis operator*(const EmptyBasisNode& rhs) const = 0;
            virtual Basis operator*(const MonomialBasisNode& rhs) const = 0;
            virtual Basis operator*(const BSplineBasisNode& rhs) const = 0;

            virtual bool operator==(const BasisNode& rhs) const;
            virtual bool operator==(const EmptyBasisNode& rhs) const;
            virtual bool operator==(const MonomialBasisNode& rhs) const;
            virtual bool operator==(const BSplineBasisNode& rhs) const;

            virtual std::string type() const;
            virtual std::string to_string() const override;
            Domain domain() const;

            virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;

            virtual void assert_vector_lenght_correct( const AnyVector& x) const;
            virtual void assert_vector_lenght_correct( const AnyTensor& x) const;

            virtual AnyTensor evaluation_grid() const;

            virtual AnyTensor const_coeff_tensor(const AnyTensor& t) const ;

            virtual int dimension() const {return 0;};  // Number of basis functions in the basis
            virtual int n_inputs() const {return 0;};  // Number of inputs of the basis

            virtual Basis derivative(int order, AnyTensor& T) const;
            virtual Basis antiderivative(int order, AnyTensor& T) const;
            virtual AnyTensor integral(const Domain& domain) const;

            virtual Basis insert_knots(const AnyVector & new_knots, AnyTensor & T) const;
            virtual Basis midpoint_refinement(int refinement, AnyTensor& T) const;
            virtual Basis degree_elevation(int elevation, AnyTensor& T) const;
            virtual Basis kick_boundary(const Domain& boundary, AnyTensor& T) const;

            AnyTensor project_to(const Basis& b) const ;
            Basis transform_to(const Basis& b, AnyTensor& T) const ;
            spline::Function basis_functions() const;

        protected:
            Domain domain_;

    };


}  // namespace spline
#endif  // CPP_SPLINE_BASISNODE_H_
