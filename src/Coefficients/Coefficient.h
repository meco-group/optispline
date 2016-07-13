#ifndef CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
#define CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H

#include <string>
#include <vector>
#include <tensor.hpp>

namespace spline {
    typedef DT AnyTensor;
    class Coefficient {
    public:
        // Coefficient(){}
        // template <class D>
        Coefficient (const AnyTensor& t) : data(t){}
        // Coefficient (const Tensor<DM> t){}

        // const std::vector< int > getShape() const;

        // Coefficient operator+ (Coefficient & other) const;
        //  o

    // private:
        const AnyTensor& data;



    }; 
} // namespace spline

#endif //CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
