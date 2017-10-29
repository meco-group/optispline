#include "optistack.h"
#include "../common.h"

using namespace casadi;

spline::Function OptiSplineAdvanced::value(const spline::Function& f) const {
  return spline::Function(f.tensor_basis(), value(f.coeff()));
}

spline::Coefficient OptiSplineAdvanced::value(const spline::Coefficient& c) const {
  return spline::Coefficient(value(c.data()));
}

AnyTensor OptiSplineAdvanced::value(const AnyTensor& t) const {
  if (t.is_DT()) return t.as_DT();
  if (t.is_MT()) return value(t.as_MT());
  spline_assert_message(false, "Value only supported for MX");
  return DT();
}

Tensor<DM> OptiSplineAdvanced::value(const Tensor<MX>& t, const std::vector< Tensor<MX> >& values) const {
  std::vector<MX> values_data;
  for (const auto& e: values) values_data.push_back(e.data());
  return Tensor<DM>(OptiAdvanced::value(t.data(), values_data), t.dims());
}

void OptiSplineAdvanced::matlab_dump(const casadi::Function& f, const std::string &fname) const {
  f.export_code("matlab", fname);
}

spline::Function OptiSplineSol::value(const spline::Function& f) const {
    return static_cast<const OptiSplineAdvanced&>(optistack_).value(f);
}
spline::Coefficient OptiSplineSol::value(const spline::Coefficient& c) const {
    return static_cast<const OptiSplineAdvanced&>(optistack_).value(c);
}

OptiSplineAdvanced OptiSpline::debug() const {
  OptiSplineAdvanced ret;
  ret.own(get());
  return ret;
}
OptiSplineAdvanced OptiSpline::advanced() const {
  return debug();
}

OptiSplineSol OptiSpline::solve() {
  return Opti::solve();
}

OptiSplineSol OptiSplineAdvanced::solve() {
  return OptiAdvanced::solve();
}

OptiSplineAdvanced::OptiSplineAdvanced(const OptiSpline& rhs) {
  own(rhs.get());
}

OptiSpline OptiSplineSol::opti() const {
  Opti s = OptiSol::opti();
  OptiSpline ret;
  ret.own(s.get());
  return ret;
}

OptiSpline::OptiSpline(const OptiSplineAdvanced& rhs) {
  own(rhs.get());
}
