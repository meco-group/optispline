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
  std::ofstream ss(fname);


  ss << "function [varargout] = " << f.name() << "(args)" << std::endl;
  ss << "feature('jit','off');feature('accel','on');" << std::endl;
  for (int i=0;i<f.n_in();++i) {
    ss << "  argin_" << i <<  " = args{" << i+1 << "};" << std::endl;
  }
  for (int i=0;i<f.n_out();++i) {
    ss << "  argout_" << i <<  " = cell(" << f.nnz_out(i) << ",1);" << std::endl;
  }
  for (int i=0;i<f.getAlgorithmSize();++i) {
    int op = f.getAtomicOperation(i);
    int i1 = f.getAtomicOutput(i);
    switch (op) {
      case OP_INPUT:
        {
          const std::pair<int, int> ip = f.getAtomicInput(i); int i2=ip.first;int i3=ip.second;
          ss << "  w" << i1 << " = " << "argin_" << i2 << "(" << i3+1 << ");" << std::endl;
        }
        break;
      case OP_OUTPUT:
        {
          const std::pair<int, int> ip = f.getAtomicInput(i); int i2=ip.first;int i3=ip.second;
          ss << "  argout_" << i1 << "{" << i3+1 << "} = w" << i2 << ";" << std::endl;
        }
        break;
      case OP_CONST:
        {
          std::ios_base::fmtflags fmtfl = ss.flags();
          ss << "  w" << i1 << " = ";
          ss << std::scientific << std::setprecision(std::numeric_limits<double>::digits10 + 1);
          ss << f.getAtomicInputReal(i) << ";" << std::endl;
          ss.flags(fmtfl);
        }
        break;
      case OP_SQ:
        {
          const std::pair<int, int> ip = f.getAtomicInput(i); int i2=ip.first;
          ss << "  w" << i1 << " = " << "w" << i2 << "^2;" << std::endl;
        }
        break;
      default:
        const std::pair<int, int> ip = f.getAtomicInput(i); int i2=ip.first;int i3=ip.second;
        if (casadi::casadi_math<double>::ndeps(op)==2) {
          ss << "  w" << i1 << " = " << casadi::casadi_math<double>::print(op, "w"+std::to_string(i2), "w"+std::to_string(i3)) << ";" << std::endl;
        } else {
          ss << "  w" << i1 << " = " << casadi::casadi_math<double>::print(op, "w"+std::to_string(i2)) << ";" << std::endl;          
        }
    }
  }

  for (int i=0;i<f.n_out();++i) {
    ss << "  varargout{" << i+1 <<  "} = [argout_" << i <<"{:}];" << std::endl;
  }
  
    ss << "feature('jit','on');feature('accel','on');" << std::endl;
    ss << "end" << std::endl;
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
