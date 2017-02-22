#include "TensorDomain.h"
#include "../common.h"

namespace spline {

    TensorDomainNode* TensorDomain::get() const { return static_cast<TensorDomainNode*>(SharedObject::get()); }
    TensorDomainNode* TensorDomain::operator->() const { return get(); }

    TensorDomainNode::TensorDomainNode(const std::vector< Domain >& dom) :
        domains_(dom), arguments_(std::vector<Argument>{}) {}
    TensorDomainNode::TensorDomainNode(const std::vector< Domain >& dom, const std::vector< Argument >& args) :
        domains_(dom), arguments_(args) {}

    TensorDomain::TensorDomain() {
        assign_node(new TensorDomainNode(std::vector< Domain >{}));
    }

    TensorDomain::TensorDomain(const Domain & allSubDomain) {
        assign_node(new TensorDomainNode(std::vector< Domain > {allSubDomain}));
    }

    TensorDomain::TensorDomain(const std::vector< Domain >& allSubDomain) {
        assign_node(new TensorDomainNode(allSubDomain));
    }

    TensorDomain::TensorDomain(const std::vector< Domain >& allSubDomain,
        const std::vector< Argument >& args) {
        assign_node(new TensorDomainNode(allSubDomain, args));
    }

    TensorDomain::TensorDomain(const std::vector< std::vector< AnyScalar > >& intervals,
        const std::vector< Argument >& args) {
        std::vector<Domain> domains(intervals.size());
        for (int i=0; i<intervals.size(); i++) {
            domains[i] = Interval(intervals[i]);
        }
        assign_node(new TensorDomainNode(domains, args));
    }

    TensorDomain::TensorDomain(const std::vector< std::vector< AnyScalar > >& intervals) {
        TensorDomain(intervals, std::vector<Argument> {});
    }

    bool TensorDomain::operator==(const TensorDomain& other) const {
        return (*this)->operator==(other);
    }

    bool TensorDomainNode::operator==(const TensorDomain& other) const {
        if (n_domains() != other.n_domains()) {
            return false;
        }
        if (! (hasArguments() && other.hasArguments())) {
            for (int i=0; i<n_domains(); i++) {
                if (!(domain(i) == other.domain(i))) {
                    return false;
                }
            }
        } else {
            Argument arg;
            for (int i=0; i<n_domains(); i++) {
                arg = getSubArgument(i);
                if (!(domain(i) == other.domain(arg))) {
                    return false;
                }
            }
        }
        return true;
    }

    int TensorDomain::n_domains() const { return (*this)->n_domains(); }
    int TensorDomainNode::n_domains() const {

        return domains_.size();
    }

    std::vector< spline::Argument > TensorDomain::arguments() const {
        return (*this)->arguments();
    }
    std::vector< Argument > TensorDomainNode::arguments() const {
        return arguments_;
    }

    spline::Argument TensorDomain::getSubArgument(int index) const {
        return (*this)->getSubArgument(index);
    }
    Argument TensorDomainNode::getSubArgument(int index) const {
        return arguments_[index];
    }

    int TensorDomain::indexArgument(Argument a) const { return (*this)->indexArgument(a); }
    int TensorDomainNode::indexArgument(Argument a) const {
        auto it = std::find(arguments_.begin(), arguments_.end(), a);
        int index;
        if (it == arguments_.end()) {
            index = -1;
        } else {
            index = std::distance(arguments_.begin(), it);
        }
        return  index;
    }

    bool TensorDomain::hasArguments() const { return (*this)->hasArguments();}
    bool TensorDomainNode::hasArguments() const {
        return arguments_.size() > 0;
    }

    std::vector< Domain > TensorDomain::domains() const { return (*this)->domains (); }
    std::vector< Domain > TensorDomainNode::domains() const {
        return domains_;
    }

    Domain TensorDomain::domain() const { return (*this)->domain(); }
    Domain TensorDomainNode::domain() const {
        spline_assert(n_domains() == 1);
        return domains()[0];
    }

    Domain TensorDomain::domain(const Argument& a) const { return (*this)->domain(a); }
    Domain TensorDomainNode::domain(const Argument& a) const {
        int index = indexArgument(a);
        spline_assert(index >= 0);
        return domain(index);
    }

    Domain TensorDomain::domain(const Index& index) const { return (*this)->domain(index); }
    Domain TensorDomainNode::domain(const Index& index) const {
        int ind = index.concrete(arguments());
        spline_assert(ind < n_domains());
        return domains()[ind];
    }


    /* std::string TensorDomainNode::getRepresentation() const { */
    /*     return "TensorDomain"  + std::to_string(arguments_.size()) + */
    /*         std::to_string(allSubDomain.size()); */
    /* } */

    /* std::string TensorDomain::getRepresentation() const { return (*this)->getRepresentation() ;} */
    /* std::ostream& operator<<(std::ostream &stream, const TensorDomain& base) { */
    /*     return stream << base.getRepresentation(); */
    /* } */

    TensorDomain TensorDomain::intersection(const TensorDomain& other) const {
        return (*this)->intersection(other);
    }
    TensorDomain TensorDomainNode::intersection(const TensorDomain& other) const {
        spline_assert_message(n_domains() == other.n_domains(),
            "Incompatible number of sub domains: " << n_domains() <<
            " != " << other.n_domains() << ".")
        std::vector< Domain > int_domains(n_domains());
        if (! (hasArguments() && other.hasArguments()) ) {
            for (int i=0; i<n_domains(); i++) {
                int_domains[i] = domain(i).intersection(other.domain(i));
            }
            return TensorDomain(int_domains);
        } else {
            Argument arg;
            for (int i=0; i<n_domains(); i++) {
                arg = getSubArgument(i);
                spline_assert_message(other.indexArgument(arg) >= 0,
                    "Domains have incompatible arguments!")
                int_domains[i] = domain(i).intersection(other.domain(arg));
            }
            return TensorDomain(int_domains, arguments());
        }
    }

} // namespace spline
