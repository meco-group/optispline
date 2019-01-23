#include "TensorDomain.h"
#include "../common.h"

namespace spline {

    TensorDomainNode* TensorDomain::get() const { return static_cast<TensorDomainNode*>(SharedObject::get()); }
    TensorDomainNode* TensorDomain::operator->() const { return get(); }

    TensorDomainNode::TensorDomainNode(const std::vector< Domain >& dom) :
        domains_(dom), arguments_(std::vector<std::string>{}) {}
    TensorDomainNode::TensorDomainNode(const std::vector< Domain >& dom, const std::vector< std::string >& args) :
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
        const std::vector< std::string >& args) {
        assign_node(new TensorDomainNode(allSubDomain, args));
    }

    bool TensorDomain::operator==(const TensorDomain& other) const {
        return (*this)->operator==(other);
    }

    bool TensorDomainNode::operator==(const TensorDomain& other) const {
        if (n_domains() != other.n_domains()) {
            return false;
        }
        if (! (hasArguments() && other.hasArguments())) {
            for (casadi_int i=0; i<n_domains(); i++) {
                if (!(domain(i) == other.domain(i))) {
                    return false;
                }
            }
        } else {
            std::string arg;
            for (casadi_int i=0; i<n_domains(); i++) {
                arg = argument(i);
                if (!(domain(i) == other.domain(arg))) {
                    return false;
                }
            }
        }
        return true;
    }

    casadi_int TensorDomain::n_domains() const { return (*this)->n_domains(); }
    casadi_int TensorDomainNode::n_domains() const {

        return domains_.size();
    }

    std::vector< std::string > TensorDomain::arguments() const {
        return (*this)->arguments();
    }
    std::vector< std::string > TensorDomainNode::arguments() const {
        return arguments_;
    }

    std::string TensorDomain::argument(casadi_int index) const {
        return (*this)->argument(index);
    }
    std::string TensorDomainNode::argument(casadi_int index) const {
        return arguments_[index];
    }

    casadi_int TensorDomain::indexArgument(std::string a) const { return (*this)->indexArgument(a); }
    casadi_int TensorDomainNode::indexArgument(std::string a) const {
        auto it = std::find(arguments_.begin(), arguments_.end(), a);
        casadi_int index;
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

    Domain TensorDomain::domain(const std::string& a) const { return (*this)->domain(a); }
    Domain TensorDomainNode::domain(const std::string& a) const {
        casadi_int index = indexArgument(a);
        spline_assert(index >= 0);
        return domain(index);
    }

    Domain TensorDomain::domain(const Argument& index) const { return (*this)->domain(index); }
    Domain TensorDomainNode::domain(const Argument& index) const {
        casadi_int ind = index.concrete(arguments());
        spline_assert(ind < n_domains());
        return domains()[ind];
    }

    std::string TensorDomain::type() const {return (*this)->type(); }
    std::string TensorDomainNode::type() const {return "TensorDomain"; }

    std::string TensorDomainNode::to_string() const {
        const std::string n_domain = (domains_.size()==1) ? "domain" : "domains";

        std::string str_domain;  // domain info
        if (hasArguments()){
            for (casadi_int i=0 ; i<domains_.size(); i++) {
                str_domain += "\t";
                str_domain += argument(i) + ": ";
                str_domain += domains_[i].to_string();
                str_domain += "\n";
            }
            return "TensorDomain containing "  + std::to_string(domains_.size()) + " " + n_domain + " in " +
                std::to_string(arguments_.size()) + " arguments: \n " + str_domain;
        }
        else {
            for (casadi_int i=0 ; i<domains_.size(); i++) {
                str_domain += "\t";
                str_domain += domains_[i].to_string();
                str_domain += "\n";
            }
            return "TensorDomain containing "  + std::to_string(domains_.size()) + " " +
                    n_domain + ":\n" + str_domain;
        }
    }

    TensorDomain TensorDomain::intersection(const TensorDomain& other) const {
        return (*this)->intersection(other);
    }
    TensorDomain TensorDomainNode::intersection(const TensorDomain& other) const {
        spline_assert_message(n_domains() == other.n_domains(),
            "Incompatible number of sub domains: " << n_domains() <<
            " != " << other.n_domains() << ".")
        std::vector< Domain > int_domains(n_domains());
        if (! (hasArguments() && other.hasArguments()) ) {
            for (casadi_int i=0; i<n_domains(); i++) {
                int_domains[i] = domain(i).intersection(other.domain(i));
            }
            return TensorDomain(int_domains);
        } else {
            std::string arg;
            for (casadi_int i=0; i<n_domains(); i++) {
                arg = argument(i);
                spline_assert_message(other.indexArgument(arg) >= 0,
                    "Domains have incompatible arguments!")
                int_domains[i] = domain(i).intersection(other.domain(arg));
            }
            return TensorDomain(int_domains, arguments());
        }
    }

} // namespace spline
