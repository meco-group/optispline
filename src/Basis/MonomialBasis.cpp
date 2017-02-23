#include "MonomialBasis.h"

#include "operations/operationsBasis.h"

namespace spline {
    std::string MonomialBasisNode::getRepresentation() const {
        return "MonomialBasis";
    }

    MonomialBasisNode* MonomialBasis::get() const {
        return static_cast<MonomialBasisNode*>(SharedObject::get());
    };
    MonomialBasisNode* MonomialBasis::operator->() const { return get(); }

    MonomialBasisNode::MonomialBasisNode(int degree) :
        UnivariateBasisNode(degree,
          Interval(-std::numeric_limits<double>::infinity(),
                   +std::numeric_limits<double>::infinity())) {};

    MonomialBasis::MonomialBasis(int degree)  {
        assign_node(new MonomialBasisNode(degree));
    }

    Basis MonomialBasisNode::operator+ (const Basis& other) const {
        return other + shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator+ (const EmptyBasis& other) const {
        return shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator+ (const BSplineBasis& other) const {
        return plus_basis (shared_from_this<MonomialBasis>(), other);
    }

    Basis MonomialBasisNode::operator+ (const MonomialBasis& other) const {
        return plus_basis (shared_from_this<MonomialBasis>(), other);
    }

    Basis MonomialBasisNode::operator* (const Basis& other) const {
        return other * shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator* (const EmptyBasis& other) const {
        return shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator* (const BSplineBasis& other) const {
        return times_basis (shared_from_this<MonomialBasis>(), other);
    }

    Basis MonomialBasisNode::operator* (const MonomialBasis& other) const {
        return times_basis (shared_from_this<MonomialBasis>(), other);
    }

    AnyTensor MonomialBasisNode::const_coeff_tensor(const AnyTensor& t) const {
        std::vector< int > coeff_size = t.dims();
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

    int MonomialBasisNode::dimension() const {
         return degree() + 1;
    }

    std::vector< std::vector < AnyScalar > > MonomialBasisNode::getEvaluationGrid() const {
      std::vector< std::vector < AnyScalar > > ret;
        for (int i = 0; i < dimension(); i++) {
            ret.push_back(std::vector<AnyScalar> {static_cast<double>(i)});
        }
        return ret;
    }

    Basis MonomialBasisNode::derivative(int order, AnyTensor& T) const {
        if (order > degree()) {
            // User tries to take a derivative which is of higher order than the basis,
            // returns all 0
            int dim = dimension();
            std::vector<double> entries(1*(dim), 0);
            // Transformation tensor to apply on coefficients of function, all zero
            T = vertcat(entries).shape({1, dim});
            return MonomialBasis(0);
        } else {
            // Derivative is of lower order than basis
            int dim = dimension();  // number of basis functions in the basis
            int curr_degree = degree();
            int dim_new = dim-1;  // dimension
            // initialization of entries of transformation matrix
            std::vector<AnyScalar> entries(dim*dim, 0);
            for (int i=0; i<dim; i++) {
                entries[i*(dim+1)] = 1;  // to make eye matrix
            }
            AnyTensor T_ = vertcat(entries).shape({dim, dim});  // initialize tensor to multiply
            Basis new_basis = MonomialBasis(curr_degree-order);  // basis to return
            for (int j = 0; j<order; j++) {  // loop over order
                entries.resize((dim_new)*(dim));
                std::fill(entries.begin(), entries.end(), 0);
                for (int i=1; i<dim; i++) {
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

    Basis MonomialBasisNode::antiderivative(int order, AnyTensor& T) const {
        int dim = dimension();
        int dim_new = dim;
        int deg = degree();
        // construct coefficient transformation matrix
        std::vector<AnyScalar> data(dim*dim, 0);
        for (int i=0; i<dim; i++) {
            data[i*(dim+1)] = 1.;
        }
        AnyTensor T_ = vertcat(data).shape({dim, dim});
        AnyScalar val;
        for (int k=0; k<order; k++) {
            deg++;
            dim_new++;
            data.resize(dim_new*dim);
            std::fill(data.begin(), data.end(), 0);
            for (int i=0; i<dim; i++) {
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
            return (basis_int({dom.max()}) -
                basis_int({dom.min()})).shape({1, dimension()+1}).mtimes(T);
        }
    }

    Basis MonomialBasisNode::degree_elevation(int elevation, AnyTensor& T) const {
        int dim = dimension();
        int dim_new = dim+elevation;
        int deg = degree();
        // construct coefficient transformation matrix
        std::vector<AnyScalar> data(dim_new*dim, 0);
        for (int i=0; i<dim; i++) {
            data[i*(dim_new+1)] = 1.;
        }
        Basis new_basis = MonomialBasis(degree()+elevation);
        T = vertcat(data).shape({dim_new, dim});
    }

} // namespace spline
