#include "optistack.h"
#include "../common.h"
#include <casadi/core/mx/mx_node.hpp>

using namespace casadi;

Optistack::Optistack() : count_(0) {

}

MX Optistack::var(int n, int m, const std::string& variable_type) {
  Dict meta_data;
  meta_data["variable_type"] = variable_type;
  meta_data["n"] = n;
  meta_data["m"] = m;
  meta_data["type"] = OPTISTACK_VAR;
  meta_data["count"] = count_;
  count_+=1;
  if (variable_type=="symmetric") {
    spline_assert(n==m);
    MX ret = MX(Sparsity::lower(n), meta(flag(MX::sym("x_" + std::to_string(count_), n*(n+1)/2), OPTISTACK_VAR), meta_data));

    return tril2symm(ret);
  } else {
    return meta(flag(MX::sym("x_" + std::to_string(count_), n, m), OPTISTACK_VAR), meta_data);
  }
}

MX Optistack::par(int n, int m, const std::string& variable_type) {
  Dict meta_data;
  meta_data["variable_type"] = variable_type;
  meta_data["n"] = n;
  meta_data["m"] = m;
  meta_data["type"] = OPTISTACK_PAR;
  meta_data["count"] = count_;
  count_+=1;
  return meta(flag(MX::sym("p_" + std::to_string(count_), n, m), OPTISTACK_PAR), meta_data);
}

MX Optistack::meta(const MX& m, const Dict& dict) {
  assert_has(m);
  auto find = meta_.find(m.get());
  if (find==meta_.end()) meta_[m.get()] = Dict();

  Dict& prev_meta = meta_[m.get()];
  for (auto& kv : dict) {
    prev_meta[kv.first] = kv.second;
  }
  return m;
}
Dict Optistack::meta(const MX& m) const {
  assert_has(m);
  auto find = meta_.find(m.get());
  if (find==meta_.end()) return Dict();
  return find->second;
}

void Optistack::assert_has(const MX& m) const {
  bool found = data_.find(m.get())!=data_.end();
  spline_assert_message(found, "Symbol not found in Optistack.");
}

MX Optistack::flag(const MX& m, VariableType type) {
  auto find = data_.find(m.get());
  // NOTE: m.get() may in fact exist already;
  //  memory may have been reclaimed
  if (find!=data_.end()) casadi::userOut() << "possible mem issue" << std::endl;
  //spline_assert(find==data_.end());
  data_[m.get()] = type;
  store_.push_back(m);
  return m;
}

OptistackSolver Optistack::solver(const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) const {
  return OptistackSolver(*this, f, g, solver, options);
}

std::vector<MX> Optistack::sort(const std::vector<MX>& v) const {
  std::map<int, MX> unordered;
  for (const auto& d : v) {
    Dict m = meta(d);
    unordered[m["count"]] = d;
  }
  std::vector<MX> ret;
  for (auto const &e : unordered)
    ret.push_back(e.second);
  return ret;
}

std::vector<MX> Optistack::symvar() const {
  std::vector<MX> ret;
  for (auto& it : data_)
    ret.push_back(MX::create(it.first));
  return sort(ret);
}

std::vector<MX> Optistack::symvar(const MX& m) const {
  return sort(MX::symvar(m));
}

std::vector<MX> ineq_unpack(const MX& a) {
  if (a.is_op(OP_LE) || a.is_op(OP_LT)) {
    std::vector<MX> lhs = ineq_unpack(a.dep(0));
    std::vector<MX> rhs = ineq_unpack(a.dep(1));
    lhs.insert(lhs.end(), rhs.begin(), rhs.end());
    return lhs;
  } else {
    return {a};
  }
}

MX Optistack::canon_expr(const MX& expr, ConstraintType& type) {
  MX c = expr;

  type = OPTISTACK_UNKNOWN;
  if (c.is_op(OP_LE) || c.is_op(OP_LT)) {
    std::vector<MX> ret;
    std::vector<MX> args = ineq_unpack(c);
    for (int j=0;j<args.size()-1;++j) {
      MX e = args[j]-args[j+1];
      if (e.is_vector()) {
        ret.push_back(e);
        spline_assert(type==OPTISTACK_UNKNOWN || type==OPTISTACK_INEQUALITY);
        type = OPTISTACK_INEQUALITY;
      } else {
        //if (args[j+1].is_scalar()) {
        //  e = DM::eye(args[j+1].size1())*args[j+1]-args[j];
        //} else if (args[j].is_scalar()) {
        //  e = args[j+1]-DM::eye(args[j+1].size1())*args[j];
        //} else {
        e = args[j+1]-args[j];
        //}

        ret.push_back(e);
        spline_assert(type==OPTISTACK_UNKNOWN || type==OPTISTACK_PSD);
        type = OPTISTACK_PSD;
      }
    }

    if (type==OPTISTACK_INEQUALITY) {
      return veccat(ret);
    } else {
      return diagcat(ret);
    }
  } else if (c.is_op(OP_EQ)) {
    spline_assert(type==OPTISTACK_UNKNOWN || type==OPTISTACK_EQUALITY);
    type = OPTISTACK_EQUALITY;
    return c.dep(0)-c.dep(1);
  } else {
    spline_assert(type==OPTISTACK_UNKNOWN || type==OPTISTACK_EXPR);
    type = OPTISTACK_EXPR;
    return c;
  }

}

std::vector<MX> Optistack::symvar(const MX& expr, VariableType type) const {
  std::vector<MX> deps = symvar(expr);

  std::vector<MX> ret;
  for (const auto& d : deps) {
    auto search = data_.find(d.get());
    if (search!=data_.end()) {
      if (search->second==type) ret.push_back(d);
    }
  }

  return ret;
}

std::map< Optistack::VariableType, std::vector<MX> > Optistack::categorize(const MX& expr) {
  std::vector<MX> deps = symvar(expr);

  std::set< MXNode* > dep_nodes;
  for (const auto& i : deps)
    dep_nodes.insert(i.get());

  std::map< VariableType, std::vector<MX> > ret;

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
  solve_prepare();
  res(solve_actual(arg_));
}

// Solve the problem
void OptistackSolver::solve_prepare() {
  arg_["x0"] = veccat(values(Optistack::OPTISTACK_VAR));
  arg_["p"]  = veccat(values(Optistack::OPTISTACK_PAR));
}

DMDict OptistackSolver::solve_actual(const DMDict& arg) const {
  return solver_(arg);
}

DM OptistackSolver::value(const MX& x) const {
  spline_assert(solved_);
  Function helper = Function("helper", std::vector<MX>{nlp_.at("x"), nlp_.at("p")}, {x});
  std::vector<DM> arg = helper(std::vector<DM>{res_.at("x"), arg_.at("p")});
  return arg[0];
}

OptistackSolver::OptistackSolver(const Optistack& sc, const MX& f, const std::vector<MX> & g) : sc_(sc) {
  solved_ = false;

  MX total_expr = vertcat(f, veccat(g));
  variables_ = sc_.categorize(total_expr);

  std::vector<MX> x = symvar(Optistack::OPTISTACK_VAR);
  std::vector<MX> p = symvar(Optistack::OPTISTACK_PAR);

  nlp_["x"] = veccat(x);
  nlp_["p"] = veccat(p);

  nlp_["f"] = f;
  constraints_ = Optistack::categorize_constraints(g);
  std::vector<MX> g_all;
  for (const auto& cm : constraints_) {
    g_all.push_back(cm.flattened);
  }

  nlp_["g"] = veccat(g_all);

  // Initialize decision variables with zero
  for (auto& v : x)
    data_[v.get()] = std::vector<DM>(1, DM::zeros(v.sparsity()));

  // Initialize decision variables with nan
  for (auto& v : p)
    data_[v.get()] = std::vector<DM>(1, DM::nan(v.sparsity()));
}

OptistackSolver::OptistackSolver(const Optistack& sc, const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) : OptistackSolver(sc, f, g) {

  int ng = nlp_["g"].is_null()? 0 : nlp_["g"].size1();
  solver_ = nlpsol("solver", solver, nlp_, options);

  // Set constraints
  std::vector<DM> lbg;lbg.reserve(ng);
  std::vector<DM> ubg;lbg.reserve(ng);
  for (const auto& cm : constraints_) {
    switch (cm.type) {
      case Optistack::OPTISTACK_EQUALITY:
        lbg.push_back(DM::zeros(cm.flattened.sparsity()));
        ubg.push_back(DM::zeros(cm.flattened.sparsity()));
        break;
      case Optistack::OPTISTACK_INEQUALITY:
        lbg.push_back(-inf*DM::ones(cm.flattened.sparsity()));
        ubg.push_back(DM::zeros(cm.flattened.sparsity()));
        break;
      case Optistack::OPTISTACK_PSD:
        spline_error("Psd constraints not implemented yet. "
          "Perhaps you intended an element-wise inequality? "
          "In that case, make sure that the matrix is flattened (e.g. mat(:)).");
      case Optistack::OPTISTACK_EXPR:
        spline_error("Constraint type unknown. Use ==, >= or <= .");
      case Optistack::OPTISTACK_UNKNOWN:
        spline_error("Bug in toolbox. Please notify authors.");
    }
  }

  arg_["lbg"] = veccat(lbg);
  arg_["ubg"] = veccat(ubg);
}

std::vector<Optistack::MetaCon> Optistack::categorize_constraints(const std::vector<MX>& g) {

  std::vector<Optistack::MetaCon> ret(g.size());

  for (int i=0;i<g.size();++i) {
    MX c = g[i];
    MetaCon& r = ret[i];
    r.original = c;
    r.flattened = canon_expr(c, r.type);
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
  return spline::Function(f.tensor_basis(), value(f.coeff()));
}

spline::Coefficient OptiSplineSolver::value(const spline::Coefficient& c) const {
  return spline::Coefficient(value(c.data()));
}

void OptiSplineSolver::value(const spline::Coefficient& c, const Tensor<DM>& d) {
  spline_assert_message(c.data().is_MT(), "Value only supported for MX");
  value(c.data().as_MT(), d);
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

spline::Function OptiSpline::Function(const spline::TensorBasis& b,
    const std::vector<int>& var_shape, const std::string& variable_type) {

  casadi_assert(var_shape.size()<=2);
  int n = var_shape.size()>=1 ? var_shape[0] : 1;
  int m = var_shape.size()==2 ? var_shape[1] : 1;

  return spline::Function(b, spline::Coefficient(coeff_var(b.dimension(), n, m, variable_type)));
}

MT OptiSpline::coeff_var(const std::vector<int> & shape, int n, int m, const std::string& variable_type) {
  int cs = spline::product(shape);

  if (variable_type=="full") {
    std::vector<int> ret_shape = shape;
    ret_shape.push_back(n);
    ret_shape.push_back(m);
    return MT(var(spline::product(ret_shape)), ret_shape);
  }

  std::vector<MX> args;
  for (int i=0;i<cs;++i)
    args.push_back(var(n, m, variable_type));

  std::vector<int> t_shape = {n, m};
  t_shape.insert(t_shape.end(), shape.begin(), shape.end());
  MT t = MT(horzcat(args), t_shape);

  std::vector<int> reorder = casadi::range(2, shape.size()+2);
  reorder.push_back(0);
  reorder.push_back(1);

  return t.reorder_dims(reorder);
}

std::vector<MX> OptistackSolver::symvar(Optistack::VariableType type) const {
  auto it = variables_.find(type);
  spline_assert(it!=variables_.end());
  return sc_.sort(it->second);
}

std::vector<DM> OptistackSolver::values(Optistack::VariableType type) const {
  std::vector<MX> v = symvar(type);
  std::vector<DM> ret;
  ret.reserve(ret.size());

  // Initialize decision variables with zero
  for (auto& e : v) {
    auto it = data_.find(e.get());
    spline_assert(it!=data_.end());
    ret.push_back(it->second[0]);
  }

  return ret;
}

OptiSplineSolver OptiSpline::solver(const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) const {
  return OptiSplineSolver(*this, f, g, solver, options);
}

OptiSplineSolver::OptiSplineSolver(const OptiSpline& sc, const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) : OptistackSolver(sc, f, g, solver, options) {

}

OptiSplineSolver::OptiSplineSolver(const OptiSpline& sc, const MX& f, const std::vector<MX> & g) : OptistackSolver(sc, f, g) {

}
