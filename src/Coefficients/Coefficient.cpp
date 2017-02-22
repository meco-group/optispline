#include "Coefficient.h"

namespace spline {


    CoefficientNode* Coefficient::get() const {
        return static_cast<CoefficientNode*>(SharedObject::get());
    };
    CoefficientNode* Coefficient::operator->() const { return get(); }

    Coefficient::Coefficient() {

    }

    CoefficientNode::CoefficientNode(const std::vector< double >& v) :
        data_(DT(DM(v), std::vector<int>{static_cast<int>(v.size()), 1, 1})) {
    }

    std::vector< int > Coefficient::dimension() const { return (*this)->dimension(); }
    std::vector< int > CoefficientNode::dimension() const {
        std::vector< int > dims_ = data_.dims();
        for (size_t i=0; i<2; i++) {
            dims_.pop_back();
        }
        return dims_;
    }

    std::vector< int > Coefficient::shape() const { return (*this)->shape(); }
    std::vector< int > CoefficientNode::shape() const {
        std::vector< int > dims = data_.dims();
        int d = dims.size();
        int i = dims[d-2];
        int j = dims[d-1];
        return {i, j};
    }

    int Coefficient::n_coeff() const { return (*this)->n_coeff(); }
    int CoefficientNode::n_coeff() const {
        /* TODO use coefficient::size */
        std::vector< int > dims = data_.dims();
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
        return Coefficient(-data());
    }
    AnyTensor Coefficient::data() const  { return (*this)->data(); }
    std::string Coefficient::getRepresentation() const { return (*this)->getRepresentation(); }

    AnyTensor Coefficient::transform(const AnyTensor& T, const NumericIndex& directions) const {
        return (*this)->transform(std::vector<AnyTensor>{T}, std::vector<NumericIndex>{directions});
    }

    AnyTensor Coefficient::transform(const std::vector<AnyTensor>& T,
          const std::vector<NumericIndex>& directions) const {
      return (*this)->transform(T, directions);
    }

    AnyTensor CoefficientNode::transform(const std::vector<AnyTensor>& T,
          const std::vector<NumericIndex>& directions) const {
        tensor_assert(T.size() == directions.size());
        AnyTensor ret_data = data();
        for (int k=0; k<T.size(); k++) {
            // check dimension of transformation matrix
            tensor_assert_message(T[k].n_dims()==2,
              "Transformation matrix should have 2 dimensions. It has " <<
              T[k].n_dims() << ".");
            // check compatibility
            tensor_assert_message(T[k].dims()[1]==ret_data.dims()[directions[k]],
                "Incompatible dimensions: 2nd dimension of transformation matrix is "
                << T[k].dims()[1] << " while transformed direction has dimension "
                << ret_data.dims()[directions[k]] << ".");
            // construct index vectors
            int n_dims = ret_data.n_dims();
            std::vector<int> ind1(n_dims);
            std::vector<int> ind2(n_dims);
            int cnt = -3;
            for (int i=0; i<ret_data.n_dims(); i++) {
                if (i == directions[k]) {
                    ind1[i] = -2;
                    ind2[i] = -1;
                } else {
                    ind1[i] = cnt;
                    ind2[i] = cnt;
                    cnt--;
                }
            }
            ret_data = ret_data.einstein(T[k], ind1, {-1, -2}, ind2);
        }
        return ret_data;
    }

    Coefficient Coefficient::add_trival_dimension(int extra_dims) const {
        return (*this)->add_trival_dimension(extra_dims);
    }
    Coefficient CoefficientNode::add_trival_dimension(int extra_dims) const {
        std::vector< int > dims_ = data_.dims();
        for (int i = 0; i < extra_dims; i++) {
            dims_.push_back(1);
        }
        return data().shape(dims_);
    }

    Coefficient Coefficient::transpose() const {
        return (*this)->transpose();
    }
    Coefficient CoefficientNode::transpose() const {
        std::vector< int > order = std::vector< int >(data().n_dims(), 0);
        for (int i=1; i<order.size(); i++) {
            order[i] = i;
        }
        order[order.size()-2] = order.size()-1;
        order[order.size()-1] = order.size()-2;

        return Coefficient(data().reorder_dims(order));
    }

    Coefficient Coefficient::cat(const NumericIndex& index,
          const std::vector< Coefficient >& coefs) const {
        return (*this)->cat(index, coefs);
    }

    Coefficient CoefficientNode::cat(const NumericIndex& index,
          const std::vector< Coefficient >& coefs) const {
        std::vector< AnyTensor > all_tensor = {data()};
        for (auto& c : coefs) {
            tensor_assert_message(shape()[1-index] == c.shape()[1-index],
              "cat has mismatched coefficients" << shape()[1-index] <<
              " != " << c.shape()[1-index] << ".");
            all_tensor.push_back(c.data());
        }

       return AnyTensor::concat(all_tensor, dimension().size() + index);
    }

}  // namespace spline
