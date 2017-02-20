#ifndef SRC_DOMAIN_TENSORDOMAIN_H_
#define SRC_DOMAIN_TENSORDOMAIN_H_

#include <iostream>
#include <string>
#include <vector>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {

class TensorDomain;
class Domain;

#ifndef SWIG

class TensorDomainNode : public SharedObjectNode {
    public:
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
        TensorDomain::TensorDomain(const std::vector< Domain >& allSubDomain);

        int n_domains() const; // Vector containing number of basis functions in each basi
        TensorDomain intersection() const;

}   // namespace spline

#endif  // SRC_DOMAIN_TENSORDOMAIN_H_
