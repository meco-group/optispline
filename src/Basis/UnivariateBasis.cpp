#include <vector>
#include "UnivariateBasis.h"

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"

namespace spline {


    //     Univariatestd::shared_ptr<Basis>::Univariatestd::shared_ptr<Basis> (int dimension, const std::vector<int> &size, const Argument &argument, int degree)
    //             : std::shared_ptr<Basis>(dimension, size, argument), degree(degree) {
    //         setDimension(1);
    //     }
    //  
    //     Univariatestd::shared_ptr<Basis>::Univariatestd::shared_ptr<Basis> (int degree, std::string argument){
    //         initUnivariatestd::shared_ptr<Basis>(degree, argument);
    //     }
    //  
    //  /    Univariatestd::shared_ptr<Basis>::Univariatestd::shared_ptr<Basis> (int degree) {
    //  /        //TODO(ErikLambre) make default code for arg
    //  /        std::string arg = "__";
    //  /        initUnivariatestd::shared_ptr<Basis>(degree, arg);
    //  /    }
    //  
    //     void Univariatestd::shared_ptr<Basis>::initUnivariatestd::shared_ptr<Basis> (int degree, std::string argument){
    //         Univariatestd::shared_ptr<Basis>::degree = degree;
    //         setArguments(argument);
    //         setDimensions(1);
    //     }
    //  
    //  
    //     void Univariatestd::shared_ptr<Basis>::increaseDegree (int d) {
    //         degree += d;
    //     }
    //  
    //  
    //     std::vector< std::vector<double> > Univariatestd::shared_ptr<Basis>::evalstd::shared_ptr<Basis>(const std::vector<double>& x) const{
    //         std::vector< std::vector<double> > r(x.size(), std::vector<double>(length()));
    //         for (int i = 0; i < x.size(); ++i) {
    //             r[i] = evalstd::shared_ptr<Basis>(x[i]);
    //         }
    //         return r;
    //     }
    //  
    //  
    //  /    TODO(ErikLambre) check order
    //     casadi::DM Univariatestd::shared_ptr<Basis>::transformation( const std::shared_ptr<Basis> &b) const {
    //         std::vector<double> grid = evaluationGrid();
    //         casadi::DM A(evalstd::shared_ptr<Basis>(grid));
    //         casadi::DM B(b.evalstd::shared_ptr<Basis>(grid));
    //         return casadi::DM::solve(A, B);
    //     }
    //  
    //  

    //     void Univariatestd::shared_ptr<Basis>::setArguments (const std::string& argument){
    //         std::vector<std::string> argumentList = {argument};
    //         std::shared_ptr<Basis>::setArguments(argumentList);
    //     }
    //  
    //  
    //     void Univariatestd::shared_ptr<Basis>::setDegree (int degree) {
    //         Univariatestd::shared_ptr<Basis>::degree = degree;
    //     }
    //  
    //     int Univariatestd::shared_ptr<Basis>::getLenght () const {
    //         return size[0];
    //     }
    //  
    UnivariateBasisNode* UnivariateBasis::get() const { return static_cast<UnivariateBasisNode*>(SharedObject::get()); };
    UnivariateBasisNode* UnivariateBasis::operator->() const { return get(); }

    // UnivariateBasis::UnivariateBasis()  {
    //     assign_node(new UnivariateBasisNode(number));
    // }

    std::string UnivariateBasisNode::getRepresentation() const {
        return "UnivariateBasis object"; 
    }

    std::string UnivariateBasis::getRepresentation() const {
        return (*this)->getRepresentation() ;
    }

    Basis UnivariateBasis::operator+ (const Basis& other) const  { 
        return (*this)->operator+(other);
    } 

    Basis UnivariateBasisNode::operator+ (const Basis& other) const  { 
        return UnivariateBasis();
    } 

    Basis UnivariateBasis::operator+ (const MonomialeBasis &other) const  { 
        return (*this)->operator+(other);
    } 

    Basis UnivariateBasisNode::operator+ (const MonomialeBasis &other) const  { 
        return UnivariateBasis();
    } 

    Basis UnivariateBasis::operator+ (const BSplineBasis &other) const  { 
        return (*this)->operator+(other);
    } 

    Basis UnivariateBasisNode::operator+ (const BSplineBasis &other) const  { 
        return UnivariateBasis();
    } 


    std::vector< const Basis* >  UnivariateBasis::getSubBasis () const {
        return std::vector< const Basis* > {this} ;
        // return (*this)->getSubBasis ();
    }

    // std::vector< Basis* >  UnivariateBasisNode::getSubBasis () const {
    //     return std::vector< Basis* > {get()} ;
    // }
}  // namespace spline

