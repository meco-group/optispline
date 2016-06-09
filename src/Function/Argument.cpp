#include "Argument.h"
namespace spline {

    std::string ArgumentNode::getRepresentation() const {return "Argument " + name ;};
    std::string Argument::getRepresentation() const { return (*this)->getRepresentation() ;};
    std::ostream& operator<<(std::ostream &stream, const Argument& argument){
        return stream << argument.getRepresentation();
    }

    ArgumentNode* Argument::get() const { return static_cast<ArgumentNode*>(SharedObject::get()); };
    ArgumentNode* Argument::operator->() const { return get(); }
    
    Argument::Argument()  { assign_node(new ArgumentNode()); };
    Argument::Argument(const std::string &name)  {
        assign_node(new ArgumentNode(name));
    };

    ArgumentNode::ArgumentNode () : name("_"){ }

    ArgumentNode::ArgumentNode (const std::string &name) : name(name){ }

    const std::string& ArgumentNode::getName () const {
        return name;
    }
    
    const std::string& Argument::getName () const {
        return (*this)->getName();
    }

    void ArgumentNode::setName (const std::string &name) {
        this->name = name;
    }
    void Argument::setName (const std::string &name) {
        (*this)->setName(name);
    }
} // namespace spline
