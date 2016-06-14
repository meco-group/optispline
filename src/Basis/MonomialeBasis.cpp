#include <math.h>       /* pow */

#include <casadi/casadi.hpp> // range

#include "MonomialeBasis.h"

#include "plus.h"
#include "times.h"

namespace spline {
    //
    //   std::vector<double> Monomialestd::shared_ptr<Basis>::evalstd::shared_ptr<Basis> (double x) const {

    //       return evaluation_basis;
    //   }
    //

    //   int Monomialestd::shared_ptr<Basis>::length () const{return this->getDegree() + 1; }
    std::string MonomialeBasisNode::getRepresentation() const {
        return "MonomialeBasis";
    }

    std::string MonomialeBasis::getRepresentation() const {
        return (*this)->getRepresentation() ;
    }

    MonomialeBasisNode* MonomialeBasis::get() const { return static_cast<MonomialeBasisNode*>(SharedObject::get()); };
    MonomialeBasisNode* MonomialeBasis::operator->() const { return get(); }

    MonomialeBasis::MonomialeBasis()  { assign_node(new MonomialeBasisNode()); };

    Basis MonomialeBasis::operator+ (const Basis& other)const  { return (*this)->operator+(other); } 
    Basis MonomialeBasisNode::operator+ (const Basis& other)const  {
        // return plusBasis(*this, other);
        return Basis();
    }

    Basis MonomialeBasis::operator+ (const MonomialeBasis& other)const  { return (*this)->operator+(other); } 
    Basis MonomialeBasisNode::operator+ (const MonomialeBasis& other)const  {
        return MonomialeBasis();
    }

    Basis MonomialeBasis::operator+ (const BSplineBasis& other)const  { return (*this)->operator+(other); } 
    Basis MonomialeBasisNode::operator+ (const BSplineBasis& other)const  {
        return Basis();
    }

    DT  MonomialeBasisNode::operator() (const std::vector< double >& x ) const {
        assert(x.size()==1);
        double x_ = x[0];
        std::vector<double> evaluation_basis(this->getLenght());
        for (int i = 0; i < getLenght(); ++i) {
              evaluation_basis[i] = pow(x_,i);
        }
        casadi::DM A(evaluation_basis);
        return DT(A,{getLenght()}); 
    }

    ST  MonomialeBasisNode::operator() (const std::vector< SX > &  x   ) const {
        assert(x.size()==1);
        return ST(vertcat(x),{3}); 
    }

    MT  MonomialeBasisNode::operator() (const std::vector< MX > &  x   ) const {
        assert(x.size()==1);
        return MT(vertcat(x),{3}); 
    }

    int MonomialeBasisNode::getLenght () const {
         return getDegree() + 1;
    }
    
    //   std::shared_ptr<Basis> Monomialestd::shared_ptr<Basis>::operator* (const BSplinestd::shared_ptr<Basis> &other)const  {
    //       return timesstd::shared_ptr<Basis>(*this, other);
    //   }
    //
    /// TODO(ErikLambr) check is grid is valid
    //   std::vector<double> Monomialestd::shared_ptr<Basis>::evaluationGrid () const {
    ///        std::vector<double> grid = casadi::range(length());
    //       std::vector<double> grid;
    //       for( int i = 0; i < length(); i++ )
    //           grid.push_back( (double)i );
    //       return grid;
    //   }
} // namespace spline
