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

namespace spline {

class TensorDomain;

#ifndef SWIG

class TensorDomainNode : public SharedObjectNode {
    public:
        TensorDomainNode(const std::vector< Domain >& allDomain);
        TensorDomainNode(const std::vector< Domain >& allDomain, const std::vector< std::string >& args);
        std::vector< std::string > arguments() const;
        std::string argument(int index) const;
        int indexArgument(std::string a) const;
        bool hasArguments() const;

        virtual std::string to_string() const ;
        virtual std::string type() const;

        int n_domains() const;
        std::vector< Domain > domains() const;
        Domain domain() const;
        Domain domain(const std::string& a) const;
        Domain domain(const Argument& ind) const;
        TensorDomain intersection(const TensorDomain& other) const;
        bool operator==(const TensorDomain& other) const;

    private:
        std::vector< Domain > domains_;
        std::vector< std::string > arguments_;
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
        TensorDomain(const std::vector< Domain >& allDomain, const std::vector< std::string >& args);

        std::vector< std::string > arguments() const;
        std::string argument(int index) const;
        int indexArgument(std::string a) const;
        bool hasArguments() const;

        std::string to_string() const ;
        std::string type() const;

        int n_domains() const;
        std::vector< Domain > domains() const;
        Domain domain() const;
        Domain domain(const std::string& a) const;
        Domain domain(const Argument& ind) const;
        TensorDomain intersection(const TensorDomain& other) const;
        bool operator==(const TensorDomain& other) const;
};

}   // namespace spline
#endif  // SRC_DOMAIN_TENSORDOMAIN_H_
