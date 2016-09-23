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

        std::vector<double>& getKnots ();
        const std::vector<double>& getKnots () const;
        void setKnots (std::vector<double>& knots) ;
        //
        //   std::vector<double> greville () const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const;
        //   BSplineBasis addKnots(const std::vector<double> newKnots, bool unique = false) const;
        //
        /// Return a string with a representation (for SWIG)
        virtual std::string getRepresentation() const ;

	virtual BSplineBasis castBSpline() const;
	
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

//  TODO(jgillis) moet ik de variant met const houden??
        std::vector<double> &getKnots ();
        const std::vector<double> &getKnots () const;
        void setKnots (std::vector<double> &knots) ;
	
        //   std::vector<double> greville () const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const;
        //   BSplineBasis addKnots(const std::vector<double> newKnots, bool unique = false) const;
        //
        /// Return a string with a representation (for SWIG)
        virtual std::string getRepresentation() const ;

	virtual BSplineBasis castBSpline() const;
#ifndef SWIG
        /// Print a representation of the object to a stream (shorthand)
        inline friend
            std::ostream& operator<<(std::ostream &stream, const BSplineBasis& obj) {
                return stream << obj.getRepresentation();
            }
#endif // SWIG

    private:
    };
    std::vector<double> generateKnots(const std::vector<double >& bounds, int degree, int numberOfIntervals);
}

#endif //CPP_SPLINE_BSPLINEBASIS_H
