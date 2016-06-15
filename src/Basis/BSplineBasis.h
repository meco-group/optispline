#ifndef CPP_SPLINE_BSPLINEBASIS_H
#define CPP_SPLINE_BSPLINEBASIS_H

#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "UnivariateBasis.h"

namespace spline{

    class MonomialeBasis;

#ifndef SWIG

    class BSplineBasisNode : public UnivariateBasisNode{

    public:
        BSplineBasisNode () {};
        BSplineBasisNode (const std::vector<double >& knots, int degree);
        BSplineBasisNode (const std::vector<double >& bounds, int degree, int numberOfIntervals);


        std::vector<double>& getKnots ();
        const std::vector<double>& getKnots () const;
        void setKnots (std::vector<double>& knots) ;

        //   //    evalution Basis
        //   virtual std::vector<double> evalBasis(double x) const;
        //
        virtual Basis operator+(const Basis& other) const ;
        virtual Basis operator+(const MonomialeBasis& other) const ;
        virtual Basis operator+(const BSplineBasis& other) const ;

        //   BSplineBasis operator*(const MonomialeBasis& other) const ;
        //   BSplineBasis operator*(const BSplineBasis& other) const ;
        //
        //   std::vector<double> greville () const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const;
        //   BSplineBasis addKnots(const std::vector<double> newKnots, bool unique = false) const;
        //
        /// Return a string with a representation (for SWIG)
        virtual std::string getRepresentation() const ;

        virtual DT operator()(const std::vector< double >& x) const;
        virtual ST operator()(const std::vector< SX >& x) const ;
        virtual MT operator()(const std::vector< MX >& x) const ;

        virtual int getLenght() const ;

    private:

        //  void initBSplineBasis (const std::vector<double >& knots, int degree, std::string argument);
        //
        //  std::vector<bool> indector(int i, double x);
        //
         std::vector<double> knots;
        //
    };

#endif // SWIG

    class BSplineBasis : public UnivariateBasis{

    public:

#ifndef SWIG
        BSplineBasisNode* get() const ;
        BSplineBasisNode* operator->() const ;
#endif // SWIG

        BSplineBasis() ;
        BSplineBasis (const std::vector<double >& knots, int degree);
        BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfIntervals);

//  TODO(jgillis) moet ik de variant met const houden??
        std::vector<double> &getKnots ();
        const std::vector<double> &getKnots () const;
        void setKnots (std::vector<double> &knots) ;
        //
        //   // TODO(jgillis): find out why this is needed to have proper swig interface
        //   virtual std::vector< std::vector<double> > evalBasis(const std::vector<double>& x) const { return UnivariateBasis::evalBasis(x); };
        //
        virtual Basis operator+(const Basis& other) const ;
        virtual Basis operator+(const MonomialeBasis& other) const ;
        virtual Basis operator+(const BSplineBasis& other) const ;

        //   BSplineBasis operator*(const MonomialeBasis& other) const ;
        //   BSplineBasis operator*(const BSplineBasis& other) const ;
        //
        //   std::vector<double> greville () const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const;
        //   BSplineBasis addKnots(const std::vector<double> newKnots, bool unique = false) const;
        //
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

        //  void initBSplineBasis (const std::vector<double >& knots, int degree, std::string argument);
        //
        //  std::vector<bool> indector(int i, double x);
        //
    };
}

#endif //CPP_SPLINE_BSPLINEBASIS_H
