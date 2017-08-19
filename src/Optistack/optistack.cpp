#include "optistack.h"
#include "../common.h"
#//include <casadi/core/mx/mx_node.hpp>

using namespace casadi;

class InternalOptiCallback : public Callback {
  public:    
  InternalOptiCallback(OptiStack* sol);

  ~InternalOptiCallback();

  // Initialize the object
  void init() override;

  // Number of inputs and outputs
  int get_n_in() override;

  Sparsity get_sparsity_in(int i);

  // Evaluate numerically
  std::vector<DM> eval(const std::vector<DM>& arg) override;

  void set_sol(OptiStack* sol) { sol_= sol;}

  private:
    OptiStack* sol_;
};


InternalOptiCallback::InternalOptiCallback(OptiStack* sol) : sol_(sol) {
}

InternalOptiCallback::~InternalOptiCallback() {  };

// Initialize the object
void InternalOptiCallback::init() {
}

// Number of inputs and outputs
int InternalOptiCallback::get_n_in() { return nlpsol_n_out();}

Sparsity InternalOptiCallback::get_sparsity_in(int i) {
  std::string n = nlpsol_out(i);
  int size = 0;
  if (n=="f") {
    size = 1;
  } else if (n=="lam_x" || n=="x") {
    size = sol_->nx();
  } else if (n=="lam_g" || n=="g") {
    size = sol_->ng();
  } else if (n=="p" || n=="lam_p") {
    size = sol_->np();
    if (size==0) return Sparsity::dense(0, 0);
  } else {
    return Sparsity::dense(0, 0);
  }
  return Sparsity::dense(size, 1);
}

// Evaluate numerically
std::vector<DM> InternalOptiCallback::eval(const std::vector<DM>& arg) {
  DMDict r;

  for (int i=0;i<nlpsol_n_out();++i) {
    r[nlpsol_out(i)] = arg[i];
  }

  sol_->res(r);


  for (OptiCallback* cb : sol_->callbacks_)
    cb->call();
  return {0};
}

void OptiStack::callback_class(OptiCallback* callback) {
  callbacks_ = {callback};
}

void OptiStack::callback_class() {
  callbacks_ = {};
}

OptiStack::OptiStack() : count_(0), count_var_(0), count_par_(0) {
  solver_has_callback_ = false;
  internal_callback_ = 0;
  mark_problem_dirty();
}

MX OptiStack::variable(int n, int m, const std::string& attribute) {

  // Prepare metadata
  MetaVar meta_data;
  meta_data.attribute = attribute;
  meta_data.n = n;
  meta_data.m = m;
  meta_data.type = OPTI_VAR;
  meta_data.count = count_++;
  meta_data.i = count_var_++;

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
  symbols_.push_back(symbol);
  initial_.push_back(DM::zeros(symbol.sparsity()));
  latest_.push_back(DM::nan(symbol.sparsity()));

  set_meta(symbol, meta_data);
  return ret;
}

MX OptiStack::parameter(int n, int m, const std::string& attribute) {
  casadi_assert(attribute=="full");

  // Prepare metadata
  MetaVar meta_data;
  meta_data.attribute = attribute;
  meta_data.n = n;
  meta_data.m = m;
  meta_data.type = OPTI_PAR;
  meta_data.count = count_++;
  meta_data.i = count_par_++;

  MX symbol = MX::sym("p_" + std::to_string(count_), n, m);
  symbols_.push_back(symbol);
  values_.push_back(DM::nan(symbol.sparsity()));

  set_meta(symbol, meta_data);
  return symbol;
}

Dict OptiStack::stats() const {
  assert_solved();
  return solver_.stats();
}

std::string OptiStack::return_status() const {
  Dict mystats = stats();
  if (mystats.find("return_status")!=mystats.end())
    return mystats.at("return_status");
  return "unkown";
}

Function OptiStack::casadi_solver() const {
  return solver_;
}

void OptiStack::set_meta(const MX& m, const MetaVar& meta) {
  meta_[m.get()] = meta;
}

const OptiStack::MetaVar& OptiStack::meta(const MX& m) const {
  assert_has(m);
  auto find = meta_.find(m.get());
  return find->second;
}

OptiStack::MetaVar& OptiStack::meta(const MX& m) {
  assert_has(m);
  auto find = meta_.find(m.get());
  return find->second;
}

OptiStack::MetaVar OptiStack::get_meta(const MX& m) const {
  return meta(m);
}

bool OptiStack::has(const MX& m) const {
  return meta_.find(m.get())!=meta_.end();
}

void OptiStack::assert_has(const MX& m) const {
  spline_assert_message(has(m), "Symbol not found in Opti stack.");
}

std::vector<MX> OptiStack::initial() const {
  std::vector<MX> ret;
  for (const auto& e : symvar()) {
    if (meta(e).type==OPTI_VAR)
      ret.push_back(e==initial_[meta(e).i]);
  }
  return ret;
}

std::vector<MX> OptiStack::value() const {
  std::vector<MX> ret;
  for (const auto& e : symvar()) {
    if (meta(e).type==OPTI_VAR)
      ret.push_back(e==latest_[meta(e).i]);
  }
  return ret;
}

void OptiStack::internal_bake() {
  casadi_assert_message(!f_.is_empty() || !g_.empty(),
    "You need to specify at least an objective (y calling 'minimize'), "
    "or a constraint (by calling 'subject_to').");

  symbol_active_.clear();
  symbol_active_.resize(symbols_.size());

  // Gather all expressions
  MX total_expr = vertcat(f_, veccat(g_));

  // Categorize the symbols appearing in those expressions
  for (const auto& d : symvar(total_expr))
    symbol_active_[meta(d).count] = true;

  std::vector<MX> x = active_symvar(OptiStack::OPTI_VAR);
  int offset = 0;
  for (const auto& v : x) {
    meta(v).start = offset;
    offset+= v.nnz();
    meta(v).stop = offset;
  }
  std::vector<MX> p = active_symvar(OptiStack::OPTI_PAR);

  // Fill the nlp definition
  nlp_["x"] = veccat(x);
  nlp_["p"] = veccat(p);

  nlp_["f"] = f_;

  // Allocate space for constraint Fmetadata
  meta_con_.clear();
  meta_con_.reserve(g_.size());

  offset = 0;
  for (int i=0;i<g_.size();++i) {
    // Store the meta-dat for each constraint 
    meta_con_.push_back(canon_expr(g_[i]));
    OptiStack::MetaCon& r = meta_con_.back();

    // Compute offsets for this constraint:
    // location into the global constraint variable
    r.start = offset;
    offset+= r.canon.nnz();
    r.stop = offset;

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
  mark_problem_dirty(false);
  userOut() << "Baked problem structure" << std::endl;
  userOut() << "  #vars: " << x.size()  << " (scalarized " << nx() << ")" << std::endl;
  if (np()!=0) {
    userOut() << "  #pars: " << p.size() << " (scalarized " << np() << ")" << std::endl;
  }
  if (ng()==0) {
    userOut() << "  unconstrained" << std::endl;
  } else {
    userOut() << "  #constraints: " << g_.size() << " (scalarized " << ng() << ")" << std::endl;
  }
}

void OptiStack::solver(const std::string& solver_name, const Dict& solver_options) {
  solver_name_ = solver_name;
  solver_options_ = solver_options;
  mark_solver_dirty();
}

std::vector<MX> OptiStack::sort(const std::vector<MX>& v) const {
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

std::vector<MX> OptiStack::symvar() const {
  return symbols_;
}

std::vector<MX> OptiStack::symvar(const MX& expr) const {
  return sort(MX::symvar(expr));
}

std::vector<MX> OptiStack::ineq_unchain(const MX& a, bool& flipped) {
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

bool OptiStack::is_parametric(const MX& expr) const {
  return symvar(expr, OptiStack::OPTI_VAR).empty();
}

OptiStack::MetaCon OptiStack::canon_expr(const MX& expr) const {
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
      con.lb = DM::zeros(e.sparsity());
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

void OptiStack::assert_solved() const {
  casadi_assert_message(solved(),
    "This action is forbidden since you have not solved the Opti stack yet (with calling 'solve').");
}
void OptiStack::assert_empty() const {
  casadi_assert(g_.empty());
  casadi_assert(f_.is_empty());
}

void OptiStack::minimize(const MX& f) {
  mark_problem_dirty();
  f_ = f;
}

void OptiStack::subject_to(const std::vector<MX>& g) {
  mark_problem_dirty();
  g_.insert(g_.end(), g.begin(), g.end());
}

void OptiStack::subject_to() {
  mark_problem_dirty();
  g_.clear();
}

std::vector<MX> OptiStack::symvar(const MX& expr, VariableType type) const {
  std::vector<MX> ret;
  for (const auto& d : symvar(expr)) {
    if (meta(d).type==type) ret.push_back(d);
  }

  return ret;
}

void OptiStack::res(const DMDict& res) {
  const std::vector<double> & x_v = res.at("x").nonzeros();
  for (const auto &v : active_symvar(OPTI_VAR)) {
    int i = meta(v).i;
    std::vector<double> & data_v = latest_[i].nonzeros();
    std::copy(x_v.begin()+meta(v).start, x_v.begin()+meta(v).stop, data_v.begin());
  }
  res_ = res;
  mark_solved();
}

// Solve the problem
OptiSol OptiStack::solve() {

  if (problem_dirty()) {
    internal_bake();

    // Verify the constraint types
    for (const auto& cm : meta_con_) {
      if (cm.type==OptiStack::OPTI_PSD)
        spline_error("Psd constraints not implemented yet. "
        "Perhaps you intended an element-wise inequality? "
        "In that case, make sure that the matrix is flattened (e.g. mat(:)).");
    }

  }

  bool need_callback = !callbacks_.empty();

  bool solver_update =  solver_dirty() ||  need_callback!=  solver_has_callback_;

  if (solver_update) {
    Dict opts = solver_options_;

    // Handle callbacks
    if (need_callback) {
      if (!internal_callback_) delete internal_callback_;
      internal_callback_ = new InternalOptiCallback(this);
      internal_callback_->construct("InternalOptiCallback", Dict());
      callback_ = *internal_callback_;
      internal_callback_->transfer_ownership();

      //callback_ = InternalOptiCallback::create();
      
      opts["iteration_callback"] = callback_;
    } else {
      if (!internal_callback_) delete internal_callback_;
      internal_callback_ = 0;
    }
    solver_has_callback_ = need_callback;

    casadi_assert_message(solver_name_!="", "You must call 'solver' on the Opti stack to select a solver.");
    solver_ = nlpsol("solver", solver_name_, nlp_, opts);
    userOut() << "Initialized " << solver_name_ << " solver." << std::endl;
  }

  solve_prepare();
  res(solve_actual(arg_));

  std::string ret = return_status();

  bool success = ret=="Solve_Succeeded" || ret=="Solved_To_Acceptable_Level";

  casadi_assert_message(success,
    "Solver failed. You may use opti.debug_value to investigate the latest values of variables. return_status is '" <<
      ret << "'");

  return copy();
}

// Solve the problem
void OptiStack::solve_prepare() {


  // Verify the constraint types
  for (const auto& cm : meta_con_) {
    if (cm.type==OptiStack::OPTI_UNKNOWN)
     spline_error("Constraint type unknown. Use ==, >= or <= .");
  }

  if (internal_callback_)
    internal_callback_->set_sol(this);

  // Get initial guess and parameter values
  arg_["x0"] = veccat(active_values(OptiStack::OPTI_VAR));
  arg_["p"]  = veccat(active_values(OptiStack::OPTI_PAR));
  casadi_assert_message(arg_["p"].is_regular(),
    "You have forgotten to assign a value to a parameter ('set_value'), or have set it to NaN/Inf.");

  // Evaluate bounds for given parameter values
  DMDict arg;
  arg["p"] = arg_["p"];
  DMDict res = bounds_(arg);
  arg_["lbg"] = res["lbg"];
  arg_["ubg"] = res["ubg"];

}

DMDict OptiStack::solve_actual(const DMDict& arg) {
  return solver_(arg);
}

DM OptiStack::dual(const MX& con) const {
  assert_solved();
  
  // Look up index of constraint
  auto it = con_lookup_.find(con.get());
  casadi_assert_message(it!=con_lookup_.end(), "Unknown constraint");
  int i = it->second;

  const OptiStack::MetaCon& c = meta_con_[i];

  int N = c.stop-c.start;
  DM ret = DM::zeros(repmat(c.original.sparsity(), 1, c.n));
  std::vector<double>& ret_nz = ret.nonzeros();

  const std::vector<double>& lam_g = res_.at("lam_g").nonzeros();
  if (c.type==OptiStack::OPTI_DOUBLE_INEQUALITY) {
    for (int i=0;i<N;++i) {
      double v = (c.flipped ? -1 : 1) * lam_g[c.start+i];
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
        ret_nz[p*block_size+k] = fabs(lam_g[c.start+i*block_size+k]);
      }
    }
  }
  return ret.T();
}

DM OptiStack::value(const MX& expr, const std::vector<MX>& values) const {
  std::vector<MX> x = symvar(expr, OPTI_VAR);
  std::vector<MX> p = symvar(expr, OPTI_PAR);

  Function helper = Function("helper", std::vector<MX>{veccat(x), veccat(p)}, {expr});
  if (helper.has_free())
    casadi_error("This expression has symbols that are not defined within Opti using variable/parameter.");


  std::map<int, MX> temp;
  for (const auto& v : values) {
    casadi_assert(v.is_op(OP_EQ));
    int i = meta(v.dep(1)).i;
    casadi_assert(v.dep(0).is_constant());
    temp[i] = v.dep(0);
  }

  bool undecided_vars = false;
  std::vector<DM> x_num;
  for (const auto& e: x) {
    int i = meta(e).i;
    x_num.push_back(latest_[i]);

    // Override when values are supplied
    auto it = temp.find(i);
    if (it==temp.end()) {
      undecided_vars = true;
    } else {
      Slice all;
      DM t = static_cast<DM>(it->second);
      x_num.back().set(t, false, all, all);
    }

  }

  if (undecided_vars) {
    assert_solved();
    for (const auto& e: x) casadi_assert_message(symbol_active_[meta(e).count], "This expression has symbols that do not appear in the constraints and objective.");
  }

  std::vector<DM> p_num;
  for (const auto& e: p) {
    p_num.push_back(values_[meta(e).i]);
  }

  std::vector<DM> arg = helper(std::vector<DM>{veccat(x_num), veccat(p_num)});
  return arg[0];
}

void OptiStack::assert_active_symbol(const MX& m) const {
  assert_has(m);
  spline_assert_message(symbol_active_[meta(m).count], "Opti symbol is not used in Solver."
    " It does not make sense to assign a value to it.");
}

void OptiStack::set_initial(const std::vector<MX>& assignments) {
  for (const auto& v : assignments) {
    casadi_assert(v.is_op(OP_EQ));
    casadi_assert(v.dep(0).is_constant());
    if (has(v.dep(1)))
      set_initial(v.dep(1), static_cast<DM>(v.dep(0)));
  }
}

void OptiStack::set_value_internal(const MX& x, const DM& v, std::vector<DM>& store) {
  casadi_assert(v.is_regular());
  if (x.is_symbolic()) {
    DM& target = store[meta(x).i];
    Slice all;
    mark_solver_dirty();
    target.set(v, false, all, all);
    return;
  }

  // Obtain symbolic primitives
  std::vector<MX> symbols = MX::symvar(x);
  MX symbols_cat = veccat(symbols);

  std::string failmessage = "You cannot set initial/value of an arbitrary expression. Use symbols or simple mappings of symbols.";

  // Assert x is linear in its symbolic primitives
  for (bool b : which_depends(x, symbols_cat, 2, false)) casadi_assert_message(!b, failmessage);

  // Evaluate jacobian of expr wrt symbols
  Function Jf("Jf", {}, {jacobian(x, veccat(symbols), {{"compact",true}})});
  DM J = Jf(std::vector<DM>{})[0];
  Sparsity sp_JT = J.T().sparsity();

  Function Ff("Ff", symbols, {x});
  DM E = Ff(std::vector<DM>(symbols.size(), 0))[0];
  std::vector<double>& e = E.nonzeros();

  // Cast the v input into the expected sparsity
  Slice all;
  DM value(x.sparsity());
  value.set(v, false, all, all);

  // Get rows and columns of the mapping
  std::vector<int> row, col;
  J.sparsity().get_triplet(row, col);
  const std::vector<double>& scaling = J.nonzeros();
  const std::vector<double>& data_original = value.nonzeros();

  std::vector<double> data; data.reserve(value.nnz());
  for (int i=0;i<value.nnz();++i) {
    double v = data_original[i];
    int nz = sp_JT.colind()[i+1]-sp_JT.colind()[i];
    casadi_assert_message(nz<=1, failmessage);
    if (nz) {
      data.push_back(v);
    } else {
      casadi_assert_message(v==e[i], "In initial/value assignment: inconsistent numerical values. "
        "At nonzero " << i << ", lhs has " << e[i] << ", while rhs has " << v << ".");
    }
  }

  // Contiguous workspace for nonzeros of all involved symbols
  std::vector<double> temp(symbols_cat.nnz(), casadi::nan);
  for (int k=0;k<data.size();++k) {
    double& lhs = temp[col[k]];
    double rhs = data[row[k]]/scaling[row[k]];
    if (std::isnan(lhs)) {
      // Assign in the workspace
      lhs = rhs;
    } else {
      casadi_assert_message(lhs==rhs, "Initial/value assignment with mapping is ambiguous.");
    }
  }

  int offset = 0;
  for (const auto & s : symbols) {
    DM& target = store[meta(s).i];
    std::vector<double>& data = target.nonzeros();
    // Loop over nonzeros in each symbol
    for (int i=0;i<s.nnz();++i) {
      // Copy from the workspace (barring fields that were not set)
      double v = temp[offset+i];
      if (!std::isnan(v)) data[i] = v;
    }
    offset+=s.nnz();
  }

}

void OptiStack::set_initial(const MX& x, const DM& v) {
  for (const auto & s: MX::symvar(x))
    casadi_assert_message(meta(s).type!=OPTI_PAR, "You cannot set an initial value for a parameter. Did you mean 'set_value'?");
  set_value_internal(x, v, initial_);
}

void OptiStack::set_value(const MX& x, const DM& v) {
  for (const auto & s: MX::symvar(x))
    casadi_assert_message(meta(s).type!=OPTI_VAR, "You cannot set a value for a variable. Did you mean 'set_initial'?");
  set_value_internal(x, v, values_);
}

std::vector<MX> OptiStack::active_symvar(OptiStack::VariableType type) const {
  std::vector<MX> ret;
  for (const auto& s: symbols_) {
    if (symbol_active_[meta(s).count] && meta(s).type==type)
      ret.push_back(s);
  }
  return ret;
}

std::vector<DM> OptiStack::active_values(OptiStack::VariableType type) const {
  std::vector<DM> ret;
  for (const auto& s: symbols_) {
    if (symbol_active_[meta(s).count] && meta(s).type==type) {
      if (type==OPTI_VAR) {
        ret.push_back(initial_[meta(s).i]);
      } else if (type==OPTI_PAR) {
        ret.push_back(values_[meta(s).i]);
      }
    }
  }
  return ret;
}

spline::Function OptiStackSpline::value(const spline::Function& f) const {
  return spline::Function(f.tensor_basis(), value(f.coeff()));
}

spline::Coefficient OptiStackSpline::value(const spline::Coefficient& c) const {
  return spline::Coefficient(value(c.data()));
}

void OptiStackSpline::set_initial(const spline::Coefficient& c, const Tensor<DM>& d) {
  spline_assert_message(c.data().is_MT(), "Value only supported for MX");
  set_initial(c.data().as_MT(), d);
}

void OptiStackSpline::set_value(const spline::Coefficient& c, const Tensor<DM>& d) {
  spline_assert_message(c.data().is_MT(), "Value only supported for MX");
  set_value(c.data().as_MT(), d);
}

void OptiStackSpline::set_value(const spline::Function& f, const spline::Function& g) {
  spline_assert_message(f.coeff().data().is_MT(), "Value only supported for MX");
  set_value(f.coeff_tensor().as_MT(), g.project_to(f.tensor_basis()).coeff_tensor().as_DT());
}

void OptiStackSpline::set_initial(const spline::Function& f, const spline::Function& g) {
  spline_assert_message(f.coeff().data().is_MT(), "Value only supported for MX");
  set_initial(f.coeff_tensor().as_MT(), g.project_to(f.tensor_basis()).coeff_tensor().as_DT());
}

AnyTensor OptiStackSpline::value(const AnyTensor& t) const {
  if (t.is_DT()) return t.as_DT();
  if (t.is_MT()) return value(t.as_MT());
  spline_assert_message(false, "Value only supported for MX");
  return DT();
}

Tensor<DM> OptiStackSpline::value(const Tensor<MX>& t, const std::vector< Tensor<MX> >& values) const {
  std::vector<MX> values_data;
  for (const auto& e: values) values_data.push_back(e.data());
  return Tensor<DM>(OptiStack::value(t.data(), values_data), t.dims());
}

void OptiStackSpline::set_value(const Tensor<MX>& t, const Tensor<DM>& d) {
  spline_assert_message(t.squeeze().dims()==d.squeeze().dims(),
    "Tensor dimensions must match. Got " << t.dims() << " and " << d.dims() <<".");
  OptiStack::set_value(t.data(), d.data());
}

void OptiStackSpline::set_initial(const Tensor<MX>& t, const Tensor<DM>& d) {
  spline_assert_message(t.squeeze().dims()==d.squeeze().dims(),
    "Tensor dimensions must match. Got " << t.dims() << " and " << d.dims() <<".");
  OptiStack::set_initial(t.data(), d.data());
}

spline::Function OptiStackSpline::Function(const spline::TensorBasis& b,
    const std::vector<int>& var_shape, const std::string& attribute) {

  casadi_assert(var_shape.size()<=2);
  int n = var_shape.size()>=1 ? var_shape[0] : 1;
  int m = var_shape.size()==2 ? var_shape[1] : 1;

  return spline::Function(b, spline::Coefficient(coeff_var(b.dimension(), n, m, attribute)));
}

MT OptiStackSpline::coeff_var(const std::vector<int> & shape, int n, int m, const std::string& attribute) {
  int cs = spline::product(shape);

  if (attribute=="full") {
    std::vector<int> ret_shape = shape;
    ret_shape.push_back(n);
    ret_shape.push_back(m);
    return MT(variable(spline::product(ret_shape)), ret_shape);
  }

  std::vector<MX> args;
  for (int i=0;i<cs;++i)
    args.push_back(variable(n, m, attribute));

  std::vector<int> t_shape = {n, m};
  t_shape.insert(t_shape.end(), shape.begin(), shape.end());
  MT t = MT(horzcat(args), t_shape);

  std::vector<int> reorder = casadi::range(2, shape.size()+2);
  reorder.push_back(0);
  reorder.push_back(1);

  return t.reorder_dims(reorder);
}

void OptiStackSpline::matlab_dump(const casadi::Function& f, const std::string &fname) const {
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

// Solve the problem
OptiSplineSol OptiStackSpline::solve() {
  OptiStack::solve();
  return copy();
}