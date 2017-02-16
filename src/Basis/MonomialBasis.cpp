#include "MonomialBasis.h"

#include "operations/operationsBasis.h"

namespace spline {
    std::string MonomialBasisNode::getRepresentation() const {
        return "MonomialBasis";
    }

    std::string MonomialBasis::getRepresentation() const {
        return (*this)->getRepresentation() ;
    }

    MonomialBasisNode* MonomialBasis::get() const {
        return static_cast<MonomialBasisNode*>(SharedObject::get());
    };
    MonomialBasisNode* MonomialBasis::operator->() const { return get(); }

    MonomialBasis::MonomialBasis(int degree)  {
        assign_node(new MonomialBasisNode(degree));
    }

    Basis MonomialBasisNode::operator+ (const Basis& other) const {
        return other + shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator+ (const DummyBasis& other) const {
        return shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator+ (const BSplineBasis& other) const {
        return plusSubBasis (shared_from_this<MonomialBasis>(), other);
    }

    Basis MonomialBasisNode::operator+ (const MonomialBasis& other) const {
        return plusSubBasis (shared_from_this<MonomialBasis>(), other);
    }

    Basis MonomialBasisNode::operator* (const Basis& other) const {
        return other * shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator* (const DummyBasis& other) const {
        return shared_from_this<MonomialBasis>();
    }

    Basis MonomialBasisNode::operator* (const BSplineBasis& other) const {
        return timesSubBasis (shared_from_this<MonomialBasis>(), other);
    }

    Basis MonomialBasisNode::operator* (const MonomialBasis& other) const {
        return timesSubBasis (shared_from_this<MonomialBasis>(), other);
    }

    AnyTensor MonomialBasisNode::const_coeff_tensor(const AnyTensor& t) const {
        //push 1 for size of tensor
        std::vector< int > coeff_size = {1};
        for (int i = 0; i < t.dims().size(); i++) {
            coeff_size.push_back(t.dims()[i]);
        }

        // make single basis function coefficient and repeat
        AnyTensor values = t.shape(coeff_size);
        // make zero valued coefficients for higher order basis functions
        AnyTensor zeros = AnyTensor::repeat(AnyTensor(AnyScalar(0)),coeff_size);

        std::vector< AnyTensor > coeffs;
        coeffs.push_back(values);
        for (int i = 1; i < getLength(); i++) {
            coeffs.push_back(zeros);
        }

        return AnyTensor::concat(coeffs,0);
    }

   AnyTensor MonomialBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        if (AnyScalar::is_double(x)) {
            return SubBasisEvalution<double>(AnyScalar::as_double(x));
        } else {
            return SubBasisEvalution<AnyScalar>(x);
        }
    }

    int MonomialBasisNode::getLength() const {
         return getDegree() + 1;
    }

    std::vector< std::vector < AnyScalar > > MonomialBasisNode::getEvaluationGrid() const {
      std::vector< std::vector < AnyScalar > > ret;
        for (int i = 0; i < getLength(); i++) {
            ret.push_back(std::vector<AnyScalar> {static_cast<double>(i)});
        }
        return ret;
    }

    Basis MonomialBasisNode::derivative(int order, AnyTensor& T) const {
        if (order > getDegree()){
            // User tries to take a derivative which is of higher order than the basis, returns all 0
            int dim = dimension();
            std::vector<double> entries(dim*(dim+1), 0);
            T = vertcat(entries).shape({dim, dim+1});  // Transformation tensor to apply on coefficients of function, all zero
            return MonomialBasis(0);
        }
        else{
            // Derivative is of lower order than basis
            int dim = dimension();  // number of basis functions in the basis
            int curr_degree = getDegree();
            int dim_new = dim-1;  // dimension
            std::vector<AnyScalar> entries(dim*dim,0);  // initialization of entries of transformation matrix
            for(int i=0; i<dim; i++){
                entries[i*(dim+1)] = 1;  // to make eye matrix
            }
            AnyTensor T_ = vertcat(entries).shape({dim,dim});  // initialize tensor to multiply
            Basis new_basis = MonomialBasis(curr_degree-order);  // basis to return
            for (int j = 0; j<order; j++){  // loop over order
                entries.resize((dim_new)*(dim));
                std::fill(entries.begin(), entries.end(), 0);
                for (int i=1; i<dim; i++){
                    entries[i*(dim) -1] = i;
                    // Makes e.g. matrix [0 1 0 0 ; 0 0 2 0 ; 0 0 0 3] for degree 3 basis, order 1
                }
                T_ = mtimes(vertcat(entries).shape({dim_new, dim}), T_);  // multiply transformation matrices, for higher order
                dim -= 1;
                dim_new -= 1;
            }
            T = T_;
            return new_basis;
        }
    }

} // namespace spline
