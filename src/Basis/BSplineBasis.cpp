#include <math.h>       /* pow */
#include <numeric>      // std::accumulate -> greville
#include <sstream>
#include "BSplineBasis.h"
#include "SubBSplineBasis.h"

#include "operations/operationsBasis.h"

namespace spline{

    BSplineBasisNode* BSplineBasis::get() const { return static_cast<BSplineBasisNode*>(SharedObject::get()); };
    BSplineBasisNode* BSplineBasis::operator->() const { return get(); }

    std::string BSplineBasisNode::getRepresentation() const {
        std::stringstream s;
        s << "BSplineBasis object";
        return s.str();
    };
    std::string BSplineBasis::getRepresentation() const { return (*this)->getRepresentation() ;};

    BSplineBasis::BSplineBasis (const std::vector<double >& knots, int degree)  {
        assign_node(new BSplineBasisNode(knots, degree));
    }

    BSplineBasis::BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfIntervals)  {
        assign_node(new BSplineBasisNode(bounds,degree,numberOfIntervals));
    }

    BSplineBasisNode::BSplineBasisNode (const std::vector<double >& knots, int degree)
    : UnivariateBasisNode(SubBSplineBasis(knots, degree)) { }

    BSplineBasisNode::BSplineBasisNode (const std::vector<double >& bounds, int degree, int numberOfIntervals)
    : UnivariateBasisNode(SubBSplineBasis(bounds, degree, numberOfIntervals)) { }

    void BSplineBasis::setKnots (std::vector< double >& knots) { return (*this)->setKnots (knots); }
    void BSplineBasisNode::setKnots (std::vector< double >& knots) {
    }

} // namespace spline
