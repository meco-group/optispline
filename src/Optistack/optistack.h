#ifndef OPTISTACK_H
#define OPTISTACK_H

#include <string>
#include <vector>

#include <casadi/casadi.hpp>

#include "../Function/Function.h"
#include "../Coefficients/Coefficient.h"

using namespace casadi;

class OptistackSolver;

class Optistack {
  friend class OptistackSolver;
public:
  enum ConstraintType {OPTISTACK_UNKNOWN,
  
    OPTISTACK_GENERIC_EQUALITY,  // g1(x,p) == g2(x,p)
    OPTISTACK_GENERIC_INEQUALITY,  // g1(x,p) <= g2(x,p)
    OPTISTACK_EQUALITY, // g(x,p) == bound(p)
    OPTISTACK_INEQUALITY,  // g(x,p) <= bound(p)
    OPTISTACK_DOUBLE_INEQUALITY,  // lb(p) <= g(x,p) <= ub(p)
    OPTISTACK_PSD, OPTISTACK_EXPR};
  enum VariableType {OPTISTACK_VAR, OPTISTACK_PAR};

  struct MetaCon {
    MX original;
    MX flattened;
    ConstraintType type;
    MX lb;
    MX ub;
    int start;
    int stop;
  };

  Optistack();
  MX var(int n=1, int m=1, const std::string& variable_type="full");
  MX par(int n=1, int m=1, const std::string& variable_type="full");

  OptistackSolver solver(const MX& f, const std::vector<MX> & g, const std::string& solver, const Dict& options=Dict()) const;

  void assert_has(const MX& m) const;

  MX meta(const MX& m, const Dict& dict);
  Dict meta(const MX& m) const;

  std::vector<MX> symvar() const;
  std::vector<MX> symvar(const MX& m) const;
  std::vector<MX> symvar(const MX& expr, VariableType type) const;

  std::vector<MX> sort(const std::vector<MX>& v) const;

  Optistack::MetaCon canon_expr(const MX& expr) const;
  bool is_parametric(const MX& expr) const;


protected:
  std::map< VariableType, std::vector<MX> > categorize(const MX& expr);

private:
  MX flag(const MX& m, VariableType type);
  std::map<MXNode*, VariableType> data_;
  std::map<MXNode*, Dict> meta_;
  std::vector<MX> store_;

  int count_;

};

class OptistackSolver {
  friend class Optistack;
public:

  // Set value
  void value(const MX& x, const DM& v);

  DM dual(const MX& x) const;

  #ifndef SWIG
  // Get value
  DM value(const MX& x) const;
  // Get value
  DM value(const DM& x) const { return x; };
  DM value(const SX& x) const { return DM::nan(x.sparsity()); };
  #endif

  void assert_has(const MX& m) const;

  std::vector<MX> symvar(Optistack::VariableType type) const;
  std::vector<DM> values(Optistack::VariableType type) const;

  // Solve the problem
  void solve();

  void solve_prepare();
  DMDict solve_actual(const DMDict& args) const;

  DMDict arg() const { return arg_; }
  void res(const DMDict& res) { res_ = res; solved_ = true; }
  Function solver() const { return solver_; }
protected:
  OptistackSolver(const Optistack& sc, const MX& f, const std::vector<MX> & g);

  OptistackSolver(const Optistack& sc, const MX& f, const std::vector<MX> & g,
      const std::string& solver, const Dict& options);
private:

  std::map<MXNode*, int > con_lookup_;

  Optistack sc_;
  std::map<MXNode*, std::vector<DM> > data_;
  std::map< Optistack::VariableType, std::vector<MX> > variables_;

  Function solver_;
  Function forder_;
  DMDict res_;
  DMDict arg_;
  MXDict nlp_;
  Function bounds_;
  DM pvalue_;
  std::vector< Optistack::MetaCon > constraints_;

  bool solved_;

};
class OptiSpline;

class OptiSplineSolver : public OptistackSolver {
  friend class OptiSpline;
public:
  using OptistackSolver::value;
  spline::Function value(const spline::Function& f) const;
  spline::Coefficient value(const spline::Coefficient& c) const;
  void value(const spline::Coefficient& c, const Tensor<DM>& d);
  void value(const spline::Function& f, const spline::Function& g);
  AnyTensor value(const AnyTensor& c) const;
  void value(const Tensor<MX>& c, const Tensor<DM>& d);
  Tensor<DM> value(const Tensor<MX>& c) const;

// Ought to be protected, but we are overloading this in Matlab
  OptiSplineSolver(const OptiSpline& sc, const MX& f, const std::vector<MX> & g);

protected:
  OptiSplineSolver(const OptiSpline& sc, const MX& f, const std::vector<MX> & g,
      const std::string& solver, const Dict& options);
};

class OptiSpline : public Optistack {
  friend class OptiSplineSolver;
public:
  using Optistack::var;
  using Optistack::symvar;

  std::vector<MX> symvar(const MT& m) const { return symvar(m.data()); }
  std::vector<MX> symvar(const MT& expr, VariableType type) const { return symvar(expr.data(), type); }

#ifndef SWIG
  MT coeff_var(const std::vector<int>& shape, int n=1, int m=1, const std::string& variable_type="full");
#endif
  spline::Function Function(const spline::TensorBasis& b, const std::vector<int>& shape=std::vector<int>(), const std::string& variable_type="full");
  OptiSplineSolver solver(const MX& f, const std::vector<MX> & g,
      const std::string& solver, const Dict& options=Dict()) const;

  void matlab_dump(const casadi::Function& f, const std::string &fname) const;
};


#endif //OPTISTACK_H
