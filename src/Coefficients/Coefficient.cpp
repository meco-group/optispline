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

    std::vector< int > Coefficient::shape() const { return (*this)->shape(); }
    std::vector< int > CoefficientNode::shape() const {
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

    std::string CoefficientNode::getRepresentation() const {return "Coefficient";};

    Coefficient::Coefficient(const AnyTensor& t) {
      assign_node(new CoefficientNode(t));
    }
    Coefficient::Coefficient(const std::vector< double >& v) {
      assign_node(new CoefficientNode(v));
    }

    Coefficient Coefficient::operator-() const {
        return Coefficient(-getData());
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

    Coefficient Coefficient::add_trival_dimention(int extra_dims) const { return (*this)->add_trival_dimention(extra_dims); }
    Coefficient CoefficientNode::add_trival_dimention(int extra_dims) const {
        std::vector< int > dims_ = data.dims();
        for(int i = 0; i < extra_dims; i++){
            dims_.push_back(1);
        }
        /* AnyTensor return_tensor = AnyTensor(getData(), dims_); */
        return getData().shape(dims_);
    }
}  // namespace spline
