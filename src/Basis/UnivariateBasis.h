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

#ifndef SWIG

    class UnivariateBasisNode : public BasisNode {
    public:
        UnivariateBasisNode(int degree, const Domain& domain);

        int degree() const;
        int n_inputs() const;
        virtual std::string to_string() const override;
        virtual std::string type() const override {return "UnivariateBasis";}

        virtual AnyTensor integral(const Domain& domain) const override;
        virtual AnyTensor integral(const Interval& domain) const = 0;
        virtual Basis kick_boundary(const Domain& domain, AnyTensor& T) const override;
        virtual Basis kick_boundary(const Interval& domain, AnyTensor& T) const;

    protected:
        int degree_;
    };

#endif // SWIG

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
