#ifndef CPP_SPLINE_SUBBSPLINEBASIS_H
#define CPP_SPLINE_SUBBSPLINEBASIS_H

#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

#include "SubBasis.h"
#include "SubUnivariateBasis.h"

#include "utils/CommonBasis.h"

namespace spline{

    class SubMonomialBasis;

#ifndef SWIG

    class SubBSplineBasisNode : public SubUnivariateBasisNode {

    public:
        SubBSplineBasisNode (const std::vector<double >& knots, int degree);
        SubBSplineBasisNode (const std::vector<double >& bounds, int degree, int numberOfIntervals);

        virtual SubBasis operator+(const SubMonomialBasis& other) const ;
        virtual SubBasis operator+(const SubBSplineBasis& other) const ;
        virtual SubBasis operator+(const SubBasis& other) const ;
        virtual SubBasis operator+(const SubBasisDummy& other) const ;

        std::vector<double>& getKnots ();
        const std::vector<double>& getKnots () const;
        void setKnots (std::vector<double>& knots) ;
        //
        //   std::vector<double> greville () const;
        //
        //   virtual std::vector<double> evaluationGrid (void) const;
        //   BSplineBasis addKnots(const std::vector<double> newKnots, bool unique = false) const;
        //
        virtual std::string getRepresentation() const ;

        virtual AnyTensor operator()(const std::vector< AnyScalar >& x) const;

<<<<<<< HEAD
        // virtual SubBasis operator+(const SubBasis& other) const ;
        // virtual SubBasis operator*(const SubBasis& other) const ;
        //
        virtual int getLenght() const ;
=======
        virtual SubBasis operator+(const SubBasis& other) const ;
        virtual SubBasis operator*(const SubBasis& other) const ;

        virtual int getLength() const ;
>>>>>>> 07a69b5... basis length typo #33 getLenght() -> getLength()
        template<class T>
        AnyTensor SubBasisEvalution (const std::vector< T >& x ) const ;

    private:

        //  std::vector<bool> indector(int i, double x);
         std::vector<double> knots;
    };

#endif // SWIG

    class SubBSplineBasis : public SubUnivariateBasis {

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

        // virtual SubBasis operator*(const SubBasis& other) const ;
        // virtual SubBasis operator*(const SubMonomialBasis& other) const ;
        // virtual SubBasis operator*(const SubBSplineBasis& other) const ;

        //   BSplineBasis operator*(const MonomialBasis& other) const ;
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

    template<class T>
    AnyTensor SubBSplineBasisNode::SubBasisEvalution (const std::vector< T > & x_) const {
        T x = x_[0];
        T b;
        double bottom;
        T basis[degree+1][knots.size()-1];

        for (int i=0; i<(knots.size()-1); i++){
            if((i < degree+1) and (knots[0] == knots[i])){
                basis[0][i] = ((x >= knots[i]) and (x <= knots[i+1]));
            }else{
                basis[0][i] = ((x > knots[i]) and (x <= knots[i+1]));
            }
        }

        for (int d=1; d<(degree+1); d++){
            for (int i=0; i < getLength(); i++){
                b = 0;
                bottom = knots[i+d] - knots[i];
                if (bottom != 0){
                    b = (x - knots[i])*basis[d-1][i]/bottom;
                }
                bottom = knots[i+d+1] - knots[i+1];
                if (bottom != 0){
                    b += (knots[i+d+1] - x)*basis[d-1][i+1]/bottom;
                }
                basis[d][i] = b;
            }
        }

        std::vector<T> r(getLength());

        for (int i = 0; i < getLength(); ++i) {
            r[i] = basis[degree][i];
        }

        return AnyTensor(vertcat(r));
    }

}

#endif //CPP_SPLINE_SUBBSPLINEBASIS_H
