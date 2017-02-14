#ifndef CPP_SPLINES_NUMERICINDEX_H
#define CPP_SPLINES_NUMERICINDEX_H

#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"
#include "Argument.h"

namespace spline {
    class NumericIndex {
    public:
        NumericIndex() : index_(-1) {};
        NumericIndex(int index) : index_(index) {};
        operator int() const { return index_; };
        int index() const { return index_; };
    private:
      int index_;
    };
} // namespace spline
#endif //CPP_SPLINES_NUMERICINDEX_H
