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
} // namespace spline
