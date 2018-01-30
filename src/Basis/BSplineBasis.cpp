#include <sstream>
#include "BSplineBasis.h"
#include "BSplineBasisNode.h"

#include "operations/operationsBasis.h"

namespace spline {

    BSplineBasisNode* BSplineBasis::get() const {
      return static_cast<BSplineBasisNode*>(SharedObject::get());
    }

    BSplineBasisNode* BSplineBasis::operator->() const { return get(); }

    std::vector<AnyScalar> BSplineBasis::greville() const { return (*this)->greville(); }
    BSplineBasis::BSplineBasis(const std::vector<AnyScalar>& knots, casadi_int degree) {
        spline_assert_message(knots.size() > degree+1, "Incompatible dimensions." <<
            " Got " << knots.size() << " knots and degree "  << degree << ".");
        assign_node(new BSplineBasisNode(knots, degree));
    }

    std::vector<AnyScalar> linspace(const AnyScalar& start, const AnyScalar& stop, casadi_int N) {
        std::vector<AnyScalar> ret(N);
        spline_assert(N>=2);
        AnyScalar d = stop-start;

        for (casadi_int i = 0; i < N; ++i) {
            ret[i] = start + (i*d)/(N-1);
        }
        return ret;
    }

    BSplineBasis::BSplineBasis(const std::vector<AnyScalar>& bounds, casadi_int degree, casadi_int n_intervals)  {
        spline_assert(n_intervals>=2);
        std::vector<AnyScalar> knot_vector = linspace(bounds[0], bounds[1], n_intervals);

        knot_vector.insert(knot_vector.begin(), degree, knot_vector[0]);
        knot_vector.insert(knot_vector.end(), degree, knot_vector.back());

        assign_node(new BSplineBasisNode(knot_vector, degree));
    }

    std::vector<AnyScalar> BSplineBasis::knots() const { return (*this)->knots(); }

    BSplineBasis BSplineBasis::from_single(const AnyVector& knots, casadi_int degree) {
      AnyTensor head = AnyTensor::repeat(knots[0], {degree});
      AnyTensor tail = AnyTensor::repeat(knots[knots.size()-1], {degree});
      AnyVector k = AnyTensor::concat({head, knots, tail}, 0);
      return BSplineBasis(k, degree);
    }

} // namespace spline
