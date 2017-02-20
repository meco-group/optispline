#ifndef SRC_DOMAIN_DOMAIN_H_
#define SRC_DOMAIN_DOMAIN_H_

#include <iostream>
#include <string>
#include <vector>
#include <any_tensor.hpp>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {

class Domain;
class Interval;

#ifndef SWIG

class DomainNode : public SharedObjectNode {
    public:
        virtual Domain intersection(const Domain & dom) const = 0;
        virtual Domain intersection(const Interval & dom) const = 0;

        virtual std::string getRepresentation() const ;
    private:
        std::vector< Domain > allDomains;
};

#endif // SWIG

class Domain : public SharedObject{
public:
#ifndef SWIG

        DomainNode* get() const;
        DomainNode* operator->() const;

#endif // SWIG
        Domain();
        Domain intersection(const Domain & dom) const;
        Domain intersection(const Interval & dom) const;

        std::string getRepresentation() const ;
#ifndef SWIG
        inline friend
            std::ostream& operator<<(std::ostream &stream, const Basis& obj) {
                return stream << obj.getRepresentation();
            }
#endif // SWIG
};

}   // namespace spline
#endif  // SRC_DOMAIN_DOMAIN_H_
