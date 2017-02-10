#include "optistack.h"
#include "../common.h"
#include <casadi/core/mx/mx_node.hpp>

using namespace casadi;

Optistack::Optistack() {

}

MX Optistack::var(int n, int m) {
  return flag(MX::sym("x", n, m), OPTISTACK_VAR);
}

MX Optistack::par(int n, int m) {
  return flag(MX::sym("p", n, m), OPTISTACK_PAR);
}

void Optistack::assert_has(const MX& m) const {
  bool found = data_.find(m.get())!=data_.end();
  spline_assert_message(found, "Symbol not found in Optistack.");
}

MX Optistack::flag(const MX& m, OptistackType type) {
  auto find = data_.find(m.get());
  spline_assert(find==data_.end());
  data_[m.get()] = type;
  return m;
}

OptistackSolver Optistack::solver(const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) const {
  return OptistackSolver(*this, f, g, solver, options);
}

std::map< OptistackType, std::vector<MX> > Optistack::categorize(const MX& expr) {
  std::vector<MX> deps = symvar(expr);

  std::set< MXNode* > dep_nodes;
  for (const auto& i : deps)
    dep_nodes.insert(i.get());

  std::map< OptistackType, std::vector<MX> > ret;

  ret[OPTISTACK_VAR] = std::vector<MX>();
  ret[OPTISTACK_PAR] = std::vector<MX>();

  for (const auto& d : data_) {
    if (dep_nodes.find(d.first)!=dep_nodes.end()) {
      ret[d.second].push_back(MX::create(d.first));
    }
  }

  return ret;
}

// Solve the problem
void OptistackSolver::solve() {

  const std::vector<MX>& x = variables_[OPTISTACK_VAR];
  const std::vector<MX>& p = variables_[OPTISTACK_PAR];
  std::vector<DM> x0_;
  x0_.reserve(x.size());

  std::vector<DM> p_;
  p_.reserve(p.size());

  // Initialize decision variables with zero
  for (auto& v : x)
    x0_.push_back(data_[v.get()][0]);

  // Initialize decision variables with nan
  for (auto& v : p)
    p_.push_back(data_[v.get()][0]);

  arg_["x0"] = veccat(x0_);
  arg_["p"]  = veccat(p_);

  res_ = solver_(arg_);

  solved_ = true;
}

DM OptistackSolver::value(const MX& x) const {
  spline_assert(solved_);
  Function helper = Function("helper", std::vector<MX>{nlp_.at("x"), nlp_.at("p")}, {x});
  std::vector<DM> arg = helper(std::vector<DM>{res_.at("x"), arg_.at("p")});
  return arg[0];
}

OptistackSolver::OptistackSolver(const Optistack& sc, const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) : sc_(sc) {
  solved_ = false;

  MX total_expr = vertcat(f, veccat(g));
  variables_ = sc_.categorize(total_expr);

  const std::vector<MX>& x = variables_[OPTISTACK_VAR];
  const std::vector<MX>& p = variables_[OPTISTACK_PAR];

  nlp_["x"] = veccat(x);
  nlp_["p"] = veccat(p);

  nlp_["f"] = f;
  constraints_ = categorize_constraints(g);
  std::vector<MX> g_all;
  for (const auto& cm : constraints_) {
    g_all.insert(g_all.end(), cm.flattened.begin(), cm.flattened.end());
  }

  nlp_["g"] = veccat(g_all);
  int ng = nlp_["g"].is_null()? 0 : nlp_["g"].size1();
  solver_ = nlpsol("solver", solver, nlp_, options);
  // Allocate numeric matrics

  // Initialize decision variables with zero
  for (auto& v : x)
    data_[v.get()] = std::vector<DM>(1, DM::zeros(v.sparsity()));

  // Initialize decision variables with nan
  for (auto& v : p)
    data_[v.get()] = std::vector<DM>(1, DM::nan(v.sparsity()));

  // Set constraints
  std::vector<DM> lbg;lbg.reserve(ng);
  std::vector<DM> ubg;lbg.reserve(ng);
  for (const auto& cm : constraints_) {
    if (cm.is_equality) {
      lbg.push_back(DM::zeros(veccat(cm.flattened).sparsity()));
      ubg.push_back(DM::zeros(veccat(cm.flattened).sparsity()));
    } else {
      lbg.push_back(-inf*DM::ones(veccat(cm.flattened).sparsity()));
      ubg.push_back(DM::zeros(veccat(cm.flattened).sparsity()));
    }
  }

  arg_["lbg"] = veccat(lbg);
  arg_["ubg"] = veccat(ubg);
}

std::vector<MetaCon> OptistackSolver::categorize_constraints(const std::vector<MX>& g) {

  std::vector<MX> pure;
  std::vector<bool> equalities;
  std::vector<MetaCon> ret(g.size());

  for (int i=0;i<g.size();++i) {
    MX c = g[i];
    MetaCon& r = ret[i];
    r.original = c;
    if (c.is_op(OP_LE) || c.is_op(OP_LT)) {
      std::vector<MX> args;
      while (c.is_op(OP_LE) || c.is_op(OP_LT)) {
        args.push_back(c.dep(1));
        c = c.dep(0);
      }
      args.push_back(c);
      for (int j=0;j<args.size()-1;++j) {
        r.flattened.push_back(args[j+1]-args[j]);
        r.is_equality = false;
      }
    } else if (c.is_op(OP_EQ)) {
        r.flattened.push_back(c.dep(0)-c.dep(1));
        r.is_equality = true;
    } else {
      spline_assert_message(false, "Constraint type unkown. Use ==, >= or <= .");
    }
  }

  return ret;
}

void OptistackSolver::assert_has(const MX& m) const {
  sc_.assert_has(m);
  bool found = data_.find(m.get())!=data_.end();
  spline_assert_message(found, "Optistack symbol is not used in Solver."
    " It does not make sense to assign a value to it.");
}

void OptistackSolver::value(const MX& x, const DM& v) {
  solved_ = false;
  assert_has(x);
  std::vector<DM>& targets = data_.at(x.get());
  DM& target = targets[0];
  Slice all;
  target.set(v, false, all, all);
}

spline::Function OptiSplineSolver::value(const spline::Function& f) const {
  return spline::Function(f.getTensorBasis(), value(f.getCoefficient()));
}

spline::Coefficient OptiSplineSolver::value(const spline::Coefficient& c) const {
  return spline::Coefficient(value(c.getData()));
}

void OptiSplineSolver::value(const spline::Coefficient& c, const Tensor<DM>& d) {
  spline_assert_message(c.getData().is_MT(), "Value only supported for MX");
  value(c.getData().as_MT(), d);
}

AnyTensor OptiSplineSolver::value(const AnyTensor& t) const {
  if (t.is_DT()) return t.as_DT();
  if (t.is_MT()) return value(t.as_MT());
  spline_assert_message(false, "Value only supported for MX");
  return DT();
}

Tensor<DM> OptiSplineSolver::value(const Tensor<MX>& t) const {
  return Tensor<DM>(OptistackSolver::value(t.data()), t.dims());
}

void OptiSplineSolver::value(const Tensor<MX>& t, const Tensor<DM>& d) {
  spline_assert_message(t.squeeze().dims()==d.squeeze().dims(),
    "Tensor dimensions must match. Got " << t.dims() << " and " << d.dims() <<".");
  OptistackSolver::value(t.data(), d.data());
}

spline::Function OptiSpline::Function(const spline::TensorBasis& b) {
  std::vector<int> shape = b.getShape();
  shape.push_back(1);
  shape.push_back(1);

  return spline::Function(b, spline::Coefficient(var(shape)));
}

MT OptiSpline::var(const std::vector<int> & shape) {
  return MT(var(product(shape)), shape);
}


OptiSplineSolver OptiSpline::solver(const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) const {
  return OptiSplineSolver(*this, f, g, solver, options);
}

OptiSplineSolver::OptiSplineSolver(const OptiSpline& sc, const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) : OptistackSolver(sc, f, g, solver, options) {

}
