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

    std::vector< int > CoefficientNode::getShape() const {
        std::vector< int > dims = data.dims();
        int d = dims.size();
        int i = dims[d-2];
        int j = dims[d-1];
        return {i, j};
    }

    int CoefficientNode::getNumberCoefficents() const {
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

    std::vector< int > Coefficient::getShape() const { return (*this)->getShape(); }

    int Coefficient::getNumberCoefficents() const { return (*this)->getNumberCoefficents(); }

    Coefficient Coefficient::operator-() const {
        return (*this)->operator-().shared_from_this<Coefficient>();
    }
    AnyTensor Coefficient::getData() const  { return (*this)->getData(); }
    std::string Coefficient::getRepresentation() const { return (*this)->getRepresentation(); }

    AnyTensor Coefficient::transform(const AnyTensor& T, const NumericIndex& direction) const {
        // check dimension of transformation matrix
        tensor_assert(T.n_dims()==2);
        AnyTensor data = getData();
        // check compatibility
        tensor_assert_message(T.dims()[1]==data.dims()[direction],
            "Incompatible dimensions: 2nd dimension of transformation matrix is "
            << T.dims() << "while dimensions of direction " << direction <<
            " of coefficient is " << data.dims()[direction] << ".");
        // construct index vectors
        int n_dims = data.n_dims();
        std::vector<int> ind1(n_dims);
        std::vector<int> ind2(n_dims);
        int cnt = -3;
        for (int i=0; i<data.n_dims(); i++){
            if (i == direction){
                ind1[i] = -2;
                ind2[i] = -1;
            } else {
                ind1[i] = cnt;
                ind2[i] = cnt;
                cnt--;
            }
        }
        return data.einstein(T, ind1, {-1, -2}, ind2);
    }

}  // namespace spline
