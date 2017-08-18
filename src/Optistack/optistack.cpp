#include "optistack.h"
#include "../common.h"
#//include <casadi/core/mx/mx_node.hpp>

using namespace casadi;

Opti::Opti() : count_(0) {

}

MX Opti::var(int n, int m, const std::string& attribute) {

  // Prepare metadata
  MetaVar meta_data;
  meta_data.attribute = attribute;
  meta_data.n = n;
  meta_data.m = m;
  meta_data.type = OPTI_VAR;
  meta_data.count = count_++;

  MX symbol, ret;

  if (attribute=="symmetric") {
    casadi_assert_message(n==m, "You specified attribute 'symmetric', "
      "while matrix is not even square, but " << n << "-by-" << m << ".");
    symbol = MX::sym("x_" + std::to_string(count_), n*(n+1)/2);
    ret = tril2symm(MX(Sparsity::lower(n), symbol));
  } else if (attribute=="full") {
    symbol = MX::sym("x_" + std::to_string(count_), n, m);
    ret = symbol;
  } else {
    casadi_error("Unknown attribute '" << attribute << "'. Choose from 'full' or 'symmetric'.");
  }

  // Store the symbol; preventing it from going ut of scope
  store_.push_back(symbol);

  set_meta(symbol, meta_data);
  return ret;
}

MX Opti::par(int n, int m, const std::string& attribute) {
  casadi_assert(attribute=="full");

  // Prepare metadata
  MetaVar meta_data;
  meta_data.attribute = attribute;
  meta_data.n = n;
  meta_data.m = m;
  meta_data.type = OPTI_PAR;
  meta_data.count = count_++;
  
  MX symbol = MX::sym("p_" + std::to_string(count_), n, m);
  store_.push_back(symbol);
  set_meta(symbol, meta_data);
  return symbol;
}

void Opti::set_meta(const MX& m, const MetaVar& meta) {
  meta_[m.get()] = meta;
}

const Opti::MetaVar& Opti::meta(const MX& m) const {
  assert_has(m);
  auto find = meta_.find(m.get());
  return find->second;
}

Opti::MetaVar Opti::get_meta(const MX& m) const {
  return meta(m);
}

void Opti::assert_has(const MX& m) const {
  bool found = meta_.find(m.get())!=meta_.end();
  spline_assert_message(found, "Symbol not found in Opti.");
}

OptiSolver Opti::solver(const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) const {
  return OptiSolver(*this, f, g, solver, options);
}

std::vector<MX> Opti::sort(const std::vector<MX>& v) const {
  // We exploit the fact that std::map is ordered

  // Populate map
  std::map<int, MX> unordered;
  for (const auto& d : v)
    unordered[meta(d).count] = d;

  // Loop over map (ordered)
  std::vector<MX> ret;
  for (auto const &e : unordered)
    ret.push_back(e.second);
  return ret;
}

std::vector<MX> Opti::symvar() const {
  return store_;
}

std::vector<MX> Opti::symvar(const MX& expr) const {
  return sort(MX::symvar(expr));
}

std::vector<MX> Opti::ineq_unchain(const MX& a, bool& flipped) {
  flipped = false; 
  casadi_assert(a.is_op(OP_LE) || a.is_op(OP_LT));

  // Is there inequalities in the left or right leaf?
  bool left  = a.dep(0).is_op(OP_LE) || a.dep(0).is_op(OP_LT);
  bool right = a.dep(1).is_op(OP_LE) || a.dep(1).is_op(OP_LT);
  casadi_assert(!left || !right);
  
  if (!left && !right)
    return {a.dep(0), a.dep(1)}; // Simple inequality

  // We have a chain of inequalities
  bool ineq = !left;
  std::vector<MX> ret = {a.dep(!ineq)};
  MX e = a.dep(ineq);
  while (e.is_op(OP_LE) || e.is_op(OP_LT)) {
    casadi_assert(!e.is_op(OP_EQ));
    casadi_assert(!e.dep(!ineq).is_op(OP_LE) && !e.dep(!ineq).is_op(OP_LT));
    ret.push_back(e.dep(!ineq));
    e = e.dep(ineq);
  }
  ret.push_back(e);
  if (left) std::reverse(ret.begin(), ret.end());
  flipped = !left;

  return ret;
}

bool Opti::is_parametric(const MX& expr) const {
  return symvar(expr, Opti::OPTI_VAR).empty();
}

Opti::MetaCon Opti::canon_expr(const MX& expr) const {
  MX c = expr;

  MetaCon con;
  con.original = expr;

  if (c.is_op(OP_LE) || c.is_op(OP_LT)) { // Inequalities
    std::vector<MX> ret;
    bool flipped;
    std::vector<MX> args = ineq_unchain(c, flipped);
    std::vector<bool> parametric;
    for (auto &a : args) parametric.push_back(is_parametric(a));

    if (args.size()==2 && (parametric[0] || parametric[1])) {
      // case: g(x,p) <= bound(p)
      MX e = args[0]-args[1];
      if (e.is_vector()) {
        casadi_assert(!parametric[0] || !parametric[1]);
        con.type = OPTI_INEQUALITY;
        if (parametric[0]) {
          con.lb = args[0]*DM::ones(e.sparsity());
          con.ub = inf*DM::ones(e.sparsity());
          con.canon = args[1];
        } else {
          con.lb = -inf*DM::ones(e.sparsity());
          con.ub = args[1]*DM::ones(e.sparsity());
          con.canon = args[0];
        }
        return con;
      }
      // Fall through to generic inequalities
    } else if (args.size()==3 && (parametric[0] || parametric[2])) {
      // lb(p) <= g(x,p) <= ub(p)
      con.type = OPTI_DOUBLE_INEQUALITY;
      con.lb = args[0]*DM::ones(args[1].sparsity());
      con.ub = args[2]*DM::ones(args[1].sparsity());
      con.canon = args[1];
      con.flipped = flipped;
      con.n = 2;
      return con;
    }

    bool type_known = false;
    for (int j=0;j<args.size()-1;++j) {
      MX e = args[j]-args[j+1];
      if (e.is_vector()) {
        // g1(x,p) <= g2(x,p)
        ret.push_back(e);
        spline_assert(!type_known || con.type==OPTI_GENERIC_INEQUALITY);
        type_known = true;
        con.type = OPTI_GENERIC_INEQUALITY;
        con.flipped = flipped;
      } else {
        // A(x,p) >= b(p)
        e = args[j+1]-args[j];
        casadi_assert_message(e.size1()==e.size2(), "Matrix inequalities must be square. Did you mean element-wise inequality instead?");

        ret.push_back(e);
        spline_assert(!type_known || con.type==OPTI_PSD);
        type_known = true;
        con.type = OPTI_PSD;
      }
    }

    if (con.type==OPTI_GENERIC_INEQUALITY) {
      con.canon = veccat(ret);
      con.lb = -inf*DM::ones(con.canon.sparsity());
      con.ub = DM::zeros(con.canon.sparsity());
      con.n = ret.size();
    } else {
      con.canon = diagcat(ret);
      con.n = ret.size();
    }
    return con;
  } else if (c.is_op(OP_EQ)) { // Inequalities
    
    MX e = c.dep(0)-c.dep(1);
    if (is_parametric(c.dep(0))) {
      con.canon = c.dep(1);
      con.lb = c.dep(0)*DM::ones(e.sparsity());
      con.type = OPTI_EQUALITY;
    } else if (is_parametric(c.dep(1))) {
      con.canon = c.dep(0);
      con.lb = c.dep(1)*DM::ones(e.sparsity());
      con.type = OPTI_EQUALITY;
    } else {
      con.lb = DM::zeros(con.canon.sparsity());
      con.canon = e;
      con.type = OPTI_GENERIC_EQUALITY;
    }
    con.ub = con.lb;
    return con;
  } else { // Something else
    con.type = OPTI_UNKNOWN;
    con.canon = c;
    return con;
  }

}

std::vector<MX> Opti::symvar(const MX& expr, VariableType type) const {
  std::vector<MX> ret;
  for (const auto& d : symvar(expr)) {
    if (meta(d).type==type) ret.push_back(d);
  }

  return ret;
}

// Solve the problem
void OptiSolver::solve() {
  solve_prepare();
  res(solve_actual(arg_));
}

// Solve the problem
void OptiSolver::solve_prepare() {

  // Get initial guess and parameter values
  arg_["x0"] = veccat(values(Opti::OPTI_VAR));
  arg_["p"]  = veccat(values(Opti::OPTI_PAR));

  // Evaluate bounds for given parameter values
  DMDict arg;
  arg["p"] = arg_["p"];
  DMDict res = bounds_(arg);
  arg_["lbg"] = res["lbg"];
  arg_["ubg"] = res["ubg"];

}

DMDict OptiSolver::solve_actual(const DMDict& arg) const {
  return solver_(arg);
}

DM OptiSolver::dual(const MX& con) const {
  spline_assert(solved_);
  
  // Look up index of constraint
  auto it = con_lookup_.find(con.get());
  casadi_assert_message(it!=con_lookup_.end(), "Unknown constraint");
  int i = it->second;

  const Opti::MetaCon& c = meta_con_[i];

  int start = start_con_[i];
  int stop = stop_con_[i];

  int N = stop-start;
  DM ret = DM::zeros(repmat(c.original.sparsity(), 1, c.n));
  std::vector<double>& ret_nz = ret.nonzeros();

  const std::vector<double>& lam_g = res_.at("lam_g").nonzeros();
  if (c.type==Opti::OPTI_DOUBLE_INEQUALITY) {
    for (int i=0;i<N;++i) {
      double v = (c.flipped ? -1 : 1) * lam_g[start+i];
      if (v<0) {
        ret_nz[i] = -v;
      } else {
        ret_nz[N+i] = v;
      }
    }
  } else {
    int block_size = N / c.n;
    for (int i=0;i<c.n;++i) {
      int p = c.flipped? c.n-i-1: i;
      for (int k=0;k<block_size;++k) {
        ret_nz[p*block_size+k] = fabs(lam_g[start+i*block_size+k]);
      }
    }
  }
  return ret.T();
}

DM OptiSolver::value(const MX& x) const {
  spline_assert(solved_);
  Function helper = Function("helper", std::vector<MX>{nlp_.at("x"), nlp_.at("p")}, {x});
  std::vector<DM> arg = helper(std::vector<DM>{res_.at("x"), arg_.at("p")});
  return arg[0];
}

OptiSolver::OptiSolver(const Opti& sc, const MX& f, const std::vector<MX> & g) : sc_(sc), g_(g), f_(f) {
  solved_ = false;

  // Gather all expressions
  MX total_expr = vertcat(f, veccat(g));

  // Categorize the symbols appearing in those expressions
  for (const auto& d : sc.symvar(total_expr))
    symbols_[sc.meta(d).type].push_back(d);

  // Fill the nlp definition
  nlp_["x"] = veccat(symvar(Opti::OPTI_VAR));
  nlp_["p"] = veccat(symvar(Opti::OPTI_PAR));

  nlp_["f"] = f;

  // Allocate space for constraint metadata
  meta_con_.reserve(g.size());
  start_con_.reserve(g.size());
  stop_con_.reserve(g.size());

  int offset = 0;
  for (int i=0;i<g.size();++i) {
    // Store the meta-dat for each constraint 
    Opti::MetaCon r = sc_.canon_expr(g[i]);
    meta_con_.push_back(r);

    // Compute offsets for this constraint:
    // location into the global constraint variable
    start_con_.push_back(offset);
    offset+= r.canon.nnz();
    stop_con_.push_back(offset);

    // Add entry to constraint lookup table
    con_lookup_[r.original.get()] = i;

  }

  // Collect bounds and canonical form of constraints
  std::vector<MX> g_all;
  std::vector<MX> lbg_all;
  std::vector<MX> ubg_all;
  for (const auto& cm : meta_con_) {
    g_all.push_back(cm.canon);
    lbg_all.push_back(cm.lb);
    ubg_all.push_back(cm.ub);
  }

  nlp_["g"] = veccat(g_all);

  // Create bounds helper function
  MXDict bounds;
  bounds["p"] = nlp_["p"];
  bounds["lbg"] = veccat(lbg_all);
  bounds["ubg"] = veccat(ubg_all);

  bounds_ = Function("bounds", bounds, {"p"}, {"lbg", "ubg"});

  // Initialize decision variables with zero
  for (auto& v : symvar(Opti::OPTI_VAR))
    data_[v.get()] = std::vector<DM>(1, DM::zeros(v.sparsity()));

  // Initialize parameters with nan
  for (auto& v : symvar(Opti::OPTI_PAR))
    data_[v.get()] = std::vector<DM>(1, DM::nan(v.sparsity()));
}

OptiSolver::OptiSolver(const Opti& sc, const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) : OptiSolver(sc, f, g) {

  solver_ = nlpsol("solver", solver, nlp_, options);

  // Verify the constraint types
  for (const auto& cm : meta_con_) {
    if (cm.type==Opti::OPTI_PSD)
      spline_error("Psd constraints not implemented yet. "
      "Perhaps you intended an element-wise inequality? "
      "In that case, make sure that the matrix is flattened (e.g. mat(:)).");
    if (cm.type==Opti::OPTI_UNKNOWN)
     spline_error("Constraint type unknown. Use ==, >= or <= .");
  }
}

void OptiSolver::assert_has(const MX& m) const {
  sc_.assert_has(m);
  bool found = data_.find(m.get())!=data_.end();
  spline_assert_message(found, "Opti symbol is not used in Solver."
    " It does not make sense to assign a value to it.");
}

void OptiSolver::value(const MX& x, const DM& v) {
  solved_ = false;
  assert_has(x);
  std::vector<DM>& targets = data_.at(x.get());
  DM& target = targets[0];
  Slice all;
  target.set(v, false, all, all);
}

std::vector<MX> OptiSolver::symvar(Opti::VariableType type) const {
  if (symbols_.find(type)==symbols_.end()) return std::vector<MX>();
  return symbols_.at(type);
}

std::vector<DM> OptiSolver::values(Opti::VariableType type) const {
  std::vector<MX> v = symvar(type);
  std::vector<DM> ret;
  ret.reserve(v.size());

  // Initialize decision variables with zero
  for (auto& e : v) {
    auto it = data_.find(e.get());
    spline_assert(it!=data_.end());
    ret.push_back(it->second[0]);
  }

  return ret;
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

void OptiSplineSolver::value(const spline::Function& f, const spline::Function& g) {
  spline_assert_message(f.coeff().data().is_MT(), "Value only supported for MX");
  value(f.coeff_tensor().as_MT(), g.project_to(f.tensor_basis()).coeff_tensor().as_DT());
}

AnyTensor OptiSplineSolver::value(const AnyTensor& t) const {
  if (t.is_DT()) return t.as_DT();
  if (t.is_MT()) return value(t.as_MT());
  spline_assert_message(false, "Value only supported for MX");
  return DT();
}

Tensor<DM> OptiSplineSolver::value(const Tensor<MX>& t) const {
  return Tensor<DM>(OptiSolver::value(t.data()), t.dims());
}

void OptiSplineSolver::value(const Tensor<MX>& t, const Tensor<DM>& d) {
  spline_assert_message(t.squeeze().dims()==d.squeeze().dims(),
    "Tensor dimensions must match. Got " << t.dims() << " and " << d.dims() <<".");
  OptiSolver::value(t.data(), d.data());
}

spline::Function OptiSpline::Function(const spline::TensorBasis& b,
    const std::vector<int>& var_shape, const std::string& attribute) {

  casadi_assert(var_shape.size()<=2);
  int n = var_shape.size()>=1 ? var_shape[0] : 1;
  int m = var_shape.size()==2 ? var_shape[1] : 1;

  return spline::Function(b, spline::Coefficient(coeff_var(b.dimension(), n, m, attribute)));
}

MT OptiSpline::coeff_var(const std::vector<int> & shape, int n, int m, const std::string& attribute) {
  int cs = spline::product(shape);

  if (attribute=="full") {
    std::vector<int> ret_shape = shape;
    ret_shape.push_back(n);
    ret_shape.push_back(m);
    return MT(var(spline::product(ret_shape)), ret_shape);
  }

  std::vector<MX> args;
  for (int i=0;i<cs;++i)
    args.push_back(var(n, m, attribute));

  std::vector<int> t_shape = {n, m};
  t_shape.insert(t_shape.end(), shape.begin(), shape.end());
  MT t = MT(horzcat(args), t_shape);

  std::vector<int> reorder = casadi::range(2, shape.size()+2);
  reorder.push_back(0);
  reorder.push_back(1);

  return t.reorder_dims(reorder);
}


OptiSplineSolver OptiSpline::solver(const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) const {
  return OptiSplineSolver(*this, f, g, solver, options);
}

OptiSplineSolver::OptiSplineSolver(const OptiSpline& sc, const MX& f, const std::vector<MX> & g,
    const std::string& solver, const Dict& options) : OptiSolver(sc, f, g, solver, options) {

}

OptiSplineSolver::OptiSplineSolver(const OptiSpline& sc, const MX& f, const std::vector<MX> & g) : OptiSolver(sc, f, g) {

}

void OptiSpline::matlab_dump(const casadi::Function& f, const std::string &fname) const {
  std::ofstream ss(fname);


  ss << "function [varargout] = " << f.name() << "(args)" << std::endl;
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
    ss << "end" << std::endl;
}
