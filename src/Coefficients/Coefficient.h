#ifndef CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
#define CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H

#include <string>
#include <vector>
#include <any_tensor.hpp>
#include "../Function/NumericIndex.h"

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {

#ifndef SWIG
    class Coefficient;

    class CoefficientNode : public SharedObjectNode {
    public:

        CoefficientNode(const AnyTensor& t) : data_(t) {}
        CoefficientNode(const std::vector< double >& v);

        std::vector< int > shape() const;
        std::vector< int > dimension() const;

        Coefficient add_trival_dimension(int i) const;
        int n_coeff() const;
        // Coefficient operator+ (Coefficient & other) const;
        AnyTensor data() const {return data_;}
        std::string getRepresentation() const ;
        void repr() const { userOut() << getRepresentation() << std::endl;}

        AnyTensor transform(const AnyTensor& T, const std::vector< NumericIndex> direction) const;
        AnyTensor transform(const std::vector<AnyTensor>& T, const std::vector<NumericIndex>& direction) const;
        Coefficient transpose() const;

        Coefficient cat(const NumericIndex& index, const std::vector< Coefficient >& coefs) const;
    private:
        AnyTensor data_;

    };
#endif


    class Coefficient : public SharedObject {
    public:

    #ifndef SWIG

        CoefficientNode* get() const ;
        CoefficientNode* operator->() const ;

        Coefficient add_trival_dimension(int i) const;
        int n_coeff() const;
    #endif // SWIG
        Coefficient();
        Coefficient(const AnyTensor& t);
        Coefficient(const std::vector< double >& v);

        std::vector< int > shape() const;  //
        std::vector< int > dimension() const;  // Related to the number of basis functions

        Coefficient operator-() const ;
        AnyTensor data() const;
        std::string getRepresentation() const;
        /// Transform a direction of the coeffient's data tensor using given transformation matrix
        AnyTensor transform(const AnyTensor& T) const;
        AnyTensor transform(const AnyTensor& T, const std::vector< NumericIndex> direction) const;
        AnyTensor transform(const AnyTensor& T, const NumericIndex& direction) const;
        AnyTensor transform(const std::vector<AnyTensor>& T, const std::vector<NumericIndex>& direction) const;
        Coefficient transpose() const;
        Coefficient cat(const NumericIndex& index, const std::vector< Coefficient >& coefs) const;
    };
} // namespace spline

#endif //CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
