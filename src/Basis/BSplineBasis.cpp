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

    BSplineBasis::BSplineBasis (const std::vector<double >& bounds, int degree, int numberOfIntervals)  { assign_node(new BSplineBasisNode(generateKnots(bounds,degree,numberOfIntervals), degree)); };

    std::vector< double >& BSplineBasis::getKnots () { return (*this)->getKnots(); } 
    std::vector< double >& BSplineBasisNode::getKnots () {
    }

    const std::vector< double >& BSplineBasis::getKnots () const { return (*this)->getKnots(); } 
    const std::vector< double >& BSplineBasisNode::getKnots () const {
    }

    void BSplineBasis::setKnots (std::vector< double >& knots) { return (*this)->setKnots (knots); } 
    void BSplineBasisNode::setKnots (std::vector< double >& knots) {
    }
    
    BSplineBasis BSplineBasis::castBSpline() const{ return (*this)->castBSpline (); } 
    BSplineBasis BSplineBasisNode::castBSpline() const {
    assert(false);
    }


    std::vector<double> generateKnots(const std::vector<double >& bounds, int degree, int numberOfIntervals){
        int numberOfKnots = 2*degree + numberOfIntervals;
	std::vector<double> knots(numberOfKnots, 0.0);
        for (int i = 0; i < degree; ++i) {
            knots[i] = bounds[0];
            knots[numberOfKnots - i - 1] = bounds[1];
        }

        for (int i = 0; i < numberOfIntervals; ++i) {
            knots[degree + i] = bounds[0] + (bounds[1] - bounds[0]) * (double)i/(numberOfIntervals-1);
        }

	return knots;
    }
} // namespace spline
