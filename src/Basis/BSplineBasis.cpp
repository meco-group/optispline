#include <math.h>       /* pow */
#include <numeric>      // std::accumulate -> greville

#include "Basis.h"

#include "plus.h"
//#include "times.h"

namespace spline{

    BSplineBasisNode* BSplineBasis::get() const { return static_cast<BSplineBasisNode*>(SharedObject::get()); };
    BSplineBasisNode* BSplineBasis::operator->() const { return get(); }

    BSplineBasis::BSplineBasis()  { assign_node(new BSplineBasisNode()); };

    std::string BSplineBasisNode::getRepresentation() const { return "BSplineBasis object"; };
    std::string BSplineBasis::getRepresentation() const { return (*this)->getRepresentation() ;};

    Basis BSplineBasis::operator+ (const Basis& other) const { return (*this)->operator+(other); } 
    Basis BSplineBasisNode::operator+ (const Basis& other) const {
        return other + shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasis::operator+ (const BSplineBasis& other) const {
        return plusUnivariateUnivariate (*this, other);
    } 

    Basis BSplineBasis::operator+ (const MonomialeBasis& other) const {
        return plusUnivariateUnivariate (*this, other);
    } 

    DT  BSplineBasisNode::operator()  (const std::vector< double > &  x_   ) const {
        assert(x_.size()==1);
        double x = x_[0];
        double b;
        double bottom;
        double basis[degree+1][knots.size()-1];

        for (int i=0; i<(knots.size()-1); i++){
            if((i < degree+1) and (knots[0] == knots[i])){
                basis[0][i] = ((x >= knots[i]) and (x <= knots[i+1]));
            }else{
                basis[0][i] = ((x > knots[i]) and (x <= knots[i+1]));
            }
        }

        for (int d=1; d<(degree+1); d++){
            for (int i=0; i < getLenght(); i++){
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

        std::vector<double> r(getLenght());

        for (int i = 0; i < getLenght(); ++i) {
            r[i] = basis[degree][i];
        }

        casadi::DM A(r);
        return DT(A,{getLenght()}); 
    }

    ST  BSplineBasisNode::operator()  (const std::vector< SX > &  x   ) const {
        assert(x.size()==1);
        return ST(vertcat(x),{3}); 
    }

    MT  BSplineBasisNode::operator()  (const std::vector< MX > &  x   ) const {
        assert(x.size()==1);
        return MT(vertcat(x),{3}); 
    }
    //   std::make_shared<Basis>::operator* (const Monomialestd::shared_ptr<Basis> &other) const {
    //       return timesstd::shared_ptr<Basis>(*this, other);
    //   }
    //
    //   std::make_shared<Basis>::operator* (const std::make_shared<Basis> &other) const{
    //       return timesstd::shared_ptr<Basis>(*this, other);
    //   }
    //
    //   std::vector<double> std::make_shared<Basis>::greville () const {
    //       int d = degree;
    //       if(d == 1){
    //           d =1;
    //       }
    //
    //       std::vector<double> g (length());
    //       double s;
    //       for (int i = 0; i < length(); ++i) {
    //           g[i] = std::accumulate(knots.begin()+i+1,knots.begin()+i+d+1, 0.0) / (d);
    //       }
    //
    //       return g;
    //   }
    //
    //   std::vector<double> std::make_shared<Basis>::evaluationGrid (void) const {
    //       return greville();
    //   }
    //
    //
    //   std::make_shared<Basis>::addKnots(const std::vector<double> newKnots, bool unique) const {
    //       std::vector<double> kn;
    //       if (unique){
    //           kn = addKnotsUnique(knots, newKnots);
    //       } else {
    //           kn = addKnotsNotUnique(knots, newKnots);
    //       }
    //       return std::make_shared<Basis>(kn, degree);
    //   }

    int BSplineBasisNode::getLenght () const{
        return knots.size() - degree - 1;
    }

    //   std::make_shared<Basis>::std::make_shared<Basis> (const std::vector<double> &bounds, int degree, int numberOfIntervals, std::string argument) : Univariatestd::shared_ptr<Basis>(degree,argument) {
    //   }
    //

    BSplineBasis::BSplineBasis (const std::vector<double >& knots, int degree)  { assign_node(new BSplineBasisNode(knots, degree)); };
    BSplineBasisNode::BSplineBasisNode (const std::vector<double >& knots, int degree) {
        setKnots(const_cast<std::vector<double>&>(knots));
        setDegree(degree);
    }

    BSplineBasis::BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfIntervals)  { assign_node(new BSplineBasisNode(bounds, degree, numberOfIntervals)); };
    BSplineBasisNode::BSplineBasisNode (const std::vector<double >& bounds, int degree, int numberOfIntervals) {
        int numberOfKnots = 2*degree + numberOfIntervals;
        knots.resize(numberOfKnots, 0);

        for (int i = 0; i < degree; ++i) {
            knots[i] = bounds[0];
            knots[numberOfKnots - i - 1] = bounds[1];
        }

        for (int i = 0; i < numberOfIntervals; ++i) {
            knots[degree + i] = bounds[0] + (bounds[1] - bounds[0]) * (double)i/(numberOfIntervals-1);
        }

        setKnots(knots);
        setDegree(degree);
    }

    std::vector< double >& BSplineBasis::getKnots () { return (*this)->getKnots(); } 
    std::vector< double >& BSplineBasisNode::getKnots () {
        return knots;
    }

    const std::vector< double >& BSplineBasis::getKnots () const { return (*this)->getKnots(); } 
    const std::vector< double >& BSplineBasisNode::getKnots () const {
        return knots;
    }

    void BSplineBasis::setKnots (std::vector< double >& knots) { return (*this)->setKnots (knots); } 
    void BSplineBasisNode::setKnots (std::vector< double >& knots) {
        knots = knots;
    }
} // namespace spline
