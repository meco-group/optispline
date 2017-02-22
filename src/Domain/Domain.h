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
        DomainNode(const std::vector<AnyScalar> & data) : data_(data) {}
        std::vector<AnyScalar> data() const { return data_; }
        virtual std::string getRepresentation() const ;
        virtual Domain intersection(const Domain & dom) const = 0;
        virtual Domain intersection(const Interval & dom) const = 0;
        virtual bool operator==(const Domain& other) const;
        virtual bool operator==(const Interval& other) const;

    protected:
        std::vector<AnyScalar> data_;

};

#endif // SWIG

class Domain : public SharedObject{
public:
#ifndef SWIG

        DomainNode* get() const;
        DomainNode* operator->() const;
#endif // SWIG
        std::vector<AnyScalar> data() const { return (*this)->data(); }
        std::string getRepresentation() const ;
        Domain intersection(const Domain & dom) const;
        Domain intersection(const Interval & dom) const;
        bool operator==(const Domain& other) const;
        bool operator==(const Interval& other) const;

#ifndef SWIG
        inline friend
            std::ostream& operator<<(std::ostream &stream, const Domain& obj) {
                return stream << obj.getRepresentation();
            }
#endif // SWIG
};

}   // namespace spline
#endif  // SRC_DOMAIN_DOMAIN_H_
