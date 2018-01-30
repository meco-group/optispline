#include "EmptyBasis.h"
#include "EmptyBasisNode.h"

#include "MonomialBasis.h"
#include "MonomialBasisNode.h"

#include "BSplineBasis.h"
#include "BSplineBasisNode.h"


#include "operations/operationsBasis.h"

namespace spline {

    MonomialBasisNode::MonomialBasisNode(casadi_int degree) :
        UnivariateBasisNode(degree,
          Interval(-std::numeric_limits<double>::infinity(),
                   +std::numeric_limits<double>::infinity())) {};


    std::string MonomialBasisNode::to_string() const {
        return "MonomialBasis of degree " + std::to_string(degree()) +
               ", on " + domain().to_string();
    }

    Basis MonomialBasisNode::operator+ (const BasisNode& other) const {
        return other + *this;
    }

    Basis MonomialBasisNode::operator+ (const EmptyBasisNode& other) const {
        return shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator+ (const BSplineBasisNode& other) const {
        return plus_basis (*this, other);
    }

    Basis MonomialBasisNode::operator+ (const MonomialBasisNode& other) const {
        return plus_basis (*this, other);
    }

    Basis MonomialBasisNode::operator* (const BasisNode& other) const {
        return other * *this;
    }

    Basis MonomialBasisNode::operator* (const EmptyBasisNode& other) const {
        return shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator* (const BSplineBasisNode& other) const {
        return times_basis (*this, other);
    }

    Basis MonomialBasisNode::operator* (const MonomialBasisNode& other) const {
        return times_basis (*this, other);
    }

    bool MonomialBasisNode::operator== (const BasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    bool MonomialBasisNode::operator== (const EmptyBasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    bool MonomialBasisNode::operator== (const BSplineBasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    bool MonomialBasisNode::operator== (const MonomialBasisNode& other) const {
      spline_assert_message(false, type() << " == ");
      return false;
    }

    AnyTensor MonomialBasisNode::const_coeff_tensor(const AnyTensor& t) const {
        std::vector< casadi_int > coeff_size = t.dims();
        coeff_size.insert(coeff_size.begin(), 1);

        AnyTensor values = t.shape(coeff_size);
        AnyTensor zeros = AnyTensor::repeat(AnyTensor(AnyScalar(0)), coeff_size);


        std::vector< AnyTensor > coeffs = std::vector< AnyTensor >(dimension(), zeros);
        coeffs[0] = values;

        return AnyTensor::concat(coeffs, 0);
    }

   AnyTensor MonomialBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        if (AnyScalar::is_double(x)) {
            return basis_evaluation<double>(AnyScalar::as_double(x));
        } else {
            return basis_evaluation<AnyScalar>(x);
        }
    }

    casadi_int MonomialBasisNode::dimension() const {
         return degree() + 1;
    }

    AnyTensor MonomialBasisNode::evaluation_grid() const {
        std::vector< double > ret = {};
        for (casadi_int i = 0; i < dimension(); i++) {
            ret.push_back( static_cast<double>(i));
        }
        return Tensor<casadi::DM>(casadi::DM(ret) ,std::vector< casadi_int >{dimension(),1} );
    }

    Basis MonomialBasisNode::derivative(casadi_int order, AnyTensor& T) const {
        if (order > degree()) {
            // User tries to take a derivative which is of higher order than the basis,
            // returns all 0
            casadi_int dim = dimension();
            std::vector<double> entries(1*(dim), 0);
            // Transformation tensor to apply on coefficients of function, all zero
            T = vertcat(entries).shape({1, dim});
            return MonomialBasis(0);
        } else {
            // Derivative is of lower order than basis
            casadi_int dim = dimension();  // number of basis functions in the basis
            casadi_int curr_degree = degree();
            casadi_int dim_new = dim-1;  // dimension
            // initialization of entries of transformation matrix
            std::vector<AnyScalar> entries(dim*dim, 0);
            for (casadi_int i=0; i<dim; i++) {
                entries[i*(dim+1)] = 1;  // to make eye matrix
            }
            AnyTensor T_ = vertcat(entries).shape({dim, dim});  // initialize tensor to multiply
            Basis new_basis = MonomialBasis(curr_degree-order);  // basis to return
            for (casadi_int j = 0; j<order; j++) {  // loop over order
                entries.resize((dim_new)*(dim));
                std::fill(entries.begin(), entries.end(), 0);
                for (casadi_int i=1; i<dim; i++) {
                    entries[i*(dim) -1] = i;
                    // Makes e.g. matrix [0 1 0 0 ; 0 0 2 0 ; 0 0 0 3] for degree 3 basis, order 1
                }
                // multiply transformation matrices, for higher order
                T_ = mtimes(vertcat(entries).shape({dim_new, dim}), T_);
                dim -= 1;
                dim_new -= 1;
            }
            T = T_;
            return new_basis;
        }
    }

    Basis MonomialBasisNode::antiderivative(casadi_int order, AnyTensor& T) const {
        casadi_int dim = dimension();
        casadi_int dim_new = dim;
        casadi_int deg = degree();
        // construct coefficient transformation matrix
        std::vector<AnyScalar> data(dim*dim, 0);
        for (casadi_int i=0; i<dim; i++) {
            data[i*(dim+1)] = 1.;
        }
        AnyTensor T_ = vertcat(data).shape({dim, dim});
        AnyScalar val;
        for (casadi_int k=0; k<order; k++) {
            deg++;
            dim_new++;
            data.resize(dim_new*dim);
            std::fill(data.begin(), data.end(), 0);
            for (casadi_int i=0; i<dim; i++) {
                data[i*(dim_new+1)+1] = 1./(i+1);
            }
            T_ = mtimes(vertcat(data).shape({dim_new, dim}), T_);
            dim++;
        }
        T = T_;
        // construct new basis
        return MonomialBasis(deg);
    }

    AnyTensor MonomialBasisNode::integral(const Interval& dom) const {
        if (dom == domain()) {
            return vertcat(std::vector<double>(dimension(),
                std::numeric_limits<double>::infinity())).shape({1, dimension()});
        } else {
            AnyTensor T;
            Basis basis_int = antiderivative(1, T);
            return (basis_int(AnyVector( dom.max() )) -
                basis_int(AnyVector( dom.min() ))).shape({1, dimension()+1}).mtimes(T);
        }
    }

    Basis MonomialBasisNode::degree_elevation(casadi_int elevation, AnyTensor& T) const {
        casadi_int dim = dimension();
        casadi_int dim_new = dim+elevation;
        // construct coefficient transformation matrix
        std::vector<AnyScalar> data(dim_new*dim, 0);
        for (casadi_int i=0; i<dim; i++) {
            data[i*(dim_new+1)] = 1.;
        }
        Basis new_basis = MonomialBasis(degree()+elevation);
        T = vertcat(data).shape({dim_new, dim});
        return new_basis;
    }

} // namespace spline
