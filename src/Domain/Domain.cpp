#include "Domain.h"
#include "../common.h"

namespace spline {

    DomainNode* Domain::get() const { return static_cast<DomainNode*>(SharedObject::get()); }
    DomainNode* Domain::operator->() const { return get(); }

    std::string Domain::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::string DomainNode::getRepresentation() const {return "Domain";};

    Domain Domain::intersection(const Domain & dom) const { return (*this)->intersection(dom);}
    Domain Domain::intersection(const Interval & dom) const { return (*this)->intersection(dom);}
} // namespace spline
