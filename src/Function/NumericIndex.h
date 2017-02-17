#ifndef CPP_SPLINES_NUMERICINDEX_H
#define CPP_SPLINES_NUMERICINDEX_H

#include <string>
#include <vector>

#include "../SharedObject/SharedObject.h"
#include "../SharedObject/SharedObjectNode.h"
#include "Argument.h"
#include "Index.h"

namespace spline {
    class NumericIndex {
    public:
        NumericIndex() : index_(-1) {};
        NumericIndex(int index) : index_(index) {};
        operator int() const { return index_; };
        int index() const { return index_; };
        static std::vector<int> as_int(const std::vector<NumericIndex>& ind){
            std::vector<int> ret(ind.size());
            for (int i=0; i<ind.size(); i++){
                ret[i] = ind[0].index();
            }
            return ret;
        }
        static std::vector<Index> as_index(const std::vector<NumericIndex>& ind){
            std::vector<Index> ret(ind.size());
            for (int i=0; i<ind.size(); i++){
                ret[i] = ind[0].index();
            }
            return ret;
        }

    private:
      int index_;
    };
} // namespace spline
#endif //CPP_SPLINES_NUMERICINDEX_H
