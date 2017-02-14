#include "Coefficient.h"

namespace spline {


    CoefficientNode* Coefficient::get() const {
        return static_cast<CoefficientNode*>(SharedObject::get());
    };
    CoefficientNode* Coefficient::operator->() const { return get(); }

    Coefficient::Coefficient() {

    }

    CoefficientNode::CoefficientNode(const std::vector< double >& v) :
        data(DT(DM(v), std::vector<int>{static_cast<int>(v.size()), 1, 1})) {
    }

    std::vector< int > Coefficient::dimension() const { return (*this)->dimension(); }
    std::vector< int > CoefficientNode::dimension () const {
        std::vector< int > dims_ = data.dims();
        for (size_t i=0; i<2; i++)
        {
            dims_.pop_back();
        }
        return dims_;
    }

    std::vector< int > Coefficient::size() const { return (*this)->size(); }
    std::vector< int > CoefficientNode::size() const {
        std::vector< int > dims = data.dims();
        int d = dims.size();
        int i = dims[d-2];
        int j = dims[d-1];
        return {i, j};
    }

    int Coefficient::getNumberCoefficents() const { return (*this)->getNumberCoefficents(); }
    int CoefficientNode::getNumberCoefficents() const {
        /* TODO use coefficient::size */
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

    Coefficient::Coefficient(const AnyTensor& t) {
      assign_node(new CoefficientNode(t));
    }
    Coefficient::Coefficient(const std::vector< double >& v) {
      assign_node(new CoefficientNode(v));
    }

    Coefficient Coefficient::operator-() const {
        return (*this)->operator-().shared_from_this<Coefficient>();
    }
    AnyTensor Coefficient::getData() const  { return (*this)->getData(); }
    std::string Coefficient::getRepresentation() const { return (*this)->getRepresentation(); }

}  // namespace spline
