#ifndef CPP_SPLINE_SUBBSPLINEBASIS_H
#define CPP_SPLINE_SUBBSPLINEBASIS_H

#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "SubUnivariatBasis.h"

namespace spline{

    class SubMonomialeBasis;

#ifndef SWIG

    class SubBSplineBasisNode : public SubUnivariatBasisNode {

    public:
        SubBSplineBasisNode (const std::vector<double >& knots, int degree);
        SubBSplineBasisNode (const std::vector<double >& bounds, int degree, int numberOfIntervals);


        std::vector<double>& getKnots ();
        const std::vector<double>& getKnots () const;
        void setKnots (std::vector<double>& knots) ;
        //
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

        virtual SubBasis operator+(const SubBasis& other) const ;
        virtual SubBasis operator*(const SubBasis& other) const ;

        virtual int getLenght() const ;

    private:

        //  std::vector<bool> indector(int i, double x);
         std::vector<double> knots;
    };

#endif // SWIG

    class SubBSplineBasis : public SubUnivariatBasis {

    public:

#ifndef SWIG
        SubBSplineBasis(){};
        SubBSplineBasisNode* get() const ;
        SubBSplineBasisNode* operator->() const ;
#endif // SWIG

        SubBSplineBasis (const std::vector<double >& knots, int degree);
        SubBSplineBasis (const std::vector<double >& bounds, int degree, int numberOfIntervals);

//  TODO(jgillis) moet ik de variant met const houden??
        std::vector<double> &getKnots ();
        const std::vector<double> &getKnots () const;
        void setKnots (std::vector<double> &knots) ;

        virtual SubBasis operator+(const SubBasis& other) const ;
        virtual SubBasis operator+(const SubMonomialeBasis& other) const ;
        virtual SubBasis operator+(const SubBSplineBasis& other) const ;

        virtual SubBasis operator*(const SubBasis& other) const ;
        virtual SubBasis operator*(const SubMonomialeBasis& other) const ;
        virtual SubBasis operator*(const SubBSplineBasis& other) const ;

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

    private:
        //  std::vector<bool> indector(int i, double x);
    };
}

#endif //CPP_SPLINE_SUBBSPLINEBASIS_H
