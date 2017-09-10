#include <casadi/casadi.hpp>
#include "Coefficient.h"
#include "../common.h"

namespace spline {

    CoefficientNode* Coefficient::get() const {
        return static_cast<CoefficientNode*>(SharedObject::get());
    };
    CoefficientNode* Coefficient::operator->() const { return get(); }

    Coefficient::Coefficient() { }

    Coefficient::Coefficient(const AnyTensor& t) {
      assign_node(new CoefficientNode(t));
    }
    Coefficient::Coefficient(const std::vector< double >& v) {
      assign_node(new CoefficientNode(v));
    }

    CoefficientNode::CoefficientNode(const std::vector< double >& v) :
        data_(DT(v, std::vector<int>{static_cast<int>(v.size()), 1, 1})) {
    }

    std::vector< int > Coefficient::dimension() const { return (*this)->dimension(); }
    std::vector< int > CoefficientNode::dimension() const {
        std::vector< int > dims_ = data_.dims();
        for (size_t i=0; i<2; i++) {
            dims_.pop_back();
        }
        return dims_;
    }

    std::string Coefficient::type() const {return (*this)->type();};
    std::string CoefficientNode::type() const {return "Coefficient";};

    std::string CoefficientNode::to_string() const {
        const std::string coeff_str = (n_coeff()==1) ? "coefficient" : "coefficients";

        std::string shape_str;
        shape_str += "[";
        shape_str += std::to_string(shape()[0]);
        shape_str += ",";
        shape_str += std::to_string(shape()[1]);
        shape_str += "]";

        std::vector<int> dims = dimension();
        std::string dim_str;
        dim_str += "[";
        for (int i = 0; i<dims.size(); i++){
            dim_str += std::to_string(dims[i]);
            dim_str += ",";
        }
        dim_str.pop_back();
        dim_str += "]";
        return "Coefficient, consisting of " + std::to_string(n_coeff()) + " " + coeff_str + " with shape " +
                shape_str + ", of dimension " + dim_str;
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
        return spline::product(dimension());
    }

    Coefficient Coefficient::operator-() const {
        return Coefficient(-data());
    }
    AnyTensor Coefficient::data() const  { return (*this)->data(); }

    AnyTensor Coefficient::transform(const AnyTensor& T) const {
        std::vector< int > direction = casadi::range((int)T.n_dims()/2);
        return (*this)->transform(T, direction);
    }

    AnyTensor Coefficient::transform(const AnyTensor& T, const NumericIndexVector& direction) const {
        return (*this)->transform(T, direction);
    }

    AnyTensor CoefficientNode::transform(const AnyTensor& T, const NumericIndexVector& direction) const {
        //check dimensions
        int n = T.n_dims()/2;
        spline_assert_message(T.n_dims()%2 == 0, "The transform tensor should have an even number of dimensions");
        spline_assert_message(n == direction.size(), "Number of directions does not match the number of dimensions.");
        //do einstein
        std::vector< int > index_t = mrange(2*n);
        std::vector< int > index_c = mrange(2*n, 2*n+data().n_dims());
        std::vector< int > index_r = mrange(0, 2*n+data().n_dims());
        index_r.erase(index_r.begin()+n, index_r.begin()+2*n);

        //scramble indices
        for (int i = 0; i < direction.size(); i++) {
            spline_assert_message(direction[i] < n, "Direction of the transform should be smaller than the transform dimension/2");
            spline_assert_message(T.dims()[n+i] == dimension()[direction[i]], "Transformation matrix dimensions mismatch. For direction " << direction[i] << ": " << T.dims()[n+i] << " is not equal to " << dimension()[direction[i]] << ".");

            index_c[direction[i]] = index_t[n+i];
            index_r.erase(index_r.begin()+n+direction[i]-i);
        }

        // Do crazy tensor shit
        return T.einstein(data(), index_t, index_c, index_r);
    }

    AnyTensor Coefficient::transform(const AnyTensor& T, const NumericIndex& directions) const {
        return (*this)->transform(std::vector<AnyTensor>{T}, NumericIndexVector{directions});
    }

    AnyTensor Coefficient::transform(const std::vector<AnyTensor>& T,
          const NumericIndexVector& directions) const {
      return (*this)->transform(T, directions);
    }

    AnyTensor CoefficientNode::transform(const std::vector<AnyTensor>& T,
          const NumericIndexVector& directions) const {

        AnyTensor ret_data = data();
        spline_assert(T.size() == directions.size());

        bool linear_expansion = ret_data.is_MT();
        for (auto& t : T) linear_expansion&= t.is_DT();

        casadi::MX s;
        if (linear_expansion) {
          s = casadi::MX::sym("temp", ret_data.numel());
          ret_data = MT(s, ret_data.dims());
        }

        for (int k=0; k<T.size(); k++) {
            // check dimension of transformation matrix
            spline_assert_message(T[k].n_dims()==2,
              "Transformation matrix should have 2 dimensions. It has " <<
              T[k].n_dims() << ".");
            // check compatibility
            spline_assert_message(T[k].dims()[1]==ret_data.dims()[directions[k]],
                "Incompatible dimensions: 2nd dimension of transformation matrix is "
                << T[k].dims()[1] << " while transformed direction has dimension "
                << ret_data.dims()[directions[k]] << ".");
            // construct index vectors
            ret_data = ret_data.transform(T[k], directions[k]);
        }
        if (!linear_expansion) return ret_data;
        casadi::Function f = casadi::Function("f", {s}, {ret_data.as_MT().data()});
        casadi::Function J = f.jacobian();
        casadi::DM jac = sparsify(J(std::vector<casadi::DM>{0, 0})[0]);
        return MT(casadi::MX::mtimes(jac, data().as_MT().data()), ret_data.dims());
    }

    AnyTensor Coefficient::data(const NumericIndex& k) const {
      AnyTensor flat = data().flatten_first(dimension().size());
      return flat.index({k, -1, -1});
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

    Coefficient Coefficient::rm_direction(const std::vector<NumericIndex>& indices) const {
        return (*this)->rm_direction(indices);
    }
    Coefficient CoefficientNode::rm_direction(const std::vector<NumericIndex>& indices) const {
        std::vector< int > dims = data_.dims();
        std::vector< int > new_dims;
        int j;
        for (int i=0; i<dims.size(); i++) {
            for (j=0; j<indices.size(); j++) {
                if (i == indices[j]) {
                    break;
                }
            }
            if (j == indices.size()) {
                new_dims.push_back(dims[i]);
            } else {
                tensor_assert_message(dims[i] == 1,
                    "Only directions with dimension 1 can be removed.")
            }
        }
        return Coefficient(data().shape(new_dims));
    }

    Coefficient Coefficient::cat(const NumericIndex& index,
          const std::vector< Coefficient >& coefs) {
        Coefficient c = coefs[0];
        std::vector< AnyTensor > all_tensor = {};
        for (int i = 0; i< coefs.size(); i++) {
            tensor_assert_message(c.shape()[1-index] == coefs[i].shape()[1-index],
              "cat has mismatched coefficients" << c.shape()[1-index] <<
              " != " << coefs[i].shape()[1-index] << ".");
            all_tensor.push_back(coefs[i].data());
        }
       return AnyTensor::concat(all_tensor, c.dimension().size() + index);
    }

    Coefficient Coefficient::reshape(const std::vector< int >& shape) const {
        return (*this)->reshape(shape);
    }

    Coefficient CoefficientNode::reshape(const std::vector< int >& shape) const {
        spline_assert(shape.size() > 0);
        std::vector< int > shape_ = dimension();
        shape_.insert(shape_.end(), shape.begin(), shape.end());

        return Coefficient(data().shape(shape_));
    }

    Coefficient Coefficient::trace() const {
        return (*this)->trace();
    }

    Coefficient CoefficientNode::trace() const {
        spline_assert_message(shape()[0] == shape()[1],
                "Trace only defined for square matrices. Dimensions are " << shape() << ".");
        AnyTensor ones = Tensor<casadi::DM>(casadi::DM::eye(shape()[0]), shape());
        int d = dimension().size();
        std::vector< int > a = mrange(d + 2);
        std::vector< int > b = {a[d], a[d+1]};
        std::vector< int > c = mrange(d);
        return data().einstein(ones,a,b,c);
    }

    Coefficient Coefficient::to_matrix_valued() const {
        return (*this)->to_matrix_valued();
    }

    Coefficient CoefficientNode::to_matrix_valued() const {
        return add_trival_dimension(2 - data().dims().size());
    }

    bool Coefficient::is_true_scalar() const {
        return (*this)->is_true_scalar();
    }

    bool CoefficientNode::is_true_scalar() const {
        return data().numel() == 1;
    }

}  // namespace spline
