#include "Basis.h"
#include "BSplineBasis.h"
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

    int Basis::getDimension () const { (*this)->getDimension(); } 
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

    int Basis::indexArgument(spline::Argument a) { return (*this)->indexArgument(a); } 
    int BasisNode::indexArgument(Argument a) {
        assert(false);
        return -1;
    }

    bool Basis::hasArguments() const{ return (*this)->hasArguments();}
    bool BasisNode::hasArguments() const{
        return allArguments.size() > 0;
    }

    bool Basis::hasValidNumberOfArguments() const{ return (*this)->hasArguments();}
    bool BasisNode::hasValidNumberOfArguments() const{
        return allArguments.size() == allSubBasis.size();
    }

    std::vector< SubBasis > Basis::getSubBasis () const { return (*this)->getSubBasis (); } 
    std::vector< SubBasis > BasisNode::getSubBasis () const {
        return allSubBasis;
    }

    Basis Basis::getSubBasis ( int index ) const { return (*this)->getSubBasis ( index ); } 
    Basis BasisNode::getSubBasis ( int index ) const {
        return Basis(std::vector<SubBasis> {allSubBasis[index]});
    }

    void Basis::addBasis (Basis basis) { (*this)->addBasis (basis);} 
    void BasisNode::addBasis (Basis basis) {
        this->allSubBasis.push_back(basis.getSubBasis()[0]);
    }

    void Basis::addBasis (SubBasis basis) { (*this)->addBasis (basis);} 
    void BasisNode::addBasis (SubBasis basis) {
        this->allSubBasis.push_back(basis);
    }

    std::vector<int> Basis::getSize () const { (*this)->getSize ();} 
    std::vector<int> BasisNode::getSize () const {
        std::vector<int> size;
        // std::vector<int> sizeSubBasis;
        // for(int i = 0; i < getDimension(); i++){
        //     sizeSubBasis = allSubBasis[i]->getSize(); 
        //     for(int j = 0; j < sizeSubBasis.size(); j++){
        //          size.push_back(sizeSubBasis[j]);
        //     }
        // }
        return size;
    }


    std::string BasisNode::getRepresentation() const {return "Basis";};
    std::string Basis::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::ostream& operator<<(std::ostream &stream, const Basis& base){
        return stream << base.getRepresentation();
    }

    // Basis Basis::operator+ (const Basis& other) const { 
    //     return plusMultivariate(*this, other);
    // }
    //
    // Basis Basis::operator+ (const MonomialBasis& other) const {
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
    // Basis Basis::operator* (const MonomialBasis& other) const {
    //     return timesMultivariate(*this, other);
    // } 
    //
    // Basis Basis::operator* (const BSplineBasis& other) const {
    //     return timesMultivariate(*this, other);
    // } 

    AnyTensor  Basis::operator() (const std::vector< AnyScalar > &  x ) const {
        return (*this)->operator()(x);
    }

    AnyTensor  BasisNode::operator() (const std::vector< AnyScalar > &  x   ) const {
        assert(x.size()==allSubBasis.size());
        AnyTensor ret = AnyTensor::unity();
        for (int i = 0; i < x.size(); ++i) {
            ret = ret.outer_product(allSubBasis[i](std::vector< AnyScalar >{x[i]}));
        }
        return ret;
    }

    // ST  Basis::operator()  (const std::vector< SX > &  x   ) const {
    //     return (*this)->operator() (x);
    // }
    //
    // ST  BasisNode::operator()  (const std::vector< SX > &  x   ) const {
    //     assert(x.size()==allSubBasis.size());
    //     ST ret(1,{});
    //     for (int i = 0; i < x.size(); ++i) {
    //         ret = ret.outer_product(allSubBasis[i](std::vector< SX >{x[i]}));
    //     }
    //     return ret;
    // }
    //
    // MT  Basis::operator()  (const std::vector< MX > &  x   ) const {
    //     return (*this)->operator() (x);
    // }
    //
    // MT  BasisNode::operator()  (const std::vector< MX > &  x   ) const {
    //     assert(x.size()==allSubBasis.size());
    //     MT ret(1,{});
    //     for (int i = 0; i < x.size(); ++i) {
    //         ret = ret.outer_product(allSubBasis[i](std::vector< MX >{x[i]}));
    //     }
    //     return ret;
    // }

    BSplineBasis Basis::castBSpline() const{return (*this)->castBSpline();}
    BSplineBasis BasisNode::castBSpline() const{
        assert(false); // not inmplemented
    }
} // namespace spline
