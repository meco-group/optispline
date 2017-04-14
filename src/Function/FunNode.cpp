#include <vector>
#include "FunNode.h"
#include "Function.h"
#include "../common.h"

namespace spline {
    FunNode::FunNode(const Coefficient& coeff) : coeff_(coeff.matrix_valued()) { }

    std::string FunNode::type() const{
        return "FunNode";
    }

    std::string FunNode::to_string() const{
        return "FuncNode";
    }

    Function FunNode::operator+(const Function& f) const {
        return operator+(*f.get());
    }

    Function FunNode::operator*(const Function& f) const {
        return operator*(*f.get());
    }

    Function FunNode::mtimes(const Function& f) const {
        return mtimes(*f.get());
    }

    bool FunNode::is_scalar() const {
        return shape()[0] == 1 && shape()[1] == 1;
    }

    std::vector< int > FunNode::shape() const {
        return coeff_.shape();
    }

    Basis FunNode::basis() const {
      spline_assert_message(false, "invalided method basis on " + to_string());
      return Basis();
    }

    Basis FunNode::basis(const Argument& i) const {
      spline_assert_message(false, "invalided method basis on " + to_string());
      return Basis();
    }

    TensorDomain FunNode::domain() const {
      spline_assert_message(false, "invalided method domain on " + to_string());
      return TensorDomain();
    }

    int FunNode::n_inputs() const {
      spline_assert_message(false, "invalided method n_inputs on " + to_string());
      return 0;
    }

    Function FunNode::insert_knots(const std::vector<AnyVector> & new_knots, const std::vector<int> & arg_ind) const {
      spline_assert_message(false, "invalid method insert_knots on " + to_string());
      return Function();
    }

    Function FunNode::midpoint_refinement(const std::vector<int>& orders, const std::vector< int >& arg_ind) const {
      spline_assert_message(false, "invalid method midpoint_refinement on " + to_string());
      return Function();
    }

    Function FunNode::degree_elevation(const std::vector<int>& orders, const std::vector< int >& arg_ind) const {
      spline_assert_message(false, "invalid method degree_elevation on " + to_string());
      return Function();
    }

    Function FunNode::kick_boundary(const TensorDomain& boundary) const {
      spline_assert_message(false, "invalid method kick_boundary on " + to_string());
      return Function();
    }

    Function FunNode::kick_boundary(const TensorDomain& boundary, const NumericIndex& arg_ind) const {
      spline_assert_message(false, "invalid method kick_boundary on " + to_string());
      return Function();
    }

    Function FunNode::kick_boundary(const TensorDomain& boundary,const std::vector<std::string> & args) const {
      spline_assert_message(false, "invalid method kick_boundary on " + to_string());
      return Function();
    }

    Function FunNode::kick_boundary(const TensorDomain& boundary, const NumericIndexVector & arg_ind) const {
      spline_assert_message(false, "invalid method kick_boundary on " + to_string());
      return Function();
    }

    Function FunNode::derivative(const std::vector<int>& orders, const std::vector< int >& arg_ind) const {
      spline_assert_message(false, "invalid method derivative on " + to_string());
      return Function();
    }

    Function FunNode::antiderivative(const std::vector<int>& orders, const std::vector< int >& arg_ind) const {
      spline_assert_message(false, "invalid method antiderivative on " + to_string());
      return Function();
    }

    std::vector<spline::Function> FunNode::jacobian() const {
      spline_assert_message(false, "invalid method vector on " + to_string());
      return std::vector<spline::Function>();
    }

    AnyTensor FunNode::integral() const {
      spline_assert_message(false, "invalid method integral on " + to_string());
      return DT();
    }

    AnyTensor FunNode::integral(const TensorDomain& domain) const {
      spline_assert_message(false, "invalid method integral on " + to_string());
      return DT();
    }

    Function FunNode::partial_integral(const TensorDomain& domain, const std::vector<std::string>& args) const {
      spline_assert_message(false, "invalid method partial_integral on " + to_string());
      return Function();
    }

    Function FunNode::partial_integral(const TensorDomain& domain, const NumericIndexVector& arg_ind) const {
      spline_assert_message(false, "invalid method partial_integral on " + to_string());
      return Function();
    }

}  // namespace spline
