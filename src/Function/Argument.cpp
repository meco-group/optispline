#include "Argument.h"
#include "../Basis/TensorBasis.h"
#include "../common.h"
#include <map>
#include <algorithm>
#include "Parameter.h"
#include <casadi/casadi.hpp>

namespace spline {

    std::string Argument::type() const{
      return (*this)->type();
    }
    std::string ArgumentNode::type() const{
      return "Argument";
    }

    bool Argument::is_all() const{
        return (*this)->is_all();
    }
    bool ArgumentNode::is_all() const{
        return false;
    }

    std::string NullArgumentNode::to_string() const { return "NullArgument ";};
    std::string StringArgumentNode::to_string() const { return "Argument " + name_ ;};
    std::string IntArgumentNode::to_string() const{ return "Argument " + std::to_string(index_) ;};

    ArgumentNode* Argument::get() const { return static_cast<ArgumentNode*>(SharedObject::get()); };
    ArgumentNode* Argument::operator->() const { return get(); }

    std::vector<Argument> Argument::from_vector(const std::vector<casadi_int>& ind) {
      std::vector<Argument> ret(ind.size());
      for (casadi_int i=0;i<ind.size();++i) ret[i] = ind[i];
      return ret;
    }

    std::vector<Argument> Argument::from_vector(const std::vector<std::string>& ind) {
      std::vector<Argument> ret(ind.size());
      for (casadi_int i=0;i<ind.size();++i) ret[i] = ind[i];
      return ret;
    }

    std::vector<Argument> Argument::from_vector(const std::vector<Parameter>& ind) {
      std::vector<Argument> ret(ind.size());
      for (casadi_int i=0;i<ind.size();++i) ret[i] = ind[i];
      return ret;
    }

    casadi_int Argument::concrete(const std::vector<std::string> & args) const {
      return (*this)->concrete(args); };

    NullArgumentNode::NullArgumentNode() { }
    IntArgumentNode::IntArgumentNode(casadi_int index) : index_(index) { }
    StringArgumentNode::StringArgumentNode(const std::string &name) : name_(name) { }

    casadi_int IntArgumentNode::concrete(const std::vector<std::string> & args) const {
      /* spline_assert_message(index_>=0, */
      /*   "Argumenting out-of-range: supplied " + std::to_string(index_) + */
      /*   ". Must be >0."); */
      if (args.size()==0) return index_;
      /* spline_assert_message(index_< args.size(), */
      /*   "Argumenting out-of-range: supplied " + std::to_string(index_) + */
      /*   ". Must be < " + std::to_string(args.size()) + "."); */
      return index_;
    }

    casadi_int StringArgumentNode::concrete(const std::vector<std::string> & args) const {
      std::vector<std::string> args_str;
      for (auto &a : args) args_str.push_back(a);
      auto it = std::find(args_str.begin(), args_str.end(), name_);
      if (it == args_str.end()) {
        /* std::stringstream ss; */
        /* for (auto &s : args_str) ss << "'" << s << "' "; */
        /* spline_error("Argument '" << name_ << "' unknown. Choose one of " << ss.str() << "."); */
        return -1;
      } else {
        return  std::distance(args_str.begin(), it);
      }
    }

    casadi_int NullArgumentNode::concrete(const std::vector<std::string> & args) const {
      spline_error("Null-index.");
      return -1;
    }

    Argument::Argument() { assign_node(new NullArgumentNode()); };
    Argument::Argument(const std::string &name) { assign_node(new StringArgumentNode(name)); };
    Argument::Argument(casadi_int index) { assign_node(new IntArgumentNode(index)); };
    Argument::Argument(const Parameter& para) { assign_node(new StringArgumentNode(para.name())); };

    bool NullArgumentNode::is_all() const{
        return true;
    }

    std::vector< casadi_int > Argument::concrete(const std::vector< Argument >& args, const std::vector< std::string >& strings){
        if (args.empty()) return casadi::range(strings.size());

        std::vector< casadi_int > ard_ind = {};
        for(auto& a : args){
            ard_ind.push_back( a.concrete(strings));
        }
        return ard_ind;
    }

    std::vector< casadi_int > Argument::concrete(const std::vector< Argument >& args, const TensorBasis& tb){
        if (tb.arguments().empty()) return casadi::range(tb.n_basis());
        if (args.empty()) return casadi::range(tb.n_basis());

        std::vector< casadi_int > ard_ind = {};
        for(auto& a : args){
            ard_ind.push_back( a.concrete(tb.arguments()));
        }
        return ard_ind;
    }

    casadi_int Argument::concrete(const Argument& arg, const TensorBasis& tb){
        return arg.concrete(tb.arguments());
    }

} // namespace spline
