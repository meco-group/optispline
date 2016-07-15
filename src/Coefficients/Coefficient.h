#ifndef CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
#define CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H

#include <string>
#include <vector>
#include <tensor.hpp>

namespace spline {
    typedef DT AnyTensor;
    typedef double AnyScalar;
    class Coefficient {
    public:
        // Coefficient(){}
        // template <class D>
        Coefficient (const AnyTensor& t) : data(t){}
        Coefficient (const std::vector<AnyScalar>& v); 

        // const std::vector< int > getShape() const;

        // Coefficient operator+ (Coefficient & other) const;
        const AnyTensor getData(){return data;}
    public:
        const AnyTensor data;
    }; 
} // namespace spline

#endif //CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
