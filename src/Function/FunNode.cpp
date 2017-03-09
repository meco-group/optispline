#include <vector>
#include "FunNode.h"
#include "../Basis/utils/EvaluationGrid.h"
#include "../Basis/TensorBasisConstant.h"
#include "../common.h"

namespace spline {

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
    }

    Basis FunNode::basis(const Argument& i) const {
        spline_assert_message(false, "invalided method basis on " + to_string());
    }

    TensorDomain FunNode::domain() const {
        spline_assert_message(false, "invalided method domain on " + to_string());
    }

    int FunNode::n_inputs() const {
        spline_assert_message(false, "invalided method n_inputs on " + to_string());
    }

    Function FunNode::insert_knots(const AnyVector & new_knots) const {
        spline_assert_message(false, "invalid method insert_knots on " + to_string());
    }

    Function FunNode::insert_knots(const AnyVector & new_knots, const NumericIndex & arg_ind) const {
        spline_assert_message(false, "invalid method insert_knots on " + to_string());
    }

    Function FunNode::insert_knots(const AnyVector & new_knots, const std::string & arg) const {
        spline_assert_message(false, "invalid method insert_knots on " + to_string());
    }

    Function FunNode::insert_knots(const std::vector<AnyVector> & new_knots, const std::vector<std::string> & arg) const {
        spline_assert_message(false, "invalid method insert_knots on " + to_string());
    }

    Function FunNode::insert_knots(const std::vector<AnyVector> & new_knots, const NumericIndexVector & arg_ind) const {
        spline_assert_message(false, "invalid method insert_knots on " + to_string());
    }

    Function FunNode::midpoint_refinement(int refinement) const {
        spline_assert_message(false, "invalid method midpoint_refinement on " + to_string());
    }

    Function FunNode::midpoint_refinement(int refinement, const NumericIndex & arg_ind) const {
        spline_assert_message(false, "invalid method midpoint_refinement on " + to_string());
    }

    Function FunNode::midpoint_refinement(int refinement, const std::string & arg) const {
        spline_assert_message(false, "invalid method midpoint_refinement on " + to_string());
    }

    Function FunNode::midpoint_refinement(const std::vector<int> & refinement, const std::vector<std::string> & arg) const {
        spline_assert_message(false, "invalid method midpoint_refinement on " + to_string());
    }

    Function FunNode::midpoint_refinement(const std::vector<int> & refinement, const NumericIndexVector & arg_ind) const {
        spline_assert_message(false, "invalid method midpoint_refinement on " + to_string());
    }

    Function FunNode::degree_elevation(int elevation) const {
        spline_assert_message(false, "invalid method degree_elevation on " + to_string());
    }

    Function FunNode::degree_elevation(int elevation, const NumericIndex& arg_ind) const {
        spline_assert_message(false, "invalid method degree_elevation on " + to_string());
    }

    Function FunNode::degree_elevation(int elevation, const std::string& arg) const {
        spline_assert_message(false, "invalid method degree_elevation on " + to_string());
    }

    Function FunNode::degree_elevation(const std::vector<int> & elevation, const std::vector<std::string> & args) const {
        spline_assert_message(false, "invalid method degree_elevation on " + to_string());
    }

    Function FunNode::degree_elevation(const std::vector<int> & elevation, const NumericIndexVector & arg_ind) const {
        spline_assert_message(false, "invalid method degree_elevation on " + to_string());
    }

    Function FunNode::kick_boundary(const TensorDomain& boundary) const {
        spline_assert_message(false, "invalid method kick_boundary on " + to_string());
    }

    Function FunNode::kick_boundary(const TensorDomain& boundary, const NumericIndex& arg_ind) const {
        spline_assert_message(false, "invalid method kick_boundary on " + to_string());
    }

    Function FunNode::kick_boundary(const TensorDomain& boundary,const std::vector<std::string> & args) const {
        spline_assert_message(false, "invalid method kick_boundary on " + to_string());
    }

    Function FunNode::kick_boundary(const TensorDomain& boundary, const NumericIndexVector & arg_ind) const {
        spline_assert_message(false, "invalid method kick_boundary on " + to_string());

    }

    Function FunNode::derivative() const {
        spline_assert_message(false, "invalid method derivative on " + to_string());
    }

    Function FunNode::derivative(int order) const {
        spline_assert_message(false, "invalid method derivative on " + to_string());
    }

    Function FunNode::derivative(int order, const std::string& arg) const {
        spline_assert_message(false, "invalid method derivative on " + to_string());
    }

    Function FunNode::derivative(int order, const NumericIndex& arg_ind) const {
        spline_assert_message(false, "invalid method derivative on " + to_string());
    }

    Function FunNode::derivative(const std::vector<int>& orders, const std::vector<std::string>& args) const {
        spline_assert_message(false, "invalid method derivative on " + to_string());
    }

    Function FunNode::derivative(const std::vector<int>& orders, const NumericIndexVector& arg_ind) const {
        spline_assert_message(false, "invalid method derivative on " + to_string());
    }

    Function FunNode::antiderivative() const {
        spline_assert_message(false, "invalid method antiderivative on " + to_string());
    }

    Function FunNode::antiderivative(int order) const {
        spline_assert_message(false, "invalid method antiderivative on " + to_string());
    }

    Function FunNode::antiderivative(int order, const std::string& arg) const {
        spline_assert_message(false, "invalid method antiderivative on " + to_string());
    }

    Function FunNode::antiderivative(int order, const NumericIndex& arg_ind) const{
        spline_assert_message(false, "invalid method antiderivative on " + to_string());
    }

    Function FunNode::antiderivative(const std::vector<int>& orders, const std::vector<std::string>& args) const {
        spline_assert_message(false, "invalid method antiderivative on " + to_string());
    }

    Function FunNode::antiderivative(const std::vector<int>& orders, const NumericIndexVector& arg_ind) const {
        spline_assert_message(false, "invalid method antiderivative on " + to_string());
    }

    std::vector<spline::Function> FunNode::jacobian() const {
        spline_assert_message(false, "invalid method vector on " + to_string())
    }


    AnyTensor FunNode::integral() const {
        spline_assert_message(false, "invalid method integral on " + to_string());
    }

    AnyTensor FunNode::integral(const TensorDomain& domain) const {
        spline_assert_message(false, "invalid method integral on " + to_string());
    }

    Function FunNode::partial_integral(const TensorDomain& domain, const std::vector<std::string>& args) const {
        spline_assert_message(false, "invalid method partial_integral on " + to_string());
    }

    Function FunNode::partial_integral(const TensorDomain& domain, const NumericIndexVector& arg_ind) const {
        spline_assert_message(false, "invalid method partial_integral on " + to_string());
    }

}  // namespace spline
