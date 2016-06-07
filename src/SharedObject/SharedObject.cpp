#include <assert.h>
#include "SharedObject.h"

namespace spline {

SharedObject::SharedObject(const SharedObject& ref) { node_ = ref.node_; incref(); }

SharedObject::~SharedObject() { decref(); }

SharedObject& SharedObject::operator=(const SharedObject& ref) {  
    if (node_ == ref.node_) return *this;
    decref();
    node_ = ref.node_;
    incref();
    return *this;
}

/// Assign the node to a node class pointer (or null)
void SharedObject::assign_node(SharedObjectNode* node) { decref(); node_ = node; incref(); };

void SharedObject::incref() {
    if (node_) node_->count_++;
}

void SharedObject::decref() {
    if (node_ && --node_->count_ == 0) {
        delete node_;
        node_ = 0;
    }
}

/// Get a const pointer to the node
SharedObjectNode* SharedObject::get() const { assert(node_!=0); return node_; }

/// Access a member function or object
SharedObjectNode* SharedObject::operator->() const { assert(node_!=0); return node_; }

} // namespace spline
