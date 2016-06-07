#include "SharedObjectNode.h"

namespace spline {

SharedObjectNode::SharedObjectNode() { count_ = 0; };

SharedObjectNode::~SharedObjectNode() {  };

/// Copy constructor
SharedObjectNode::SharedObjectNode(const SharedObjectNode& node) { count_ = 0; }

/// Assignment operator
SharedObjectNode& SharedObjectNode::operator=(const SharedObjectNode& node) { return *this; };

} // namespace spline
