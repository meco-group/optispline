#ifndef SRC_DOMAIN_TENSORDOMAIN_H_
#define SRC_DOMAIN_TENSORDOMAIN_H_

#include <iostream>
#include <string>
#include <vector>
#include <any_tensor.hpp>

#include "Domain.h"
#include "Interval.h"
#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"
#include "../Function/Argument.h"
#include "../Function/Index.h"

namespace spline {

class TensorDomain;

#ifndef SWIG

class TensorDomainNode : public SharedObjectNode {
    public:
        TensorDomainNode(const std::vector< Domain >& allDomain);
        TensorDomainNode(const std::vector< Domain >& allDomain, const std::vector< Argument >& args);
        std::vector< spline::Argument > arguments() const;
        spline::Argument getSubArgument(int index) const;
        int indexArgument(Argument a) const;
        bool hasArguments() const;

        int n_domains() const;
        std::vector< Domain > domains() const;
        Domain domain() const;
        Domain domain(const Argument& a) const;
        Domain domain(const Index& ind) const;
        TensorDomain intersection(const TensorDomain& other) const;
    private:
        std::vector< Domain > allDomains;
        std::vector< Argument > allArguments;
};

#endif // SWIG

class TensorDomain : public SharedObject{
public:
#ifndef SWIG

        TensorDomainNode* get() const;
        TensorDomainNode* operator->() const;

#endif // SWIG
        TensorDomain();
        TensorDomain(const Domain & allSubDomain);
        TensorDomain(const std::vector< Domain >& allDomain);
        TensorDomain(const std::vector< Domain >& allDomain, const std::vector< Argument >& args);
        TensorDomain(const std::vector< std::vector<AnyScalar> >& intervals);
        // TensorDomain(const std::vector< std::vector< AnyScalar > >& intervals, const std::vector< Argument >& args);

        std::vector< spline::Argument > arguments() const;
        spline::Argument getSubArgument(int index) const;
        int indexArgument(Argument a) const;
        bool hasArguments() const;

        int n_domains() const;
        std::vector< Domain > domains() const;
        Domain domain() const;
        Domain domain(const Argument& a) const;
        Domain domain(const Index& ind) const;
        TensorDomain intersection(const TensorDomain& other) const;
};

}   // namespace spline
#endif  // SRC_DOMAIN_TENSORDOMAIN_H_
