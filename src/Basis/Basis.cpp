#include "Basis.h"
#include "SubBasisDummy.h"
#include "BSplineBasis.h"
#include "operations/operationsBasis.h"
#include "../common.h"

namespace spline {

    BasisNode* Basis::get() const { return static_cast<BasisNode*>(SharedObject::get()); };
    BasisNode* Basis::operator->() const { return get(); }

    Basis::Basis (){
        assign_node(new BasisNode(std::vector< SubBasis >{}));
    }

    Basis::Basis (const std::vector< Basis >& allBasis){
        std::vector< SubBasis > allSubBasis {};
        for ( auto &basis : allBasis ) {
            for ( auto &subBasis : basis.getSubBasis() ) {
                allSubBasis.push_back(subBasis);
            }
        }
        assign_node(new BasisNode(allSubBasis));
    }

    Basis::Basis (const std::vector< SubBasis >& allSubBasis){
        assign_node(new BasisNode(allSubBasis));
    }

    BasisNode::BasisNode (const std::vector< SubBasis >& allSubBasis_) : allSubBasis(allSubBasis_), allArguments(std::vector<Argument>{}){}

    int Basis::getDimension () const { return (*this)->getDimension(); }
    int BasisNode::getDimension () const {
        return allSubBasis.size();
    }

    void Basis::setArguments (const std::vector< spline::Argument >& args) { return (*this)->setArguments(args);}
    void BasisNode::setArguments (const std::vector< Argument >& args) {
        allArguments = args;
    }

    std::vector< spline::Argument > Basis::getArguments () const { return (*this)->getArguments();}
    std::vector< Argument > BasisNode::getArguments () const {
        return allArguments;
    }

    spline::Argument Basis::getSubArgument ( int index ) const { return (*this)->getSubArgument ( index ); }
    Argument BasisNode::getSubArgument ( int index ) const {
        return allArguments[index];
    }

    int Basis::indexArgument(Argument a) const { return (*this)->indexArgument(a); }
    int BasisNode::indexArgument(Argument a) const {
        auto it = std::find(allArguments.begin(), allArguments.end(), a);
        int index;
        if (it == allArguments.end())
        {
            index = -1;
        } else
        {
            index = std::distance(allArguments.begin(), it);
        }
        return  index;
    }

    bool Basis::hasArguments() const{ return (*this)->hasArguments();}
    bool BasisNode::hasArguments() const{
        return allArguments.size() > 0;
    }

    std::vector< SubBasis > Basis::getSubBasis () const { return (*this)->getSubBasis (); }
    std::vector< SubBasis > BasisNode::getSubBasis () const {
        return allSubBasis;
    }

    SubBasis Basis::getSubBasis ( Argument a) const { return (*this)->getSubBasis ( a ); }
    SubBasis BasisNode::getSubBasis ( Argument a ) const {
        int index = indexArgument(a);
        // if(index == allSubBasis.size()){
        if(index < 0){
            return SubBasisDummy();
        } else {
            return allSubBasis[index];
        }
    }

    Basis Basis::getSubBasis ( int index ) const { return (*this)->getSubBasis ( index ); }
    Basis BasisNode::getSubBasis ( int index ) const {
        return Basis(std::vector<SubBasis> {allSubBasis[index]});
    }

    void Basis::addBasis (Basis basis) { (*this)->addBasis (basis);}
    void BasisNode::addBasis (Basis basis) {
        for ( auto &subBasis : basis.getSubBasis() ) {
            allSubBasis.push_back(subBasis);
        }
    }

    void Basis::addBasis (SubBasis basis) { (*this)->addBasis (basis);}
    void BasisNode::addBasis (SubBasis basis) {
        this->allSubBasis.push_back(basis);
    }

    std::vector<int> Basis::getShape () const { return (*this)->getShape ();}
    std::vector<int> BasisNode::getShape () const {
        std::vector<int> shape;
        for(auto const& b : getSubBasis()){
            for(int s : b.getShape()){
                shape.push_back(s);
            }
        }
        return shape;
    }

    std::string BasisNode::getRepresentation() const {return "Basis"  + std::to_string(allArguments.size()) + std::to_string(allSubBasis.size());};
    std::string Basis::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::ostream& operator<<(std::ostream &stream, const Basis& base){
        return stream << base.getRepresentation();
    }

    Basis Basis::operator+ (const Basis& other) const {
        return plusBasis(*this, other);
    }

    // Basis Basis::operator* (const Basis& other) const {
    //     return timesBasis(this, other);
    // }

    AnyTensor  Basis::operator() (const std::vector< AnyScalar > &  x ) const { return (*this)->operator()(x); }
    AnyTensor  BasisNode::operator() (const std::vector< AnyScalar > &  x   ) const {
        spline_assert(x.size()==allSubBasis.size());
        AnyTensor ret = AnyTensor::unity();
        for (int i = 0; i < x.size(); ++i) {
            ret = ret.outer_product(allSubBasis[i](std::vector< AnyScalar >{x[i]}));
        }
        return ret;
    }

    BSplineBasis Basis::castBSpline() const{return (*this)->castBSpline();}
    BSplineBasis BasisNode::castBSpline() const{
        spline_assert(false); // not inmplemented
    }

    int Basis::totalNumberSubBasis() const{ return (*this)->totalNumberSubBasis();}
    int BasisNode::totalNumberSubBasis() const{
        int r = 1;
        for(int i : getShape()){
            r *= i;
        }
        return r;
    }
} // namespace spline
