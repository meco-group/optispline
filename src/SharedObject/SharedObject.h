#ifndef SHAREDOBJECT
#define SHAREDOBJECT

#include <string>
#include "SharedObjectNode.h"
#include "PrintableObject.h"

namespace spline{

class SharedObject : public spline::PrintableObject<SharedObject> {
public:

    virtual std::string to_string() const override {
      if (node_) {
        return (*this)->to_string();
      } else {
        return "Null Node";
      }
    }

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
