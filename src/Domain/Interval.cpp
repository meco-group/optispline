#include "Interval.h"
#include "../common.h"

namespace spline {

    IntervalNode* Interval::get() const { return static_cast<IntervalNode*>(SharedObject::get()); }
    IntervalNode* Interval::operator->() const { return get(); }

    std::string IntervalNode::getRepresentation() const {return "Interval";};

    Domain IntervalNode::intersection(const Domain & other) const {
        return other.intersection(shared_from_this<Interval>());
    }

    Domain IntervalNode::intersection(const Interval & dom) const {
        spline_assert_message(false, "missing interval-interval implementation"); //RUBEN
        return dom;
    }

} // namespace spline
