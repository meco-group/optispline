#include "Interval.h"
#include "../common.h"

namespace spline {

    IntervalNode* Interval::get() const { return static_cast<IntervalNode*>(SharedObject::get()); }
    IntervalNode* Interval::operator->() const { return get(); }

    IntervalNode::IntervalNode(const std::vector<AnyScalar>& data) : DomainNode(data) {
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

    bool IntervalNode::operator==(const Domain& other) const {
        return other==(shared_from_this<Interval>());
    }

    bool IntervalNode::operator==(const Interval& other) const {
        if (vertcat(data()).is_DT() && vertcat(other.data()).is_DT()) {
            bool ret = true;
            ret = (min().as_double() == other.min().as_double());
            ret = (max().as_double() == other.max().as_double()) && ret;
            return ret;
        }
        return false;
    }

    AnyScalar Interval::min() const {
        return (*this)->min();
    }
    AnyScalar IntervalNode::min() const {
        return data_[0];
    }

    AnyScalar Interval::max() const {
        return (*this)->max();
    }
    AnyScalar IntervalNode::max() const {
        return data_[1];
    }

    std::string IntervalNode::getRepresentation() const {
        if (vertcat(data()).is_DT()) {
            return "Interval [" + std::to_string(min().as_double()) + ", " +
            std::to_string(max().as_double()) + "]";
        }
        return "Interval";
    };

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
