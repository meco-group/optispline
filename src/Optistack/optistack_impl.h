#ifndef OPTI_H
#define OPTI_H

#include <string>
#include <vector>

#include <casadi/casadi.hpp>

#include "../Function/Function.h"
#include "../Coefficients/Coefficient.h"

using namespace casadi;

class OptiSplineSol;
class OptiSplineDebug;

template<class T>
class OptiSplineInterface {
  public:

    OptiSplineSol solve() {
      return Opti::solve();
    }

    void OptiSplineInterfaceset_initial(const spline::Coefficient& c, const Tensor<DM>& d) {
      spline_assert_message(c.data().is_MT(), "Value only supported for MX");
      Opti::set_initial(c.data().as_MT(), d);
    }
    void set_initial(const spline::Function& f, const spline::Function& g) {
      spline_assert_message(f.coeff().data().is_MT(), "Value only supported for MX");
      Opti::set_initial(f.coeff_tensor().as_MT(), g.project_to(f.tensor_basis()).coeff_tensor().as_DT());
    }
    void set_initial(const Tensor<MX>& c, const Tensor<DM>& d) {
      spline_assert_message(t.squeeze().dims()==d.squeeze().dims(),
      "Tensor dimensions must match. Got " << t.dims() << " and " << d.dims() <<".");
      Opti::set_initial(t.data(), d.data());
    }
    //using OptiStack::set_initial;

    //using OptiStack::set_value;
    void set_value(const spline::Coefficient& c, const Tensor<DM>& d) {
      spline_assert_message(c.data().is_MT(), "Value only supported for MX");
      set_value(c.data().as_MT(), d);
    }
    void set_value(const spline::Function& f, const spline::Function& g) {
      spline_assert_message(f.coeff().data().is_MT(), "Value only supported for MX");
      set_value(f.coeff_tensor().as_MT(), g.project_to(f.tensor_basis()).coeff_tensor().as_DT());
    }
    void set_value(const Tensor<MX>& c, const Tensor<DM>& d) {
      spline_assert_message(t.squeeze().dims()==d.squeeze().dims(),
        "Tensor dimensions must match. Got " << t.dims() << " and " << d.dims() <<".");
      OptiStack::set_value(t.data(), d.data());
    }

    spline::Function Function(const spline::TensorBasis& b, const std::vector<int>& shape=std::vector<int>(),const std::string& attribute="full") {
      casadi_assert(var_shape.size()<=2);
      int n = var_shape.size()>=1 ? var_shape[0] : 1;
      int m = var_shape.size()==2 ? var_shape[1] : 1;

      return spline::Function(b, spline::Coefficient(coeff_var(b.dimension(), n, m, attribute)));
    }
};

class OptiSpline: public Opti, public OptiSplineInterface<OptiSpline> {
    /// Copy
    OptiStackSpline copy() const { return *this; }

}

class OptiSplineAdvanced: public OptiAdvanced, public OptiSplineInterface<OptiSplineAdvanced> {
  AnyTensor value(const AnyTensor& c) const;
  #ifndef SWIG
  Tensor<DM> value(const Tensor<MX>& c, const std::vector< Tensor<MX> >& values=std::vector< Tensor<MX> >()) const;
  #endif
  spline::Function value(const spline::Function& f) const;
  spline::Coefficient value(const spline::Coefficient& c) const;
  //using OptiStack::value;

  std::vector<MX> symvar(const MT& m) const { return symvar(m.data()); }
  std::vector<MX> symvar(const MT& expr, VariableType type) const { return symvar(expr.data(), type); }

#ifndef SWIG
  MT coeff_var(const std::vector<int>& shape, int n=1, int m=1, const std::string& attribute="full");
#endif

  void matlab_dump(const casadi::Function& f, const std::string &fname) const;
};

#endif //OPTI_H
