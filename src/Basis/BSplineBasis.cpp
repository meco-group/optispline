#include <math.h>       /* pow */
#include <numeric>      // std::accumulate -> greville
#include <sstream>
#include "BSplineBasis.h"

#include "operations/operationsBasis.h"

namespace spline{

    BSplineBasisNode* BSplineBasis::get() const { return static_cast<BSplineBasisNode*>(SharedObject::get()); };
    BSplineBasisNode* BSplineBasis::operator->() const { return get(); }

    std::string BSplineBasisNode::getRepresentation() const {
        std::stringstream s;
        s << "BSplineBasis object" << getKnots();
        return s.str();
    };
    std::string BSplineBasis::getRepresentation() const { return (*this)->getRepresentation() ;};

    Basis BSplineBasis::operator+ (const Basis& other) const {
        assert(false);
	return Basis();
	// return (*this)->operator+(other); 
    } 

    Basis BSplineBasisNode::operator+ (const Basis& other) const {
        assert(false);
	return Basis();
	// return other + shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasis::operator+ (const BSplineBasis& other) const {
        assert(false);
	return Basis();
	// return plusUnivariateUnivariate (*this, other);
    } 

    Basis BSplineBasis::operator+ (const MonomialeBasis& other) const {
        assert(false);
	return Basis();
	// return plusUnivariateUnivariate (*this, other);
    } 

    Basis BSplineBasis::operator* (const Basis& other) const {
        assert(false);
	return Basis();
	// return (*this)->operator*(other); 
    } 

    Basis BSplineBasisNode::operator* (const Basis& other) const {
        assert(false);
	return Basis();
	// return other + shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasis::operator* (const BSplineBasis& other) const {
        assert(false);
	return Basis();
	// return timesUnivariateUnivariate (*this, other);
    } 

    Basis BSplineBasis::operator* (const MonomialeBasis& other) const {
        assert(false);
	return Basis();
	// return timesUnivariateUnivariate (*this, other);
    } 

    DT  BSplineBasisNode::operator()  (const std::vector< double > &  x_   ) const {
        casadi::DM A(1);
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
        return  - 1;
    }

    BSplineBasis::BSplineBasis (const std::vector<double >& knots, int degree)  {
        assign_node(new BSplineBasisNode(knots, degree)); 
    }

    BSplineBasisNode::BSplineBasisNode (const std::vector<double >& knots, int degree) 
    : UnivariateBasisNode(degree) { }

    BSplineBasis::BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfIntervals)  { assign_node(new BSplineBasisNode(bounds, degree, numberOfIntervals)); };
    BSplineBasisNode::BSplineBasisNode (const std::vector<double >& bounds, int degree, int numberOfIntervals) : UnivariateBasisNode(degree) {
            }

    std::vector< double >& BSplineBasis::getKnots () { return (*this)->getKnots(); } 
    std::vector< double >& BSplineBasisNode::getKnots () {
    }

    const std::vector< double >& BSplineBasis::getKnots () const { return (*this)->getKnots(); } 
    const std::vector< double >& BSplineBasisNode::getKnots () const {
    }

    void BSplineBasis::setKnots (std::vector< double >& knots) { return (*this)->setKnots (knots); } 
    void BSplineBasisNode::setKnots (std::vector< double >& knots) {
    }
} // namespace spline
