#ifndef CPP_SPLINE_SUBBASISDUMMY
#define CPP_SPLINE_SUBBASISDUMMY

#include <casadi/casadi.hpp>
#include <string>

#include "SubBasis.h"

namespace spline {

#ifndef SWIG

    class SubMonomialBasis;
    class SubBSplineBasis;
    class SubBasisDummyNode : public SubBasisNode {
    public:
        virtual SubBasis operator+(const SubBasis& other) const ;
        virtual SubBasis operator+(const SubMonomialBasis& other) const ;
        virtual SubBasis operator+(const SubBSplineBasis& other) const ;

        virtual SubBasis operator*(const SubBasis& other) const ;
        virtual SubBasis operator*(const SubMonomialBasis& other) const ;
        virtual SubBasis operator*(const SubBSplineBasis& other) const ;

        virtual std::string getRepresentation() const ;
    };


#endif // SWIG
    class SubBasisDummy : public SubBasis {
    public:
#ifndef SWIG
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        SubBasisDummyNode* get() const ;
        SubBasisDummyNode* operator->() const ;
#endif // SWIG

        SubBasisDummy() ;
        virtual std::string getRepresentation() const ;
    };
} // namespace spline

#endif //CPP_SPLINE_SUBBASISDUMMY
