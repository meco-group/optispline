#include "Index.h"
#include "../common.h"
#include <map>
#include <algorithm>
namespace spline {

    std::string StringIndexNode::getRepresentation() const { return "Index " + name_ ;};
    std::string IntIndexNode::getRepresentation() const{
      return "Index " + std::to_string(index_) ;};
    std::string NullIndexNode::getRepresentation() const { return "NullIndex ";};

    std::string Index::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::ostream& operator<<(std::ostream &stream, const Index& argument) {
        return stream << argument.getRepresentation();
    }

    IndexNode* Index::get() const { return static_cast<IndexNode*>(SharedObject::get()); };
    IndexNode* Index::operator->() const { return get(); }

    std::vector<Index> Index::from_vector(const std::vector<int>& ind) {
      std::vector<Index> ret(ind.size());
      for (int i=0;i<ind.size();++i) ret[i] = ind[i];
      return ret;
    }

    int Index::concrete(const std::vector<std::string> & args) const {
      return (*this)->concrete(args); };

    NullIndexNode::NullIndexNode() { }
    IntIndexNode::IntIndexNode(int index) : index_(index) { }
    StringIndexNode::StringIndexNode(const std::string &name) : name_(name) { }

    int IntIndexNode::concrete(const std::vector<std::string> & args) const {
      spline_assert_message(index_>=0,
        "Indexing out-of-range: supplied " + std::to_string(index_) +
        ". Must be >0.");
      if (args.size()==0) return index_;
      spline_assert_message(index_< args.size(),
        "Indexing out-of-range: supplied " + std::to_string(index_) +
        ". Must be < " + std::to_string(args.size()) + ".");
      return index_;
    }

    int StringIndexNode::concrete(const std::vector<std::string> & args) const {
      std::vector<std::string> args_str;
      for (auto &a : args) args_str.push_back(a);
      auto it = std::find(args_str.begin(), args_str.end(), name_);
      if (it == args_str.end()) {
        std::stringstream ss;
        for (auto &s : args_str) ss << "'" << s << "' ";
        spline_error("Index '" << name_ << "' unknown. Choose one of " << ss.str() << ".");
        return -1;
      } else {
        return  std::distance(args_str.begin(), it);
      }
    }

    int NullIndexNode::concrete(const std::vector<std::string> & args) const {
      spline_error("Null-index.");
      return -1;
    }

    Index::Index() { assign_node(new NullIndexNode()); };
    Index::Index(const std::string &name) { assign_node(new StringIndexNode(name)); };
    Index::Index(int index) { assign_node(new IntIndexNode(index)); };
} // namespace spline
