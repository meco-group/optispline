#include "Interval.h"
#include "../common.h"

namespace spline {

    IntervalNode* Interval::get() const { return static_cast<IntervalNode*>(SharedObject::get()); }
    IntervalNode* Interval::operator->() const { return get(); }

    IntervalNode::IntervalNode(const std::vector<AnyScalar>& data) {
        spline_assert_message(data.size() == 2,
            "Interval is determined by 2 elements (min and max), got " << data.size() << ".")
    }

    Interval::Interval() {
        assign_node(new IntervalNode(std::vector<AnyScalar>{0, 0}));
    }

    Interval::Interval(const AnyScalar& min, const AnyScalar& max) {
        assign_node(new IntervalNode(std::vector<AnyScalar>{min, max}));
    }

    Interval::Interval(const std::vector<AnyScalar>& data) {
        assign_node(new IntervalNode(data));
    }

    AnyScalar Interval::min() const {
        return (*this)->min();
    }
    AnyScalar IntervalNode::min() const {
        return data[0];
    }

    AnyScalar Interval::max() const {
        return (*this)->max();
    }
    AnyScalar IntervalNode::max() const {
        return data[1];
    }

    std::string IntervalNode::getRepresentation() const { return "Interval"; };

    Domain IntervalNode::intersection(const Domain & other) const {
        return other.intersection(shared_from_this<Interval>());
    }

    Domain IntervalNode::intersection(const Interval & other) const {
        AnyScalar min2 = AnyScalar::max(min(), other.min());
        AnyScalar max2 = AnyScalar::min(max(), other.max());
        AnyScalar factor = AnyScalar::max(max2 - min2, 0.)/(max2 - min2);
        return Interval(min2*factor, max2*factor);
    }

} // namespace spline
