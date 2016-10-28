#include "Coefficient.h"

namespace spline {


    CoefficientNode* Coefficient::get() const { return static_cast<CoefficientNode*>(SharedObject::get()); };
    CoefficientNode* Coefficient::operator->() const { return get(); }

    Coefficient::Coefficient (){
        
    }
    
    CoefficientNode::CoefficientNode (const std::vector< double >& v) :
        data(DT(DM(v),std::vector<int>{static_cast<int>(v.size()),1,1})){
    }

    std::vector< int > CoefficientNode::getShape() const{
        std::vector< int > dims = data.dims();
        std::cout << "dims coef : " << dims << std::endl;
        int d = dims.size();
        int i = dims[d-2];
        int j = dims[d-1];
        return *(new std::vector< int > {i,j}); // ouch
    }

    int CoefficientNode::getNumberCoefficents() const{
        std::vector< int > dims = data.dims();
        int d = dims.size();
        int i = dims[d-2];
        int j = dims[d-1];
        return i*j;
    }

    CoefficientNode CoefficientNode::operator-() const {
        return CoefficientNode( -data);
    }

    std::string CoefficientNode::getRepresentation() const {return "Coefficient";};
    
    
    Coefficient::Coefficient (const AnyTensor& t) {
      assign_node(new CoefficientNode(t));
    }
    Coefficient::Coefficient (const std::vector< double >& v) {
      assign_node(new CoefficientNode(v));
    }

    std::vector< int > Coefficient::getShape() const { return (*this)->getShape(); }
    
    int Coefficient::getNumberCoefficents() const { return (*this)->getNumberCoefficents(); }

    Coefficient Coefficient::operator-() const { return (*this)->operator-().shared_from_this<Coefficient>(); }
    AnyTensor Coefficient::getData() const  { return (*this)->getData(); }
    std::string Coefficient::getRepresentation() const { return (*this)->getRepresentation(); }
    
}  // namespace spline
