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

    Interval::Interval(const Domain& dom) {
        spline_assert_message(dom.data().size() == 2,
            "Domain can not be casted as Interval. Domain's data size = " <<
            dom.data().size() << ", while it should be 2.")
        assign_node(new IntervalNode(dom.data()));
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

    std::string IntervalNode::type() const {
        return "Interval";
    };

    std::string IntervalNode::to_string() const {
        if (vertcat(data()).is_DT()) {            
            std::stringstream min_ss;
            std::stringstream max_ss;
            min_ss << std::fixed << std::setprecision(2) << min().as_double();
            max_ss << std::fixed << std::setprecision(2) << max().as_double();
            return "interval [" + min_ss.str() + ", " +
                   max_ss.str() + "]";
        }
        else if (vertcat(data()).is_ST()) {
            return "interval [" + str(min().as_SX()) + ", " +
            str(max().as_SX()) + "]";
        }
        else if (vertcat(data()).is_MT()) {
            return "interval [" + str(min().as_MX()) + ", " +
            str(max().as_MX()) + "]";
        }
        return "Undefined interval";
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
