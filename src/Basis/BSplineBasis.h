#ifndef CPP_SPLINE_SUBBSPLINEBASIS_H
#define CPP_SPLINE_SUBBSPLINEBASIS_H

#include <any_tensor.hpp>
#include <vector>

#include "Basis.h"
#include "UnivariateBasis.h"

namespace spline{
    typedef std::vector<AnyScalar> AnyVector;
    class MonomialBasis;

#ifndef SWIG

    class BSplineBasisNode : public UnivariateBasisNode {

    public:
        BSplineBasisNode (const AnyVector& knots, int degree);
        BSplineBasisNode (const AnyVector& bounds, int degree, int numberOfIntervals);

        virtual Basis operator+(const MonomialBasis& other) const ;
        virtual Basis operator+(const BSplineBasis& other) const ;
        virtual Basis operator+(const Basis& other) const ;
        virtual Basis operator+(const DummyBasis& other) const ;

        virtual Basis operator*(const MonomialBasis& other) const ;
        virtual Basis operator*(const BSplineBasis& other) const ;
        virtual Basis operator*(const Basis& other) const ;
        virtual Basis operator*(const DummyBasis& other) const ;

        AnyVector getKnots () const;
        void setKnots (const AnyVector& knots) ;

        AnyVector greville () const;
        //
        //   BSplineBasis addKnots(const std::vector<double> newKnots, bool unique = false) const;
        //
        virtual std::string getRepresentation() const ;

        virtual AnyTensor operator()(const AnyVector& x) const;

        virtual int getLength() const ;

        template<class T>
        AnyTensor SubBasisEvalution (const std::vector< T >& x ) const ;

        virtual void getEvaluationGrid(std::vector< std::vector < AnyScalar > > * eg) const;
    private:

        //  std::vector<bool> indector(int i, double x);
        AnyVector knots_;
    };

#endif // SWIG

    class BSplineBasis : public UnivariateBasis {

    public:

#ifndef SWIG
        BSplineBasis(){};
        BSplineBasisNode* get() const ;
        BSplineBasisNode* operator->() const ;
#endif // SWIG

        BSplineBasis (const AnyVector& knots, int degree);
        BSplineBasis (const AnyVector& bounds, int degree, int numberOfIntervals);

        std::vector<double> getKnots () const;
        void setKnots (const AnyVector& knots) ;

        std::vector<double> greville () const;
        //
        //   BSplineBasis addKnots(const std::vector<double> newKnots, bool unique = false) const;
        //

        virtual void foo() const {};

    private:
        //  std::vector<bool> indector(int i, double x);
    };

    template<class T>
    AnyTensor BSplineBasisNode::SubBasisEvalution (const std::vector< T > & x_) const {
        T x = x_[0];
        T b;
        T bottom;
        T basis[degree+1][knots_.size()-1];

        for (int i=0; i<(knots_.size()-1); i++){
            if((i < degree+1) and (knots_[0] == knots_[i])){
                basis[0][i] = ((x >= knots_[i]) and (x <= knots_[i+1]));
            }else{
                basis[0][i] = ((x > knots_[i]) and (x <= knots_[i+1]));
            }
        }

        for (int d=1; d<(degree+1); d++){
            for (int i=0; i < getLength(); i++){
                b = 0;
                bottom = knots_[i+d] - knots_[i];
                if (bottom != 0){
                    b = (x - knots_[i])*basis[d-1][i]/bottom;
                }
                bottom = knots_[i+d+1] - knots_[i+1];
                if (bottom != 0){
                    b += (knots_[i+d+1] - x)*basis[d-1][i+1]/bottom;
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
