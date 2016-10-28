#ifndef OPTISTACK_H
#define OPTISTACK_H

#include <string>
#include <vector>

#include <casadi/casadi.hpp>

#include "../Function/Function.h"
#include "../Coefficients/Coefficient.h"

using namespace casadi;

#ifndef SWIG
enum OptistackType {OPTISTACK_VAR, OPTISTACK_PAR};

struct MetaCon {
  MX original;
  std::vector<MX> flattened;
  bool is_equality;
};

#endif

class OptistackSolver;

class Optistack {
  friend class OptistackSolver;
public:
  Optistack();
  MX var(int n=1, int m=1);
  MX par(int n=1, int m=1);

  OptistackSolver solver(const MX& f, const std::vector<MX> & g, const std::string& solver, const Dict& options=Dict()) const;

  void assert_has(const MX& m) const;

protected:
  std::map< OptistackType, std::vector<MX> > categorize(const MX& expr);

private:
  MX flag(const MX& m, OptistackType type);
  std::map<MXNode*, OptistackType> data_;

};

class OptistackSolver {
  friend class Optistack;
public:

  // Set value
  void value(const MX& x, const DM& v);

  // Get value
  DM value(const MX& x) const;
  // Get value
  DM value(const DM& x) const { return x; };
  DM value(const SX& x) const { return DM::nan(x.sparsity()); };

  void assert_has(const MX& m) const;

  // Solve the problem
  void solve();
protected:
  OptistackSolver(const Optistack& sc, const MX& f, const std::vector<MX> & g, const std::string& solver, const Dict& options);
private:


  static std::vector< MetaCon > categorize_constraints(const std::vector<MX>& g);
  Optistack sc_;
  std::map<MXNode*, std::vector<DM> > data_;
  std::map< OptistackType, std::vector<MX> > variables_;

  Function solver_;
  Function forder_;
  DMDict res_;
  DMDict arg_;
  MXDict nlp_;
  DM pvalue_;
  std::vector< MetaCon > constraints_;

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
  AnyTensor value(const AnyTensor& c) const;
  void value(const Tensor<MX>& c, const Tensor<DM>& d);
  Tensor<DM> value(const Tensor<MX>& c) const;

protected:
  OptiSplineSolver(const OptiSpline& sc, const MX& f, const std::vector<MX> & g, const std::string& solver, const Dict& options);
};

class OptiSpline : public Optistack {
  friend class OptiSplineSolver;
public:
  using Optistack::var;
  MT var(const std::vector<int>& shape);
  spline::Function Function(const spline::TensorBasis& b);
  OptiSplineSolver solver(const MX& f, const std::vector<MX> & g, const std::string& solver, const Dict& options=Dict()) const;
};


#endif //OPTISTACK_H
