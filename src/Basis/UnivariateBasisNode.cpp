#include <vector>
#include "UnivariateBasisNode.h"

#include "operations/operationsBasis.h"
#include "../common.h"

namespace spline {
    UnivariateBasisNode::UnivariateBasisNode(int degree, const Domain& domain) :
    BasisNode(domain), degree_(degree) { }
    int  UnivariateBasisNode::degree() const { return degree_ ; }

    std::string UnivariateBasisNode::to_string() const { return "UnivariateBasis of degree " +
                                                         std::to_string(degree()) + " on " + domain().to_string();}

    int UnivariateBasisNode::n_inputs() const { return 1; }

    AnyTensor UnivariateBasisNode::integral(const Domain& domain) const {
        return integral(static_cast<Interval>(domain));
    }

    Basis UnivariateBasisNode::kick_boundary(const Domain& boundary, AnyTensor& T) const {
        return kick_boundary(static_cast<Interval>(boundary), T);
    }

    Basis UnivariateBasisNode::kick_boundary(const Interval& boundary, AnyTensor& T) const {
        spline_assert_message(false, type() << " can not kick its boundaries.");
        return Basis();
    }

}  // namespace spline
