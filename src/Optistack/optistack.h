#ifndef OPTISPLINE_H
#define OPTISPLINE_H

#include "optistack_interface.h"

using namespace casadi;

class OptiSplineAdvanced;
class OptiSplineSol;

class OptiSpline: public Opti, public OptiSplineInterface<OptiSpline> {
  friend class OptiSplineAdvanced;
public:
    OptiSpline copy() const { return *this; }
    OptiSplineAdvanced debug() const;
    OptiSplineAdvanced advanced() const;
    OptiSplineSol solve();

    using OptiSplineInterface<OptiSpline>::set_initial;
    using OptiSplineInterface<OptiSpline>::set_value;
    using Opti::set_initial;
    using Opti::set_value;
    OptiSpline(const OptiSplineAdvanced& rhs);
    OptiSpline() {}
#ifndef SWIG
    typedef Opti type_base;
#endif
};

class OptiSplineAdvanced: public OptiAdvanced, public OptiSplineInterface<OptiSplineAdvanced> {
    friend class OptiSpline;
public:
  OptiSplineSol solve();

  AnyTensor value(const AnyTensor& c) const;
  #ifndef SWIG
  Tensor<DM> value(const Tensor<MX>& c, const std::vector< Tensor<MX> >& values=std::vector< Tensor<MX> >()) const;
  #endif
  spline::Function value(const spline::Function& f) const;
  spline::Coefficient value(const spline::Coefficient& c) const;
  using OptiAdvanced::value;

  std::vector<MX> symvar(const MT& m) const { return symvar(m.data()); }
  std::vector<MX> symvar(const MT& expr, VariableType type) const { return symvar(expr.data(), type); }
  using OptiAdvanced::symvar;

  void matlab_dump(const casadi::Function& f, const std::string &fname) const;

#ifndef SWIG
    typedef OptiAdvanced type_base;
#endif

  using OptiSplineInterface<OptiSplineAdvanced>::set_initial;
  using OptiSplineInterface<OptiSplineAdvanced>::set_value;
  using OptiAdvanced::set_initial;
  using OptiAdvanced::set_value;
  OptiSplineAdvanced(const OptiSpline& rhs);
protected:
  OptiSplineAdvanced() {};
};

class OptiSplineSol : public OptiSol {
  friend class OptiSplineAdvanced;
  friend class OptiSpline;
  public:
    spline::Function value(const spline::Function& f) const;
    spline::Coefficient value(const spline::Coefficient& c) const;
    using OptiSol::value;
    OptiSpline opti() const;
    OptiSplineSol(const OptiSpline& opti) : OptiSol(opti) {}
  protected:
    OptiSplineSol(const OptiSol& sol) : OptiSol(sol) {}
};


#endif //OPTISPLINE_H
