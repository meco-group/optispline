#ifndef CPP_SPLINE_UNIVARIATEBASIS_H
#define CPP_SPLINE_UNIVARIATEBASIS_H

#include <casadi/casadi.hpp>
#include <string>

#include "../common.h"
#include "Basis.h"
#include "../Domain/Interval.h"

namespace spline {

    class MonomialBasisNode;
    class BSplineBasisNode;

    class MonomialBasis;
    class BSplineBasis;

    class UnivariateBasisNode;
    class UnivariateBasis : public Basis {
    public:
#ifndef SWIG
        //   virtual std::vector<double> evaluationGrid (void) const = 0;
        UnivariateBasis() { };
        UnivariateBasisNode* get() const ;
        UnivariateBasisNode* operator->() const ;
#endif // SWIG

        int degree () const ;
    };
} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASIS_H
