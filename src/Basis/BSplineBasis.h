//
// Created by erik on 12/04/16.
//

#ifndef CPP_SPLINE_BSPLINEBASIS_H
#define CPP_SPLINE_BSPLINEBASIS_H


class MonomialeBasis;
class BSplineBasis;

#include <vector>
#include "UnivariateBasis.h"
#include "MonomialeBasis.h"


namespace spline{

class BSplineBasis : public UnivariateBasis{


public:
    BSplineBasis (const std::vector<double >& knots, int degree);
    BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfInternalKnots);

    ~BSplineBasis ();

    const std::vector<double> &getKnots () const;
    void setKnots (const std::vector<double> &knots) ;

    //    evalution Basis
    virtual std::vector<double> evalBasis(double x) const;

    // TODO(jgillis): find out why this is needed to have proper swig interface
    virtual std::vector< std::vector<double> > evalBasis(const std::vector<double>& x) const { return UnivariateBasis::evalBasis(x); };

    int length() const;

    BSplineBasis operator+(const MonomialeBasis& other) const ;
    BSplineBasis operator+(const BSplineBasis& other) const ;

    BSplineBasis operator*(const MonomialeBasis& other) const ;
    BSplineBasis operator*(const BSplineBasis& other) const ;

    std::vector<double> greville () const;

    virtual std::vector<double> evaluationGrid (void) const;
    BSplineBasis addKnots(const std::vector<double> newKnots, bool unique = false) const;

    /// Return a string with a representation (for SWIG)
    std::string getRepresentation() const { return "BSplineBasis object"; };

#ifndef SWIG
    /// Print a representation of the object to a stream (shorthand)
    inline friend
      std::ostream& operator<<(std::ostream &stream, const BSplineBasis& obj) {
      return stream << obj.getRepresentation();
    }
#endif // SWIG

private:
    std::vector<bool> indector(int i, double x);

private:
    std::vector<double> knots;

};


}

#endif //CPP_SPLINE_BSPLINEBASIS_H
