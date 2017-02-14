#ifndef SHAREDOBJECTNODE
#define SHAREDOBJECTNODE

namespace spline {

class SharedObject;

#ifndef SWIG

class SharedObjectNode {
    friend class SharedObject;
public:
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
    int count_;
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
