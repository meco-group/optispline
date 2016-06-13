#include <math.h>       /* pow */
#include <numeric>      // std::accumulate -> greville

#include "Basis.h"

#include "plus.h"
//#include "times.h"

namespace spline{

    //    std::make_shared<Basis>::std::make_shared<Basis> (const std::vector<double> &knots, int degree){
    //        std::string arg = "__";
    //        initstd::make_shared<Basis>(knots, degree, arg);
    //    }
    //
    //    std::make_shared<Basis>::std::make_shared<Basis> (const std::vector<double> &bounds, int degree, int numberOfIntervals){
    //        std::string arg = "__";
    //        std::make_shared<Basis>(bounds, degree, numberOfIntervals, arg);
    //    }

    //   std::make_shared<Basis>::std::make_shared<Basis> (const std::vector<double> &knots, int degree, std::string argument) : Univariatestd::shared_ptr<Basis>(degree,argument), knots(knots){
    ///        initstd::make_shared<Basis>(knots, degree, argument);
    //   }
    //
    //   std::make_shared<Basis>::std::make_shared<Basis> (const std::vector<double> &bounds, int degree, int numberOfIntervals, std::string argument) : Univariatestd::shared_ptr<Basis>(degree,argument) {
    //       int numberOfKnots = 2*degree + numberOfIntervals;
    //       knots.resize(numberOfKnots, 0);
    //
    //       for (int i = 0; i < degree; ++i) {
    //           knots[i] = bounds[0];
    //           knots[numberOfKnots - i - 1] = bounds[1];
    //       }
    //
    //       for (int i = 0; i < numberOfIntervals; ++i) {
    //           knots[degree + i] = bounds[0] + (bounds[1] - bounds[0]) * (double)i/(numberOfIntervals-1);
    //       }
    //
    ///        initstd::make_shared<Basis>(knots, degree, argument);
    //       setKnots(knots);
    //   }
    //
    //   void std::make_shared<Basis>::initstd::make_shared<Basis> (const std::vector<double >& knots, int degree, std::string argument) {
    //       setKnots(knots);
    //       setDegree(degree);
    //       setArguments(argument);
    //   }
    //
    //
    //   std::make_shared<Basis>::~std::make_shared<Basis> () {
    //   }
    //
    //   const std::vector<double> &std::make_shared<Basis>::getKnots () const {
    //       return knots;
    //   }
    //
    //
    //   void std::make_shared<Basis>::setKnots (const std::vector<double> &knots) {
    //       std::make_shared<Basis>::knots = knots;
    //   }
    //
    //   std::vector<double> std::make_shared<Basis>::evalstd::shared_ptr<Basis> (double x) const {
    //       double b;
    //       double bottom;
    //       double basis[degree+1][knots.size()-1];
    //
    //       for (int i=0; i<(knots.size()-1); i++){
    //           if((i < degree+1) and (knots[0] == knots[i])){
    //               basis[0][i] = ((x >= knots[i]) and (x <= knots[i+1]));
    //           }else{
    //               basis[0][i] = ((x > knots[i]) and (x <= knots[i+1]));
    //           }
    //       }
    //
    //       for (int d=1; d<(degree+1); d++){
    //           for (int i=0; i < length(); i++){
    //               b = 0;
    //               bottom = knots[i+d] - knots[i];
    //               if (bottom != 0){
    //                   b = (x - knots[i])*basis[d-1][i]/bottom;
    //               }
    //               bottom = knots[i+d+1] - knots[i+1];
    //               if (bottom != 0){
    //                   b += (knots[i+d+1] - x)*basis[d-1][i+1]/bottom;
    //               }
    //               basis[d][i] = b;
    //           }
    //       }
    //
    //       std::vector<double> r(length());
    //
    //       for (int i = 0; i < length(); ++i) {
    //           r[i] = basis[degree][i];
    //       }
    //       return r;
    //   }
    //
    //   int std::make_shared<Basis>::length () const{
    //       return knots.size() - degree - 1;
    //   }
    //
    //   std::vector<bool> std::make_shared<Basis>::indector (int i, double x) {
    //       std::vector<bool> r(knots.size() - 1);
    //
    //       for (int i = 0; i < knots.size() - 1; ++i) {
    //          r[i] =  (x >= knots[i]) * (x < knots[i + 1]);
    //       }
    //
    //       return r;
    //   }
    //
    BSplineBasisNode* BSplineBasis::get() const { return static_cast<BSplineBasisNode*>(SharedObject::get()); };
    BSplineBasisNode* BSplineBasis::operator->() const { return get(); }

    BSplineBasis::BSplineBasis()  { assign_node(new BSplineBasisNode()); };

    std::string BSplineBasisNode::getRepresentation() const { return "BSplineBasis object"; };
    std::string BSplineBasis::getRepresentation() const { return (*this)->getRepresentation() ;};

    Basis BSplineBasis::operator+ (const Basis& other) const {
        return (*this)->operator+(other);
    }

    Basis BSplineBasisNode::operator+ (const Basis& other) const {
        return other + shared_from_this<BSplineBasis>();
    }

    Basis BSplineBasis::operator+ (const MonomialeBasis& other) const {
        return (*this)->operator+(other);
    }

    Basis BSplineBasisNode::operator+ (const MonomialeBasis& other) const {
        return Basis();
    }

    Basis BSplineBasis::operator+ (const BSplineBasis& other) const{
        return (*this)->operator+(other);
        //        return plusBasis(*this, other);
    }

    Basis BSplineBasisNode::operator+ (const BSplineBasis& other) const{
        return BSplineBasis();
    }

    DT  BSplineBasisNode::operator()  (const std::vector< double > &  x   ) const {
        assert(x.size()==1);
        casadi::DM A(std::vector< double > {1.0, 2.0, 3.0});
        return DT(A,{3}); 
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


} // namespace spline
