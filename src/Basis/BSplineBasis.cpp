#include <sstream>
#include "BSplineBasis.h"

/* #include "operations/operationsBasis.h" */

namespace spline {

    BSplineBasisNode* BSplineBasis::get() const {
      return static_cast<BSplineBasisNode*>(SharedObject::get());
    }

    BSplineBasisNode* BSplineBasis::operator->() const { return get(); }

    std::vector<AnyScalar> BSplineBasis::greville() const { return (*this)->greville(); }
    BSplineBasis::BSplineBasis(const std::vector<AnyScalar>& knots, int degree) {
        spline_assert_message(knots.size() > degree+1, "Incompatible dimensions." <<
            " Got " << knots.size() << " knots and degree "  << degree << ".");
        assign_node(new BSplineBasisNode(knots, degree));
    }

    BSplineBasis::BSplineBasis(const std::vector<AnyScalar>& bounds, int degree, int n_intervals)  {
        int n_knots = 2*degree + n_intervals;
        std::vector<AnyScalar> knot_vector(n_knots);

/*         for (int i = 1; i < n_intervals - 1; ++i) { */
/*             knot_vector[degree + i] = (i*bounds[0] + (n_intervals - i - 1) *bounds[1])/(n_intervals - 1.0); */
/*         } */

        for (int i = 1; i < n_intervals - 1; ++i) {
            double f = static_cast<double>(i)/(n_intervals-1);
            knot_vector[degree + i] = bounds[0] + (bounds[1] - bounds[0])*f;
        }

        for (int i = 0; i < degree + 1; ++i) {
            knot_vector[i] = bounds[0];
            knot_vector[n_knots - i - 1] = bounds[1];
        }
      assign_node(new BSplineBasisNode(knot_vector, degree));
    }

    std::vector<AnyScalar> BSplineBasis::knots() const { return (*this)->knots(); }

    BSplineBasis BSplineBasis::from_single(const AnyVector& knots, int degree) {
      AnyTensor head = AnyTensor::repeat(knots[0], {degree});
      AnyTensor tail = AnyTensor::repeat(knots[knots.size()-1], {degree});
      AnyVector k = AnyTensor::concat({head, knots, tail}, 0);
      return BSplineBasis(k, degree);
    }

} // namespace spline
