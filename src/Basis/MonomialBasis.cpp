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

    void MonomialBasisNode::getEvaluationGrid(
          std::vector< std::vector < AnyScalar > > * grid) const {
        for (int i = 0; i < getLength(); i++) {
            grid->push_back(std::vector<AnyScalar> {static_cast<double>(i)});
        }
    }
} // namespace spline
