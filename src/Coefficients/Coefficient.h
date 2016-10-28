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

        std::vector< int > getShape() const;
        int getNumberCoefficents() const;

        // Coefficient operator+ (Coefficient & other) const;
        Coefficient operator-() const ;
        AnyTensor getData() const {return data;}
        std::string getRepresentation() const ;
        void repr() const { userOut() << getRepresentation() << std::endl;}
    public:
        AnyTensor data;
    };
} // namespace spline

#endif //CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
