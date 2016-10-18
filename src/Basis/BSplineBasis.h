#ifndef CPP_SPLINE_BSPLINEBASIS_H
#define CPP_SPLINE_BSPLINEBASIS_H

#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "UnivariateBasis.h"

namespace spline{

    class MonomialBasis;

#ifndef SWIG

    class BSplineBasisNode : public UnivariateBasisNode {

    public:
        BSplineBasisNode (const std::vector<double >& knots, int degree);
        BSplineBasisNode (const std::vector<double >& bounds, int degree, int numberOfIntervals);

        void setKnots (std::vector<double>& knots) ;
        /// Return a string with a representation (for SWIG)
        virtual std::string getRepresentation() const ;

    private:

    };

#endif // SWIG

    class BSplineBasis : public UnivariateBasis {

    public:

#ifndef SWIG
        BSplineBasis(){};
        BSplineBasisNode* get() const ;
        BSplineBasisNode* operator->() const ;
#endif // SWIG

        BSplineBasis (const std::vector<double >& knots, int degree);
        BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfIntervals);

        void setKnots (std::vector<double> &knots) ;

        /// Return a string with a representation (for SWIG)
        virtual std::string getRepresentation() const ;

#ifndef SWIG
        /// Print a representation of the object to a stream (shorthand)
        inline friend
            std::ostream& operator<<(std::ostream &stream, const BSplineBasis& obj) {
                return stream << obj.getRepresentation();
            }
#endif // SWIG

    private:
    };
}

#endif //CPP_SPLINE_BSPLINEBASIS_H
