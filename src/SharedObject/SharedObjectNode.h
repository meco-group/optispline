#ifndef SHAREDOBJECTNODE
#define SHAREDOBJECTNODE

#include <string>
#include "PrintableObject.h"
#include "../common.h"


namespace spline {

class SharedObject;

#ifndef SWIG

class SharedObjectNode : public spline::PrintableObject<SharedObjectNode> {
    friend class SharedObject;
public:

    std::string to_string() const override { return "SharedObjectNode"; }
    SharedObjectNode() ;

    /// Copy constructor
    SharedObjectNode(const SharedObjectNode& node) ;

    /// Assignment operator
    SharedObjectNode& operator=(const SharedObjectNode& node) ;

    /// Destructor
    virtual ~SharedObjectNode();

    template<class T>
        T shared_from_this();

    template<class T>
        const T shared_from_this() const;

private:
    casadi_int count_;
};

template<class T>
T SharedObjectNode::shared_from_this() {
    T ret;
    ret.assign_node(this);
    return ret;
}

template<class T>
const T SharedObjectNode::shared_from_this() const {
    T ret;
    ret.assign_node(const_cast<SharedObjectNode*>(this));
    return ret;
}
#endif // SWIG

} // namespace spline
#endif   //  SHAREDOBJECTNODE
