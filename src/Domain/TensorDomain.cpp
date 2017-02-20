#include "TensorDomain.h"
#include "Domain.h"
#include "../common.h"

namespace spline {

    TensorDomainNode* TensorDomain::get() const {
        return static_cast<TensorDomainNode*>(SharedObject::get());
    }

    TensorDomainNode* TensorDomain::operator->() const { return get(); }

    TensorDomain::TensorDomain() {
        assign_node(new TensorDomainNode(std::vector< Domain >{}));
    }

    TensorDomain::TensorDomain(const Domain & allSubDomain) {
        assign_node(new TensorDomainNode(std::vector< Domain > {allSubDomain}));
    }

    TensorDomain::TensorDomain(const std::vector< Domain >& allSubDomain) {
        assign_node(new TensorDomainNode(allSubDomain));
    }

    int TensorDomain::n_domains() const { return (*this)->n_domains();}
    int TensorDomainNode::n_domains() const {
        return allDomains.size();
    }

    /* std::string TensorDomainNode::getRepresentation() const { */
    /*     return "TensorDomain"  + std::to_string(allArguments.size()) + */
    /*         std::to_string(allSubDomain.size()); */
    /* } */

    /* std::string TensorDomain::getRepresentation() const { return (*this)->getRepresentation() ;} */
    /* std::ostream& operator<<(std::ostream &stream, const TensorDomain& base) { */
    /*     return stream << base.getRepresentation(); */
    /* } */

    TensorDomain TensorDomain::intersection() const { return (*this)->dimension ();}
    TensorDomain TensorDomainNode::intersection() const {
        spline_assert_message(false, "test lengtes en zo");
        return TensorDomain();
    }

} // namespace spline
