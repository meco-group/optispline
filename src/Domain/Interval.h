#ifndef SRC_DOMAIN_INTERVAL_H_
#define SRC_DOMAIN_INTERVAL_H_

#include <iostream>
#include <string>
#include <vector>
#include <any_tensor.hpp>

#include "Domain.h"

namespace spline {

class Interval;

#ifndef SWIG

class IntervalNode : public DomainNode {
    public:
        virtual Domain intersection(const Domain & dom) const = 0;
        virtual Domain intersection(const Interval & dom) const = 0;

        virtual std::string getRepresentation() const ;
    private:
        float data; //RUBEN
};

#endif // SWIG

class Interval : public Domain {
public:
#ifndef SWIG

        IntervalNode* get() const;
        IntervalNode* operator->() const;
#endif // SWIG

//RUBEN ik denk dat de volgende code niet nodig is voor een output te geven
/* #ifndef SWIG */
/*         inline friend */
/*             std::ostream& operator<<(std::ostream &stream, const Interval& obj) { */
/*                 return stream << obj.getRepresentation(); */
/*             } */
/* #endif // SWIG */
};

}   // namespace spline
#endif  // SRC_DOMAIN_DOMAIN_H_
