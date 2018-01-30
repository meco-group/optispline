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
        data_(DT(v, std::vector<casadi_int>{static_cast<casadi_int>(v.size()), 1, 1})) {
    }

    std::vector< casadi_int > Coefficient::dimension() const { return (*this)->dimension(); }
    std::vector< casadi_int > CoefficientNode::dimension() const {
        std::vector< casadi_int > dims_ = data_.dims();
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

        std::vector<casadi_int> dims = dimension();
        std::string dim_str;
        dim_str += "[";
        for (casadi_int i = 0; i<dims.size(); i++){
            dim_str += std::to_string(dims[i]);
            dim_str += ",";
        }
        dim_str.pop_back();
        dim_str += "]";
        return "Coefficient, consisting of " + std::to_string(n_coeff()) + " " + coeff_str + " with shape " +
                shape_str + ", of dimension " + dim_str;
    }

    std::vector< casadi_int > Coefficient::shape() const { return (*this)->shape(); }
    std::vector< casadi_int > CoefficientNode::shape() const {
        std::vector< casadi_int > dims = data_.dims();
        casadi_int d = dims.size();
        casadi_int i = dims[d-2];
        casadi_int j = dims[d-1];
        return {i, j};
    }

    casadi_int Coefficient::n_coeff() const { return (*this)->n_coeff(); }
    casadi_int CoefficientNode::n_coeff() const {
        return casadi::product(dimension());
    }

    Coefficient Coefficient::operator-() const {
        return Coefficient(-data());
    }
    AnyTensor Coefficient::data() const  { return (*this)->data(); }

    AnyTensor Coefficient::transform(const AnyTensor& T) const {
        std::vector< casadi_int > direction = casadi::range((casadi_int)T.n_dims()/2);
        return (*this)->transform(T, direction);
    }

    AnyTensor Coefficient::transform(const AnyTensor& T, const NumericIndexVector& direction) const {
        return (*this)->transform(T, direction);
    }

    AnyTensor CoefficientNode::transform(const AnyTensor& T, const NumericIndexVector& direction) const {
        //check dimensions
        casadi_int n = T.n_dims()/2;
        spline_assert_message(T.n_dims()%2 == 0, "The transform tensor should have an even number of dimensions");
        spline_assert_message(n == direction.size(), "Number of directions does not match the number of dimensions.");
        //do einstein
        std::vector< casadi_int > index_t = mrange(2*n);
        std::vector< casadi_int > index_c = mrange(2*n, 2*n+data().n_dims());
        std::vector< casadi_int > index_r = mrange(0, 2*n+data().n_dims());
        index_r.erase(index_r.begin()+n, index_r.begin()+2*n);

        //scramble indices
        for (casadi_int i = 0; i < direction.size(); i++) {
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

        for (casadi_int k=0; k<T.size(); k++) {
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

    Coefficient Coefficient::add_trival_dimension(casadi_int extra_dims) const {
        return (*this)->add_trival_dimension(extra_dims);
    }
    Coefficient CoefficientNode::add_trival_dimension(casadi_int extra_dims) const {
        std::vector< casadi_int > dims_ = data_.dims();
        for (casadi_int i = 0; i < extra_dims; i++) {
            dims_.push_back(1);
        }
        return data().shape(dims_);
    }

    Coefficient Coefficient::transpose() const {
        return (*this)->transpose();
    }
    Coefficient CoefficientNode::transpose() const {
        std::vector< casadi_int > order = std::vector< casadi_int >(data().n_dims(), 0);
        for (casadi_int i=1; i<order.size(); i++) {
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
        std::vector< casadi_int > dims = data_.dims();
        std::vector< casadi_int > new_dims;
        casadi_int j;
        for (casadi_int i=0; i<dims.size(); i++) {
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
        for (casadi_int i = 0; i< coefs.size(); i++) {
            tensor_assert_message(c.shape()[1-index] == coefs[i].shape()[1-index],
              "cat has mismatched coefficients" << c.shape()[1-index] <<
              " != " << coefs[i].shape()[1-index] << ".");
            all_tensor.push_back(coefs[i].data());
        }
       return AnyTensor::concat(all_tensor, c.dimension().size() + index);
    }

    Coefficient Coefficient::reshape(const std::vector< casadi_int >& shape) const {
        return (*this)->reshape(shape);
    }

    Coefficient CoefficientNode::reshape(const std::vector< casadi_int >& shape) const {
        spline_assert(shape.size() > 0);
        std::vector< casadi_int > shape_ = dimension();
        shape_.insert(shape_.end(), shape.begin(), shape.end());

        return Coefficient(data().shape(shape_));
    }

    Coefficient Coefficient::trace() const {
        return (*this)->trace();
    }

    Coefficient Coefficient::sum(casadi_int axis) const {
        return (*this)->sum(axis);
    }

    Coefficient Coefficient::sum() const {
        return (*this)->sum();
    }

    Coefficient CoefficientNode::trace() const {
        spline_assert_message(shape()[0] == shape()[1],
                "Trace only defined for square matrices. Dimensions are " << shape() << ".");
        AnyTensor ones = Tensor<casadi::DM>(casadi::DM::eye(shape()[0]), shape());
        casadi_int d = dimension().size();
        std::vector< casadi_int > a = mrange(d + 2);
        std::vector< casadi_int > b = {a[d], a[d+1]};
        std::vector< casadi_int > c = mrange(d);
        return data().einstein(ones,a,b,c);
    }
    Coefficient CoefficientNode::sum(casadi_int axis) const {
        std::vector<casadi_int> dim = data().dims();
        dim[axis+dimension().size()] = 1;
        return data().sum(axis+dimension().size()).shape(dim);
    }

    Coefficient CoefficientNode::sum() const {
        if (is_vector() && !is_column()) return sum(1).transpose();
        return sum(0);
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

    bool Coefficient::is_scalar() const {
        return (*this)->is_scalar();
    }

    bool Coefficient::is_vector() const {
        return (*this)->is_vector();
    }

    bool Coefficient::is_column() const {
        return (*this)->is_column();
    }

    bool CoefficientNode::is_scalar() const {
        return shape()[0] == 1 && shape()[1] == 1;
    }

    bool CoefficientNode::is_vector() const {
        return shape()[0] == 1 || shape()[1] == 1;
    }

    bool CoefficientNode::is_column() const {
        return shape()[1] == 1;
    }

}  // namespace spline
