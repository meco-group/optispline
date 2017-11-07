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

        virtual std::string type() const;
        virtual std::string to_string() const override;

        Coefficient add_trival_dimension(int i) const;
        int n_coeff() const;
        // Coefficient operator+ (Coefficient & other) const;
        AnyTensor data() const {return data_;}

        AnyTensor transform(const AnyTensor& T, const NumericIndexVector& direction) const;
        AnyTensor transform(const std::vector<AnyTensor>& T, const NumericIndexVector& direction) const;
        Coefficient transpose() const;
        Coefficient rm_direction(const std::vector<NumericIndex>& indices) const;

        Coefficient reshape(const std::vector< int >& shape) const;
        Coefficient trace() const;
        Coefficient sum(int axis) const;
        Coefficient sum() const;

        Coefficient to_matrix_valued() const;

        bool is_true_scalar() const ;

        bool is_scalar() const;
        bool is_vector() const;
        bool is_column() const;
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

        std::string type() const;
        Coefficient operator-() const ;
        AnyTensor data() const;
        AnyTensor data(const NumericIndex& k) const;
        /// Transform a direction of the coeffient's data tensor using given transformation matrix
        AnyTensor transform(const AnyTensor& T) const;
        AnyTensor transform(const AnyTensor& T, const NumericIndexVector& direction) const;
        AnyTensor transform(const AnyTensor& T, const NumericIndex& direction) const;
        AnyTensor transform(const std::vector<AnyTensor>& T, const NumericIndexVector& direction) const;
        Coefficient transpose() const;
        Coefficient rm_direction(const std::vector<NumericIndex>& indices) const;
        Coefficient reshape(const std::vector< int >& shape) const;
        Coefficient trace() const;
        Coefficient sum(NumericIndex axis) const;
        Coefficient sum() const;

        Coefficient to_matrix_valued() const;

        static Coefficient cat(const NumericIndex& index, const std::vector< Coefficient >& coefs);

        bool is_true_scalar() const ;

        bool is_scalar() const;
        bool is_vector() const;
        bool is_column() const;

    };
} // namespace spline

#endif //CPP_SPLINE_COEFFICIENTS_COEFFICIENT_H
