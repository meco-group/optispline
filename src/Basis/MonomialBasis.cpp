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

   AnyTensor MonomialBasisNode::operator() (const std::vector<AnyScalar> & x) const {
        assert(x.size()==n_inputs());
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

    Basis MonomialBasisNode::univariate_derivative(int order, AnyTensor& T) const {
        int curr_degree = this->getDegree();
        Basis new_basis = MonomialBasis(curr_degree-1);

        std::vector<int> coeffs;  // Get coeffs of new basis
        int count = 1;
        for (int i = 0; i<=curr_degree-1; i++){
            coeffs.push_back(count);
            count++;
        }
        std::vector<int> new_degree = {curr_degree-1};
        T = AnyTensor(DTensor(coeffs, new_degree));  // Transformation tensor to apply on coefficients of function
        return new_basis;
    }

} // namespace spline
