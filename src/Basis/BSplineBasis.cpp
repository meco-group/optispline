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

      int BSplineBasisNode::getLenght () const{
          return knots.size() - degree - 1;
      }

} // namespace spline
