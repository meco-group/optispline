#include "Basis.h"
#include "plus.h"

namespace spline {

    BasisNode* Basis::get() const { return static_cast<BasisNode*>(SharedObject::get()); };
    BasisNode* Basis::operator->() const { return get(); }

    Basis::Basis (){
        assign_node(new BasisNode(std::vector< Basis >{}));
    }

    Basis::Basis (const std::vector< Basis >& allSubBasis){
        assign_node(new BasisNode(allSubBasis));
    }

    BasisNode::BasisNode (const std::vector< Basis >& allSubBasis_) : allSubBasis(allSubBasis_) ,  argument(*new Argument()) {}
    BasisNode::BasisNode (const std::vector< Basis >& allSubBasis_, Argument argument) : allSubBasis(allSubBasis_) ,  argument(argument) {}
 
    // int Basis::getDimension () const {
    //     return allSubBasis.size();
    // }

    void Basis::setArgument (const Argument& argument) { (*this)->setArgument(argument); } 
    void BasisNode::setArgument (const Argument& argument) {
        this->argument = argument;
    }

    void Basis::setArgument (const std::string& argument) { (*this)->setArgument(argument); } 
    void BasisNode::setArgument (const std::string& argument) {
        this->argument = Argument(argument);
    }

    const Argument& Basis::getArgument () const { return (*this)->getArgument();} 
    const Argument& BasisNode::getArgument () const {
        return argument;
    }

    Argument& Basis::getArgument () { return (*this)->getArgument();} 
    Argument& BasisNode::getArgument () {
        return argument;
    }

    std::vector< Basis > Basis::getSubBasis () const { return (*this)->getSubBasis (); } 
    std::vector< Basis > BasisNode::getSubBasis () const {
        return allSubBasis;
    }

    Basis Basis::getSubBasis ( int index ) const { return (*this)->getSubBasis ( index ); } 
    Basis BasisNode::getSubBasis ( int index ) const {
        return allSubBasis[index];
    }

    std::vector< Argument > Basis::getSubArgument () const { return (*this)->getSubArgument (); } 
    std::vector< Argument > BasisNode::getSubArgument () const {
        std::vector< Argument > returnArgumentList;
        for (auto subBasis : this->getSubBasis()) {
           returnArgumentList.push_back( subBasis.getArgument() );
        }
        return returnArgumentList;
    }

    Argument Basis::getSubArgument ( int index ) const { return (*this)->getSubArgument ( index ); } 
    Argument BasisNode::getSubArgument ( int index ) const {
        return allSubBasis[index].getArgument();
    }

    void Basis::addBasis (Basis basis) { (*this)->addBasis (basis);} 
    void BasisNode::addBasis (Basis basis) {
         this->allSubBasis.push_back(basis);
    }
    // std::vector<int> Basis::getSize () const {
    //     std::vector<int> size;
    //     std::vector<int> sizeSubBasis;
    //     for(int i = 0; i < getDimension(); i++){
    //         sizeSubBasis = allSubBasis[i]->getSize(); 
    //         for(int j = 0; j < sizeSubBasis.size(); j++){
    //              size.pussh_back(sizeSubBasis[j]);
    //         }
    //     }
    //     return size;
    // }


    std::string BasisNode::getRepresentation() const {return "Basis";};
    std::string Basis::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::ostream& operator<<(std::ostream &stream, const Basis& base){
        return stream << base.getRepresentation();
    }

    Basis Basis::operator+ (const Basis& other) const { 
        return Basis();
        // return plusMultivariateMultivariate (*this, other);
    }

    Basis Basis::operator+ (const MonomialeBasis& other) const {
        return plusMultivariateUnivariate (*this, other);
    } 

    Basis Basis::operator+ (const BSplineBasis& other) const {
        return plusMultivariateUnivariate (*this, other);
    } 

    // Basis Basis::operator+ (const MonomialeBasis& other) const { return (*this)->operator+(other); } 
    // Basis BasisNode::operator+ (const MonomialeBasis& other) const {
    //     return plusMultivariateUnivariate (*this, other);
    // }
    //
    // Basis Basis::operator+ (const BSplineBasis& other) const{ return (*this)->operator+(other); } 
    // Basis BasisNode::operator+ (const BSplineBasis& other) const{
    //     return plusMultivariateUnivariate (*this, other);
    // }

    DT  Basis::operator() (const std::vector< double > &  x ) const {
        return (*this)->operator()(x);
    }

    DT  BasisNode::operator() (const std::vector< double > &  x   ) const {
        assert(x.size()==allSubBasis.size());
        DT ret(1,{});
        for (int i = 0; i < x.size(); ++i) {
            ret = ret.outer_product(allSubBasis[i](std::vector< double >{x[i]}));
        }
        return ret;
    }
   
    ST  Basis::operator()  (const std::vector< SX > &  x   ) const {
        return (*this)->operator() (x);
    }

    ST  BasisNode::operator()  (const std::vector< SX > &  x   ) const {
        assert(x.size()==allSubBasis.size());
        ST ret(1,{});
        for (int i = 0; i < x.size(); ++i) {
            ret = ret.outer_product(allSubBasis[i](std::vector< SX >{x[i]}));
        }
        return ret;
    }
   
    MT  Basis::operator()  (const std::vector< MX > &  x   ) const {
        return (*this)->operator() (x);
    }

    MT  BasisNode::operator()  (const std::vector< MX > &  x   ) const {
        assert(x.size()==allSubBasis.size());
        MT ret(1,{});
        for (int i = 0; i < x.size(); ++i) {
            ret = ret.outer_product(allSubBasis[i](std::vector< MX >{x[i]}));
        }
        return ret;
    }
} // namespace spline
