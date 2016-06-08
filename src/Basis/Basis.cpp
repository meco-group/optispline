#include "Basis.h"
#include "plus.h"

namespace spline {

    // Basis::Basis(){
    //     std::vector< Basis > allSubBasis;
    // }

    //Basis::Basis(std::vector<  Basis > allSubBasis) : allSubBasis(allSubBasis){}

    //    std::shared_ptr<Basis>::std::shared_ptr<Basis> (int dimension, const std::vector<int> &size, const Argument &argument) : dimension(dimension),
    //                                                                                          size(size),
    //                                                                                           argument(argument) { }

    // int Basis::getDimension () const {
    //     return allSubBasis.size();
    // }

    //    void Basis::addArgument (const Argument &argument) {
    //        this->argument = argument;
    //    }
    //
    //    void Basis::addArgument (const std::string& argument) {
    //        this->argument = Argument(argument);
    //    }

    //    void Basis::setArgument (const Argument &argument) {
    //        this->argument = argument;
    //    }
    //
    //    void Basis::setArgument (const std::string& argument) {
    //        this->argument = Argument(argument);
    //    }
    //
    //    Argument Basis::getArgument () const {
    //        return argument;
    //    }
    //
    // std::vector<int> Basis::getSize () const {
    //     std::vector<int> size;
    //     std::vector<int> sizeSubBasis;
    //     for(int i = 0; i < getDimension(); i++){
    //         sizeSubBasis = allSubBasis[i]->getSize(); 
    //         for(int j = 0; j < sizeSubBasis.size(); j++){
    //              size.push_back(sizeSubBasis[j]);
    //         }
    //     }
    //     return size;
    // }

    BasisNode* Basis::get() const { return static_cast<BasisNode*>(SharedObject::get()); };
    BasisNode* Basis::operator->() const { return get(); }
    Basis::Basis()  { assign_node(new BasisNode()); };

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
