#ifndef CPP_SPLINE_UNIVARIATEBASISNODE_H_
#define CPP_SPLINE_UNIVARIATEBASISNODE_H_

/* #include <casadi/casadi.hpp> */
/* #include <string> */

#include "../common.h"
#include "BasisNode.h"
#include "../Domain/Interval.h"

namespace spline {

/*     class MonomialBasisNode; */
/*     class BSplineBasisNode; */

/*     class MonomialBasis; */
/*     class BSplineBasis; */

    class UnivariateBasis;

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

} // namespace spline

#endif //CPP_SPLINE_UNIVARIATEBASISNODE_H_
