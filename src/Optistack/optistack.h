#ifndef OPTI_H
#define OPTI_H

#include <string>
#include <vector>

#include <casadi/casadi.hpp>

#include "../Function/Function.h"
#include "../Coefficients/Coefficient.h"

using namespace casadi;

class OptiSolver;

class Opti {
  friend class OptiSolver;
public:
  enum ConstraintType {
    OPTI_GENERIC_EQUALITY,  // g1(x,p) == g2(x,p)
    OPTI_GENERIC_INEQUALITY,  // g1(x,p) <= g2(x,p)
    OPTI_EQUALITY, // g(x,p) == bound(p)
    OPTI_INEQUALITY,  // g(x,p) <= bound(p)
    OPTI_DOUBLE_INEQUALITY,  // lb(p) <= g(x,p) <= ub(p)
    OPTI_PSD, // A(x,p) >= b(p)
    OPTI_UNKNOWN};
  enum VariableType {
    OPTI_VAR, // variable
    OPTI_PAR  // parameter
    };

  struct MetaCon {
    MetaCon() :  n(1), flipped(false) {}
    MX original;  // original expression
    MX canon; // Canonical expression
    ConstraintType type;
    MX lb;
    MX ub;
    int n;
    bool flipped;
  };
  struct MetaVar {
    std::string attribute;
    int n;
    int m;
    VariableType type;
    int count;
    Dict extra;
  };

  /** \brief Create Opti Context
  */
  Opti();

  /** \brief Create a decision variable (symbol)
  *
  * The order of creation matters.
  * The order will be reflected in the optimization problem.
  * It is not required for decision variables to actualy appear in the optimization problem.
  */
  MX var(int n=1, int m=1, const std::string& attribute="full");
  /** \brief Create a parameter (symbol); fixed during optimization
  *
  * The order of creation does not matter.
  * It is not required for parameter to actualy appear in the optimization problem.
  * Parameters that do appear, must be given a value before the problem can be solved.
  */
  MX par(int n=1, int m=1, const std::string& attribute="full");

  /** Construct a solver object */
  OptiSolver solver(const MX& f, const std::vector<MX> & g, const std::string& solver, const Dict& options=Dict()) const;

  /// return true if expression is only dependant on Opti parameters, not variables
  bool is_parametric(const MX& expr) const;

  /// @{
  /** \brief Get symbols present in expression 
  * 
  *  Returned vector is ordered according to the order of
  *  var()/par() calls used to create the variables
  */
  std::vector<MX> symvar() const;
  std::vector<MX> symvar(const MX& expr) const;
  std::vector<MX> symvar(const MX& expr, VariableType type) const;
  /// @}

  /// Interpret an expression (for internal use only)
  Opti::MetaCon canon_expr(const MX& expr) const;

  /// Get meta-data of symbol (for internal use only)
  MetaVar get_meta(const MX& m) const;

  /// Set meta-data of an expression
  void set_meta(const MX& m, const MetaVar& meta);

#ifndef SWIGMATLAB
  /** \brief Construct a double inequality
  *
  * Constructs:  lb(p) <= g(x,p) <= ub(p)
  * 
  * Python prohibits such syntax directly
  */
  static MX bounded(const MX& lb, const MX& expr, const MX& ub) { return (lb<=expr)<= ub; }
#endif

private:
  /** \brief decompose a chain of inequalities
  *
  * a<=b -> [a,b]
  * a<=b<=c [a,b,c]
  *
  * When flipped is set, [a,b,c] corresponds to c>=b>=a
  */
  static std::vector<MX> ineq_unchain(const MX& a, bool& SWIG_OUTPUT(flipped));
 
  /// Get meta-dat by const-ref
  const MetaVar& meta(const MX& m) const;

  /// Sort symbols according to Opti order
  std::vector<MX> sort(const std::vector<MX>& v) const;

  /// Throw an error 
  void assert_has(const MX& m) const;

  /// Map symbols to metadata
  std::map<MXNode*, MetaVar> meta_;

  /// Store references to all var/par symbols
  std::vector<MX> store_;

  /// Symbol counter
  int count_;

};

class OptiSolver {
  friend class Opti;
public:

  // Set value of variable
  void value(const MX& x, const DM& v);

  DM dual(const MX& con) const;

  #ifndef SWIG
  // Get value
  DM value(const MX& x) const;
  // Get value
  DM value(const DM& x) const { return x; };
  DM value(const SX& x) const { return DM::nan(x.sparsity()); };
  #endif

  void assert_has(const MX& m) const;

  std::vector<MX> symvar(Opti::VariableType type) const;
  std::vector<DM> values(Opti::VariableType type) const;

  // Solve the problem
  void solve();

  void solve_prepare();
  DMDict solve_actual(const DMDict& args) const;

  DMDict arg() const { return arg_; }
  void res(const DMDict& res) { res_ = res; solved_ = true; }
  Function solver() const { return solver_; }
  std::vector<MX> g() const { return g_; }
  MX f() const { return f_; }

protected:
  OptiSolver(const Opti& sc, const MX& f, const std::vector<MX> & g);

  OptiSolver(const Opti& sc, const MX& f, const std::vector<MX> & g,
      const std::string& solver, const Dict& options);
private:

  /// Constraint lookup table: constraint -> index
  std::map<MXNode*, int > con_lookup_;

  Opti sc_;
  std::map<MXNode*, std::vector<DM> > data_;
  std::map< Opti::VariableType, std::vector<MX> > symbols_;

  Function solver_;
  Function forder_;
  DMDict res_;
  DMDict arg_;
  MXDict nlp_;

  // Bounds helper function: p -> lbg, ubg
  Function bounds_;
  DM pvalue_;
  std::vector< Opti::MetaCon > meta_con_;
  
  /// Starting indices for constraints
  std::vector< int > start_con_;
  /// Stopping indices for constraints
  std::vector< int > stop_con_;
  std::vector<MX> g_;
  MX f_;

  bool solved_;

};









class OptiSpline;

class OptiSplineSolver : public OptiSolver {
  friend class OptiSpline;
public:
  using OptiSolver::value;
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

class OptiSpline : public Opti {
  friend class OptiSplineSolver;
public:
  using Opti::var;
  using Opti::symvar;

  std::vector<MX> symvar(const MT& m) const { return symvar(m.data()); }
  std::vector<MX> symvar(const MT& expr, VariableType type) const { return symvar(expr.data(), type); }

#ifndef SWIG
  MT coeff_var(const std::vector<int>& shape, int n=1, int m=1, const std::string& attribute="full");
#endif
  spline::Function Function(const spline::TensorBasis& b, const std::vector<int>& shape=std::vector<int>(), const std::string& attribute="full");
  OptiSplineSolver solver(const MX& f, const std::vector<MX> & g,
      const std::string& solver, const Dict& options=Dict()) const;

  void matlab_dump(const casadi::Function& f, const std::string &fname) const;
};


#endif //OPTI_H
