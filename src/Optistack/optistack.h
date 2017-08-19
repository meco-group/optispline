#ifndef OPTI_H
#define OPTI_H

#include <string>
#include <vector>

#include <casadi/casadi.hpp>

#include "../Function/Function.h"
#include "../Coefficients/Coefficient.h"

using namespace casadi;

class InternalOptiCallback;


typedef DM nativeDM;

class OptiCallback {
public:
  virtual void call() {
    userOut() << "This is a simple callback" << std::endl;
  }
  virtual ~OptiCallback() {}
};

class OptiSol;
class OptiDebug;
class OptiStack {
  friend class InternalOptiCallback;
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

  struct IndexAbstraction {
    IndexAbstraction() : start(0), stop(0) {}
    int start;
    int stop;
  };
  struct MetaCon : IndexAbstraction {
    MetaCon() :  n(1), flipped(false) {}
    MX original;  // original expression
    MX canon; // Canonical expression
    ConstraintType type;
    MX lb;
    MX ub;
    int n;
    bool flipped;
  };
  struct MetaVar : IndexAbstraction {
    std::string attribute;
    int n;
    int m;
    VariableType type;
    int count;
    int i;
    Dict extra;
  };

  /** \brief Create Opti Context
  */
  OptiStack();

  /** \brief Create a decision variable (symbol)
  *
  * The order of creation matters.
  * The order will be reflected in the optimization problem.
  * It is not required for decision variables to actualy appear in the optimization problem.
  */
  MX variable(int n=1, int m=1, const std::string& attribute="full");
  /** \brief Create a parameter (symbol); fixed during optimization
  *
  * The order of creation does not matter.
  * It is not required for parameter to actualy appear in the optimization problem.
  * Parameters that do appear, must be given a value before the problem can be solved.
  */
  MX parameter(int n=1, int m=1, const std::string& attribute="full");

  /** \brief Set objective */
  void minimize(const MX& f);

  /// @{
  /** \brief Add to constraints */
  void subject_to(const MX& g) { subject_to(std::vector<MX>{g}); }
  void subject_to(const std::vector<MX>& g);
  /// @}

  /// Clear constraints
  void subject_to();

  /** Set a solver */
  void solver(const std::string& solver, const Dict& options=Dict());

  /// Set value of initial guess
  void set_initial(const MX& x, const DM& v);
  void set_initial(const std::vector<MX>& assignments);

  /// Set value of parameter
  void set_value(const MX& x, const DM& v);

  /// Crunch the numbers; solve the problem
  OptiSol solve();

  /// Get dual/lagrange multiplier 
  nativeDM dual(const MX& con) const;

  // Get value
  nativeDM value(const MX& x, const std::vector<MX>& values=std::vector<MX>()) const;
  // Get value
  nativeDM value(const DM& x, const std::vector<MX>& values=std::vector<MX>()) const { return x; };
  nativeDM value(const SX& x, const std::vector<MX>& values=std::vector<MX>()) const { return DM::nan(x.sparsity()); };

  /// Copy
  OptiStack copy() const { return *this; }

  /// Get statistics
  Dict stats() const;

  /// Get return status of solver
  std::string return_status() const;

  /// Get the underlying CasADi solver of the Opti stack
  Function casadi_solver() const;

  /// get assignment expressions for initial values
  std::vector<MX> initial() const;

  /// get assignment expressions for latest values
  std::vector<MX> value() const;

  void callback_class(OptiCallback* callback);
  void callback_class();

  /// return true if expression is only dependant on Opti parameters, not variables
  bool is_parametric(const MX& expr) const;

  /// @{
  /** \brief Get symbols present in expression 
  * 
  *  Returned vector is ordered according to the order of
  *  variable()/parameter() calls used to create the variables
  */
  std::vector<MX> symvar() const;
  std::vector<MX> symvar(const MX& expr) const;
  std::vector<MX> symvar(const MX& expr, VariableType type) const;
  /// @}

  /// Interpret an expression (for internal use only)
  OptiStack::MetaCon canon_expr(const MX& expr) const;

  /// Get meta-data of symbol (for internal use only)
  MetaVar get_meta(const MX& m) const;

  /// Set meta-data of an expression
  void set_meta(const MX& m, const MetaVar& meta);



  void assert_active_symbol(const MX& m) const;

  std::vector<MX> active_symvar(OptiStack::VariableType type) const;
  std::vector<DM> active_values(OptiStack::VariableType type) const;


  void solve_prepare();
  DMDict solve_actual(const DMDict& args);

  DMDict arg() const { return arg_; }
  void res(const DMDict& res);
  std::vector<MX> constraints() const { return g_; }
  MX objective() const { return f_; }

  int nx() const {
    return nlp_.at("x").size1();
  }

  int np() const {
    return nlp_.at("p").size1();
  }

  int ng() const {
    return nlp_.at("g").size1();
  }


  void assert_empty() const;


  /// Fix the structure of the optimization problem
  void internal_bake();
private:

  /// Set value of symbol
  void set_value_internal(const MX& x, const DM& v, std::vector<DM>& store);

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
  /// Get meta-dat by ref
  MetaVar& meta(const MX& m);

  /// Sort symbols according to Opti order
  std::vector<MX> sort(const std::vector<MX>& v) const;

  /// Throw an error 
  void assert_has(const MX& m) const;

  bool has(const MX& m) const;

  // Data members

  /// Map symbols to metadata
  std::map<MXNode*, MetaVar> meta_;

  /// Store references to all symbols
  std::vector<MX> symbols_;

  /// Symbol counter
  int count_;

  int count_var_;
  int count_par_;

  /// Constraint lookup table: constraint -> index
  std::map<MXNode*, int > con_lookup_;
  /// Meta-data for constraints
  std::vector< OptiStack::MetaCon > meta_con_;

  /// Storing latest values for all parameters (including inactive)
  std::vector<DM> values_;
  /// Storing initial values for all variables (including inactive)
  std::vector<DM> initial_;
  /// Storing latest values for all variables (including inactive)
  std::vector<DM> latest_;

  /// Is symbol present in problem?
  std::vector<bool> symbol_active_;

  /// Solver
  Function solver_;

  /// Result of solver
  DMDict res_;
  DMDict arg_;
  MXDict nlp_;

  /// Bounds helper function: p -> lbg, ubg
  Function bounds_;

  /// Constraints verbatim as passed in with 'subject_to'
  std::vector<MX> g_;

  /// Objective verbatim as passed in with 'minimize'
  MX f_;

  std::vector<OptiCallback*> callbacks_;
  InternalOptiCallback* internal_callback_;
  Function callback_;


  std::string solver_name_;
  Dict solver_options_;
  bool solver_has_callback_;
public:


  
  bool problem_dirty_;
  void mark_problem_dirty(bool flag=true) { problem_dirty_=flag; mark_solver_dirty(); }
  bool problem_dirty() const { return problem_dirty_; }

  bool solver_dirty_;
  void mark_solver_dirty(bool flag=true) { solver_dirty_=flag; mark_solved(false); }
  bool solver_dirty() const { return solver_dirty_; }

  bool solved_;
  void mark_solved(bool flag=true) { solved_ = flag;}
  bool solved() const { return solved_; }

  void assert_solved() const;
};

class Opti : private OptiStack {
  public:
    using OptiStack::variable;
    using OptiStack::parameter;
    using OptiStack::minimize;
    using OptiStack::subject_to;
    using OptiStack::solver;
    using OptiStack::solve;
    OptiStack debug() { return *this; }
    Opti copy() { return *this; }
    using OptiStack::set_initial;
    using OptiStack::set_value;
    using OptiStack::initial;
    using OptiStack::constraints;
    using OptiStack::objective;
    using OptiStack::callback_class;
    #ifndef SWIGMATLAB
    /** \brief Construct a double inequality
    *
    * Constructs:  lb(p) <= g(x,p) <= ub(p)
    * 
    * Python prohibits such syntax directly
    */
    static MX bounded(const MX& lb, const MX& expr, const MX& ub) { return (lb<=expr)<= ub; }
    #endif

};

class OptiSol : private OptiStack {
  public:
    OptiSol(const OptiStack& opti) : OptiStack(opti) {};
    using OptiStack::value;
    using OptiStack::dual;
    using OptiStack::stats;
    using OptiStack::casadi_solver;
    OptiStack debug() { return copy(); }
};

/**
{
  "Opti stack."

  if (state_==OPTI_FRESH) {
    ss << "  state: FRESH" << std::endl;
    ss << "  #symbols declared: " << count_ << std::endl;
    ss << "  #constraints declared: " << g_.size() << std::endl;
    ss << "  objective defined?: " << !f_.is_empty() << std::endl;
  } else if (state_==OPTI_BAKED) {
    ss << "  state: BAKED" << std::endl;
    ss << "  #scalar vars: " << nlp_["x"].size1() << std::endl;
    ss << "  #scalar pars: " << nlp_["p"].size1() << std::endl;
    ss << "  #scalar constraints: " << nlp_["g"].size() << std::endl;
  } else if (state_==OPTI_SOLVED) {
    ss << "  state: SOLVED" << std::endl;
  }

}
*/

class OptiSplineSol;
class OptiSplineDebug;

class OptiStackSpline : public OptiStack {
public:

  /// Copy
  OptiStackSpline copy() const { return *this; }

  /// Crunch the numbers; solve the problem; return false if failed
  OptiSplineSol solve();

  void set_initial(const spline::Coefficient& c, const Tensor<DM>& d);
  void set_initial(const spline::Function& f, const spline::Function& g);
  void set_initial(const Tensor<MX>& c, const Tensor<DM>& d);
  using OptiStack::set_initial;

  using OptiStack::set_value;
  void set_value(const spline::Coefficient& c, const Tensor<DM>& d);
  void set_value(const spline::Function& f, const spline::Function& g);
  void set_value(const Tensor<MX>& c, const Tensor<DM>& d);

  AnyTensor value(const AnyTensor& c) const;
  #ifndef SWIG
  Tensor<DM> value(const Tensor<MX>& c, const std::vector< Tensor<MX> >& values=std::vector< Tensor<MX> >()) const;
  #endif
  spline::Function value(const spline::Function& f) const;
  spline::Coefficient value(const spline::Coefficient& c) const;
  using OptiStack::value;

  using OptiStack::variable;
  using OptiStack::symvar;

  std::vector<MX> symvar(const MT& m) const { return symvar(m.data()); }
  std::vector<MX> symvar(const MT& expr, VariableType type) const { return symvar(expr.data(), type); }

#ifndef SWIG
  MT coeff_var(const std::vector<int>& shape, int n=1, int m=1, const std::string& attribute="full");
#endif
  spline::Function Function(const spline::TensorBasis& b, const std::vector<int>& shape=std::vector<int>(), const std::string& attribute="full");

  void matlab_dump(const casadi::Function& f, const std::string &fname) const;
};



class OptiSpline : private OptiStackSpline {
  public:
    using OptiStackSpline::variable;
    using OptiStackSpline::parameter;
    using OptiStackSpline::minimize;
    using OptiStackSpline::subject_to;
    using OptiStackSpline::solver;
    using OptiStackSpline::solve;
    OptiStackSpline debug() { return *this; }
    using OptiStackSpline::set_initial;
    using OptiStackSpline::set_value;
    using OptiStackSpline::initial;
    using OptiStackSpline::constraints;
    using OptiStackSpline::objective;
    using OptiStackSpline::callback_class;
    OptiSpline copy() { return *this; }
    #ifndef SWIGMATLAB
    /** \brief Construct a double inequality
    *
    * Constructs:  lb(x,p) <= g(x,p) <= ub(x,p)
    * 
    * Python prohibits such syntax directly
    */
    static MX bounded(const MX& lb, const MX& expr, const MX& ub) { return (lb<=expr)<= ub; }
    #endif
    using OptiStackSpline::Function;
};

class OptiSplineSol : private OptiStackSpline {
  public:
    OptiSplineSol(const OptiStackSpline& opti) : OptiStackSpline(opti) {};
    using OptiStack::value;
    using OptiStackSpline::value;
    using OptiStackSpline::dual;
    using OptiStackSpline::stats;
    OptiStack debug() { return copy(); }
    using OptiStackSpline::casadi_solver;
};

#endif //OPTI_H
