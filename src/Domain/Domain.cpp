#include "Domain.h"
#include "../common.h"

namespace spline {

    DomainNode* Domain::get() const { return static_cast<DomainNode*>(SharedObject::get()); }
    DomainNode* Domain::operator->() const { return get(); }

    std::string Domain::type() const { return (*this)->type() ;};
    std::string DomainNode::type() const {return "Domain";};

    std::string Domain::to_string() const { return (*this)->to_string() ;};
    std::string DomainNode::to_string() const {return "Domain";};

    Domain Domain::intersection(const Domain & dom) const { return (*this)->intersection(dom);}
    Domain Domain::intersection(const Interval & dom) const { return (*this)->intersection(dom);}

    bool Domain::operator==(const Domain& other) const {return (*this)->operator==(other); }
    bool Domain::operator==(const Interval& other) const {return (*this)->operator==(other); }
    bool DomainNode::operator==(const Domain& other) const {return false; }
    bool DomainNode::operator==(const Interval& other) const {return false; }

} // namespace spline
