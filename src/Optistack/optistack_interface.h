#ifndef OPTISPLINE_INTERFACE_H
#define OPTISPLINE_INTERFACE_H

#include <string>
#include <vector>

#include <casadi/casadi.hpp>

#include "../Function/Function.h"
#include "../Coefficients/Coefficient.h"
#include "../common.h"

using namespace casadi;


template<class T>
class OptiSplineInterface {
  public:

    void set_initial(const spline::Coefficient& c, const Tensor<DM>& d) {
      spline_assert_message(c.data().is_MT(), "Value only supported for MX");
      set_initial(c.data().as_MT(), d);
    }
    void set_initial(const spline::Function& f, const spline::Function& g) {
      spline_assert_message(f.coeff().data().is_MT(), "Value only supported for MX");
      set_initial(f.coeff_tensor().as_MT(), g.project_to(f.tensor_basis()).coeff_tensor().as_DT());
    }
    void set_initial(const Tensor<MX>& t, const Tensor<DM>& d) {
      spline_assert_message(t.squeeze().dims()==d.squeeze().dims(),
      "Tensor dimensions must match. Got " << t.dims() << " and " << d.dims() <<".");
      static_cast<T &>(*this).T::type_base::set_initial(t.data(), d.data());
    }

    void set_value(const spline::Coefficient& c, const Tensor<DM>& d) {
      spline_assert_message(c.data().is_MT(), "Value only supported for MX");
      set_value(c.data().as_MT(), d);
    }
    void set_value(const spline::Function& f, const spline::Function& g) {
      spline_assert_message(f.coeff().data().is_MT(), "Value only supported for MX");
      set_value(f.coeff_tensor().as_MT(), g.project_to(f.tensor_basis()).coeff_tensor().as_DT());
    }
    void set_value(const Tensor<MX>& t, const Tensor<DM>& d) {
      spline_assert_message(t.squeeze().dims()==d.squeeze().dims(),
        "Tensor dimensions must match. Got " << t.dims() << " and " << d.dims() <<".");
      static_cast<T &>(*this).T::type_base::set_value(t.data(), d.data());
    }

    void update_user_dict(const Tensor<MX>& t, const Dict& meta) {
      static_cast<T &>(*this).T::type_base::update_user_dict(t.data(), meta);
    }
    void update_user_dict(const spline::Function& f, const Dict& meta) {
      spline_assert_message(f.coeff().data().is_MT(), "update_user_dict only supported for MX");
      update_user_dict(f.coeff_tensor().as_MT(), meta);
    }
    void update_user_dict(const spline::Coefficient& c, const Dict& meta) {
      spline_assert_message(c.data().is_MT(), "update_user_dict only supported for MX");
      update_user_dict(c.data().as_MT(), meta);
    }
    spline::Function Function(const spline::TensorBasis& b,
        const std::vector<int>& shape=std::vector<int>(),
        const std::string& attribute="full",
        const std::string& opti_type="variable") {
      casadi_assert(shape.size()<=2, "shape must be 2D at most; got " + str(shape.size()));
      int n = shape.size()>=1 ? shape[0] : 1;
      int m = shape.size()==2 ? shape[1] : 1;

      MT c;
      if (opti_type=="variable") {
        c = coeff_variable(b.dimension(), n, m, attribute);
      } else if (opti_type=="parameter") {
        c = coeff_parameter(b.dimension(), n, m, attribute);
      }
      return spline::Function(b, spline::Coefficient(c));
    }

  private:
    #ifndef SWIG
      typedef std::function<MX(T&, int, int, const std::string&)> Opti_create;
      MT coeff_varpar(Opti_create& opti_create, const std::vector<int>& shape, int n=1, int m=1, const std::string& attribute="full") {
        int cs = spline::product(shape);

        if (attribute=="full") {
          std::vector<int> ret_shape = shape;
          ret_shape.push_back(n);
          ret_shape.push_back(m);
          return MT(opti_create(static_cast<T &>(*this), spline::product(ret_shape), 1, "full"), ret_shape);
        }

        std::vector<MX> args;
        for (int i=0;i<cs;++i)
          args.push_back(opti_create(static_cast<T &>(*this), n, m, attribute));

        std::vector<int> t_shape = {n, m};
        t_shape.insert(t_shape.end(), shape.begin(), shape.end());
        MT t = MT(horzcat(args), t_shape);

        std::vector<int> reorder = casadi::range(2, shape.size()+2);
        reorder.push_back(0);
        reorder.push_back(1);

        return t.reorder_dims(reorder);
      }
      MT coeff_variable(const std::vector<int>& shape, int n=1, int m=1, const std::string& attribute="full") {
        Opti_create opti_creator = [](T& opti, int nn, int mm, const std::string& aattribute) { return opti.variable(nn, mm, aattribute); };
        return coeff_varpar(opti_creator, shape, n, m, attribute);
      }
      MT coeff_parameter(const std::vector<int>& shape, int n=1, int m=1, const std::string& attribute="full") {
        Opti_create opti_creator = [](T& opti, int nn, int mm, const std::string& aattribute) { return opti.parameter(nn, mm, aattribute); };
        return coeff_varpar(opti_creator, shape, n, m, attribute);
      }
    #endif
};

#endif //OPTISPLINE_INTERFACE_H
