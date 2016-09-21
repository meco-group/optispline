#include "Basis.h"
#include "operations/operationsBasis.h"

namespace spline {

    BasisNode* Basis::get() const { return static_cast<BasisNode*>(SharedObject::get()); };
    BasisNode* Basis::operator->() const { return get(); }

    Basis::Basis (){
        assign_node(new BasisNode(std::vector< SubBasis >{}));
    }

    Basis::Basis (const std::vector< SubBasis >& allSubBasis){
        assign_node(new BasisNode(allSubBasis));
    }

    BasisNode::BasisNode (const std::vector< SubBasis >& allSubBasis_) : allSubBasis(allSubBasis_), allArguments(std::vector<Argument>{}){}
    // int Basis::getDimension () const {
    //     return allSubBasis.size();
    // }

    void Basis::setArgument (const Argument& argument) { (*this)->setArgument(argument); } 
    void BasisNode::setArgument (const Argument& argument) {
        return;
    }

    void Basis::setArgument (const std::string& argument) { (*this)->setArgument(*new Argument ( argument )); } 
    

    const Argument& Basis::getArgument () const { return (*this)->getArgument();} 
    const Argument& BasisNode::getArgument () const {
        return *new Argument();
    }

    Argument& Basis::getArgument () { return (*this)->getArgument();} 
    Argument& BasisNode::getArgument () {
        return *new Argument();
    }

    std::vector< SubBasis > Basis::getSubBasis () const { return (*this)->getSubBasis (); } 
    std::vector< SubBasis > BasisNode::getSubBasis () const {
        return allSubBasis;
    }

    Basis Basis::getSubBasis ( int index ) const { return (*this)->getSubBasis ( index ); } 
    Basis BasisNode::getSubBasis ( int index ) const {
        return Basis(std::vector<SubBasis> {allSubBasis[index]});
    }

    std::vector< Argument > Basis::getSubArgument () const { return (*this)->getSubArgument (); } 
    std::vector< Argument > BasisNode::getSubArgument () const {
        return allArguments;
    }

    Argument Basis::getSubArgument ( int index ) const { return (*this)->getSubArgument ( index ); } 
    Argument BasisNode::getSubArgument ( int index ) const {
        return allArguments[index];
    }

    void Basis::addBasis (Basis basis) { (*this)->addBasis (basis);} 
    void BasisNode::addBasis (Basis basis) {
         this->allSubBasis.push_back(basis.getSubBasis()[0]);
    }

    void Basis::addBasis (SubBasis basis) { (*this)->addBasis (basis);} 
    void BasisNode::addBasis (SubBasis basis) {
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

    // Basis Basis::operator+ (const Basis& other) const { 
    //     return plusMultivariate(*this, other);
    // }
    //
    // Basis Basis::operator+ (const MonomialeBasis& other) const {
    //     return plusMultivariate(*this, other);
    // } 
    //
    // Basis Basis::operator+ (const BSplineBasis& other) const {
    //     return plusMultivariate(*this, other);
    // } 
    //
    // Basis Basis::operator* (const Basis& other) const { 
    //     return timesMultivariate(*this, other);
    // }
    //
    // Basis Basis::operator* (const MonomialeBasis& other) const {
    //     return timesMultivariate(*this, other);
    // } 
    //
    // Basis Basis::operator* (const BSplineBasis& other) const {
    //     return timesMultivariate(*this, other);
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

     int Basis::indexArgument(Argument a) { return (*this)->indexArgument(a); } 
     int BasisNode::indexArgument(Argument a) {
         assert(false);
         return -1;

    }
} // namespace spline
