#ifndef SRC_DOMAIN_INTERVAL_H_
#define SRC_DOMAIN_INTERVAL_H_

#include <iostream>
#include <sstream> 
#include <iomanip> // setprecision
#include <string>
#include <vector>
#include <any_tensor.hpp>

#include "Domain.h"

namespace spline {

class Interval;

#ifndef SWIG

class IntervalNode : public DomainNode {
    public:
        IntervalNode(const std::vector<AnyScalar>& data);

        AnyScalar min() const;
        AnyScalar max() const;
        virtual std::string to_string() const ;
        virtual std::string type() const ;
        virtual Domain intersection(const Domain & dom) const;
        virtual Domain intersection(const Interval & dom) const override;
        virtual bool operator==(const Domain& other) const override;
        virtual bool operator==(const Interval& other) const override;
};

#endif // SWIG

class Interval : public Domain {
public:
#ifndef SWIG

        IntervalNode* get() const;
        IntervalNode* operator->() const;
#endif // SWIG
        Interval();
        Interval(const Domain& dom);
        Interval(const AnyScalar& min, const AnyScalar& max);
        Interval(const std::vector<AnyScalar>& data);
        AnyScalar min() const;
        AnyScalar max() const;


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
