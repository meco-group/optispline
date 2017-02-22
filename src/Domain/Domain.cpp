#include "Domain.h"
#include "../common.h"

namespace spline {

    DomainNode* Domain::get() const { return static_cast<DomainNode*>(SharedObject::get()); }
    DomainNode* Domain::operator->() const { return get(); }

    std::string Domain::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::string DomainNode::getRepresentation() const {return "Domain";};

    Domain Domain::intersection(const Domain & dom) const { return (*this)->intersection(dom);}
    Domain Domain::intersection(const Interval & dom) const { return (*this)->intersection(dom);}

    bool Domain::operator==(const Domain& other) const {return (*this)->operator==(other); }
    bool Domain::operator==(const Interval& other) const {return (*this)->operator==(other); }
    bool DomainNode::operator==(const Domain& other) const {return false; }
    bool DomainNode::operator==(const Interval& other) const {return false; }

} // namespace spline
