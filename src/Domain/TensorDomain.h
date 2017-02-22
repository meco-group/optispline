#ifndef SRC_DOMAIN_TENSORDOMAIN_H_
#define SRC_DOMAIN_TENSORDOMAIN_H_

#include <iostream>
#include <string>
#include <vector>
#include <any_tensor.hpp>

#include "Domain.h"

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {

class TensorDomain;

#ifndef SWIG

class TensorDomainNode : public SharedObjectNode {
    public:
        TensorDomainNode(const std::vector< Domain >& allDomain);
        int n_domains() const;
        TensorDomain intersection() const;
    private:
        std::vector< Domain > allDomains;
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

        int n_domains() const;
        TensorDomain intersection() const;
};

}   // namespace spline
#endif  // SRC_DOMAIN_TENSORDOMAIN_H_
