#ifndef CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
#define CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H

#include <string>
#include <vector>
#include <any_tensor.hpp>

namespace spline {

    class Coefficient {
    public:
        // Coefficient(){}
        // template <class D>
        Coefficient (const AnyTensor& t) : data(t){}
        Coefficient (const std::vector< double >& v);

        Coefficient value(const OptistackSolver& sol) const;

        std::vector< int > getShape() const;
        int getNumberCoefficents() const;
        // const std::vector< int > getShape() const;

        // Coefficient operator+ (Coefficient & other) const;
        Coefficient operator-() const ;
        const AnyTensor getData(){return data;}
    public:
        const AnyTensor data;
    };
} // namespace spline

#endif //CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
