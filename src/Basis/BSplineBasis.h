#ifndef CPP_SPLINE_SUBBSPLINEBASIS_H
#define CPP_SPLINE_SUBBSPLINEBASIS_H

#include <any_tensor.hpp>
#include <vector>

#include <casadi/core/callback.hpp>

#include "Basis.h"
#include "BSplineBasisNode.h"
#include "UnivariateBasis.h"

namespace spline{
    class MonomialBasis;

    class BSplineBasis : public UnivariateBasis {

    public:

#ifndef SWIG
        BSplineBasis(){};
        BSplineBasisNode* get() const ;
        BSplineBasisNode* operator->() const ;
        BSplineBasis(const std::vector<AnyScalar>& knots, casadi_int degree);
        BSplineBasis(const std::vector<AnyScalar>& bounds, casadi_int degree, casadi_int numberOfIntervals);
#endif // SWIG

        BSplineBasis(const AnyVector& knots, casadi_int degree) :
          BSplineBasis(knots.to_scalar_vector(), degree) {};
        BSplineBasis(const AnyVector& bounds, casadi_int degree, casadi_int numberOfIntervals) :
          BSplineBasis(bounds.to_scalar_vector(), degree, numberOfIntervals) {};

        /** \brief Construct basis using knots with single multiplicity
        *
        * The repetition at the start and end of the knot list will be done by this method.
        */
        static BSplineBasis from_single(const AnyVector& knots, casadi_int degree);

        std::vector<AnyScalar> knots() const;
        std::vector<AnyScalar> greville() const;

    };

}

#endif //CPP_SPLINE_SUBBSPLINEBASIS_H
