#ifndef SHAREDOBJECT
#define SHAREDOBJECT

#include "SharedObjectNode.h"

namespace spline{

class SharedObject {
public:
#ifndef SWIG
    /// Default constructor
    SharedObject() : node_(0) {};

    /// Copy constructor (shallow copy)
    SharedObject(const SharedObject& ref) ;

    ~SharedObject() ;

    /// Assignment operator
    SharedObject& operator=(const SharedObject& ref) ;

    /// Assign the node to a node class pointer (or null)
    void assign_node(SharedObjectNode* node) ;

    void incref() ;

    void decref() ;

    /// Get a const pointer to the node
    SharedObjectNode* get() const ;

    /// Access a member function or object
    SharedObjectNode* operator->() const ;
private:
    SharedObjectNode* node_;
#endif // SWIG
};

}
#endif   //  SHAREDOBJECT
