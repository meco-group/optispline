#include "Basis.h"
#include "plus.h"

namespace spline {

    BasisNode* Basis::get() const { return static_cast<BasisNode*>(SharedObject::get()); };
    BasisNode* Basis::operator->() const { return get(); }
    Basis::Basis()  { assign_node(new BasisNode()); };
    BasisNode::BasisNode () : argument(*new Argument()) {
         std::vector< Basis* > allSubBasis;
    };

    // int Basis::getDimension () const {
    //     return allSubBasis.size();
    // }

    void Basis::setArgument (const Argument& argument) {
        (*this)->setArgument(argument);
    }

    void BasisNode::setArgument (const Argument& argument) {
        this->argument = argument;
    }

    void Basis::setArgument (const std::string& argument) {
        (*this)->setArgument(argument);
    }

    void BasisNode::setArgument (const std::string& argument) {
        this->argument = Argument(argument);
    }

    Argument& Basis::getArgument () const {
        return (*this)->getArgument ();
    }
    
    Argument& BasisNode::getArgument () const {
        return argument;
    }

    std::vector< const Basis* > Basis::getSubBasis () const {
        return (*this)->getSubBasis ();
    }

    std::vector< const Basis* > BasisNode::getSubBasis () const {
        return allSubBasis;
    }

    void  Basis::addBasis  (Basis*  basis ) {
        (*this)->addBasis (basis );
    }
    
    void  BasisNode::addBasis  (Basis*  basis ) {
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
        return (*this)->operator+(other);
    }

    Basis BasisNode::operator+ (const Basis& other) const {
        return other + shared_from_this<Basis>();
    }

    Basis Basis::operator+ (const MonomialeBasis& other) const {
        return (*this)->operator+(other);
    }

    Basis BasisNode::operator+ (const MonomialeBasis& other) const {
        return Basis();
    }

    Basis Basis::operator+ (const BSplineBasis& other) const{
        return (*this)->operator+(other);
    }

    Basis BasisNode::operator+ (const BSplineBasis& other) const{
        return Basis();
    }

} // namespace spline
